#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

FILE *f = NULL; //batch mode
//Linked List
struct node
{
    char *data;
    struct Node *next;
};

char error_message[30] = "An error has occured\n"; //error checking

/*
* cd method
*/
int smash_cd(char **tokens)
{
    if (tokens[1] == NULL)
    {
        //error
    }
    else
    {
        if (chdir(tokens[1]) != 0)
        {
            //error
        }
    }
    return 1;
}

// }

// int smash_redirection ( char **args, char **output){
//     int
// }
/*
* Parse input
*/
#define DELIM " \t\r\n\a"
char **smash_parse_input(char *line)
{
    int size = 1000;
    char **tokens = malloc(size * sizeof(char *)); //realloc if bufsize too small
    int iter = 0;
    char *tok;

    //malloc-ing error
    if (tokens == NULL)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    tok = strtok(line, DELIM);
    while (tok != NULL)
    {
        tokens[iter] = tok;
        iter++;

        //if buffer size too small
        if (iter >= size)
        {
            size += 1000;
            tokens = realloc(tokens, size * sizeof(char *));
            //realloc error
            if (tokens == NULL)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
        }
        tok = strtok(NULL, DELIM);
    }
    tokens[iter] = NULL;
    return tokens;
}

void smash_exit(char *line)
{
    printf("NOW HERE");
    if (strcmp(line, "exit") != 0)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    else
    {
        exit(0);
    }
}

//TODO: what foes in the params?
void smash_redirect( ){
    if ( )



}



//add, remove, clear
struct node *smash_path(char *token, char *new_path)
{
    struct node *head = NULL;
    struct node *newNode, *temp, *prev;
    int i;
    char *data = "/bin/";
    head = (struct node *)malloc(sizeof(struct node));
    head->next = NULL;
    strcpy(head->data, data);
    //if the path arguments are not add or remove or clear
    if ((strcmp(token, "add") != 0) || (strcmp(token, "remove") != 0) || (strcmp(token, "clear") != 0))
    {
        //error msg
    }
    else if ((strcmp(token, "add") == 0))
    {
        temp = head;
        for (i = 1; i < sizeof(token); i++)
        {
            newNode = (struct node *)malloc(sizeof(struct node));
            strcpy(newNode->data = data, data);
            newNode->next = NULL;
            temp->next = newNode;
            temp = temp->next;
        }
    }
    else if ((strcmp(token, "remove") == 0))
    {
        prev = head;
        temp = head;
        if (temp != NULL && temp->data == new_path)
        {
            head = temp->next; // Changed head
            free(temp);        // free old head
        }
        while (temp != NULL && (strcmp(temp->data, new_path) != 0))
        {
            prev = temp;
            temp = temp->next;
            if (temp == NULL)
            {
                //error message path not found
            }
        }
    }
    else
    { //clear
        head = NULL;
    }

    return head;
}

char *smash_ls(char *path, char *args[])
{
    if (access(path, X_OK))
    {
        execv(path, args);
    }
    else
    {
        //error ls
    }
}

int main(int argc, const char *argv[])
{
    //int status = 0;
    char *line = NULL; //interactive mode
    size_t len = 0;
    pid_t pid;
    int iter = 0;
    //builtins: exit, cd, path

    //path
    char *path;

    char **tokens = NULL;

    //too many args
    if (argc > 2)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    while (1)
    {
        printf("smash> ");
        getline(&line, &len, stdin);
        //TODO: batch version
        tokens = smash_parse_input(line);

        while (iter < sizeof(tokens))
        {
            pid = fork();
            //fork error
            if (pid == -1)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else if (pid == 0)
            {
                if (strstr(line, "exit") != NULL)
                {
                    printf("HERE ");
                    smash_exit(line);
                }
                if (strstr(line, "path") != NULL)
                {
                    //TODO: delimited by space so how do i get all of the args in token???
                    path = smash_path(tokens[iter], tokens[iter + 1]);
                }
                if (strstr(line, "ls") != NULL)
                {
                    char *args[sizeof(tokens)];
                    int i = 1; 
                    while(tokens[i] != NULL){
                        args[i] = tokens[i+1];
                        i++;
                    }
                    printf("%s", args);
                    //TODO: how do i get all of the args after ls ???
                    smash_ls(path, args);
                }
                //what would be the param hjere?
                if (strstr(line, "cd") != NULL)
                {
                    smash_cd(tokens);
                }
                if(strstr(line, ">") != NULL){

                    smash_redirect();
                }

                exit(0);
            }
            else
            {
                wait(NULL);
            }
            iter++;
        }
    }

    free(tokens);

    return 0;
}
