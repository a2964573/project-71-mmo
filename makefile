CXX = g++
CXXFLAGS = -Wall -lssl -lcrypto -g

BUILDDIR = build
APPLYDIR = application

OBJS =  $(BUILDDIR)/main.o \

TARGET = Uniqueptr

all: main

main: main.cpp
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	@if [ ! -d $(APPLYDIR) ]; then mkdir -p $(APPLYDIR); fi
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(BUILDDIR)/main.o

clean:
	rm -f $(OBJS)

upver:
	rm -f $(APPLYDIR)/$(TARGET)
	$(CXX) $(CXXFLAGS) -o $(APPLYDIR)/$(TARGET) $(OBJS)

