#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>

void cd(char* args[]){
    //if the argument passed is .., go to parent directory
    if (strcmp(args[1], "..\n") == 0) {
        chdir("..");
    }
    //if the argument passed is -P, go to previous directory
    else if (strcmp(args[1], "-P\n") == 0) {
        chdir(getenv("OLDPWD"));
    }
    //if arguments are passed, go to the directory specified
    else {
        //remove the new line character from the argument
        char* arg = strtok(args[1], "\n");
        if (chdir(arg) != 0) {
            //print the argument if it is not a valid directory
            printf("%s: No such file or directory\n", arg);
            perror("Error");
        }
    }
}

//method to implement echo command with options -n and -e and throw error if invalid option is passed using perror
void echo(char* args[]){
    int i = 1;
    //check if the argument passed is -n
    if (strcmp(args[1], "-n") == 0) {
        //remove the new line character from the argument and print the argument in a for loop
        for (i = 2; args[i] != NULL; i++) {
            char* arg = strtok(args[i], "\n");
            printf("%s ", arg);
        }
    }
    //check if the argument passed is -e
    else if (strcmp(args[1], "-e") == 0) {
        //remove the new line character from the argument and print the argument in a loop
        for (i = 2; args[i] != NULL; i++) {
            char* arg = strtok(args[i], "\n");
            printf("%s ", arg);
        }
        printf("\n");
    }
    //check if the argument passed is -ne or -en
    else if (strcmp(args[1], "-ne") == 0 || strcmp(args[1], "-en") == 0) {
        //remove the new line character from the argument and print the argument in a loop
        for (i = 2; args[i] != NULL; i++) {
            char* arg = strtok(args[i], "\n");
            printf("%s", arg);
        }
    }
    //check if the argument passed is invaild
    else if (args[1][0] == '-') {
        printf("Invalid option '%s'\n", args[1]);
        perror("Error");
    }
    //if no option is passed, print the argument with a new line
    else {
        //remove the new line character from the argument and print the argument in a loop
        for (i = 1; args[i] != NULL; i++) {
            char* arg = strtok(args[i], "\n");
            printf("%s ", arg);
        }
        printf("\n");
    }
}

void pwd_no_arg(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd)); //gets the current working directory and store it in cwd 
    printf("%s\n", cwd);
}
void pwd_L(){
    printf("%s\n", getenv("PWD"));
}
void pwd_P(){
    printf("%s \n", getcwd(NULL, 0));
}

//method to handle threading using pthreads
void *thread(void *arg){
    char *filename = (char *)arg;
    system(filename);
    return NULL;
}

//main method to run the shell program and execute the commands cd, pwd, echo
void main(){
    char *args[1024];
    char *token;
    char *line = NULL;
    size_t len = 0;
    ssize_t user_input;
    int i;
    //infinite loop to keep the shell running
    while(1){
        //clear the args array
        for (i = 0; i < 1024; i++) {
            args[i] = NULL;
        }
        //prompt to enter command and user_input the command entered by the user using getline function and store it in line variable
        printf("> ");
        user_input = getline(&line, &len, stdin);
        //if the user_input is EOF, exit the shell
        if (user_input == EOF) {
            exit(0);
        }
        //if the user enters exit, exit the shell
        else if (strcmp(line, "exit\n") == 0) {
            exit(0);
        }
        //check the string for the occurance of "cd" and if it is found, call the cd method
        else if (strstr(line, "cd") != NULL) {
            //if no argument is passed, go to home directory
            if (strcmp(line, "cd\n") == 0){
                chdir(getenv("HOME"));
            }
            else {
                //tokenize the string using space as the delimiter and store it in args
                token = strtok(line, " ");
                i = 0;
                while (token != NULL) {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                //call the cd method
                cd(args);
            }
        }
        //check the string for the occurance of "pwd" and if it is found, call the pwd method
        else if (strstr(line, "pwd") != NULL) {
            //tokenize the string using strtok and store it in args
            token = strtok(line, " ");
            i = 0;
            while (token != NULL) {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }
            //if no argument is passed, call the pwd_no_arg method
            if (args[1] == NULL) {
                pwd_no_arg();
            }
            //if -L is passed, call the pwd_L method
            else if (strcmp(args[1], "-L\n") == 0 || strcmp(args[1], "-L") == 0 || strcmp(args[1], "--logical\n") == 0) {
                pwd_L();
            }
            //if -P is passed, call the pwd_P method
            else if (strcmp(args[1], "-P\n") == 0 || strcmp(args[1], "-P") == 0 || strcmp(args[1], "--physical\n") == 0) {
                pwd_P();
            }
            //if any other argument is passed, throw an error
            else {
                printf("pwd: invalid option '%s'\n", args[1]);
            }
        }
        //check the string for the occurance of "echo" and if it is found, call the echo method
        else if (strstr(line, "echo") != NULL) {
            //if only echo is passed, print a new line
            if (strcmp(line, "echo\n") == 0) {
                printf("\n");
            }
            else{
                //tokenize the string using strtok and store it in args
                token = strtok(line, " ");
                i = 0;
                while (token != NULL) {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                //call the echo method
                echo(args);
            }
        }
        else{
            //flag for threading
            int thread_flag = 0;
            //if the user_input has "&t" at the end, create a thread and execute the command 
            if (strstr(line, "&t") != NULL) {
                thread_flag = 1;
                pthread_t thread_id; // thread id
                char* filename = (char*)malloc(1024);
                //remove the "&t" from the string and store it in filename
                char* remove_t = strtok(line, "&t");
                strcpy(filename, remove_t);
                //create a thread and call the thread method
                pthread_create(&thread_id, NULL, thread, filename);
                pthread_join(thread_id, NULL);
            }
            else if (strstr(line, "&") != NULL) {
                printf("Error: Invalid command '%s'\n", line);
                exit(1);
            }
            else if (thread_flag == 0) {
                //tokenize the input using strtok
                token = strtok(line, " \n");
                i = 0;
                while (token != NULL) {
                    args[i] = token;
                    token = strtok(NULL, " \n");
                    i++;
                }
                args[i] = NULL;
                //fork a child process
                pid_t pid = fork();
                //if the child process is not created successfully, throw error
                if (pid < 0) {
                    printf("Error: Child process not created successfully\n");
                    exit(1);
                }
                //if the child process is created successfully, execute the command
                else if (pid == 0) {
                    //call the execvp function to execute the cat command from the cat program
                    if (strcmp(args[0], "cat") == 0) {
                        execvp("./cat", args);
                    }
                    //call the execvp function to execute the ls command from the ls program
                    else if (strcmp(args[0], "ls") == 0) {
                        execvp("./ls", args);
                    }
                    //call the execvp function to execute the wc command from the date program
                    else if (strcmp(args[0], "date") == 0) {
                        execvp("./date", args);
                    }
                    //call the execvp function to execute the wc command from the mkdir program
                    else if (strcmp(args[0], "mkdir") == 0) {
                        execvp("./mkdir", args);
                    }
                    //call the execvp function to execute the wc command from the rm program
                    else if (strcmp(args[0], "rm") == 0) {
                        execvp("./rm", args);
                    }
                    //else, throw error
                    else{
                        printf("Invalid command %s \n", args[0]);
                        exit(1);
                    }
                }
                //if the child process is created successfully, wait for the child process to finish executing
                else {
                    wait(NULL);
                }
            }
            else{
                continue;
            }
        }
    }
}
