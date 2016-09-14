GTEST= ~/googletest
CFLAGS= -Wall -g -isystem $(GTEST)/googletest/include \
	-isystem $(GTEST)/googlemock/include \
	-isystem ../inc -isystem ../pub
 
LDFLAGS= -pthread 
LIBS= $(GTEST)/libgtest.a $(GTEST)/libgmock.a
SOURCES= ../src/main.cpp ../src/CWorkQueue.cpp ../src/CThPool.cpp ../src/CTaskTMP.cpp
INC= ../inc 
TESTS= ../tests/src/TWorkQueue.cpp
CXX= g++
NAME= Server
TNAME= Tname

OBJECTS= $(SOURCES:.cpp=.o)

OBJECTST= $(TESTS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(NAME)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

test:	$(TNAME)

$(TNAME): $(OBJECTST)
	$(CXX) $(LDFLAGS) $(OBJECTST) $(OBJECTS)  -o $(TNAME)

%.o: %.cpp 
	$(CXX) -c $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(NAME)
