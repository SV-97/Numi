from collections import deque
from operator import add, sub, mul, truediv, mod
from functools import wraps
from pprint import pprint


class Stack(deque):
    """deque with aliases for append and appendleft and pop_left"""

    def push(self, *args, **kwargs):
        return self.append(*args, **kwargs)

    def push_left(self, *args, **kwargs):
        return self.appendleft(*args, **kwargs)

    def pop_left(self, *args, **kwargs):
        return self.popleft(*args, **kwargs)


def parse(source):
    if type(source) != Stack:
        source = Stack(source)
    instructions = Stack()
    try:
        while True:
            char = source[0]
            if char == "[":
                source.pop_left()
                instructions.push(parse(source))
                continue
            if char == "]":
                source.pop_left()
                return instructions
            if char.isnumeric():
                instructions.push(parse_number(source))
            elif char.isspace():
                source.pop_left()
                continue
            else:
                instructions.push(parse_function(source))
    except IndexError:
        print("returning: ")
        pprint(instructions)
        return instructions


def parse_number(source):
    buf = Stack()
    try:
        while True:
            c = source.pop_left()
            if c.isnumeric():
                buf.push(c)
            else:
                source.push_left(c)
                break
    finally:
        return float("".join(buf))


def parse_function(source):
    buf = Stack()
    try:
        while True:
            c = source.pop_left()
            if not c.isspace():
                buf.push(c)
            else:
                source.push_left(c)
                break
    finally:
        return "".join(buf)


class Interpreter():
    def __init__(self, instructions, functions):
        self.i_stack = instructions
        self.functions = functions
        self.e_stack = None

    def interpret(self):
        if self.e_stack is None:
            e_stack = Stack()
        else:
            e_stack = self.e_stack
        try:
            while True:
                instruction = self.i_stack.pop_left()
                t = type(instruction)
                if t == float or t == Stack:
                    e_stack.push(instruction)
                elif t == str:
                    try:
                        f = self.functions[instruction]
                        f(e_stack)
                    except IndexError:
                        raise ValueError(f"Unknown function: {instruction}")
                else:
                    raise TypeError(f"Instruction has invalid type {t}")
        except IndexError:
            return e_stack


def lift_binary_to_stack(f):
    @wraps(f)
    def g(stack):
        l = stack.pop()
        r = stack.pop()
        stack.push(f(l, r))
    return g


def call(e_stack):
    frame = e_stack.pop_left()
    interpreter = Interpreter(frame, BUILTINS)
    interpreter.e_stack = e_stack
    return interpreter.interpret()


BUILTINS = {
    "+": lift_binary_to_stack(add),
    "-": lift_binary_to_stack(sub),
    "*": lift_binary_to_stack(mul),
    "/": lift_binary_to_stack(truediv),
    "mod": lift_binary_to_stack(mod),
    "print": lambda x: print(x.pop_left()),
    "call": call}


def interpret(source):
    if type(source) == Stack:
        instructions = source
    else:
        instructions = parse(source)

    interpreter = Interpreter(instructions, BUILTINS)
    return interpreter.interpret()


if __name__ == "__main__":
    source = input()
    result = interpret(source)
    print(result)
