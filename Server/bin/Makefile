GTEST= ~/googletest
CFLAGS= -Wall -g
LDFLAGS= -isystem $(GTEST)$(GTEST)/include -pthread
LIBS= $(GTEST)/libgtest.a
SOURCES= ../src/main.cpp ../src/CWorkQueue.cpp
INC= ../inc 
tests= ../tests
CXX= g++
NAME= Server

OBJECTS= $(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CXX) -c $(CFLags) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(NAME)
