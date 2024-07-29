neoshell: functions.o main.o
	gcc functions.o main.o -o neoshell -g

main.o: main.c
	gcc -c main.c -o main.o -g

functions.o: functions.c
	gcc -c functions.c -o functions.o -g
