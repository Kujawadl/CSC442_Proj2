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
	void print();
private:
	LAScanner scan;
	std::stack<std::string>* world;
	int x, y, arm_x, arm_y;
	std::vector<std::string> errList;
	bool errors;
	void error(std::string message, int lineNumber);
	void program();
	void declarations();
	void actions();
	int* coordinate();
	int* arm();
	std::string id();
	void action();
};

#endif