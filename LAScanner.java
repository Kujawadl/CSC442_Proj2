/*

Dylan Jager-Kujawa
Matt Richardson

Dr. Strader
CSC 442
8 December 2015

Blocks World

LAScanner.java
Implements the scanner, which reads input character by character and outputs a list of 
tokens as defined by the grammar in the handout.

*/

import java.io.*;

public class LAScanner {

	private String charQueue;	// The input file as a string
	private int lineCount;		// The current line number.
	private int pos;					// The current index position when traversing charQueue.
	boolean currVar;					// Is the token currently being read a <variable>?

	// Constructor
	LAScanner(String fileName) {
		// Initialize variables
		charQueue = "";
		lineCount = 1;
		pos = 0;
		currVar = false;

		try {
			FileReader fileIn;
			int in;

			// Open file, read all characters into inputFile, close file.
			fileIn = new FileReader(fileName);
			while ((in = fileIn.read()) != -1)
				charQueue += (char)in;
			fileIn.close();
		} catch (FileNotFoundException ex) {
			System.out.println("File \"" + fileName + "\" does not exist!");
			System.exit(1);
		// Catch any generic I/O exception, print stack and exit with code 1.
		} catch (IOException ex) {
			ex.printStackTrace();
			System.exit(1);
		}
	}

	// Return the original program as a string.
	public String getInputTable() {
		return charQueue;
	}

	public int getLineNumber() {
		return lineCount;
	}

	// Peek at the next token without dequeueing it.
	public String peek() {
		int tmp = pos;
		String retVal = nextToken();
		try {
			// If next token is newline, decrement lineCount to offset effects
			// of nextToken() and return "".
			if (charQueue.charAt(tmp) == '\n')
				lineCount--;
		} catch (IndexOutOfBoundsException ex) {
			return "";
		}
		// Reset the position pointer.
		pos = tmp;
		return retVal;
	}

	// Get the next token in the file and dequeue it.
	public String nextToken() {
		char curr = '\u0000';
		char next = '\u0000';
		String retVal = "";

		// Get the current character; if out of bounds, return empty string.
		try {
			curr = charQueue.charAt(pos);
		} catch (IndexOutOfBoundsException ex) {
			return "";
		}
		// Get next character; if out of bounds, increment pos.
		try {
			next = charQueue.charAt(pos + 1);
		} catch (IndexOutOfBoundsException ex) {
			pos++;
		}
		pos++;

		// Handle whitespace.
		if (Character.isWhitespace(curr)) {
			if (curr == '\n')
				lineCount++;	// If new line, increment lineCount.
			return nextToken();
		// Handle letters.
		} else if (Character.isLetter(curr)) {
			// Current token must be some kind of variable (contains letters/digits).
			currVar = true;
			try {
				// If next char is letter or digit, keep going.
				if (Character.isLetterOrDigit(next)) {
					retVal += (curr + nextToken());
				// If next char is whitespace or symbol, end of token, return curr.
				} else if (Character.isWhitespace(next) || isSymbol(next)) {
					retVal += curr;
				// Unexpected character. Add to current token and end current token.
				} else {
					retVal += curr + next;
				}
			} finally { 
				// Always reset currVar to false.
				currVar = false;
			}
		// Handle digits
		} else if (Character.isDigit(curr)) {
			// If next char is digit or if current token is a variable and next char is letter, keep going.
			if (Character.isDigit(next) || (Character.isLetter(next) && currVar)) {
				retVal += (curr + nextToken());
			// If next char is whitespace, end of token, return curr
			} else if (Character.isWhitespace(next) || isSymbol(next)) {
				retVal += curr;
			// Unexpected character. Add to current token and end current token.
			} else {
				retVal += curr + next;
			}
		// Handle all other characters
		} else {
			retVal += curr;
		}
		return retVal;
	}

	// Check for recognized symbols
	private boolean isSymbol(char c) {
		switch (c) {
			case '>': case '<': case '+': case '-':
			case '/': case '*': case ',': case ';':
			case '{': case '}': case '(': case ')':
			case '[': case ']': case '=': case ':':
				return true;
			default:
				return false;
		}
	}
}