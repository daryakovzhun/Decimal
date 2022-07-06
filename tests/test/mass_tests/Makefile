CC = gcc -g #-fsanitize=address

SOURCES:= ../../../bbbbb

OBJECTS:= $(SOURCES:.c=.o)
RM:= rm -rf

# all: test_add test_sub test_mul test_add test_div
PHONY: all

test_int_add: $(SOURCES) $(OBJECTS) addition_test.c
	@$(CC) $(OBJECTS) addition_test.c -o $@ -lgmp -lm
	@rm -rf test_int_add

test_int_sub: $(SOURCES) $(OBJECTS) subtraction_test.c
	@$(CC) $(OBJECTS) subtraction_test.c -o $@ -lgmp -lm
	@rm -rf test_int_sub

test_int_mul: $(SOURCES) $(OBJECTS) multiplication_test.c
	@$(CC) $(OBJECTS) multiplication_test.c -o $@ -lgmp -lm
	@rm -rf test_int_mul

test_int_div: $(SOURCES) $(OBJECTS) division_test.c
	@$(CC) $(OBJECTS) division_test.c -o $@ -lgmp -lm
	@rm -rf test_int_div

test_float_add: $(SOURCES) $(OBJECTS) float_addition_test.c
	@$(CC) $(OBJECTS) float_addition_test.c -o $@ -lgmp -lm
	@rm -rf test_float_add

test_float_sub: $(SOURCES) $(OBJECTS) float_subtraction_test.c
	@$(CC) $(OBJECTS) float_subtraction_test.c -o $@ -lgmp -lm
	@rm -rf test_float_sub

test_float_mul: $(SOURCES) $(OBJECTS) float_multiplication_test.c
	@$(CC) $(OBJECTS) float_multiplication_test.c -o $@ -lgmp -lm
	@rm -rf test_float_mul

test_float_div: $(SOURCES) $(OBJECTS) float_division_test.c
	@$(CC) $(OBJECTS) float_division_test.c -o $@ -lgmp -lm
	@rm -rf test_float_div

%.o: %.c
	@$(CC) -c $< -o $@ -lm

clean:
	@$(RM) ../../another/*.o \
	../../arithmetic/*.o \
	../../comparison/*.o \
	../../convertion/*.o \
	../../core/*.o \
	*out \
	*.o test_int_add test_int_sub \
	test_int_mul test_int_div \
	test_float_add test_float_sub \
	test_float_div test_float_mul

rebuild: clean test