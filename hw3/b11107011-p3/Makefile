CC = g++
CFLAGS = -Wall -O3 -march=native
SRCS = main.cpp my_struct.cpp function.cpp
OBJS = $(SRCS:.cpp=.o)

all : picRouting

picRouting: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

check:
	./pa3_resources/pic_routing_verification pa3_resources/pic5x5.in pic5x5.out
	./pa3_resources/pic_routing_verification pa3_resources/pic20x20.in pic20x20.out
	./pa3_resources/pic_routing_verification pa3_resources/pic60x60.in pic60x60.out

out:
	./picRouting pa3_resources/pic5x5.in pic5x5.out
	./picRouting pa3_resources/pic20x20.in pic20x20.out
	./picRouting pa3_resources/pic60x60.in pic60x60.out

clean:
	rm -f picRouting $(OBJS)