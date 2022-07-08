# Project 2: Command Line Shell (v 1.0)

This project mimicks the outermost part of an operating system, the shell. The shell prints a prompt and waits for a user input.
The shell runs commands in a current directory and follows pathes, as well. 

See https://www.cs.usfca.edu/~mmalensek/cs326/assignments/project-2.html

## Building

To compile and run:

```bash
make
./crash
```

## Program Options

```bash
$ ./crash

Options:
    * cd              Change directory
    * ls              List contents
```

## Included Files

* **shell.c** -- Contains the main method, handles commands
* **ui.c** -- Handles UI functionality
* **leetify.c** -- Handles piping

## Testing

To execute the test cases, use `make test`. To pull in updated test cases, run `make testupdate`. You can also run a specific test case instead of all of them:

```
# Run all test cases:
make test

# Run a specific test case:
make test run=4

# Run a few specific test cases (4, 8, and 12 in this case):
make test run='4 8 12'

# Run a test case in gdb:
make test run=4 debug=on
```

If you are satisfied with the state of your program, you can also run the test cases on the grading machine. Check your changes into your project repository and then run:

```
make grade
```

## Demo Run

```
[heshimek@heidi P2-heidishimek]$ ./crash
ls
crash	   leetify.c	logger.h   shell.c	 ui.c	 util.h
history.c  leetify.h	Makefile   shell.o	 ui.h	 util.o
...(more)...

cd tests

ls
 01-Command-Execution-1.sh     '15-Pipes-&-Redirection-1.sh'
 02-Scripting-1.sh	        16-SIGINT-handler-1.sh
...(more)...
 
cd ..

ls
crash	   leetify.c	logger.h   shell.c	 ui.c	 util.h
history.c  leetify.h	Makefile   shell.o	 ui.h	 util.o
...(more)...

ls -l
total 196
...(more)...

exit
[heshimek@heidi P2-heidishimek]$ 
```
