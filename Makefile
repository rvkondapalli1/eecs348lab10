CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
TARGET = lab10

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:r
	rm -f $(TARGET)

.PHONY: all clean
