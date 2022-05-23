CXXFLAGS+=-std=c++14 -w -O2 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm
CXX=g++

all: out out/main.o

run: out out/main.o
	./out/main.o

out/main.o: prog/prog.cpp
	$(CXX) $^ $(CXXFLAGS) -o out/main.o

pack:
	zip solution.zip -r Makefile prog

out:
	mkdir -p out

clean:
	rm -rf out