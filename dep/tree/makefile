headers= tree.h treeprint.h coord2d.h stringstack.h
src= main.c tree.c treeprint.c coord2d.c stringstack.c
cc= gcc
out= main.exe
flags= -O1 -pedantic -std=c99

$(out):$(src) $(headers) makefile
	$(cc) $(src) -o $(out) $(flags)
