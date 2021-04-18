# CMPLR-P3 for CS 4280 @ UMSL

How to Run:

make clean:
	clears up any left over binaries and object files of an older run

make allcean:
	removes the output file "frontEnd" and object files

make:
  compiles the entire program

frontEnd *filename*:
  runs the program *filename* is the file you wish to run in this program
  NOTE: sample tests are given, they are labeled with .sp21 extensions
	!!!THE PROGRAM ONLY READS IN FILES WITH EXTENSION OF .sp21!!!
	
Known Errors:
	some tokens are not kept in vector and are being overwritten. Still fixing
