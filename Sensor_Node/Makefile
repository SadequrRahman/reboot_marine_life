CXX = g++
CXXFLAG = -Wall -Werror -o3
CXXLD = -lpthread -lrt ##-lwiringPi
TARGET := app


SRCS := $(wildcard *.cpp)
INCS := $(wildcard *.h)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))


all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAG) $(CXXLD) -o $@ $^
	
%.o: %.cpp
	$(CXX) $(CXXFLAG) $(CXXLD) -c -o $@ $^
	
clean:
	rm -rf $(TARGET) *.o
	
