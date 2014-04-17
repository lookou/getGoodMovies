PROJ_HOME := $(shell pwd)/..

INC_JSON := -I$(PROJ_HOME)/lib/jsoncpp/include
LIB_JSON := -L$(PROJ_HOME)/lib/jsoncpp/lib -ljson

INC_CURL := -I$(PROJ_HOME)/lib/curl/include
LIB_CURL := -L$(PROJ_HOME)/lib/curl/lib  -lcurl 

RM = /bin/rm -f
CP = /bin/cp

CC = g++
CPPFLAGS = -g   -Wall -D_REENTRANT

LIBDIR = $(LIB_JSON) $(LIB_CURL)  -ldl -lrt
INCL = $(INC_JSON)  $(INC_CURL)  -I../include

SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))

TARGET = ../lib/libhttpapi.a

all: $(TARGET)

$(TARGET): $(OBJS)
	ar -rc  $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $(INCL) $<

clean:
	$(RM) $(OBJS) $(TARGET)
