all: RDParser

LAScanner: LAScanner.hpp LAScanner.cpp
	g++ --std=c++11 -g -c LAScanner.cpp

RDParser: LAScanner RDParser.hpp RDParser.cpp
	g++ --std=c++11 -g -o blocks LAScanner.cpp RDParser.cpp