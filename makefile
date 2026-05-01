CXX = g++
CXXFLAGS = -Wall -lssl -lcrypto -g -std=c++20

BUILDDIR = build
APPLYDIR = application

#	$(BUILDDIR)/conv.o
#	$(BUILDDIR)/main.o
#	$(BUILDDIR)/reactor.o
OBJS = \
	$(BUILDDIR)/netpack.o

TARGET = execute

all: netpack 

main: main.cpp
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	@if [ ! -d $(APPLYDIR) ]; then mkdir -p $(APPLYDIR); fi
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(BUILDDIR)/main.o

conv: conv.cpp
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	@if [ ! -d $(APPLYDIR) ]; then mkdir -p $(APPLYDIR); fi
	$(CXX) $(CXXFLAGS) -c conv.cpp -o $(BUILDDIR)/conv.o

reactor: reactor.cpp
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	@if [ ! -d $(APPLYDIR) ]; then mkdir -p $(APPLYDIR); fi
	$(CXX) $(CXXFLAGS) -c reactor.cpp -o $(BUILDDIR)/reactor.o

netpack: netpack.cpp
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	@if [ ! -d $(APPLYDIR) ]; then mkdir -p $(APPLYDIR); fi
	$(CXX) $(CXXFLAGS) -c netpack.cpp -o $(BUILDDIR)/netpack.o

clean:
	rm -f $(OBJS)

upver:
	rm -f $(APPLYDIR)/$(TARGET)
	$(CXX) $(CXXFLAGS) -o $(APPLYDIR)/$(TARGET) $(OBJS)

