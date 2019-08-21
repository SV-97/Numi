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
    source = Stack(source)
    instructions = Stack()
    try:
        while True:
            char = source[0]
            #if char == "[": parse_frame(source)
            if char.isnumeric():
                instructions.push(parse_number(source))
            elif char.isspace():
                source.pop_left()
                continue
            else:
                instructions.push(parse_function(source))
    except IndexError:
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

    def interpret(self):
        e_stack = Stack()
        try:
            while True:
                instruction = self.i_stack.pop_left()
                t = type(instruction)
                if t == float:
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


def interpret(source):
    instructions = parse(source)
    functions = {
        "+": lift_binary_to_stack(add),
        "-": lift_binary_to_stack(sub),
        "*": lift_binary_to_stack(mul),
        "/": lift_binary_to_stack(truediv),
        "mod": lift_binary_to_stack(mod),
        "print": lambda x: print(x.pop_left())}
    interpreter = Interpreter(instructions, functions)
    return interpreter.interpret()


if __name__ == "__main__":
    source = input()
    result = interpret(source)
    print(result)
