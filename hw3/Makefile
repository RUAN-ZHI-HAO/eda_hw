CC = g++
CFLAGS = -Wall -O3 -march=native
SRCS = main.cpp my_struct.cpp function.cpp gnu_plot.cpp
OBJS = $(SRCS:.cpp=.o)

all : picRouting

picRouting: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f picRouting $(OBJS)