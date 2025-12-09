CXX = g++
# CXXFLAGS = -std=c++11 -Wall -O0 -gdwarf-2 检测内存溢出和栈溢出
CXXFLAGS = -std=c++11 -Wall -g 
TARGET = loganalyzer
SOURCES = main.cpp LinkedList.cpp CircularQueue.cpp BST.cpp KMP.cpp UndoStack.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	del /q $(TARGET).exe

.PHONY: clean