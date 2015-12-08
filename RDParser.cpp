/*

Dylan Jager-Kujawa
Matt Richardson

Dr. Strader
CSC 442
8 December 2015

Blocks World

LAScanner.cpp
Implements the scanner, which reads input character by character and outputs a list of 
tokens as defined by the grammar in the handout.

*/

#define DEBUG
//undef DEBUG

#include "RDParser.hpp"

RDParser::RDParser(std::string fileName) {
	#ifdef DEBUG
	std::cerr << "In Parser constructor with filename " << fileName << std::endl;
	#endif

	scan = LAScanner(fileName);
	errors = false;

	program();

	#ifdef DEBUG
	std::cerr << "Leaving Parser constructor" << std::endl << std::endl;
	#endif
}

void RDParser::error(std::string message, int lineNumber) {
	#ifdef DEBUG
	std::cerr << "Entering error function with message: \"" << message << "\" at line " << lineNumber << std::endl;
	#endif

	do {
		scan.nextToken();
	} while (scan.peek().compare(";") != 0 || scan.peek().compare("]") != 0 \
		|| scan.peek().compare("}") != 0 || scan.peek().compare("") != 0);
	errList.insert(errList.end(), "Line " + std::to_string(lineNumber) + ": " + message);
	errors = true;

	#ifdef DEBUG
	std::cerr << "Leaving error function. Next token is " << scan.peek() \
		<< " at line " << scan.getLineNumber() << std::endl;
	#endif
}

void RDParser::printInput() {
	std::cout << "Input file:" << std::endl;

	std::stringstream ss(scan.getInputTable().c_str());
	std::string tmp = "";
	int line = 1;
	// Print the file, appending the line number to the front of each line.
	if (scan.getInputTable().c_str() != NULL) {
		while (std::getline(ss, tmp, '\n')) {
			std::cout << "\t" << std::setw(3) << line << " | " << tmp << std::endl;
			line++;
		}
	}
}

void RDParser::printErrors() {

}

void RDParser::print() {

}

void RDParser::program() {
	#ifdef DEBUG
	std::cerr << "Entering <program>" << std::endl;
	std::cerr << "Starting world declaration:" << std::endl;
	#endif

	// Always get current line number before popping
	int line = scan.getLineNumber();
	// Expect "WORLD"
	if (scan.nextToken().compare("WORLD") != 0)
		error("Expected WORLD", line);
	// Expect a variable
	id();
	int* dim = coordinate();
	// Expect :
	if (scan.nextToken().compare(":") != 0)
		error("Expected :", line);
	// Check validity of dimensions
	if (*dim != -1) {
		x = dim[0];
		y = dim[1];
		world = new std::stack<std::string> [x*y];
	// If getting world dimensions failed, print errors and exit
	} else {
		printErrors();
		exit(1);
	}

	#ifdef DEBUG
	std::cerr << "Ending world declaration" << std::endl << std::endl;
	std::cerr << "Starting block declarations" << std::endl;
	#endif

	// Expect declarations
	declarations();

	#ifdef DEBUG
	std::cerr << "Leaving block declarations" << std::endl << std::endl;
	std::cerr << "Starting actions block" << std::endl;
	#endif

	// Expect actions; actions() returns stack machine
	actions();

	#ifdef DEBUG
	std::cerr << "Leaving actions block" << std::endl << std::endl;
	#endif
}

void RDParser::declarations() {
	int line = scan.getLineNumber();
	// Expect BLOCKS
	if (scan.nextToken().compare("BLOCKS") != 0)
		error("Expected BLOCKS", line);
	line = scan.getLineNumber();
	// Expect {
	if (scan.nextToken().compare("{") != 0)
		error("Expected {", line);
	// While next token is not arm
	while (scan.peek().compare("arm") != 0) {
		line = scan.getLineNumber();
		// Get name
		std::string name = id();
		// Get location
		int* loc = coordinate();
		// Expect ;
		if (scan.nextToken().compare(";") != 0)
			error("Expected ;", line);
		// Add name to world at location, assuming within range
		if (*loc != -1 && loc[0]*loc[1] <= x*y) {
			world[(loc[0] - 1) + (loc[1] - 1) * y].push(name);
		} else if (*loc == -1) {
			error("Expected coordinate", line);
		} else {
			error("Invalid coordinate", line);
		}
		// If next token is }, error (arm not declared)
		if (scan.peek().compare("}") == 0 || scan.peek().compare("") == 0) 
			error("Expected arm declaration", scan.getLineNumber());
	}
	// Expect arm
	if (scan.nextToken().compare("arm") != 0) 
		error("Expected arm declaration)", scan.getLineNumber());
	// Get arm location
	int* armloc = arm();
	// If armloc is -1, set armloc to default (1,1)
	if (*armloc == -1) {
		armloc[0] = 1;
		armloc[1] = 1;
	}
	arm_x = armloc[0];
	arm_y = armloc[1];
	// Expect ;
	if (scan.nextToken().compare(";") != 0)
		error("Expected ;", scan.getLineNumber());
	// Expect }
	if (scan.nextToken().compare("}") != 0)
		error("Expected }", scan.getLineNumber());
	// Expect ;
	if (scan.nextToken().compare(";") != 0)
		error("Expected ;", scan.getLineNumber());
}

void RDParser::actions() {

}

// Expects a coordinate value; returns 2-element array containing {x, y}.
// Returns -1 on error.
int* RDParser::coordinate() {
	int* dim = new int[2];
	int line = scan.getLineNumber();

	#ifdef DEBUG
	std::cerr << "Entering coordinate()" << std::endl;
	#endif

	try {
		// Expect (
		if (scan.nextToken().compare("(") != 0)
			error("Expected (", line);
		// Try and get x
		else
			dim[0] = stoi(scan.nextToken());
		#ifdef DEBUG
		std::cerr << "Got X value: " << dim[0] << std::endl;
		#endif

		// Expect ,
		if (scan.nextToken().compare(",") != 0)
			error ("Expected ,", line);
		// Try and get y
		else 
			dim[1] = stoi(scan.nextToken());
		#ifdef DEBUG
		std::cerr << "Got Y value: " << dim[1] << std::endl;
		#endif

		// Expect ):
		if (scan.nextToken().compare(")") != 0)
			error ("Expected )", line);
	} catch (std::exception &e) {
		error("Expected integer within coordinate declaration", line);
		// On error, return -1
		dim[0] = -1;
		dim[1] = -1;
	}

	#ifdef DEBUG
	std::cerr << "Returning (" << dim[0] << "," << dim[1] << ") from coordinate()" << std::endl;
	#endif

	return dim;
}

// Expects a coordinate value or empty parentheses; returns 2-element array containing {x, y}.
// Returns -1 on error.
int* RDParser::arm() {
	int* dim = new int[2];
	int line = scan.getLineNumber();

	#ifdef DEBUG
	std::cerr << "Entering arm()" << std::endl;
	#endif

	try {
		// Expect (
		if (scan.nextToken().compare("(") != 0)
			error("Expected (", line);
		if (scan.peek().compare(")") == 0) {
			#ifdef DEBUG
			std::cerr << "Arm declared using default value (1,1)" << std::endl;
			#endif
			scan.nextToken();
			dim[0] = -1;
			dim[1] = -1;
			return dim;
		// Try and get x
		} else
			dim[0] = stoi(scan.nextToken());
		#ifdef DEBUG
		std::cerr << "Got X value: " << dim[0] << std::endl;
		#endif

		// Expect ,
		if (scan.nextToken().compare(",") != 0)
			error ("Expected ,", line);
		// Try and get y
		else 
			dim[1] = stoi(scan.nextToken());
		#ifdef DEBUG
		std::cerr << "Got Y value: " << dim[1] << std::endl;
		#endif

		// Expect ):
		if (scan.nextToken().compare(")") != 0)
			error ("Expected )", line);
	} catch (std::exception &e) {
		error("Expected integer within arm declaration", line);
		// On error, return -1
		dim[0] = -1;
		dim[1] = -1;
	}

	#ifdef DEBUG
	std::cerr << "Returning (" << dim[0] << "," << dim[1] << ") from arm()" << std::endl;
	#endif

	return dim;
}

std::string RDParser::id() {
	int line = scan.getLineNumber();
	// Get next token as c_string
	const char* name = scan.nextToken().c_str();

	#ifdef DEBUG
	std::cerr << "Entering id() with token " << name << std::endl;
	#endif

	int i = 0;
	// For each character in string
	while (name[i]) {
		// If first character is not a lowercase letter, error
		if (i == 0 && !std::islower(name[i], std::locale())) {
			error("Variable id expected; must begin with lowercase letter", line);
			return "";
		// If any character is not lowercase letter or digit, error
		} else if (!std::islower(name[i], std::locale()) && !std::isdigit(name[i], std::locale())) {
			error("Variable id expected; must contain only lowercase letters and digits", line);
			return "";
		}
		i++;
	}

	#ifdef DEBUG
	std::cerr << "Token passed id check; returning." << std::endl;
	#endif

	// If no errors, return name
	return name;
}

void RDParser::action() {

}

int main (int c, char** v) {
	RDParser parser("input");
	parser.printInput();
}