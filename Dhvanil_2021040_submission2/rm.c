#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

//method to remove using rm command with option no option ,-r or -f
void remove_file(char *path, int flag){
    //check if flag is set to 0 (rm)
    if (flag == 0){
        //check if file exists
        if (access(path, F_OK) != -1){
            //check if file is a directory
            if (opendir(path) != NULL){
                //print error message and exit program with error status 1 if file is a directory
                printf("rm: %s: is not a file\n", path);
                exit(1);
            }
            //remove file
            else{
                //remove file and check for errors
                //print error message and exit program with error status 1 if file cannot be removed
                if (remove(path) != 0){
                    perror("file cannot be removed (remove)");
                    exit(1);
                }
            }
        }
        //print error message and exit program with error status 1 if file does not exist
        else{
            printf("rm: %s ,file / directory doesn't exist\n", path);
            exit(1);
        }
    }
    //check if flag is set to 1 (rm -r)
    else if (flag == 1){
        //check if file exists
        if (access(path, F_OK) != -1){
            //check if path is a directory or a file and remove accordingly
            if (opendir(path) != NULL){
                //open directory and check for errors
                DIR *directory;
                //print error message and exit program with error status 1 if directory cannot be opened
                if ((directory = opendir(path)) == NULL){
                    perror("directory cannot be opened (opendir)");
                    exit(1);
                }
                //read directory
                struct dirent *directory_entry;
                //read directory and check for errors
                //print error message and exit program with error status 1 if directory cannot be read
                //if the directory is empty remove the directory
                if ((directory_entry = readdir(directory)) == NULL){
                    if (rmdir(path) != 0){
                        perror("directory cannot be removed (rmdir)");
                        exit(1);
                    }
                }
                //loop through directory
                while (directory_entry != NULL){
                    //check if directory entry is not . or ..
                    if (strcmp(directory_entry->d_name, ".") != 0 && strcmp(directory_entry->d_name, "..") != 0){
                        //create path to directory entry
                        char directory_entry_path[1024];
                        strcpy(directory_entry_path, path);
                        strcat(directory_entry_path, "/");
                        strcat(directory_entry_path, directory_entry->d_name);
                        //check if directory entry is a directory
                        if (opendir(directory_entry_path) != NULL){
                            //call remove_file method recursively
                            remove_file(directory_entry_path, 1);
                        }
                        //remove file
                        else{
                            //remove file and check for errors
                            //print error message and exit program with error status 1 if file cannot be removed
                            if (remove(directory_entry_path) != 0){
                                perror("remove file error (remove)");
                                exit(1);
                            }
                        }
                    }
                    //if directory is empty remove the directory
                    if ((directory_entry = readdir(directory)) == NULL){
                        if (rmdir(path) != 0){
                            perror("directory cannot be removed (rmdir)");
                            exit(1);
                        }
                    }
                }
                //close directory
                closedir(directory);
                //remove directory and check for errors
                //print error message and exit program with error status 1 if directory cannot be removed
                if (rmdir(path) != 0){
                    perror("directory cannot be removed (rmdir)");
                    exit(1);
                }
            }
            //remove file
            else{
                //remove file and check for errors
                //print error message and exit program with error status 1 if file cannot be removed
                if (remove(path) != 0){
                    perror("file cannot be removed (remove)");
                    exit(1);
                }
            }
        }
        //print error message and exit program with error status 1 if file does not exist
        else{
            printf("rm: %s ,file / directory doesn't exist\n", path);
            exit(1);
        }
    }
    //check if flag is set to 2 (rm -f)
    else if (flag == 2){
        //check if file exists
        if (access(path, F_OK) != -1){
            //check if file is a directory
            if (opendir(path) != NULL){
                //remove directory
                //open directory
                DIR *directory;
                //open directory and check for errors
                //print error message and exit program with error status 1 if directory cannot be opened
                if ((directory = opendir(path)) == NULL){
                    perror("directory cannot be opened (opendir)");
                    exit(1);
                }
                //read directory
                struct dirent *directory_entry;
                //read directory and check for errors
                //print error message and exit program with error status 1 if directory cannot be read
                if ((directory_entry = readdir(directory)) == NULL){
                    perror("directory cannot be read (readdir)");
                    exit(1);
                }
                //loop through directory
                while (directory_entry != NULL){
                    //check if directory entry is not . or ..
                    if (strcmp(directory_entry->d_name, ".") != 0 && strcmp(directory_entry->d_name, "..") != 0){
                        //create path to directory entry
                        char directory_entry_path[1024];
                        strcpy(directory_entry_path, path);
                        strcat(directory_entry_path, "/");
                        strcat(directory_entry_path, directory_entry->d_name);
                        //check if directory entry is a directory
                        if (opendir(directory_entry_path) != NULL){
                            //call remove_file method recursively
                            remove_file(directory_entry_path, 2);
                        }
                        //remove file
                        else{
                            //remove file and check for errors
                            //print error message and exit program with error status 1 if file cannot be removed
                            if (remove(directory_entry_path) != 0){
                                perror("file cannot be removed (remove)");
                                exit(1);
                            }
                        }
                    }
                    //read directory and check for errors
                    //print error message and exit program with error status 1 if directory cannot be read
                    if ((directory_entry = readdir(directory)) == NULL){
                        perror("directory cannot be read (readdir)");
                        exit(1);
                    }
                }
                //close directory
                closedir(directory);
                //remove directory and check for errors
                //print error message and exit program with error status 1 if directory cannot be removed
                if (rmdir(path) != 0){
                    perror("directory cannot be removed (rmdir)");
                    exit(1);
                }
            }
            //remove file
            else{
                //remove file and check for errors
                //print error message and exit program with error status 1 if file cannot be removed
                if (remove(path) != 0){
                    perror("file cannot be removed (remove)");
                    exit(1);
                }
            }
        }
        else{
            printf("rm: %s ,file / directory doesn't exist\n", path);
            exit(1);
        }
    }
}

//main method
int main(int argc, char *argv[]){
    //check if there are no arguments
    if (argc == 1){
        //print error message and exit program with error status 1
        printf("rm: not enough arguments \n");
        exit(1);
    }
    //check if there is only one argument
    else if (argc == 2){
        //check if argument is -r or -f
        if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--recursive") == 0 || strcmp(argv[1], "--force") == 0){
            //print error message and exit program with error status 1
            printf("rm: invalid operand after '%s'\n", argv[1]);
            exit(1);
        }
        //remove file
        else{
            //call remove_file method
            remove_file(argv[1], 0);
        }
    }
    //check if there are more than 2 arguments
    else if (argc > 2){
        //check if first argument is -r
        if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--recursive") == 0){
            //loop through arguments
            for (int i = 2; i < argc; i++){
                //call remove_file method
                remove_file(argv[i], 1);
            }
        }
        //check if first argument is -f
        else if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--force") == 0){
            //loop through arguments
            for (int i = 2; i < argc; i++){
                //call remove_file method
                remove_file(argv[i], 2);
            }
        }
        //remove files
        else{
            //loop through arguments
            for (int i = 1; i < argc; i++){
                //call remove_file method
                remove_file(argv[i], 0);
            }
        }
    }
    //exit program with success status 0
    exit(0);
}