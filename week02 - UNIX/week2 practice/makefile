cc = gcc
target = FMT
objects = main.o String_Manipulation.o Output.o

$(target) : $(objects)
	$(cc) -g -o $(target) $(objects)

$(objects) : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
