clear

clang -c -Wall -Weverything -g bless.c -o bless.o

clang bless.o -g "test.c" -o "test"
#clang main.c -Wall -Weverything -g -o cpi_c

./"test"
