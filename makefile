CXX = g++
CXXFLAGS = -Wall -lssl -lcrypto -g -std=c++20

BUILDDIR = build
APPLYDIR = application

OBJS = \
	$(BUILDDIR)/client.o
#	$(BUILDDIR)/class.o \
#	$(BUILDDIR)/main.o

TARGET = client

# all: class main
all: client

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

client: client.cpp
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	@if [ ! -d $(APPLYDIR) ]; then mkdir -p $(APPLYDIR); fi
	$(CXX) $(CXXFLAGS) -c client.cpp -o $(BUILDDIR)/client.o

class: class.cpp
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	@if [ ! -d $(APPLYDIR) ]; then mkdir -p $(APPLYDIR); fi
	$(CXX) $(CXXFLAGS) -c class.cpp -o $(BUILDDIR)/class.o

clean:
	rm -f $(OBJS)

upver:
	rm -f $(APPLYDIR)/$(TARGET)
	$(CXX) $(CXXFLAGS) -o $(APPLYDIR)/$(TARGET) $(OBJS)

