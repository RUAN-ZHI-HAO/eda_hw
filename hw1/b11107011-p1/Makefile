CC = g++
CFLAGS = -Wall
SRCS = main.cpp myStruct.cpp gnuPlot.cpp
OBJS = $(SRCS:.cpp=.o)

all : genPlot

genPlot: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f genPlot $(OBJS)