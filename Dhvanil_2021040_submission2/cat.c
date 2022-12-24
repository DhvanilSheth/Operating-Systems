#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

//method to print the file information using cat [file] command with option -n or -b
void print_file_info_with_parameters(char *path, int flag){
    //open file
    FILE *file;
    //open file and check for errors
    //print error message and exit program with error status 1 if file cannot be opened
    if ((file = fopen(path, "r")) == NULL){
        perror("fopen");
        exit(1);
    }
    //print file information if flag is set to 1 (cat -n)
    if (flag == 1){
        //print line number and file contents
        int line_number = 1;
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL){
            printf("%d %s", line_number, line);
            line_number++;
        }
    }
    //print file information if flag is set to 2 (cat -b)
    else if (flag == 2){
        //print line number and file contents
        int line_number = 1;
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL){
            //check if line is not empty
            if (line[0] != '\n'){
                printf("%d %s", line_number, line);
                line_number++;
            }
            //print empty line
            else{
                printf("%s", line);
            }
        }
    }
    //close file
    fclose(file);
}

//method to create / append to file using cat [file] command with > or >> 
void create_append_file(char *path, int flag, char *file_name){
    //open file
    FILE *file;
    //open file and check for errors
    //print error message and exit program with error status 1 if file cannot be opened
    if ((file = fopen(path, "r")) == NULL){
        perror("fopen error"); 
        exit(1);
    }
    //create file if flag is set to 1 (cat > [file])
    if (flag == 1){
        //create file
        FILE *new_file;
        //open file and check for errors
        //print error message and exit program with error status 1 if file cannot be opened
        if ((new_file = fopen(file_name, "w")) == NULL){
            perror("fopen error");
            exit(1);
        }
        //print file contents to new file
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL){
            fprintf(new_file, "%s", line);
        }
        //close file
        fclose(new_file);
    }
    //append to file if flag is set to 2 (cat >> [file])
    else if (flag == 2){
        //append to file
        FILE *new_file;
        //open file and check for errors
        //print error message and exit program with error status 1 if file cannot be opened
        if ((new_file = fopen(file_name, "a")) == NULL){
            perror("fopen error");
            exit(1);
        }
        //print file contents to new file
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL){
            fprintf(new_file, "%s", line);
        }
        //close file
        fclose(new_file);
    }
    //close file
    fclose(file);
}

//method to print the file information using cat [file] command
void print_file_info(char *path){
    //open file
    FILE *file;
    //open file and check for errors
    //print error message and exit program with error status 1 if file cannot be opened
    if ((file = fopen(path, "r")) == NULL){
        perror("fopen error"); 
        exit(1);
    }
    //print file information
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL){
        printf("%s", line);
    }
    //close file
    fclose(file);
}

//main method
int main(int argc, char *argv[]){
    //check if there are no arguments
    if (argc == 1){
        //print error message and exit program with error status 1
        printf("cat: not enough arguments\n");
        exit(1);
    }
    //check if there are arguments
    else{
        //check if there are more than 2 arguments
        if (argc > 2){
            //check if the first argument is -n or -b
            if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--number") == 0 || strcmp(argv[1], "--number-nonblank") == 0){
                //check if the second argument is a file
                struct stat file_format;
                //check if the second argument is a file and check for errors
                //print error message and exit program with error status 1 if file cannot be opened
                if (stat(argv[2], &file_format) == -1){
                    printf("cat: %s: file or directory doesn't exist \n", argv[2]);
                    exit(1);
                }
                //check if the second argument is a file
                if (S_ISREG(file_format.st_mode)){
                    //check if the first argument is -n
                    if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--number") == 0){
                        //print file information with -n
                        print_file_info_with_parameters(argv[2], 1);
                    }
                    //check if the first argument is -b
                    else if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--number-nonblank") == 0){
                        //print file information with -b
                        print_file_info_with_parameters(argv[2], 2);
                    }
                }
                //check if the second argument is not a file
                else{
                    //print error message and exit program with error status 1
                    printf("cat: %s: file or directory doesn't exist \n", argv[2]);
                    exit(1);
                }
            }
            //check if the first argument is > or >>
            else if (strcmp(argv[1], ">") == 0 || strcmp(argv[1], ">>") == 0){
                //check if the second argument is a file
                struct stat file_format;
                //check if the second argument is a file and check for errors
                //print error message and exit program with error status 1 if file cannot be opened
                if (stat(argv[2], &file_format) == -1){
                    printf("cat: %s: file or directory doesn't exist \n", argv[2]);
                    exit(1);
                }
                //check if the second argument is a file
                if (S_ISREG(file_format.st_mode)){
                    //check if the first argument is >
                    if (strcmp(argv[1], ">") == 0){
                        //create file with >
                        create_append_file(argv[2], 1, argv[3]);
                    }
                    //check if the first argument is >>
                    else if (strcmp(argv[1], ">>") == 0){
                        //append to file with >>
                        create_append_file(argv[2], 2, argv[3]);
                    }
                }
                //check if the second argument is not a file
                else{
                    //print error message and exit program with error status 1
                    printf("cat: %s: file or directory doesn't exist \n", argv[2]);
                    exit(1);
                }
            }
            //check if the first argument is not -n, -b, > or >>
            else{
                //print error message and exit program with error status 1
                printf("cat: invalid option '%s' \n", argv[1]);
                exit(1);
            }
        }
        //check if there are 2 arguments
        else if (argc == 2){
            //check if the first argument is -n or -b
            if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "-b") == 0){
                //print error message and exit program with error status 1
                printf("cat: not enough arguments\n");
                exit(1);
            }
            //check if the first argument is > or >>
            else if (strcmp(argv[1], ">") == 0 || strcmp(argv[1], ">>") == 0){
                //print error message and exit program with error status 1
                printf("cat: not enough arguments\n");
                exit(1);
            }
            //check if the first argument is not -n, -b, > or >>
            else{
                //check if the first argument is a file
                struct stat file_format;
                //check if the first argument is a file and check for errors
                //print error message and exit program with error status 1 if file cannot be opened
                if (stat(argv[1], &file_format) == -1){
                    printf("cat: %s: file or directory doesn't exist \n", argv[1]);
                    exit(1);
                }
                //check if the first argument is a file
                if (S_ISREG(file_format.st_mode)){
                    //print file information
                    print_file_info(argv[1]);
                }
                //check if the first argument is not a file
                else{
                    //print error message and exit program with error status 1
                    printf("cat: %s: file or directory doesn't exist \n", argv[1]);
                    exit(1);
                }
            }
        }
        else{
            //print error message and exit program with error status 1
            printf("cat: too many arguments\n");
            exit(1);
        }
    }
    //exit program with success status 0
    return 0;
}