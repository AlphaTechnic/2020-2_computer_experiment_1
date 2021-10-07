cc=gcc
cflags = -W -g
target = starwars

objects = main.o print_the_numbers.o get_num_of_TestCase.o register_TestCase_at_dynamic_array.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

main.o print_the_numbers.o get_num_of_TestCase.o register_TestCase_at_dynamic_array.o : Header.h

.PHONY : clean

clean :
	rm $(target) $(objects)
