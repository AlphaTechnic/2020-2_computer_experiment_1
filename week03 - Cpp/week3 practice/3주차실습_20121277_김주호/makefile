cc = g++
target = rangearray_practice
objects = main.o rangearray.o array.o
cflags = -W -g

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

.PHONY : clean
clean :
	rm $(target) $(objects)
