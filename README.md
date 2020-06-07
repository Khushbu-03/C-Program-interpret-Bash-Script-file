# C-Program-interpret-Bash-Script-file
C program that reads and interpret a script file that consists of Bash commands. Each line of the file may contain one or more commands, separated by ; useing Unix input/output system calls 

Basically, program is reading from the file, line by line. Then, for each command (and possible
arguments), program creates a child process that will exec to the command. The
parent process waits for its child to terminate before processing the next command.

Example of a script file used in program: myScript.txt:
clear
ls /usr/bin
date; more myScript.txt
wc -l myScript.txt
echo Good bye
