WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --show-leak-kinds=all --verbose --track-origins=yes

TESTFALGS = #-DDEBUG
SRCS = pa2.c evaluate.c build.c
OBJS = $(SRCS:%.c=%.o)
pa2: $(OBJS)
	$(GCC) $(TESTFALGS) $(OBJS) -o pa2
.c.o:
	$(GCC) $(TESTFALGS) -c $*.c

testeval: pa2
	./pa2 -e ./examples/invalidtree0.b
	./pa2 -e ./examples/invalidtree1.b
	./pa2 -e ./examples/invalidtree2.b
	./pa2 -e ./examples/tree0.b
	./pa2 -e ./examples/tree1.b
	./pa2 -e ./examples/tree2.b
	./pa2 -e ./examples/tree3.b
test0: pa2
	./pa2 -b ./examples/ops0.b tree0.b
	cmp tree0.b ./examples/tree0.b
test1: pa2
	./pa2 -b ./examples/ops1.b tree1.b
	cmp tree1.b ./examples/tree1.b

test2: pa2
	./pa2 -b ./examples/ops2.b tree2.b
	cmp tree2.b ./examples/tree2.b

test3: pa2
	./pa2 -b ./examples/ops3.b tree3.b
	cmp tree3.b ./examples/tree3.b

testall: test0 test1 test2 test3

memtest: pa2
	$(VAL) ./pa2 -b ./examples/ops3.b tree3.b

stat:
	find . "(" -name "*.c" -or -name "*.h" -or -name "makefile" ")" -print | xargs wc -l

push:
	git add *.c *.h makefile ./examples *.pdf
	git commit -m "evaluate part done."
	git push

clean:
	rm -f pa2 *.o memcheck* *.b