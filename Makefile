CC := g++
CFLAGS= -Wall

main.exe: main.o matrix.o
	$(CC) $(CFLAGS) -o main main.o matrix.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o