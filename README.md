# CSC 442 Project 2: Block World
### Dylan Jager-Kujawa and Matt Richardson
---

Project 2 serves to create and implement a language using a context-free grammar. The language is interpreted using three java programs: 
 - the scanner, which reads the input programs and returns a sequence of tokens
 - the parser, which reads the tokens, generates symbol and constant tables, an error table (if necessary), and returns an intermediate language, similar to assembly
 - the interpreter, which takes the intermediate code and runs through it line-by-line
If at any point an error is encountered, the current program stops and does not pass along its input to the next stage of compilation. It would not make sense to try and interpret code that has not been parsed properly; similarly, it would not make sense to parse a program containing unexpected/unhandled tokens.

## Contents:
 - [Status](#status)
 - [To do](#todo)
 - [Context-Free Grammar](#cfg)
 - [Sample Program](#program)
 - [Sample Output](#output)

<a name="status" />
## Status:
Currently, a roughly working version of the scanner has been written, although it is not the best looking code in terms of documentation and cosmetics. Not all features of the scanner have been implemented yet, however it has been tested and verified to return the given program's tokens correctly (it does not split single tokens into multiple or combine multiple tokens into one).

No work has been done on the parser or the interpreter to date.

<a name="todo" />
## To Do:
 - [x] ~~Write Scanner~~
 - [ ] Add comment recognition to Scanner
 - [ ] Test Scanner
 - [ ] Write Parser
 - [ ] Test Parser
 - [ ] Write Interpreter (Project2.java)
 - [ ] Test sample programs/error checking
 - [ ] Write/Maintain debug code throughout all classes!

<a name="cfg" />
## Context-Free Grammar:
    <program> ::= WORLD <variable>: BLOCKS { <declarations> }; MOVES [ <actions> ];
    <declarations> ::= {<variable>;}* <arm>;
    <actions> ::= <action>; {<action>;}*
    <variable> ::= <id> ( <coordinate> )
    <arm> ::= arm ( <coordinate> ) | arm ()
    <coordinate> ::= <location>, <location>
    <location> ::= <nonzero> {<digit>}*
    <id> ::= <alpha> {<alpha>|<digit>}*
    <alpha> ::= a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z
    <nonzero> ::= 1|2|3|4|5|6|7|8|9
    <digit> ::= 0|<nonzero>
    <action> ::= GRAB ( {<coordinate>|<id>} ) | UNSTACK ( {<coordinate>|<id>} ) | 
      MOVE ( <coordinate> ) | DROP | STACK | PRINT
    <eolcomment> ::= //
    <startcomment> ::= /*
    <endcomment> ::= */

<a name="program" />
## Sample Program:
    WORLD proj2(5,5):
    BLOCKS {
    	var1(1,1);
    	var2(1,2);
    	var3(2,4);
    	arm();
    };
    MOVES [
    	MOVE(1,1);
    GRAB(var1);
    	MOVE(2,2);
    	DROP;
    	MOVE(1,2);
    GRAB(var2);
    MOVE(2,2);
    STACK;
    	//Errors:
    	UNSTACK(var1); //ERROR: var 1 not at top of stack
    	MOVE(6,5);     //ERROR: (6,5) not located within proj2
    	DROP;          //ERROR: not holding anything
    ];

<a name="output" />
## Example output (from code above excluding errors):
    Initial position:
        # blocks / location:
               1  2  3  4  5
            1  1  1  _  _  _
            2  _  _  _  1  _
            3  _  _  _  _  _
            4  _  _  _  _  _
            5  _  _  _  _  _
        Location contents:
          (1,1):
              var1
          (1,2):
              var2
          (2,4):
              var3

    Final position:
        # blocks / location:
               1  2  3  4  5
            1  _  _  _  _  _
            2  _  2  _  1  _
            3  _  _  _  _  _
            4  _  _  _  _  _
            5  _  _  _  _  _
    Location contents:
        (2,2):
            STACK:
            [top]
                var2
                var1
            [bottom]
        (2,4):
            var3