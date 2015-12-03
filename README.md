## To do:
 - [x] ~~Write Scanner~~
 - [ ] Add comment recognition to Scanner
 - [ ] Test Scanner
 - [ ] Write Parser
 - [ ] Test Parser
 - [ ] Write Interpreter (Project2.java)
 - [ ] Test sample programs/error checking

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