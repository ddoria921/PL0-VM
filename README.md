PL0-VM
======

A tiny PL/0 virtual machine implemented in C. 


BEFORE COMPILING
=================
Make sure that you have the following files in the same directory before compiling the program. They are necessary for the program to run properly. The files will work if they are blank (Keep in mind that input.txt should contain the PL0 code).
	
Files:
	input.txt
	mcode.txt
	stacktrace.txt

If the files are not in the directory you can create them by running following commands from the command line:
	
	touch input.txt
	touch mcode.txt
	touch stacktrace.txt


To compile
==============
Run the following commands to compile the dependencies:

	gcc scanner.c -o scanner
	gcc parser.c -o parser
	gcc vm.c -o vm

Now run the following command to compile the main file:

	gcc main.c -o main


Running
==============
To run the program type:

	./main

Pass in any appropriate parameters as so:
	
	./main –l –a –v
	./main –v	


Input
==============
Any input being tested should be in a file named "input.txt", this file will contain the PL0 code.
It will look something like this:
var x, y;
begin
	y := 3;
	x := y + 56;
end.


Output
==============
The output will be shown on screen depending on the command line paramaters passed into the main.c program. If there are no paramaters passed into the program, then the output from the scanner will be in "lexemelist.txt", the output from the parser will be in "mcode.txt", and the ouput from the virtual machine will be in "stacktrace.txt".

