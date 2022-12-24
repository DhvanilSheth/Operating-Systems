#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

//method for handling ls -l, ls -a, ls -i, ls
void list(char *path, int flag){
    //open directory
    DIR *dir;
    //struct for directory entry (file) information 
    struct dirent *entry;
    //struct for file information
    struct stat f_status = {0};
    //open directory and check for errors
    //print error message and exit program with error status 1 if directory cannot be opened
    if ((dir = opendir(path)) == NULL){
        perror("opendir");
        exit(1);
    }
    //read directory entries
    while ((entry = readdir(dir)) != NULL)
    {
        //check for errors in stat function call and print error message if error occurs
        if (stat(entry->d_name, &f_status) == -1){
            perror("stat");
            exit(1);
        }
        //print file information if flag is set to 1 (ls -l)
        if (flag == 1){
            //print file information (permissions, number of links, owner, group, size, last modified, file name)
            //check if file name is not . or ..
            if ((entry->d_name[0] != '.' || entry->d_name[1] != '\0') && (entry->d_name[0] != '.' && entry->d_name[1] != '.') ){
                //print file permissions
                printf((S_ISDIR(f_status.st_mode)) ? "d" : "-");
                //print file permissions for owner
                printf((f_status.st_mode & S_IRUSR) ? "r" : "-");
                //print file permissions for owner
                printf((f_status.st_mode & S_IWUSR) ? "w" : "-");
                //print file permissions for owner
                printf((f_status.st_mode & S_IXUSR) ? "x" : "-");
                //print file permissions for group
                printf((f_status.st_mode & S_IRGRP) ? "r" : "-");
                //print file permissions for group
                printf((f_status.st_mode & S_IWGRP) ? "w" : "-");
                //print file permissions for group
                printf((f_status.st_mode & S_IXGRP) ? "x" : "-");
                //print file permissions for others
                printf((f_status.st_mode & S_IROTH) ? "r" : "-");
                //print file permissions for others
                printf((f_status.st_mode & S_IWOTH) ? "w" : "-");
                //print file permissions for others
                printf((f_status.st_mode & S_IXOTH) ? "x" : "-");
                //print number of links
                printf(" %ld", f_status.st_nlink);
                //print owner
                printf(" %s", getpwuid(f_status.st_uid)->pw_name);
                //print group
                printf(" %s", getgrgid(f_status.st_gid)->gr_name);
                //print size
                printf(" %ld", f_status.st_size);
                //print last modified in format: day month date time hh:mm:ss year
                char* time = ctime(&f_status.st_mtime);
                strftime(time, 20, "%b %d %H:%M %Y", localtime(&f_status.st_mtime));
                printf(" %s", time);
                //print file name
                printf(" %s ", entry->d_name);
                printf("\n");
            }
        }
        //print file information if flag is set to 2 (ls -a)
        else if (flag == 2){
            //file name
            printf("%s \n", entry->d_name);
        }
        //print file name if flag is set to 3 (ls -i)
        else if (flag == 3){
            //inode number and file name
            printf("%ld %s \n", f_status.st_ino, entry->d_name);
        }
        //print file name if flag is set to 4 (ls)
        else if (flag == 4){
            //check if file name is not . or ..
            if ((entry->d_name[0] != '.' || entry->d_name[1] != '\0') && (entry->d_name[0] != '.' && entry->d_name[1] != '.') ){
                printf("%s \n", entry->d_name);
            }
        }
    }
    printf("\n");
    closedir(dir);
}

int main(int argc, char *argv[]){
    //check for correct number of arguments
    if (argc > 2){
        printf("cat: not enough arguments\n");
        exit(1);
    }
    //set flag to 0
    int flag = 0;
    //if no arguments are given, set flag to 4 (ls)
    if (argc == 1){
        flag = 4;
    }
    //if one argument is given, check if argument is -l, -a, or -i
    else if (argc == 2){
        //if argument is -l, set flag to 1 (ls -l)
        if (strcmp(argv[1], "-l") == 0){
            flag = 1;
        }
        //if argument is -a, set flag to 2 (ls -a)
        else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--all") == 0){
            flag = 2;
        }
        //if argument is -i, set flag to 3 (ls -i)
        else if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--inode") == 0){
            flag = 3;
        }
        //if argument is not -l, -a, or -i, print error message and exit program with error status 1
        else{
            printf("ls: invalid option '%s' \n", argv[1]);
            exit(1);
        }
    }
    //call method to handle ls -l, ls -a, ls -i, and ls
    list(".", flag);
    return 0;
}