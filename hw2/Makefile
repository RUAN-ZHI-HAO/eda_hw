CC = g++
CFLAGS = -Wall
SRCS = main.cpp myStruct.cpp 
OBJS = $(SRCS:.cpp=.o)

all : legalizer

legalizer: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f legalizer $(OBJS)