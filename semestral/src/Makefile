LD = 	g++
CXX = 	g++
LDFLAGS = -fsanitize=address
FLAGS = -std=c++17 -pedantic -Wall -fsanitize=address -g 


all: miner

miner: main.o map.o inventory.o tile.o item.o player.o game.o trap.o
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c -o $@ $<

clean:
	rm -f *.o miner