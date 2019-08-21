level1: vector.o node.o linkedList.o parser.o stack.o tests.o hashmap.o interpreter.o level1.o
	gcc vector.o node.o linkedList.o parser.o stack.o tests.o hashmap.o interpreter.o level1.o -lm -g -o level1

level1.o: level1.c
	gcc -Wall -c -g level1.c

vector.o: vector.c
	gcc -Wall -c -g vector.c

node.o: node.c
	gcc -Wall -c -g node.c

ĺinkedList.o: ĺinkedList.c
	gcc -Wall -c -g ĺinkedList.c

parser.o: parser.c
	gcc -Wall -c -g parser.c

stack.o: stack.c
	gcc -Wall -c -g stack.c

tests.o: tests.c
	gcc -Wall -c -g tests.c

hashmap.o: hashmap.c
	gcc -Wall -c -g hashmap.c

interpreter.o: interpreter.c
	gcc -Wall -c -g interpreter.c

clean:
	rm *.o level1

run: level1
	./level1

leaks: level1
	valgrind --tool=memcheck --leak-check=full ./level1