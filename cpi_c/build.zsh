clear
FLAGS="-Wall -Wextra -Wmost -g -Wno-unsafe-buffer-usage"

clang -c $FLAGS vm.c -o vm.o
clang vm.o $FLAGS test.c -o "test"
# clang main.c $FLAGS -o cpi_c

./"test"
