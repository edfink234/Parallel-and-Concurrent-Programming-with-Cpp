# Download Images Challenge Makefile
CXX		 = g++
TARGET	 = download_images_challenge
SRC		 = download_images_challenge.cpp
LDLIBS	 = -lcurl

# Boost library path
BOOST_LIB_PATH = /opt/homebrew/Cellar/boost/1.84.0

# Boost include path
BOOST_INCLUDE = $(BOOST_LIB_PATH)/include

CXXFLAGS = -Wall -std=c++20 -I$(BOOST_INCLUDE)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
