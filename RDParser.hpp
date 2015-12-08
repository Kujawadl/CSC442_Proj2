/*

Dylan Jager-Kujawa
Matt Richardson

Dr. Strader
CSC 442
8 December 2015

Blocks World

RDParser.cpp

*/

#ifndef RDPARSER
#define RDPARSER

#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <locale>
#include <sstream>
#include "LAScanner.hpp"

class RDParser {

public:
	RDParser(std::string fileName);
	void printInput();
	void printErrors();
	void printWorld();
	void print();
private:
	LAScanner scan;
	std::stack<std::string>* world;
	int x, y, arm_x, arm_y;
	std::string arm_val;
	std::vector<std::string> errList;
	bool errors;
	void error(std::string message, int lineNumber);
	void program();
	void declarations();
	int* coordinate();
	int* arm();
	std::string id();
	void actions();
	void action();
};

#endif