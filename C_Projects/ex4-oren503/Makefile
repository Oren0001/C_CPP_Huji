.PHONY: all clean
FLAGS = -Wall -Wextra -Wvla -Werror -g -lm -std=c99

all: libhashmap.a libhashmap_tests.a
clean:
	rm -rf *.o *.a

############### Creates a static library of hash map ###############
libhashmap.a: vector.o hashmap.o pair.o
	ar rcs -o $@ $^

vector.o: vector.c vector.h
	gcc $(FLAGS) -c $<

hashmap.o: hashmap.c hashmap.h vector.h pair.h
	gcc $(FLAGS) -c $<

pair.o: pair.c pair.h
	gcc $(FLAGS) -c $<
############### End of hash map ###############

############### Creates a static library of hash map tests ###############
libhashmap_tests.a: test_suite.o
	ar rcs -o $@ $^

test_suite.o: test_suite.c test_suite.h test_pairs.h hash_funcs.h hashmap.h pair.h
	gcc $(FLAGS) -c $<
############### End of hash map tests ###############
