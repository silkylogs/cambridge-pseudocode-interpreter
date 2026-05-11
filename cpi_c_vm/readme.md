# CPI C: VM

## Motivation
I wish to bring a language that I've written on paper, by hand, to life. Even if I dislike the semantics of said language very much.

## 11 May, 2026
I ought to list as many requirements as I could think of so I don't forget about them later. This are rough notes.
- The development environment should be as portable and as pain-free as possible
  - Stick to a language that is available on at least Windows, Linux and Mac: C89
  - Use the most minimal possible build system (A shell script wrapping something like `CC main.c -o cpi $(FLAGS)`)
  - Be able to painlessly hot-reload on any platform. The easiest way to do that is to write a VM, then write tooling and software for it.
- IO will be from
  - disk (input, output)
  - IO devices (stdout and stdin)
  - memory (read, write)
- Operations in the language are observed to take the following shape
  - Binary operators
  - Unary operators
  - Functions with arbitary number of arguments
  - Procedures with arbitary number of arguments
  - Or, simplifying, functions with arbitary number of arguments and [0, 1] number of return values
- Builtin operations
  - Arithmetic (+ - * /) (Recommend three way operations)
  - IO call
  - Proc calls

