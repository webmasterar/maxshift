ifdef SystemRoot
   RM = del /Q
   FixPath = $(subst /,\,$1)
   EXT = .exe
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      FixPath = $1
      EXT = 
   endif
endif

CC = g++
FLAGS = -O3 -msse4.2 -funroll-loops -lm -std=c++11

all: maxshift
	$(CC) $(FLAGS) -I . -o main$(EXT) main.o maxshift.o

maxshift:
	$(CC) $(FLAGS) -I . -c main.cc maxshift.cc

clean:
	$(RM) main.o maxshift.o main$(EXT)
