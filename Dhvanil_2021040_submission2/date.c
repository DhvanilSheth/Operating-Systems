#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//method for printing the date and time using date command with option no option , -u or -I
void print_date_time(int flag){
    //get current time
    time_t current_time;
    time(&current_time);
    //print current time
    if (flag == 0){
        struct tm *local_time;
        local_time = localtime(&current_time);
        char temp[100];
        strftime(temp, 100, "%a %b %d %H:%M:%S %Z %Y", local_time);
        printf("%s\n", temp);
    }
    //print current time in UTC
    else if (flag == 1){
        struct tm *utc_time;
        utc_time = gmtime(&current_time);
        char temp[100];
        strftime(temp, 100, "%a %b %d %H:%M:%S UTC %Y", utc_time);
        printf("%s\n", temp);
    }
    //print current time in ISO 8601 format
    else if (flag == 2){
        struct tm *time_info;
        time_info = localtime(&current_time);
        char temp[100];
        strftime(temp, 100, "%Y-%m-%d", time_info);
        printf("%s\n", temp);
    }
}

//main method
int main(int argc, char *argv[]){
    //check if there is an argument
    if (argc == 1){
        //print current date and time
        print_date_time(0);
    }
    //check if there is an argument
    else if (argc == 2){
        //check if argument is -u
        if (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--utc") == 0){
            //print current date and time in UTC
            print_date_time(1);
        }
        //check if argument is -I
        else if (strcmp(argv[1], "-I") == 0 || strcmp(argv[1], "--iso-8601") == 0){
            //print current date and time in ISO 8601 format
            print_date_time(2);
        }
        //print error message and exit program with error status 1 if argument is not -u or -I
        else{
            printf("date: invalid option '%s' \n", argv[1]);
            exit(1);
        }
    }
    //print error message and exit program if there are more than 2 arguments
    else{
        printf("date: extra operand '%s' \n", argv[2]);
        exit(1);
    }
    return 0;
}
