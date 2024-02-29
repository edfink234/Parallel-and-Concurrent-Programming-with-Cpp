# Merge Sort Challenge Makefile
CXX		 = g++
CXXFLAGS = -Wall -g -std=c++17 -Xpreprocessor -fopenmp
TARGET	 = merge_sort_challenge
SRC		 = merge_sort_challenge.cpp
LDLIBS	 =
LDFLAGS = -I/opt/homebrew/opt/libomp/include/

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $(TARGET) $(LDFLAGS) -L/opt/homebrew/opt/libomp/lib -lomp

.PHONY: clean
clean:
	rm -f $(TARGET)
