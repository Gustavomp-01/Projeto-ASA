CXX = g++
CXXFLAGS = -pedantic -Wall -std=c++11 -O3

projeto: projeto.cpp
	$(CXX) $(CXXFLAGS) projeto.cpp -o projeto -lm

clean:
	rm -f projeto