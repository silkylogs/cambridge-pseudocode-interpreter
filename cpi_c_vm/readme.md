# CPI C: VM

## Motivation
I wish to bring a language that I've written on paper, by hand, to life. Even if I dislike the semantics of said language very much.

## 11 May, 2026
I ought to list as many requirements as I could think of so I don't forget about them later:
- The development environment should be as portable and as pain-free as possible
  - Stick to a language that is available on at least Windows, Linux and Mac: C89
  - Use the most minimal possible build system (A shell script wrapping something like `CC main.c -o cpi $(FLAGS)`)
  - Be able to painlessly hot-reload on any platform. The easiest way to do that is to write a VM, then target it.
- Language IO details: (Note: I don't know why this language has decided to taxonomise IO so much.) (TODO: simplify)
  - Input
    - Newline-terminated arbitary string input (via the `INPUT` command)
    - Whole text file input (`READ` mode)
    - Arbitary binary file input (`RANDOM` mode)
  - Output
    - Length-based string output (`OUTPUT`)
    - Whole text file output (`WRITE` and `APPEND` modes)
    - Arbitary binary file output (`RANDOM` mode)
- Processing:
  - All computation involves named variables of some type
  - All computation has the shape input -> process -> output
