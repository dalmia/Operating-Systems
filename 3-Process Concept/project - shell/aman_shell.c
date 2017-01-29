#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef int bool;

#define true 1
#define false 0
#define LSH_RL_BUFSIZE 1024 /* buffer size for reading user input */
#define LSH_TOK_BUFSIZE 64 /* buffer size for splitting the arguments */
#define LSH_HIST_SIZE 10 /* buffer size for storing history of commands */
#define LSH_TOK_DELIM " \t\r\n\a" /* delimiters for parsing the arguments */

/* global variable to check parent and child process concurrency */
bool conc = false;
/* global variable to point to the last command executed */
int cur_pos = -1;
/* global variable storing the history of commands executed */
char *history[LSH_HIST_SIZE];
int cur_bufsize = LSH_TOK_BUFSIZE;

/* Function declarations for built-in shell commands */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/* List of built-in commands, followed by their corresponding functions */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit
};

int lsh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

/* Built-in function implementations */
int lsh_cd(char **args)
{
    if(args[1] == NULL){
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }else{
        if(chdir(args[1]) != 0){
            perror("lsh");
        }
    }

    return 1;
}

int lsh_help(char **args)
{
    int i;
    printf("Aman Dalmia's LSH\n");
    printf("Type program names and arguments, and press enter.\n");
    printf("Append \"&\" after the arguments for concurrency between parent-child process.\n");
    printf("The following are built in:\n");

    for(i = 0; i < lsh_num_builtins(); i++){
        printf(" %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lsh_exit(char **args)
{
    return 0;
}

/* Launch a program */
int lsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){ /* child process */
        if(execvp(args[0], args) == -1) perror("lsh");
        exit(EXIT_FAILURE);
    }else if(pid > 0){ /* parent process */
        if(!conc){
            do{
                wpid =  waitpid(pid, &status, WUNTRACED);
            }while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }else{ /* error forking */
        perror("lsh");
    }

    conc = false;
    return 1;
}

/* Parse input to get the arguments */
char **lsh_split_line(char *line){
    cur_bufsize = LSH_TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(cur_bufsize * sizeof(char*));
    char *token;

    if(!tokens){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= cur_bufsize){
            cur_bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, cur_bufsize * sizeof(char*));
            if(!tokens){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    if(position > 0 && strcmp(tokens[position - 1], "&") == 0) {
        conc = true;
        tokens[position - 1] = NULL;
    }
    tokens[position] = NULL;
    return tokens;
}

/* History of commands */
int lsh_history(char **args)
{
    if(cur_pos == -1 || history[cur_pos] == NULL){
        fprintf(stderr, "No commands in history\n");
        exit(EXIT_FAILURE);
    }

    if(strcmp(args[0], "history") == 0){
        int last_pos = 0, position = cur_pos, count = 0;

        if(cur_pos != LSH_HIST_SIZE && history[cur_pos + 1] != NULL){
            last_pos = cur_pos + 1;
        }

        count = (cur_pos - last_pos + LSH_HIST_SIZE) % LSH_HIST_SIZE + 1;

        while(count > 0){
            char *command = history[position];
            printf("%d %s\n", count, command);
            position = position - 1;
            position = (position + LSH_HIST_SIZE) % LSH_HIST_SIZE;
            count --;
        }
    }else{
        char **cmd_args;
        char *command;
        if(strcmp(args[0], "!!") == 0){
            command = malloc(sizeof(history[cur_pos]));
            strcat(command, history[cur_pos]);
            printf("%s\n", command);
            cmd_args = lsh_split_line(command);
            int i;
            for (i = 0; i < lsh_num_builtins(); i++){
                if(strcmp(cmd_args[0], builtin_str[i]) == 0){
                    return (*builtin_func[i])(cmd_args);
                }
            }
            return lsh_launch(cmd_args);
        }else if(args[0][0] == '!'){
            if(args[0][1] == '\0'){
                fprintf(stderr, "Expected arguments for \"!\"\n");
                exit(EXIT_FAILURE);
            }
            /* position of the command to execute */
            int offset = args[0][1] - '0';
            int next_pos = (cur_pos + 1) % LSH_HIST_SIZE;
            if(next_pos != 0 && history[cur_pos + 1] != NULL){
                offset = (cur_pos + offset) % LSH_HIST_SIZE;
            }else{
                offset--;
            }
            if(history[offset] == NULL){
                fprintf(stderr, "No such command in history\n");
                exit(EXIT_FAILURE);
            }
            command = malloc(sizeof(history[cur_pos]));
            strcat(command, history[offset]);
            cmd_args = lsh_split_line(command);
            int i;
            for (i = 0; i < lsh_num_builtins(); i++){
                if(strcmp(cmd_args[0], builtin_str[i]) == 0){
                    return (*builtin_func[i])(cmd_args);
                }
            }
            return lsh_launch(cmd_args);
        }else{
            perror("lsh");
        }
    }
}

/* Execute the parsed arguments */
int lsh_execute(char *line){
    int i;
    //printf("%s\n", line);

    char **args = lsh_split_line(line);

    if(args[0] == NULL){ /* empty command was entered */
        return 1;
    }else if(strcmp(args[0], "history") == 0 ||
             strcmp(args[0], "!!") == 0 || args[0][0] == '!'){
        return lsh_history(args);
    }

    cur_pos = (cur_pos + 1) % LSH_HIST_SIZE;
    history[cur_pos] = malloc(cur_bufsize * sizeof(char));
    char **temp_args = args;
    int count=0;

    while(*temp_args != NULL){
        strcat(history[cur_pos], *temp_args);
        strcat(history[cur_pos], " ");
        temp_args++;
    }
    //printf("%s\n", history[cur_pos]);
    //history[count] = '\0';
    if(cur_pos > 0)
    printf("Inserted %s\n", history[cur_pos-1]);

    for (i = 0; i < lsh_num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}

/* Read input from stdin */
char *lsh_read_line(void)
{
    cur_bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * cur_bufsize);
    int c;

    if(!buffer){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        /* Read a character */
        c = getchar();

        if(c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        }else{
            buffer[position] = c;
        }
        position++;

        /* If buffer exceeded, reallocate buffer */
        if(position >= cur_bufsize){
            cur_bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, cur_bufsize);
            if(!buffer){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/* Loop for getting input and executing it */
void lsh_loop(void)
{
    char *line;
    int status;

    do {
        printf(">");
        line = lsh_read_line();
        status = lsh_execute(line);

        free(line);
    } while(status);
}

int main(void)
{
    lsh_loop();

    return EXIT_SUCCESS;
}
