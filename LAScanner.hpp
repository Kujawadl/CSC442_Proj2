#ifndef LASCANNER
#define LASCANNER

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>

class LAScanner {
	public:
		LAScanner(std::string fileName);
		std::string getInputTable();
		int getLineNumber();
		std::string peek();
		std::string nextToken();
	private:
		bool isSymbol(char c);
		std::string charQueue;	// The input file as a string.
		int lineCount;		// The current line number.
		int pos;			// The current index position when traversing charQueue.
		bool currVar;		// Is the token currently being read a <variable> token?
};

#endif

#ifndef FILENOTFOUNDEXCEPTION
#define FILENOTFOUNDEXCEPTION
class FileNotFoundException : std::exception { public: FileNotFoundException() {} };
#endif