all: RDParser

LAScanner: LAScanner.hpp LAScanner.cpp
	g++ -g -c LAScanner.cpp

RDParser: LAScanner RDParser.hpp RDParser.cpp
	g++ -g -o blocks LAScanner.cpp RDParser.cpp