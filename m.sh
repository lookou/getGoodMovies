#!/bin/bash
g++ getGoodMovie.cpp -I./include -I./lib/jsoncpp/include -L./lib/ -lhttpapi -L./lib/curl/lib/ -lcurl -L./lib/jsoncpp/lib/ -ljson -lrt
