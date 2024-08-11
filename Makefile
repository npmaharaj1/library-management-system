# Default rule, build main executable.
main: main.o menu.o basicFunctions.o bookDates.o
	gcc main.o menu.o basicFunctions.o bookDates.o -lncurses -o main

# Build main object file.
main.o: main.c otherFunctions.h
	gcc -Wextra -c main.c

menu.o: menu.c otherFunctions.h
	gcc -Wextra -c menu.c

basicFunctions.o: basicFunctions.c otherFunctions.h
	gcc -Wextra -c basicFunctions.c

bookDates.o: bookDates.c otherFunctions.h
	gcc -Wextra -c bookDates.c

# Remove all object files and executable.
clean:
	rm -f *.o main
