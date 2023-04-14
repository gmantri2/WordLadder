CXX=clang++
INCLUDES=-I includes/
CXX_FLAGS=-std=c++20 -g -O0 -Wall -Wextra -Werror -Iincludes/

exec: bin/main
tests: bin/tests

clean:
	rm -rf bin/*

bin/main: src/main.cc includes/word_graph.hpp includes/game.hpp src/word_graph.cc src/game.cc
	${CXX} ${CXX_FLAGS} src/main.cc src/word_graph.cc src/game.cc -o bin/exec

bin/tests: tests/tests.cc includes/word_graph.hpp includes/game.hpp src/word_graph.cc src/game.cc
	${CXX} ${CXX_FLAGS} tests/tests.cc src/word_graph.cc src/game.cc -o bin/tests

.DEFAULT_GOAL := exec
.PHONY: clean exec tests

# MAIN FUNCTION:
# make
# bin/exec

# TEST CASES: 
# make tests
# bin/tests