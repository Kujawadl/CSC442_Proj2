<!--
Quick overview of .md (markdown) syntax:
*text* = italics
**text** = bold
***text*** = bold italics
~~text~~ = strikethrough
#text = h1
##text = h2
###text = h3
... h5, h6
 - text = bulleted list
 1. text = numbered list
 - [ ] = checkbox list
> text = block quote
`text` = inline code snippet
```
text
``` = large code snippet
[text](link) = hyperlink

Markdown supports HTML code and comments as well.
-->

# CSC 442 Project 2: Block World
### Dylan Jager-Kujawa and Matt Richardson
---

Project 2 serves to create and implement a language using a context-free grammar. The language is interpreted using two programs: 
 - the scanner, which reads the input programs and returns a sequence of tokens
 - the parser, which reads the tokens, and runs the code line by line, generating an error list as it does so.

If at any point an error is encountered, the current program stops and does not pass along its input to the next stage of compilation. It would not make sense to try and interpret code that has not been parsed properly; similarly, it would not make sense to parse a program containing unexpected/unhandled tokens.

## Contents:
 - [How to Run](#how2)
 - [To Do](#todo)
 - [Context-Free Grammar](#cfg)
 - [Syntax Overview](#syntax)
  - [Comments](#comments)
  - [Program Layout](#layout)                                                                        
  - [Block Declarations](#block)
  - [Move Commands](#moves)
 - [Sample Program](#program)
 - [Sample Output](#output)

<a name="how2" />
## How to Run:
```
make
./blocks [filename]
```

<a name="todo" />
## To Do:
 - [x] ~~Write Scanner~~
 - [x] ~~Add comment recognition to Scanner~~
 - [ ] Test Scanner
 - [x] ~~Write Parser~~
  - [x] ~~Write WORLD section~~
  - [x] ~~Write BLOCKS section~~
  - [x] ~~Write MOVES section~~
 - [ ] Test Parser
 - [ ] Test sample programs/error checking
 - [x] ~~Fix line number function in Scanner~~
 - [ ] Write/Maintain debug code throughout all classes! (Ongoing)

<a name="cfg" />
## Context-Free Grammar:
```
<program> ::= WORLD <id><coordinate>: BLOCKS { <declarations> }; MOVES [ <actions> ];
<declarations> ::= {<variable>;}* <arm>;
<arm> ::= arm {<coordinate> | ()}
<coordinate> ::= ( <int>, <int> )
<id> ::= <alpha> {<alpha>|<digit>}*
<actions> ::= <action>; {<action>;}*
<action> ::= {GRAB|UNSTACK} ( <id> ) | MOVE <coordinate> | 
  DROP | STACK | PRINT
<alpha> ::= a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z
<nonzero> ::= 1|2|3|4|5|6|7|8|9
<digit> ::= 0|<nonzero>
<int> ::= <nonzero> {<digit>}*
<eolcomment> ::= //
<startcomment> ::= /*
<endcomment> ::= */
```

<a name="syntax" />
## Syntax Overview
<a name="comments"/>
### Comments
Block World uses c-style comments:  
```
// This is an inline comment.
/*
  This is a
  multi-line
  comment
*/
```  
Note that nested comments are not supported.

<a name="layout" />
### Program Layout
Block World programs are divided into three sections:
 1. The world declaration, wherein the world is named and given dimensions
 2. The block declaratons, wherein all block objects are declared and initialized
 3. The move commands, which control the robot arm to move blocks between locations.  
The structure is roughly as follows:  
```
WORLD id(#,#):
BLOCKS { ... };
MOVES [ ... ];
```
As you will see in the examples below, Blocks World uses the braces and punctuation to differentiate various parts of code; new lines are ignored. For example, the one-line `MOVES [ ... ];` statement above can be rewritten as  
```
MOVES [
...
];
``` 
or even
```
MOVES
[
...
];
```  
Similarly, tabs and indentation are ignored, and are purely for cosmetic purposes. By convention, all declarations and statements within `BLOCKS{};` or `MOVES[];` statements should be indented one tab, to indicate that they fall within that program block's domain.

<a name="block" />
### Block Declarations
All blocks are declared using an id, which is any string beginning with a letter, followed by one or more alphanumeric characters. Following the id is the initial location of the block, written as a coordinate, for example, `(1,2)`. Coordinates are numbered from 1 to the size of the specified dimension. Finally, the declaration ends with a semicolon. See the sample program below for examples.

The last declaration in the `BLOCKS` section must be `arm()`. Note that by default, arm starts at location `(1,1)`. However, an initial location for arm may also be declared by supplying a coordinate instead of the empty parentheses, e.g. `arm(2,3);`.

<a name="moves" />
### Move Commands
Block World supports the following actions:
 - MOVE ( coordinate );
 - GRAB ( id );
 - UNSTACK ( id );
 - DROP;
 - STACK;
 - PRINT;

Note that the Block World interpreter will automatically output the initial and final positions of all blocks, in addition to the relevant compilation output (the input file w/ line numbers, error table if any).

<a name="program" />
## Sample Program:
```
/**
Dylan Jager-Kujawa and Matt Richardson
Block World Sample Input
*/

WORLD proj2(5,6): // Declare world with dimensions 5x6
BLOCKS {
  var1(1,1);      // var1 declaration
  var2(1,2);      // var2 declaration
  var3(2,4);      // var3 declaration
  arm();          // Default arm location = (1,1)
};
MOVES [
  MOVE(1,1);      // Move arm to (1,1) (unnecessary)
  GRAB(var1);     // Pickup var1
  MOVE(2,2);      // Move arm to (2,2)
  DROP;           // Drop var1
  MOVE(1,2);      // Move arm to (1,2)
  GRAB(var2);     // Pickup var2
  MOVE(2,2);      // Move arm to (2,2)
  STACK;          // Stack var2 on var1
  /* 
  //Errors:
  //UNSTACK(var1);  //ERROR: var 1 not at top of stack
  //MOVE(6,5);      //ERROR: (6,5) not located within proj2
  //DROP;           //ERROR: not holding anything
  */
];
```

<a name="output" />
## Sample Output (from code above excluding errors):
```
Initial position:
    # blocks / location:
           1  2  3  4  5
        1  1  _  _  _  _
        2  1  _  _  _  _
        3  _  _  _  _  _
        4  _  1  _  _  _
        5  _  _  _  _  _
        6  _  _  _  _  _
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
        2  _  2  _  _  _
        3  _  _  _  _  _
        4  _  1  _  _  _
        5  _  _  _  _  _
        6  _  _  _  _  _
Location contents:
    (2,2):
        STACK:
        [top]
            var2
            var1
        [bottom]
    (2,4):
        var3
```