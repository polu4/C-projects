/* 
Author: Luke Poley adapted from  Jeff Ondich's shell.h
12 Novmeber 2025 
shell.c: help, command execution, > redirection, and | pipes
*/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define COMMAND_BUFFER_SIZE       102
#define MAX_COMMAND_SIZE          (COMMAND_BUFFER_SIZE - 2)

#define COMMAND_INPUT_SUCCEEDED   0
#define COMMAND_INPUT_FAILED      1
#define COMMAND_END_OF_FILE       2
#define COMMAND_TOO_LONG          3

#define MAX_ARGS                  32

int get_command(char *command_buffer, int buffer_size);
void execute_command(char *command_line);

int parse_command(char *cmd, char **argv);
int find_token(char **argv, char *token);

int main() {
    const char *prompt = "shell208> ";
    char command_line[COMMAND_BUFFER_SIZE];

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        int result = get_command(command_line, COMMAND_BUFFER_SIZE);
        if (result == COMMAND_END_OF_FILE) {
            break;
        } else if (result == COMMAND_INPUT_FAILED) {
            fprintf(stderr, "There was a problem reading your command. Please try again.\n");
            break;
        } else if (result == COMMAND_TOO_LONG) {
            fprintf(stderr, "Commands are limited to length %d. Please try again.\n", MAX_COMMAND_SIZE);
        } else {
            execute_command(command_line);
        }
    }

    return 0;
}

int get_command(char *command_buffer, int buffer_size) {
    assert(buffer_size > 0);
    assert(command_buffer != NULL);

    if (fgets(command_buffer, buffer_size, stdin) == NULL) {
        if (feof(stdin)) {
            return COMMAND_END_OF_FILE;
        } else {
            return COMMAND_INPUT_FAILED;
        }
    }

    int command_length = strlen(command_buffer);
    if (command_buffer[command_length - 1] != '\n') {
        char ch = getchar();
        while (ch != '\n' && ch != EOF) {
            ch = getchar();
        }
        return COMMAND_TOO_LONG;
    }

    command_buffer[command_length - 1] = '\0';
    return COMMAND_INPUT_SUCCEEDED;
}

int parse_command(char *cmd, char **argv) {
    int argc = 0;
    char *token = strtok(cmd, " \t");
    while (token && argc < MAX_ARGS - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }
    argv[argc] = NULL;
    return argc;
}

int find_token(char **argv, char *token) {
    for (int i = 0; argv[i]; ++i) {
        if (strcmp(argv[i], token) == 0) return i;
    }
    return -1;
}

void execute_command(char *command_line) {
    // ignore blank lines
    while (*command_line == ' ' || *command_line == '\t') command_line++;
    if (*command_line == 0) return;

    // built-in help
    if (strcmp(command_line, "help") == 0) {
        printf("shell208 supports:\n");
        printf("  help\n");
        printf("  command [args]\n");
        printf("  command > file\n");
        printf("  cmd1 | cmd2\n");
        return;
    }

    char linebuf[COMMAND_BUFFER_SIZE];
    strncpy(linebuf, command_line, COMMAND_BUFFER_SIZE - 1);
    linebuf[COMMAND_BUFFER_SIZE - 1] = 0;

    // check for pipe
    char *pipe_loc = strchr(linebuf, '|');
    if (pipe_loc) {
        *pipe_loc = '\0';
        char *left = linebuf;
        char *right = pipe_loc + 1;
        while (*left == ' ' || *left == '\t') left++;
        while (*right == ' ' || *right == '\t') right++;

        char *argv_left[MAX_ARGS], *argv_right[MAX_ARGS];
        parse_command(left, argv_left);
        parse_command(right, argv_right);

        int pipefd[2];
        pipe(pipefd);

        if (fork() == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            execvp(argv_left[0], argv_left);
            exit(1);
        }
        if (fork() == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execvp(argv_right[0], argv_right);
            exit(1);
        }
        close(pipefd[0]);
        close(pipefd[1]);
        wait(NULL);
        wait(NULL);
        return;
    }

    // check for >
    char *argv[MAX_ARGS];
    parse_command(linebuf, argv);
    int redirect_idx = find_token(argv, ">");
    if (redirect_idx >= 0) {
        argv[redirect_idx] = NULL;
        int fd = open(argv[redirect_idx+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fork() == 0) {
            dup2(fd, STDOUT_FILENO);
            close(fd);
            execvp(argv[0], argv);
            exit(1);
        }
        close(fd);
        wait(NULL);
        return;
    }

    // normal command
    if (fork() == 0) {
        execvp(argv[0], argv);
        exit(1);
    }
    wait(NULL);
}
