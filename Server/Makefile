CFLAGS= -std=c++11 -Wall -g  -isystem ../Common \
	-isystem inc -isystem pub -isystem tests/inc
LDFLAGS= -pthread 
LIBS= ../Common/libgtest.a ../Common/libgmock.a
SOURCES= src/main.cpp src/CWorkQueue.cpp src/CThPool.cpp \
	src/CDatabaseHandler.cpp src/CConnectionHandler.cpp \
	src/CMessageHandler.cpp
S4T= src/CWorkQueue.cpp
INC= inc 
TESTS= tests/src/TWorkQueue.cpp
CXX= g++
NAME= bin/server
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
