## Problem Statement

Shell: Write a C program to implement the command line shell that does the
following:
- A prompt should be displayed upon starting the execution of a program and
it should allow the user to type basic commands one after the other. The
program must support at least any 6 commands such as ls, cd, mkdir,
rmdir, rm, cp, mv, etc.
- For each of the user’s commands entered, a separate child process should be
created to execute the command and display the output if any.
Note: Execution should be terminated only when the user types the command exit
on prompt.

### Instructions

Follow the steps to get the code up and running!

- Compile the file protoShell.c using the following command
                        `gcc protoShell.c -o protoShell.exe -lreadline`
- Run the protoShell.exe file thus created-
                        `./protoshell.exe`
- Use `help` command to know the available commands
- Use `exit` to close the terminal and stop the shell.

### Requirements
- Readline Library
- A Linux Based Operating System
