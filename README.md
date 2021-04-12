# CMPLR-P1 for CS 4280 @ UMSL - REUPLOAD TO GITHUB DUE TO MESSY CODE

IMPLEMENTATION 3
FSA Table is located inside the main directory with the entire program (fsa map.csv)
Driver function is token scan(std::istream& in, const int lineNum, const intcharNum)
	This is located in scanner.cpp
	fsa.h has the pairings of values { ASCII, tokenID},

How to Run:

make linclean:
cleans the left over objects, binaries, and compiled items in the directory
  
make:
  compiles the entire program

scanner *filename*:
  runs the program *filename* is the file you wish to run in this program
  NOTE: test.sp21 is included with the program for an example run
	!!!THE PROGRAM ONLY READS IN FILES WITH EXTENSION OF .SP21!!!
