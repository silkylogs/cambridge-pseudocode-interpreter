clear
FLAGS="-Wall -Wextra -g -Wno-unsafe-buffer-usage"

clang -c $FLAGS bless.c -o bless.o
clang bless.o $FLAGS "test.c" -o "test"
# clang main.c $FLAGS -o cpi_c

./"test"
