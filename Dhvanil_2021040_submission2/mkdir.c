#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
//method to create a directory with options -v
void create_directory(char *directory, bool v) {
    //check if the directory already exists or not and create it if it doesn't exist 
    //print error message and exit program with error status 1 if directory cannot be created
    if (mkdir(directory, 0777) == -1) {
        perror("mkdir");
        exit(1);
    }
    //print directory name if -v option is set
    if (v) {
        printf("mkdir: directory created with name '%s' \n", directory);
    }
}

//method to create a directory with options -p
void create_directory_with_Parent(char *directory, bool v) {
    //check if the directory already exists or not and create it if it doesn't exist
    if (mkdir(directory, 0777) == -1) {
        //check if the directory doesn't exist
        if (errno == ENOENT) {
            //create parent directory
            char *parent_directory = strdup(directory); //copy directory name to parent_directory variable 
            char *last_slash = strrchr(parent_directory, '/'); // get the last slash in the path to get the parent directory
            *last_slash = '\0'; //remove last slash from parent directory
            create_directory_with_Parent(parent_directory, v);
            //create directory
            create_directory(directory, v);
        }
        //print error message and exit program with error status 1 if directory cannot be created
        else {
            perror("mkdir");
            exit(1);
        }
    }
    if (v) {
        printf("mkdir: directory created with name '%s' \n", directory);
    }
}

int main(int argc, char *argv[]) {
    //check if the user has entered the correct number of arguments
    if (argc < 2) {
        printf("mkdir: not enough arguments \n");
        exit(1);
    }
    //check if the user has entered the correct options
    if (argc == 2) {
        //create the directory with the name entered by the user
        create_directory(argv[1], false);
    }
    else if (argc == 3) {
        //check if the user has entered the correct options
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0) {
            //create the directory with the name entered by the user and print the directory name
            create_directory(argv[2], true);
        }
        else if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--parents") == 0) {
            //create the directory with the name entered by the user and create the parent directory if it doesn't exist
            create_directory_with_Parent(argv[2], false);
        }
        else {
            printf("mkdir: invalid option '%s'\n", argv[1]);
            exit(1);
        }
    }
    else if (argc == 4) {
        //check if the user has entered the correct options
        if ((strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0) && (strcmp(argv[2], "-p") == 0 || strcmp(argv[2], "--parents") == 0)  || (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--parents") == 0)  && (strcmp(argv[2], "-v") == 0 || strcmp(argv[2], "--verbose") == 0)) {
            //create the directory with the name entered by the user and create the parent directory if it doesn't exist
            //print the directory name
            create_directory_with_Parent(argv[3], true);
        }
        else {
            printf("mkdir: invalid option '%s' '%s'\n", argv[1], argv[2]);
            exit(1);
        }
    }
    else {
        printf("mkdir: too many arguments passed\n");
        exit(1);
    }
    return 0;
}