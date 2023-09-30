/*****************************************************
 * Copyright Grégory Mounié 2008-2015                *
 *           Simon Nieuviarts 2002-2009              *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wordexp.h>
#include <bsd/string.h>

#include "variante.h"
#include "readcmd.h"

#ifndef VARIANTE
#error "Variante non défini !!"
#endif

/* Guile (1.8 and 2.0) is auto-detected by cmake */
/* To disable Scheme interpreter (Guile support), comment the
 * following lines.  You may also have to comment related pkg-config
 * lines in CMakeLists.txt.
 */

#if USE_GUILE == 1
#include <libguile.h>

int exec_command(char **command, int input, int output, int is_background, bool is_pipe);
int exec_line(struct cmdline *l);

int question6_executer(char *line)
{
	/* Question 6: Insert your code to execute the command line
	 * identically to the standard execution scheme:
	 * parsecmd, then fork+execvp, for a single command.
	 * pipe and i/o redirection are not required.
	 */
	struct cmdline *l = parsecmd(&line);
	if (exec_line(l) == EXIT_FAILURE)
	{
		exit(EXIT_FAILURE);
	}
	return 0;
}

SCM executer_wrapper(SCM x)
{
	return scm_from_int(question6_executer(scm_to_locale_stringn(x, 0)));
}
#endif

struct background_command
{
	int pid;
	char *command;
	struct background_command *next;
};

struct background_command *head = NULL;

void jobs()
{
	struct background_command *current_cell = head;
	int status;
	while (current_cell != NULL)
	{
		printf("Pid of sleeping command : %i, sleeping command : %s \n", current_cell->pid, current_cell->command);
		printf("waitpid %i \n", waitpid(current_cell->pid, &status, WNOHANG));
		if (waitpid(current_cell->pid, &status, WNOHANG) == 0)
		{
			printf("Process pid : %i, command launched : %s, current status : UNFINISHED \n", current_cell->pid, current_cell->command);
		}
		else
		{
			printf("Process pid : %i, command launched : %s, current status : FINISHED \n", current_cell->pid, current_cell->command);
		}
		current_cell = current_cell->next;
	}
}

void add_to_background(int pid, char *command)
{
	struct background_command *new_command = malloc(sizeof(struct background_command));
	new_command->pid = pid;
	new_command->command = malloc(sizeof(char) * strlen(command));
	strcpy(new_command->command, command);

	if (head == NULL)
	{
		head = new_command;
		head->next = NULL;
	}
	else
	{
		struct background_command *temp = head;
		new_command->next = temp;
		head = new_command;
	}
}

void free_background()
{
	struct background_command *current = head;
	struct background_command *lag = current;
	if (current != NULL)
	{
		free(current->command);
		current = current->next;
		free(lag);
		lag = current;
	}
}

int exec_line(struct cmdline *l)
{
	int nb_cmd = 0;
	for (int i = 0; l->seq[i] != NULL; i++)
	{
		nb_cmd++;
	}
	int list_process[nb_cmd];
	int fd[nb_cmd - 1][2]; // array of n arrays of pipes

	int input = (l->in) ? open(l->in, O_RDONLY) : -2;
	if (input == -1)
	{
		perror("Input redirection failed on open");
		return (EXIT_FAILURE);
	}
	int output;
	bool is_pipe = false;

	for (int i = 0; i < nb_cmd; i++)
	{
		// compute command, is_background, input, output
		char **command = l->seq[i];

		/* Check if a pipe is needed */
		if (l->seq[i + 1] != NULL)
		{
			is_pipe = true;
			if (pipe(fd[i]) == -1)
			{
				perror("Pipe failed!");
				return (EXIT_FAILURE);
			}
		}
		output = (l->seq[i + 1] != NULL) ? fd[i][1] : ((l->out) ? open(l->out, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO) : -2);
		if (output == -1)
		{
			perror("Output direction failed on open");
			return (EXIT_FAILURE);
		}
		list_process[i] = exec_command(command, input, output, l->bg, is_pipe);
		if (list_process[i] == EXIT_FAILURE)
		{
			return (EXIT_FAILURE);
		}
		if (is_pipe) close(fd[i][1]);
		if (l->seq[i + 1] != NULL) 	input = fd[i][0];
	}

	for (int i = 0; i < nb_cmd; i++)
	{
		printf("List process %i, %i\n", i, list_process[i]);
		if (list_process[i] == EXIT_FAILURE)
		{
			exit(EXIT_FAILURE);
		}
		else if (list_process[i] != -3)
		{
			waitpid(list_process[i], NULL, 0);
		}
	}
	return EXIT_SUCCESS;
}

int exec_command(char **command, int input, int output, int is_background, bool is_pipe){
	/* Expansion with wordexp here*/
	wordexp_t result;
	if (wordexp(command[0], &result, 0) != 0)
	{
		printf("Expand failed for command %s : Command not recognised \n", command[0]);
		return EXIT_FAILURE;
	}
	for (int i = 1; command[i] != NULL; i++)
	{
		if (wordexp(command[i], &result, WRDE_APPEND) != 0)
		{
			printf("Expand failed for arguments %s: Syntax error! \n", command[i]);
			return EXIT_FAILURE;
		}
	}
	int pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (strcmp(result.we_wordv[0], "jobs") == 0)
		{
			jobs();
			return EXIT_SUCCESS;
		}
		if (input != -2)
		{
			if (dup2(input, STDIN_FILENO) == -1)
			{
				perror("dup2 failed for input");
				return (EXIT_FAILURE);
			}
			close(input);
		}
		if (output != -2)
		{
			if (!is_pipe)
				ftruncate(output, 0);
			if (dup2(output, STDOUT_FILENO) == -1)
			{
				perror("dup2 failed for output");
				return EXIT_FAILURE;
			}
			close(output);
		}
		if (execvp(result.we_wordv[0], result.we_wordv) == -1)
		{
			perror("Command not recognised");
			return (EXIT_FAILURE);
		}
		return EXIT_SUCCESS;
	}

	else
	{
		if (is_background == 1){
			printf("Pid to background : %i, command : %s\n", pid, result.we_wordv[0]);
			add_to_background(pid, result.we_wordv[0]);
			return -3;
		}
		return pid;
	}
	wordfree(&result);
}

void terminate(char *line)
{
#if USE_GNU_READLINE == 1
	/* rl_clear_history() does not exist yet in centOS 6 */
	clear_history();
#endif
	if (line)
		free(line);
	printf("exit\n");
	exit(0);
}

int main()
{
	printf("Variante %d: %s\n", VARIANTE, VARIANTE_STRING);

#if USE_GUILE == 1
	scm_init_guile();
	/* register "executer" function in scheme */
	scm_c_define_gsubr("executer", 1, 0, 0, executer_wrapper);
#endif
	while (1)
	{
		struct cmdline *l;
		char *line = 0;
		int i, j;
		char *prompt = "ensishell @ nguyenan >";

		/* Readline use some internal memory structure that
		   can not be cleaned at the end of the program. Thus
		   one memory leak per command seems unavoidable yet */
		line = readline(prompt);
		if (line == 0 || !strncmp(line, "exit", 4))
		{
			terminate(line);
		}

#if USE_GNU_READLINE == 1
		add_history(line);
#endif

#if USE_GUILE == 1
		/* The line is a scheme command */
		if (line[0] == '(')
		{
			char catchligne[strlen(line) + 256];
			sprintf(catchligne, "(catch #t (lambda () %s) (lambda (key . parameters) (display \"mauvaise expression/bug en scheme\n\")))", line);
			scm_eval_string(scm_from_locale_string(catchligne));
			free(line);
			continue;
		}
#endif

		/* parsecmd free line and set it up to 0 */
		l = parsecmd(&line); // returns struct cmd*

		/* If input stream closed, normal termination */
		if (!l)
		{
			terminate(0);
		}
		if (l->err)
		{
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		/* Display each command of the pipe */

		for (i = 0; l->seq[i] != 0; i++)
		{
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
			for (j = 0; cmd[j] != 0; j++)
			{
				printf("'%s' ", cmd[j]);
			}
			printf("\n");
		}
		if (exec_line(l) == EXIT_FAILURE)
		{
			exit(EXIT_FAILURE);
		}
	}
	free_background();
}
