CFLAGS= -std=c++11 -Wall -g  -isystem ../Common \
	-isystem inc -isystem pub -Wpedantic
LDFLAGS= -pthread 
LIBS= ../Common/libgtest.a ../Common/libgmock.a
SOURCES= src/main.cpp src/CConnectionHandler.cpp \
	src/CMenu.cpp src/Globals.cpp
S4T= src/CWorkQueue.cpp
INC= inc 
TESTS= tests/src/TWorkQueue.cpp
CXX= g++
NAME= bin/client
TNAME= bin/Tests

OBJECTS= $(SOURCES:.cpp=.o)

OBJECTST= $(TESTS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(NAME)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

test:	$(TNAME)

$(TNAME):
	$(CXX) $(LDFLAGS) $(TESTS) $(S4T) $(CFLAGS) $(LIBS) -o $(TNAME)


clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTST)
	rm -f $(NAME)
	rm -f $(TNAME)
