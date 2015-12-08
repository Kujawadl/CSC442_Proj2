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
#undef DEBUG

#include "RDParser.hpp"

RDParser::RDParser(std::string fileName) {
	#ifdef DEBUG
	std::cerr << "In Parser constructor with filename " << fileName << std::endl;
	#endif

	scan = LAScanner(fileName);
	errors = false;
	arm_val = "";

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
		<< " at line " << scan.getLineNumber() << std::endl << std::endl;
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
	std::cout << std::endl;
}

void RDParser::printErrors() {
	#ifdef DEBUG
	if (!errors) {
		errors = true;
		errList.insert(errList.end(), "No errors generated.");
		errList.insert(errList.end(), "These messages test printErrors() functionality");
		errList.insert(errList.end(), "when no real errors are present.");
	}
	#endif

	std::cout << "Errors: " << std::endl;
	if (!errors)
		std::cout << "\t(none)" << std::endl;
	else 
		for (int i = 0; i < errList.size(); i++)
			std::cout << "\t" << errList[i] << std::endl;
	std::cout << std::endl;
}

void RDParser::printWorld() {
	#ifdef DEBUG
	std::cerr << "Entering printWorld(). Adding two elements to stack at (1,1)" << std::endl;
	world[0].push("debug1");
	world[0].push("debug2");
	#endif

	// Print out a diagram of the grid, representing the # of blocks per space
	std::cout << "# blocks in each position:" << std::endl;
	// For each row
	for (int i = 0; i <= y; i++) {
		std::cout << "    ";
		// For each column
		for (int j = 0; j <= x; j++) {
			// If upper-left corner, print nothing
			if (i == 0 && j == 0)
				std::cout << "   ";
			// If column header, print column number
			else if (i == 0)
				std::cout << std::setw(3) << j;
			// If row header, print row number
			else if (j == 0)
				std::cout << std::setw(3) << i;
			// Otherwise, print stack size. If 0, print "-"
			else {
				int num = world[(j - 1) + (i - 1) * x].size();
				if (num == 0) 
					std::cout << std::setw(3) << "-";
				else
					std::cout << std::setw(3) << num;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// Print out the contents of any nonempty spaces
	std::cout << "Location contents:" << std::endl;
	// For each row
	for (int i = 0; i <= y; i++) {
		// For each column
		for (int j = 0; j <= x; j++) {
			std::stack<std::string> current = world[(j - 1) + (i - 1) * x];
			// If only one element, just print that element
			if (current.size() == 1) {
				std::cout << "    " << "(" << j << "," << i << "):" << std::endl;
				std::cout << "\t" << current.top() << std::endl;
			// If stack, print all elements in a stack format
			} else if (current.size() > 1) {
				std::cout << "    " << "(" << j << "," << i << "):" << std::endl;
				std::cout << "\t" << "[top]" << std::endl;
				std::stack<std::string> reverse; // Reverse of the current stack
				// For every element in stack
				while (current.size() > 0) {
					// Print the element
					std::cout << "\t    " << current.top() << std::endl;
					// Add the element to the reverse stack
					reverse.push(current.top());
					// Pop
					current.pop();
				}
				std::cout << "\t" << "[bottom]" << std::endl;
				// Restore current stack to original state
				// For each element in reverse stack
				while (reverse.size() > 0) {
					// Add to current stack
					current.push(reverse.top());
					// Pop
					reverse.pop();
				}
			}
		}
	}
	std::cout << std::endl;

	#ifdef DEBUG
	std::cerr << "Leaving printWorld()" << std::endl << std::endl;
	#endif
}

void RDParser::print() {
	printInput();
	printErrors();
	printWorld();
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
		if (*loc != -1 && loc[0] <= x && loc[1] <= y) {
			world[(loc[0] - 1) + (loc[1] - 1) * x].push(name);
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
	std::cerr << "Returning (" << dim[0] << "," << dim[1] << ") from coordinate()" << std::endl << std::endl;
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
	std::cerr << "Returning (" << dim[0] << "," << dim[1] << ") from arm()" << std::endl << std::endl;
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
	std::cerr << "Token passed id check; returning." << std::endl << std::endl;
	#endif

	// If no errors, return name
	return name;
}

void RDParser::actions() {
	#ifdef DEBUG
	std::cerr << "Entering actions()" << std::endl;
	#endif

	int line = scan.getLineNumber();
	if (scan.nextToken().compare("MOVES") != 0)
		error("Expected MOVES", line);
	line = scan.getLineNumber();
	if (scan.nextToken().compare("[") != 0)
		error("Expected [", line);
	while (scan.peek().compare("]") != 0) {
		line = scan.getLineNumber();
		action();
		if (scan.nextToken().compare(";") != 0)
			error("Expected ;", line);
	}
	scan.nextToken();
	line = scan.getLineNumber();
	if (scan.nextToken().compare(";") != 0)
		error("Expected ;", line);

	#ifdef DEBUG
	std::cerr << "Leaving actions()" << std::endl;
	#endif
}

void RDParser::action() {
	/* Possible actions:
		GRAB
		UNSTACK
		MOVE
		DROP
		STACK
		PRINT
	*/
	int line = scan.getLineNumber();
	std::string fn = scan.nextToken();

	#ifdef DEBUG
	std::cerr << "In action() with fn = " << fn << " at line " << line << std::endl;
	#endif

	if (fn.compare("GRAB") == 0) {
		#ifdef DEBUG
		std::cerr << "Entering GRAB checks; arm_val=" << arm_val << ", loc=(" << arm_x << "," \
			<< arm_y << ")" << std::endl;
		#endif
		if (scan.nextToken().compare("(") != 0)
			error("Expected (", line);
		std::string name = id();
		if (scan.nextToken().compare(")") != 0)
			error("Expected )", line);
		if (arm_val.compare("") != 0)
			error("Attempt to unstack while arm is full", line);
		else if (world[(arm_x - 1) + (arm_y - 1) * x].size() == 0)
			error("Attempt to grab from an empty space", line);
		else if (world[(arm_x - 1) + (arm_y - 1) * x].size() > 1)
			error("Attempt to grab from a stack", line);
		else if (world[(arm_x - 1) + (arm_y - 1) * x].top().compare(name) != 0)
			error("Attempt to unstack element not at specified location", line);
		else {
			arm_val = name;
			world[(arm_x - 1) + (arm_y - 1) * x].pop();
		}
	} else if (fn.compare("UNSTACK") == 0) {
		#ifdef DEBUG
		std::cerr << "Entering UNSTACK checks; arm_val=" << arm_val << ", loc=(" << arm_x << "," \
			<< arm_y << ")" << std::endl;
		#endif
		if (scan.nextToken().compare("(") != 0)
			error("Expected (", line);
		std::string name = id();
		if (scan.nextToken().compare(")") != 0)
			error("Expected )", line);
		if (arm_val.compare("") != 0)
			error("Attempt to unstack while arm is full", line);
		else if (world[(arm_x - 1) + (arm_y - 1) * x].size() <= 1)
			error("Attempt to unstack from a non-stack space", line);
		else if (world[(arm_x - 1) + (arm_y - 1) * x].top().compare(name) != 0)
			error("Attempt to unstack element not at top of stack", line);
		else {
			arm_val = name;
			world[(arm_x - 1) + (arm_y - 1) * x].pop();
		}
	} else if (fn.compare("MOVE") == 0) {
		#ifdef DEBUG
		std::cerr << "Entering MOVE checks; arm_val=" << arm_val << ", loc=(" << arm_x << "," \
			<< arm_y << ")" << std::endl;
		#endif
		int* new_loc = coordinate();
		if (*new_loc != -1 && new_loc[0] <= x && new_loc[1] <= y) {
			arm_x = new_loc[0];
			arm_y = new_loc[1];
		} else {
			error("Attempt to move to invalid coordinate", line);
		}
	} else if (fn.compare("DROP") == 0) {
		#ifdef DEBUG
		std::cerr << "Entering DROP checks; arm_val=" << arm_val << ", loc=(" << arm_x << "," \
			<< arm_y << ")" << std::endl;
		#endif
		if (arm_val.compare("") == 0)
			error("Attempt to drop while arm is empty", line);
		else if (world[(arm_x - 1) + (arm_y - 1) * x].size() > 1)
			error("Attempt to drop on a stack", line);
		else {
			world[(arm_x - 1) + (arm_y - 1) * x].push(arm_val);
			arm_val = "";
		}
	} else if (fn.compare("STACK") == 0) {
		#ifdef DEBUG
		std::cerr << "Entering STACK checks; arm_val=" << arm_val << ", loc=(" << arm_x << "," \
			<< arm_y << ")" << std::endl;
		#endif
		if (arm_val.compare("") == 0)
			error("Attempt to stack while arm is empty", line);
		else if (world[(arm_x - 1) + (arm_y - 1) * x].size() == 0)
			error("Attempt to stack on an empty space", line);
		else {
			world[(arm_x - 1) + (arm_y - 1) * x].push(arm_val);
			arm_val = "";
		}
	} else if (fn.compare("PRINT") == 0) {
		std::cout << "World's state as of line " << line << ":" << std::endl;
		printWorld();
	}
}

int main (int c, char** v) {
	RDParser parser("input");
	parser.print();
}