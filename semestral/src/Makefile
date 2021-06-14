LD = 	g++
CXX = 	g++
LDFLAGS = -fsanitize=address -g
FLAGS = -std=c++17 -pedantic -Wall -fsanitize=address -g 


all: miner

miner: main.o map.o inventory.o tile.o player.o game.o entity.o bomb.o fairy.o teleport.o entityHandler.o
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c -o $@ $<

clean:
	rm -f *.o miner