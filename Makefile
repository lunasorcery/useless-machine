useless-machine: main.cpp
	${CXX} main.cpp -o useless-machine -lstdc++ -std=c++17

clean:
	rm -f useless-machine

.PHONY: clean
