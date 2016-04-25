# Operating Systems - Project 4A

Run the following command to compile and run the shell...

	make run

OR

	gcc -c myshell.c
	gcc myshell.o -o myshell
	./myshell


### Explanation of Shell
Use it just like you would any other shell, you have access to
all of the bash commands, as well as the few special ones
listed below...

- C *file1 file2* - Copy *file1* one into *file2*, without deleting *file1*
- D *file*        - Delete *file*
- E *comment*     - Display *comment* on screen
- H               - Display the user manual
- L               - List the contents of the current directory
- M *file*        - Modify *file* by opening a text editor (will create the file if it does not exist)
- P *file*        - Prints *file* to the screen
- Q               - Quit the shell
- S               - Launch the FireFox web browser
- W               - Clear screen
- X *program*     - Execute *program*


### Functions
Two functions were added to the original file given.

- displayHelp()
	- This function displays the user manual via printf's

- convertCommand()
	- This function converts the custom shell commands into the standard linux commands

### Resources
Linux Programmer's Manual
	- WAIT(2)
	- EXEC(3)
	- ERRNO(3)
