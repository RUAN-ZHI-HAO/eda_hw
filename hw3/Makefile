CC = g++
CFLAGS = -Wall -O3 -march=native
SRCS = main.cpp myStruct.cpp function.cpp gnuPlot.cpp
OBJS = $(SRCS:.cpp=.o)

all : legalizer

legalizer: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f legalizer $(OBJS)