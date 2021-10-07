cc=g++
target=string
cflags=-W -g
objects=main.o str.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

.PHONY : clean
clean :
	rm $(target) $(objects)