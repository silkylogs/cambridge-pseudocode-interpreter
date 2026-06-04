clang main.c -o cpi -DCPI_RUN_TESTS=1 -DPLATFORM_APPLE -g -fsanitize=address -fsanitize=undefined
