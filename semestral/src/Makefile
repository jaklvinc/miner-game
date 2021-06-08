LD = 	g++
CXX = 	g++
LDFLAGS = -fsanitize=address -lncurses
FLAGS = -std=c++17 -pedantic -Wall -fsanitize=address -g 


all: game

game: main.o map.o entity.o inventory.o tile.o item.o player.o
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c -o $@ $<

clean:
	rm -f *.o miner