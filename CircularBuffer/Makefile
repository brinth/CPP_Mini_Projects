TARGET_APP=test_app

SOURCES :=$(wildcard *.cpp)
OBJS    :=$(patsubst %.cpp,%.o,$(SOURCES))

CC=gcc
CXX=g++


CPP_FLAGS  =-Wall 
CPP_FLAGS +=-I.
CPP_FLAGS +=-ggdb
CPP_FLAGS +=-O3

LDFLAGS  =-L.
LDFLAGS +=-lpthread

.PHONY: all, clean
all: $(TARGET_APP)
	@echo SOURCES : $(SOURCES)
	@echo OBJS    : $(OBJS)
	@echo APP     : $(TARGET_APP)
	@echo DONE

$(TARGET_APP): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

%.o : %.cpp
	$(CXX) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET_APP)