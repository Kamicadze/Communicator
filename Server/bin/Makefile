GTEST= ~/googletest
CFLAGS= -Wall -g -isystem ../../Gincludes \
	-isystem ../inc -isystem ../pub -isystem ../tests/inc
 
LDFLAGS= -pthread 
LIBS= ../../Gincludes/libgtest.a ../../Gincludes/libgmock.a
SOURCES= ../src/main.cpp ../src/CWorkQueue.cpp ../src/CThPool.cpp ../src/CTaskTMP.cpp

INC= ../inc 
TESTS= ../tests/src/TWorkQueue.cpp
CXX= g++
NAME= Server
TNAME= Tname

OBJECTS= $(SOURCES:.cpp=.o)

OBJECTST= $(TESTS:.cpp=.o)

all: prog test 

prog: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(NAME)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

test:	$(TNAME)

$(TNAME): $(OBJECTST) 
	$(CXX) $(LDFLAGS) $(OBJECTST) ../src/CWorkQueue.o -o $(TNAME)

%.o: %.cpp 
	$(CXX) -c $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTST)
	rm -f $(NAME)
