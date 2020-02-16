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

void smash_cd(char *path);
char **smash_parse_input(char *line, char **tokens);
int smash_path(char **tokens, struct node *head);
void smash_fork(char **tokens);
void smash_execute(char **tokens, char *line, struct node *head);
void smash_redirect(char *arg, struct node *path);
int count(char **tokens);
/*
* cd method
*/
void smash_cd(char *path)
{
    chdir(path);
    if (chdir(path) != 0)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
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
* Parse input interactive
*/
#define DELIM " \t\r\n\a"
char **smash_parse_input(char *line, char **tokens)
{
    // tokens = malloc(size * sizeof(char *)); //realloc if bufsize too small
    //int tok = 0;
    int tok = 0;

    //malloc-ing error
    if (tokens == NULL)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    line = strtok(line, DELIM);

    printf("tokens[0] = %s\n", tokens[0]);

    while (line != NULL)
    {
        tokens[tok] = malloc(strlen(line) * sizeof(char));
        tokens[tok] = line;
        if (tokens[tok] == NULL)
        {
            //error message
        }
        tok++;
        line = strtok(NULL, DELIM);
    }
    tokens[tok] = NULL;

    return tokens;
}

/*
*batch mode parse
*/
// #define DELIM " \t\r\n\a"

// char **smash_parse_batch(FILE *f)
// {
//     int size = 1000;
//     char **tokens = malloc(size * sizeof(char *)); //realloc if bufsize too small
//     int tok = 0;
//     int ret = 0;
//     char line[500], bb[1000] = "";
//     //malloc-ing error
//     if (tokens == NULL)
//     {
//         write(STDERR_FILENO, error_message, strlen(error_message));
//         exit(1);
//     }
//     if (f == NULL)
//         printf("File not opened");
//     else
//     {
//         printf("File opened successfully\n");
//         while ((ret = fscanf(f, " %s ", line))) // use of fscanf
//         {
//             if (ret == EOF)
//             {
//                 break;
//             }

//             strcat(bb, line);
//             strcat(bb, "\n");
//         }
//         char *tok = strtok(bb, DELIM);
//         while (tok != NULL)
//         {
//             printf("%s", tok);
//             tok = strtok(NULL, DELIM); // NULL instead of pointer
//         }
//     }

//     tokens[tok] = NULL;
//     return tokens;
// }

//add, remove, clear

int printLinkedList(struct node *path_list)
{

    struct node *curr = path_list;
    printf("%s -> ", curr->data);

    while (curr->next != NULL)
    {
        curr = curr->next;
        printf("%s -> ", curr->data);
    }
    printf("\n");

    return 0;
}

int smash_add(char *path, struct node *head)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = path;
    struct node *temp = head->next;

    head->next = newNode;
    newNode->next = temp;

    return 0;
}

int smash_remove(char *path, struct node *head)
{

    int found = 0;

    struct node *prev = (struct node *)malloc(sizeof(struct node));
    struct node *temp = head->next;

    while (head->next != NULL)
    {
        prev = head;
        head = head->next;
        if (strcmp(head->data, path) == 0)
        {
            prev->next = head->next;
            head = prev;
            found = 1;
        }
    }
    if (found == 0)
    {
        //err;
    }

    return 0;
}

int smash_clear(struct node *head)
{
    head->next = NULL;
    printLinkedList(head);
    return 0;
}
int smash_path(char **tokens, struct node *head)
{
    int cnt = count(tokens);
    char *path = NULL;
    if (cnt < 2 || cnt > 3)
    {
        //err;
        return -1;
    }

    //if the path arguments are not add or remove or clear
    if ((strcmp(tokens[1], "add") != 0) && (strcmp(tokens[1], "remove") != 0) && (strcmp(tokens[1], "clear") != 0))
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return -1;
    }
    printf("%d", cnt);
    if ((strcmp(tokens[1], "clear") == 0) && cnt == 2)
    {
        printf("im here ");
        smash_clear(head);
        return 0;
    }
    path = malloc(strlen(tokens[2]));
    strcpy(path, tokens[2]);
    if ((strcmp(tokens[1], "add") == 0) && cnt == 3)
    {
        smash_add(path, head);
        return 0;
    }
    if ((strcmp(tokens[1], "remove") == 0) && cnt == 3)
    {
        smash_remove(path, head);
        return 0;
    }

    return 0;
}

void smash_fork(char **tokens, struct node *head)
{
    pid_t pid;
    int i = 0;

    char *path = malloc(sizeof(char) * 100);
    strcpy(path, tokens[0]);

    while ( head != NULL ){

        
    }
    pid = fork();

    //fork error
    if (pid == -1)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    else if (pid == 0)
    {
        if (tokens[0] != NULL)
        {
            printf("Printing tokens \n");
            for (int i = 0; i < 10; i++)
            {
                printf("tokens[%d] : %s \n", i, tokens[i]);
            }
            char *arr[] = {"ls", "-l", NULL};
            execv("/bin/ls", tokens);
            if (execv(tokens[0], tokens) == -1)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }
    }
    else
    {
        wait(NULL);
    }
    free(path);
}
void smash_execute(char **tokens, char *line, struct node *head)
{
    int i = 0;
    char *path = malloc(sizeof(char) * 100);
    strcpy(path, "/bin/");
    char *path2 = malloc(sizeof(char) * 100);

    // while (tokens[i] != NULL)
    // {
    //     //how to filter and put only the path name?
    //     // if( strcmp(tokens[i], ) ){
    //     strcat(path, tokens[i]);
    //     //;
    //     path2 = strdup(path);
    //     printf("path to execv is : %s \n", path2);
    //     i++;
    //     // }
    // }

    //multiple commands means ; or pll means &
    // printf("Printing tokens \n");
    // for (int i = 0; i < 10; i++)
    // {
    //     printf("tokens[%d] : %s \n", i, tokens[i]);
    // }
    //built-ins
    if (strcmp(tokens[0], "cd") == 0)
    {
        if (tokens[1] == NULL || (tokens[2] != NULL))
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        strcat(path, tokens[1]);
        strcat(path, "/");
        path2 = strdup(path);
        // // printf("path to execv is : %s \n", path2);

        // if (chdir(path2) != 0)
        // {
        //     write(STDERR_FILENO, error_message, strlen(error_message));
        // }
        smash_cd(path2);
    }
    else if (strcmp(tokens[0], "path") == 0)
    {
        smash_path(tokens, head);
    }
    else if (strcmp(tokens[0], "exit") == 0)
    {
        if (tokens[1] != NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        exit(0);
    }
    else
    {
        smash_fork(tokens);
    }
}

int count(char **tokens)
{
    int i = 0;
    while (tokens[i] != NULL)
    {
        printf("thes is tokens[i] %s\n", tokens[i]);
        i++;
    }
    return i;
}

int main(int argc, const char *argv[])
{

    char *line = NULL; //interactive mode
    size_t len = 0;
    struct node *head = (struct node *)malloc(sizeof(struct node));

    struct node *default_path = (struct node *)malloc(sizeof(struct node));

    head->next = default_path;
    head->data = "head";
    char *default_p = (char *)malloc(strlen("/bin"));
    strcpy(default_p, "/bin");
    default_path->data = default_p;
    default_path->next = NULL;
    printLinkedList(head);
    //FILE *f;

    char **tokens = (char **)malloc(1000 * sizeof(char *));

    //enter batch mode
    // if (argc >= 2)
    //{
    //TODO: is it argv[1]???
    //  f = fopen(argv[1], "r");
    //if (f == NULL)
    //{
    //  write(STDERR_FILENO, error_message, strlen(error_message));
    //}
    //tokens = smash_parse_batch(f);
    // }
    //

    while (1)
    {
        printf("smash> ");
        printf("\n");
        printLinkedList(head);
        fflush(stdout);
        getline(&line, &len, stdin);
        // printf("line is : %s \n", line);
        if (line == NULL || tokens == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else
        {
            //mult
            // if (strstr(line, ";"))
            // {
            //     //parallel
            //     if (strstr(line, "&"))
            //     {
            tokens = smash_parse_input(line, tokens);
            // }
            // }
        }

        // printf("Printing tokens \n");
        // for (int i = 0; i < 10; i++)
        // {
        //     printf("tokens[%d] : %s \n", i, tokens[i]);
        // }

        smash_execute(tokens, line, head); //pll and mult commands will be send separately in this func
    }
    free(tokens);

    return 0;
}
