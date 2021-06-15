#with help from David Bernhauer
CXXFLAGS = -Wall -pedantic -std=c++14

HEADERS = $(wildcard src/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

.PHONY: all
all: compile doc

.PHONY: run
run: compile
	./jaklvinc

.PHONY: compile
compile: jaklvinc

jaklvinc: build/main
	cp $< $@

build/main: $(OBJECTS)
	mkdir -p $(@D)
	g++ $^ -o $@ -lstdc++fs

build/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $< -c -o $@

doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf jaklvinc build/ doc/ 2>/dev/null


build/bomb.o: src/bomb.cpp src/bomb.h src/entity.h src/player.h src/inventory.h \
 src/stone.h src/item.h src/iron.h src/gold.h src/diamond.h src/coral.h \
 src/bone.h src/map.h src/tile.h
build/bone.o: src/bone.cpp src/bone.h src/item.h
build/coral.o: src/coral.cpp src/coral.h src/item.h
build/diamond.o: src/diamond.cpp src/diamond.h src/item.h
build/entity.o: src/entity.cpp src/entity.h src/player.h src/inventory.h \
 src/stone.h src/item.h src/iron.h src/gold.h src/diamond.h src/coral.h \
 src/bone.h src/map.h src/tile.h
build/entityHandler.o: src/entityHandler.cpp src/entityHandler.h src/fairy.h \
 src/entity.h src/player.h src/inventory.h src/stone.h src/item.h \
 src/iron.h src/gold.h src/diamond.h src/coral.h src/bone.h src/map.h \
 src/tile.h src/bomb.h src/teleport.h
build/fairy.o: src/fairy.cpp src/fairy.h src/entity.h src/player.h \
 src/inventory.h src/stone.h src/item.h src/iron.h src/gold.h \
 src/diamond.h src/coral.h src/bone.h src/map.h src/tile.h
build/game.o: src/game.cpp src/game.h src/entityHandler.h src/fairy.h \
 src/entity.h src/player.h src/inventory.h src/stone.h src/item.h \
 src/iron.h src/gold.h src/diamond.h src/coral.h src/bone.h src/map.h \
 src/tile.h src/bomb.h src/teleport.h src/enum.h
build/gold.o: src/gold.cpp src/gold.h src/item.h
build/inventory.o: src/inventory.cpp src/inventory.h src/stone.h src/item.h \
 src/iron.h src/gold.h src/diamond.h src/coral.h src/bone.h src/enum.h
build/iron.o: src/iron.cpp src/iron.h src/item.h
build/item.o: src/item.cpp src/item.h
build/main.o: src/main.cpp src/game.h src/entityHandler.h src/fairy.h \
 src/entity.h src/player.h src/inventory.h src/stone.h src/item.h \
 src/iron.h src/gold.h src/diamond.h src/coral.h src/bone.h src/map.h \
 src/tile.h src/bomb.h src/teleport.h
build/map.o: src/map.cpp src/map.h src/tile.h src/enum.h
build/player.o: src/player.cpp src/player.h src/inventory.h src/stone.h \
 src/item.h src/iron.h src/gold.h src/diamond.h src/coral.h src/bone.h \
 src/map.h src/tile.h src/enum.h
build/stone.o: src/stone.cpp src/stone.h src/item.h
build/teleport.o: src/teleport.cpp src/teleport.h src/entity.h src/player.h \
 src/inventory.h src/stone.h src/item.h src/iron.h src/gold.h \
 src/diamond.h src/coral.h src/bone.h src/map.h src/tile.h
build/tile.o: src/tile.cpp src/tile.h src/enum.h
