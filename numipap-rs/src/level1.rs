use std::collections::{HashMap, VecDeque};
use std::default::Default;
use std::env;
use std::ops;
use std::str::{Chars, FromStr};

static EMPTY_STACK: &'static str = "Function encountered stack with too few arguments.";

type OperandStack = VecDeque<Node>;
type FunctionSet<'a> = HashMap<&'a str, Box<Fn(&mut OperandStack)>>;

fn main() {
    let equation = env::args().skip(1).collect::<Vec<_>>().join(" ");
    execute(equation);
}

fn built_ins<'a>() -> FunctionSet<'a> {
    use ops::*;
    let mut fs: FunctionSet<'_> = HashMap::new();

    macro_rules! insert {
        ( $key: expr, $f: expr ) => {{
            fs.insert($key, Box::new($f));
        }};
    }

    insert!("sin", sin);
    insert!("+", num_binop(Add::add));
    insert!("-", num_binop(Sub::sub));
    insert!("*", num_binop(Mul::mul));
    insert!("/", num_binop(Div::div));
    insert!("mod", num_binop(Rem::rem));
    insert!("drop", drop);
    insert!("dup", dup);
    fs
}

fn execute(equation: String) {
    let chars = equation.chars();
    let mut parser = Parser::new(chars);
    let mut instruction_stack = parser.parse();
    let fs = built_ins();
    let mut interpreter = Interpreter::new(&mut instruction_stack, fs);
    let result_stack = interpreter.interpret();
    dbg!(result_stack.iter().rev().collect::<Vec<_>>());
}

struct Parser<'a> {
    chars: Chars<'a>,
    current_char: Option<char>,
    stack: OperandStack,
}

impl<'a> Parser<'a> {
    pub fn new(mut chars: Chars<'a>) -> Self {
        let current_char = chars.next();
        Parser {
            chars,
            current_char,
            stack: Default::default(),
        }
    }

    pub fn advance(&mut self) {
        self.current_char = self.chars.next();
    }

    pub fn parse(&mut self) -> OperandStack {
        while let Some(c) = self.current_char {
            match c {
                c if c.is_digit(10) => self.parse_number(),
                c if c.is_whitespace() => self.advance(),
                _ => self.parse_function(),
            }
        }
        self.stack.clone()
    }

    fn parse_number(&mut self) {
        let mut buf = String::new();
        while let Some(c) = self.current_char {
            if c.is_digit(10) || c == '.' {
                self.advance();
                buf.push(c);
            } else {
                break;
            }
        }
        let parsed = f64::from_str(&buf);
        self.stack.push_back(match parsed {
            Ok(val) => Node::NumberLiteral(val),
            Err(_) => Node::Error("Failed to parse".to_string()),
        });
    }

    fn parse_function(&mut self) {
        let mut buf = String::new();
        while let Some(c) = self.current_char {
            if c.is_whitespace() {
                break;
            } else {
                buf.push(c);
                self.advance();
            }
        }
        self.stack.push_back(Node::FunctionCall(buf));
    }
}

#[derive(Debug, Clone)]
enum Node {
    FunctionCall(String),
    NumberLiteral(f64),
    Error(String),
}

struct Interpreter<'a> {
    instruction_stack: &'a mut OperandStack,
    function_set: FunctionSet<'a>,
}

impl<'a> Interpreter<'a> {
    pub fn new(instruction_stack: &'a mut OperandStack, function_set: FunctionSet<'a>) -> Self {
        Interpreter {
            instruction_stack,
            function_set,
        }
    }

    pub fn interpret(&mut self) -> OperandStack {
        let mut exec_stack = VecDeque::new();
        while let Some(node) = self.instruction_stack.pop_front() {
            match node {
                node @ Node::NumberLiteral(_) => exec_stack.push_back(node),
                Node::Error(message) => println!("{}", message),
                Node::FunctionCall(name) => {
                    let name: &str = &name;
                    match self.function_set.get(name) {
                        Some(function) => function(&mut exec_stack),
                        None => println!("Unknown function: {}", name),
                    }
                }
            }
        }
        exec_stack
    }
}

fn dup(stack: &mut OperandStack) {
    let to_dup = stack.pop_back().expect(EMPTY_STACK);
    stack.push_back(to_dup.clone());
    stack.push_back(to_dup);
}

fn drop(stack: &mut OperandStack) {
    stack.pop_back().expect(EMPTY_STACK);
}

fn sin(stack: &mut OperandStack) {
    let x = stack.pop_back().expect(EMPTY_STACK);
    match x {
        Node::NumberLiteral(x) => stack.push_back(Node::NumberLiteral(x.sin())),
        _ => unimplemented!(),
    }
}

/// Any binary operation on two numbers that produces a number
fn num_binop(f: impl Fn(f64, f64) -> f64) -> impl Fn(&mut OperandStack) {
    move |stack: &mut OperandStack| {
        let l = stack.pop_back().expect(EMPTY_STACK);
        let r = stack.pop_back().expect(EMPTY_STACK);
        match (l, r) {
            (Node::NumberLiteral(l), Node::NumberLiteral(r)) => {
                stack.push_back(Node::NumberLiteral(f(l, r)))
            }
            _ => unimplemented!(),
        }
    }
}
