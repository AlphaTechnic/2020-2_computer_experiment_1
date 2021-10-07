cc=g++
target=growablearray
cflags=-W -g
objects=main.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

.PHONY : clean

clean :
	rm $(target) $(objects)