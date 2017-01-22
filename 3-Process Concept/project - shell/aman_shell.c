#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef int bool;

#define true 1
#define false 0
#define LSH_RL_BUFSIZE 1024 /* buffer size for reading user input */
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/* global variable to check parent and child process concurrency */
bool conc = false;

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
    printf("Type program names and arguments, and press enter\n");
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
int lsh_launch(char **args){
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

/* Execute the parsed arguments */
int lsh_execute(char **args){
    int i;

    if(args[0] == NULL){ /* empty command was entered */
        return 1;
    }

    for (i = 0; i < lsh_num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}

/* Parse input to get the arguments */
char **lsh_split_line(char *line){
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufsize){
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    if(strcmp(tokens[position - 1], "&") == 0) {
        conc = true;
        tokens[position - 1] = NULL;
    }
    tokens[position] = NULL;
    return tokens;
}

/* Read input from stdin */
char *lsh_read_line(void)
{
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
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
        if(position >= bufsize){
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
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
    char **args;
    int status;

    do {
        printf(">");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while(status);
}

int main(void)
{
    lsh_loop();

    return EXIT_SUCCESS;
}
