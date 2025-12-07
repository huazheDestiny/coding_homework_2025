CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = loganalyzer
SOURCES = main.cpp LinkedList.cpp CircularQueue.cpp BST.cpp KMP.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean