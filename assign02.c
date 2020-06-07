

#include <stdlib.h>
#include <stdio.h>		
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int readLine (int, char *);
long int fileSize;

int main (int argc, char *argv[])
{
  char buffer[256];
  const char *del1 = ";";
  const char *del2 = " ";
  int fd, end, i = 0;
  char *token;
  char *parsed_token;
  int pid = 0;
  int status;
  char *arr_token[100];
  char *arr_parsed_token[100];
  int j = 0;
  int j1 = 0;
  int read_data = 1;

  if (argc != 2)
    {
      printf ("Call model: %s <inputFileName>\n", argv[0]);
      exit (0);
    }

  fd = open (argv[1], O_RDONLY);

  if (fd == -1)
    {
      perror ("File opening problem!");
      exit (1);
    }

  fileSize = lseek (fd, 0L, SEEK_END);
  lseek (fd, 0L, SEEK_SET);
  // printf("filesize=%d",fileSize);

  while (read_data != 0)
    {
      read_data = readLine (fd, buffer);
      printf ("\n\n==> Line Read : %s\n\n", buffer);
      token = strtok (buffer, del1);

     /***** Reading Line by Line and tokenizing the line using ';' delimiter to get command*****/
      while (token != NULL)
	{
	  arr_token[j] = malloc (strlen (token) * sizeof (char));
	  arr_token[j] = token;


	  token = strtok (NULL, del1);
	  //      printf("\n==Tokens :  %s==\n",arr_token[j]);

	  parsed_token = strtok (arr_token[j], del2);

		 /***** Reading each Command and tokenizing the it using ' ' delimiter *****/
	  while (parsed_token != NULL)
	    {
	      arr_parsed_token[j1] = malloc (strlen (parsed_token) * sizeof (char));

	      arr_parsed_token[j1] = parsed_token;
	      printf ("=== Token : %s ===", arr_parsed_token[j1]);
	      j1++;
	      parsed_token = strtok (NULL, del2);
	    }
	  arr_parsed_token[j1] = NULL;
	  printf ("\n");

	  pid = fork ();

	  if (pid == -1)
	    {
	      perror ("Impossible to fork");
	      exit (1);
	    }

	  if (pid > 0)
	    {			// Parent Process 
	      printf ("Parent pid=%u: child created... \n", getppid ());
	      wait (&status);

	      printf ("Parent: child terminated with status = %d\n", WEXITSTATUS (status));
	    }


	  if (pid == 0)
	    {			// Child Process
	      printf ("Child: pid=%u\n", getpid ());
	      execvp (arr_parsed_token[0], arr_parsed_token);
	      printf ("Could not execute the command\n"); // only executes if execvp fails
	      exit (10);
	    }

	  j++;
	  /**** Empty array of commands****/
	  memset (&arr_parsed_token[0], 0, sizeof (arr_parsed_token));
	  j1 = 0;

	}

    	/********* Reset everything **************/
      memset (&buffer[0], 0, sizeof (buffer));
      memset (&arr_parsed_token[0], 0, sizeof (arr_parsed_token));
      j1 = 0;

    }

  close (fd);
  return (0);
}


int readLine (int fd, char *line)
{
  int i = 0, end;
  static long int linesize = 0;
  while ((end = read (fd, &(line[i]), 1)) != 0)
    {
      if (line[i] == '\n')
	{
	  line[i] = '\0';
	  linesize++;
	  break;
	}
      else
	{
	  i++;
	  linesize++;
	}

    }
  if (end == -1)
    {
      perror ("Reading from input");
      return (1);
    }
  else
    {
      if (linesize == fileSize)
	{
	  line[i] = '\0';
	  i = 0;
	}

    }

  return i;
}
