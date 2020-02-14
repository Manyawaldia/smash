#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

//Linked List
struct node
{
    char *data;
    struct node *next;
};

char error_message[30] = "An error has occured\n"; //error checking

/*
* cd method
*/
int smash_cd(char **tokens, char *args[])
{

    if (tokens[1] == NULL)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    else
    {
        if (chdir(tokens[1]) != 0)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
    }
    return 1;
}

/*
*Redirection method
*/
void smash_redirect(char *arg, struct node *path)
{
    printf("redirect ");
    int fd1 = creat((char *)path, 0644);
    dup2(fd1, STDOUT_FILENO);
    close(fd1);
}

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

// void smash_exit(char *line)
// {
//     printf("NOW HERE  ");

// }

//add, remove, clear
struct node *smash_path(char **tokens, char *new_path[])
{
    struct node *head = NULL;
    struct node *newNode, *temp;
    // struct node *prev;
    int i;
    char *data= "/bin/";
    head = (struct node *)malloc(sizeof(struct node));
    head->next = NULL;
    strcpy(head->data, data);
    //if the path arguments are not add or remove or clear

    int path_iter = 0;
    for (path_iter = 0; path_iter < sizeof(tokens); path_iter++)
    {
        strcat(data, tokens[path_iter]); // path = "/bin/...."
    }

    int tok_iter = 1; // tokens[0] = path, tokens[1] = add...etc
    while (tok_iter < sizeof(tokens))
    {
        if ((strcmp(tokens[tok_iter], "add") != 0) || (strcmp(tokens[tok_iter], "remove") != 0) || (strcmp(tokens[tok_iter], "clear") != 0))
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else if ((strcmp(tokens[tok_iter], "add") == 0))
        {
            temp = head;
            for (i = 1; i < sizeof(tokens); i++)
            {
                newNode = (struct node *)malloc(sizeof(struct node));
                strcpy(newNode->data = data, data);
                newNode->next = NULL;
                temp->next = newNode;
                temp = temp->next;
            }
        }
        else if ((strcmp(tokens[tok_iter], "remove") == 0))
        {
            // prev = head;
            temp = head;
            if (temp != NULL && temp->data == data)
            {
                head = temp->next; // Changed head
                free(temp);        // free old head
            }
            while (temp != NULL && (strcmp(temp->data, data) != 0))
            {

                // prev = temp;
                temp = temp->next;
                if (temp == NULL)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                }
            }
        }
        else
        { //clear
            head = NULL;
        }
        tok_iter++;
    }
    return head;
}

void smash_ls(struct node *path, char *args[])
{
    if (access((char *)path, X_OK))
    {
        printf("if ls");
        execv((char *)path, args);
    }
    else
    {
        printf("else ls ");
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
}

void smash_fork(char **tokens, char *args[])
{
    pid_t child_pid;
    int iter = 0;

    while (iter < sizeof(tokens))
    {
        child_pid = fork();
        //fork error
        if (child_pid == -1)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
            break;
        }
        else if (child_pid == 0)
        {
           
            execv(tokens[iter], args);

            /* If execv returns, it must have failed. */
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
        }
        else
        {
            wait(NULL);
        }
    }
}

int main(int argc, const char *argv[])
{

    //int status = 0;
    char *line = NULL; //interactive mode
    size_t len = 0;
    //builtins: exit, cd, path

    //path
    // struct node *path;

    char **tokens = NULL;
    int iter = 0;

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
            //builtins
            if (strstr(line, "exit") != NULL)
            {
                if (strcmp(line, "exit") != 0)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                }
                else
                {
                    exit(0);
                }
                break;
            }
            else if (strstr(line, "cd") != NULL)
            {
                char *args[sizeof(tokens)];
                if (sizeof(tokens) == 1 || sizeof(tokens) > 2)
                {
                    //err
                }
                int i = 0;
                while (i < sizeof(tokens))
                {
                    args[i] = tokens[iter + 1];
                    i++;
                }

                smash_cd(tokens, args);
                break;
            }

            else if (strstr(line, "path") != NULL)
            {
                char *args[sizeof(tokens)];
                if (sizeof(tokens) == 1)
                {
                    //err
                }
                int i = 0;
                while (i < sizeof(tokens))
                {
                    args[i] = tokens[iter + 1];
                    i++;
                }

                smash_path(tokens, args);
                break;
            }
            else
            {

                //TODO: delimited by space so how do i get all of the args in token???
                char *args[sizeof(tokens)];

                int i = 1;
                while (i < sizeof(tokens))
                {
                    args[i] = tokens[i + 1];
                    i++;
                }
                smash_fork(tokens, args);
            }
            iter++;
        }
    }

    free(tokens);

    return 0;
}
