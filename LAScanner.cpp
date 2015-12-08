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

#include "LAScanner.hpp"

#define DEBUG
//undef DEBUG

LAScanner::LAScanner(std::string fileName) {
	#ifdef DEBUG
	std::cerr << "Entering LAScanner constructor." << std::endl;
	#endif

	// Initialize variables
	charQueue = "";
	lineCount = 1;
	pos = 0;
	currVar = false;
	std::ifstream fileIn;

	try {
		// Attempt to open file for reading.
		fileIn.open(fileName, std::ifstream::in);
		char in;

		// If file not found, throw exception.
		if (!fileIn) 
			throw FileNotFoundException();

		// Read all characters and close input file.
		while (fileIn.get(in)) 
			charQueue += in;
		fileIn.close();
	} catch (FileNotFoundException& e) {
		std::cerr << "File \"" + fileName + "\" does not exist!";
		exit (EXIT_FAILURE);
	} catch (std::exception& e) {
		std::cerr << "Caught unhandled exception." << std::endl << e.what();
		fileIn.close();
		exit (EXIT_FAILURE);
	}

	#ifdef DEBUG
	std::cerr << "Successfully read file. File contents: " << std::endl \
		<< charQueue << std::endl;
	std::cerr << "Leaving LAScanner constructor." << std::endl;
	#endif
}

LAScanner::LAScanner() {}

// Return the original program as a string.
std::string LAScanner::getInputTable() {
	#ifdef DEBUG
	std::cerr << "In getInputTable(): charQueue is - " << std::endl \
		<< charQueue << std::endl << "Leaving getInputTable()" << std::endl;
	#endif
	return charQueue;
}

// Return the current line number
int LAScanner::getLineNumber() {
	#ifdef DEBUG
	std::cerr << "In getLineNumber(): lineCount is " << lineCount << std::endl \
		<< "Leaving getLineNumber()" << std::endl;
	#endif
	return lineCount;
}

// Peek at the next token without dequeueing it.
std::string LAScanner::peek() {
	int tmp = pos;
	int line_backup = lineCount;
	std::string retVal = nextToken();
	#ifdef DEBUG
	std::cerr << "In peek()" << std::endl;
	std::cerr << "nextToken is " << retVal << std::endl;
	#endif
	// Reset the position pointer.
	pos = tmp;
	// Reset lineCount
	lineCount = line_backup;
	#ifdef DEBUG
	std::cerr << "Leaving peek()" << std::endl;
	#endif
	return retVal;
}

std::string LAScanner::nextToken() {
	#ifdef DEBUG
	std::cerr << "Entering nextToken()." << std::endl;
	#endif
	char curr = '\0';
	char next = '\0';
	std::string retVal = "";

	// Get the current character; if out of bounds, return empty string.
	try {
		curr = charQueue.at(pos);
	} catch (std::out_of_range& e) {
		#ifdef DEBUG
		std::cerr << "nextToken() attempted to access char out of  " \
			" range while getting current character." << std::endl;
		#endif
		return "";
	}
	// Get next character; if out of bounds, increment pos.
	try {
		next = charQueue.at(pos + 1);
	} catch (std::out_of_range& e) {
		#ifdef DEBUG
		std::cerr << "nextToken() attempted to access char out of " \
			" range while getting next character." << std::endl;
		#endif
		pos++;
	}
	pos++;
	#ifdef DEBUG
	std::cerr << "Successfully retrieved curr and next: " << curr << ", & " \
		<< next << ", respectively." << std::endl;
	#endif

	// Handle one-line comments.
	if (curr == '/' && next == '/') {
		try {
			// Set pos to position of next newline character
			while (charQueue.at(pos) != '\n')
				pos++;
			// Continue finding tokens
			return nextToken();
		} catch (std::out_of_range& e) {
			#ifdef DEBUG
			std::cerr << "nextToken() out of bounds while looking for \\n" << std::endl;
			#endif
			pos++;
		}
	// Handle multi-line comments.
	} else if (curr == '/' && next == '*') {
		try {
			// Set pos to position of the * in next */
			while (charQueue.at(pos) != '*' || charQueue.at(pos + 1) != '/') {
				if (charQueue.at(pos) == '\n') {
					lineCount++;
					#ifdef DEBUG
					std::cerr << "\t\tINCREMENTING LINECOUNT TO " << lineCount << std::endl;
					#endif
				}
				pos++;
			}
			// Skip over */
			pos += 2;
			// Continue finding tokens
			return nextToken();
		} catch (std::out_of_range& e) {
			#ifdef DEBUG
			std::cerr << "nextToken() out of bounds while looking for */" << std::endl;
			#endif
			pos++;
		}
	// Handle whitespace.
	} else if (isspace(curr)) {
		if (curr == '\n') {
			lineCount++;
			#ifdef DEBUG
			std::cerr << "\t\tINCREMENTING LINECOUNT TO " << lineCount << std::endl;
			#endif
		}
		return nextToken();
	// Handle letters
	} else if (isalpha(curr)) {
		// Current token must be some kind of variable (contains letters/digits).
		currVar = true;
		// If next char is alphanumeric, keep going.
		if (isalnum(next)) {
			retVal += curr + nextToken();
		// If next char is whitespace or symbol, end of token, return.
		} else if (isspace(next) || isSymbol(next)) {
			retVal += curr;
		// Unexpected character. Add to current token and end current token.
		} else {
			retVal += curr;
			retVal += next;
			pos++;
			#ifdef DEBUG
			std::cerr << "Undefined character: " << next << std::endl;
			#endif
		}
		// Always reset currVar to false.
		currVar = false;
	// Handle digits
	} else if (isdigit(curr)) {
		// If next char is digit or if curr token is var and next char is alpha, keep going.
		if (isdigit(next) || (isalpha(next) && currVar)) {
			retVal += curr;
			retVal += nextToken();
		// If next char is whitespace or symbol, end of token, return curr.
		} else if (isspace(next) || isSymbol(next)) {
			retVal += curr;
		// Unexpected character. Add to current token and end current token.
		} else {
			retVal += curr;
			retVal += next;
			pos++;
			#ifdef DEBUG
			std::cerr << "Undefined character: " << next << std::endl;
			#endif
		}
	// Handle all other characters
	} else {
		retVal += curr;
		#ifdef DEBUG
		std::cerr << "Undefined character: " << curr << std::endl;
		#endif
	}
	#ifdef DEBUG
	std::cerr << "Leaving nextToken(); returning token " << retVal << "." << std::endl;
	#endif
	return retVal;
}

bool LAScanner::isSymbol(char c) {
	#ifdef DEBUG
	std::cerr << "Entering isSymbol()" << std::endl;
	#endif
	switch (c) {
		case '>': case '<': case '+': case '-':
		case '/': case '*': case ',': case ';':
		case '{': case '}': case '(': case ')':
		case '[': case ']': case '=': case ':':
			#ifdef DEBUG
			std::cerr << "Returning true; character is: " << c << std::endl;
			#endif
			return true;
		default:
			#ifdef DEBUG
			std::cerr << "Returning false; character is: " << c << std::endl;
			#endif
			return false;
	}
}

#undef DEBUG
#ifdef DEBUG
int main (int c, char** v) {
	LAScanner scanner = LAScanner("input");
	while (scanner.peek().compare("") != 0)
		std::cout << scanner.nextToken() << std::endl;
}
#endif