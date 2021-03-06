// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:___________Michael Weyandt___________ Date:_____4/19/2016_____

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


#define MAX_ARGS	64
#define MAX_ARG_LEN	16
#define MAX_LINE_LEN	80
#define WHITESPACE	" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void convertCommand(struct command_t *);
void displayHelp();

int main(int argc, char *argv[]) {
   int pid;
   int pid2;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;
   int bg = 0;
   int err;

   //...
   /* Shell initialization */
   //...

   while (1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;
      /* Convert custom commands into standards */
      convertCommand(&command);
      /* check if process should be ran in the background 
	 and convert accordingly */
      if (*(command.name + (strlen(command.name) - 1)) == '&') {
        int size = strlen(command.name) - 1;
        char* new_command;
        new_command = (char *) malloc(size);
        strncpy(new_command,command.name,size);
        free(command.name);
        command.name = new_command;
        bg = 1;
      }
      else
        bg = 0;
      
      /* fork the process */
      if ((pid = fork()) == 0) {

        if(strcmp(command.name,"") == 0)
          return 0;

        else if (strcmp(command.name,"H") == 0) {
          displayHelp();
	  return 0;
        }

        else if (strcmp(command.name,"L") == 0) {
          if ((pid = fork()) == 0) {
	    execl("/bin/pwd", "pwd", (char *) NULL);
          }

          waitpid(pid, &status, 0);
          printf("\n");
          execl("/bin/ls", "ls", "-l", (char *) NULL);

	  return 0;
        }
        else {
        int status = execvp(command.name, command.argv);
        if (status == -1)
	  printf("ERROR: %s\n", strerror(errno));
          return 1;
        }
      }
      
      /* Wait for the child to terminate 
	 if it is meant to be a background 
 	 process, do not wait */
      if (bg)
        waitpid(pid, &status, WNOHANG);
      else
	waitpid(pid, &status, 0);

   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char* promptString = "linux(maw120)|>";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
    fgets(buffer, MAX_LINE_LEN, stdin);   
    
    if(*buffer == '\n')
      return;
}
/* End printPrompt and readCommand */

void convertCommand(struct command_t* command) {
  if (strcmp(command->name,"C") == 0) {
    free(command->name);
    command->name = (char *) malloc(sizeof("cp"));
    strcpy(command->name, "cp");
  }
  else if (strcmp(command->name,"D") == 0) {
    free(command->name);
    command->name = (char *) malloc(sizeof("rm"));
    strcpy(command->name, "rm");
  }
  else if (strcmp(command->name,"E") == 0) {
    free(command->name);
    command->name = (char *) malloc(sizeof("echo"));
    strcpy(command->name, "echo");
  }
  else if (strcmp(command->name,"M") == 0) {
    free(command->name);
    command->name = (char *) malloc(sizeof("nano"));
    strcpy(command->name, "nano");
  }
  else if (strcmp(command->name,"P") == 0) {
    free(command->name);
    command->name = (char *) malloc(sizeof("more"));
    strcpy(command->name, "more");
  }
  else if (strcmp(command->name,"Q") == 0) {
    exit(0);
  }
  else if (strcmp(command->name,"S") == 0) {
    free(command->name);
    command->name = (char *) malloc(sizeof("firefox&"));
    strcpy(command->name, "firefox&");
  }
  else if (strcmp(command->name,"W") == 0) {
    free(command->name);
    command->name = (char *) malloc(sizeof("clear"));
    strcpy(command->name, "clear");
  }
  else if (strcmp(command->name,"X") == 0) { 
    if (command->argc >= 2) {
      int index;
      free(command->name);  
      command->name = command->argv[1];
      for(index = 1; index < command->argc - 1; ++index)
        command->argv[index] = command->argv[index + 1];
    
      command->argv[index] = NULL;
    }
  }


  return;

}

void displayHelp() {

  printf("\nUser Manual:\n\n");
  printf("  C file1 file2\n");
  printf("    - Create file 2 copy all bytes of file1 to file2 without deleting file1.\n");
  printf("  D file\n");
  printf("    - Delete file.\n");
  printf("  E comment\n");
  printf("    - Display comment on screen followed by a newline.\n");
  printf("  H\n");
  printf("    - Display the user manual.\n");
  printf("  L\n");
  printf("    - List the current directory.\n");
  printf("  M file\n");
  printf("    - Create file by launching a text editor.\n");
  printf("  P file\n");
  printf("    - Display content of file on screen.\n");
  printf("  Q\n");
  printf("    - Quit the shell.\n");
  printf("  S\n");
  printf("    - Launch firefox to surf the web.\n");
  printf("  W\n");
  printf("    - Clear the screen.\n");
  printf("  X program");
  printf("    - Execute program.\n\n");

  return;
}
