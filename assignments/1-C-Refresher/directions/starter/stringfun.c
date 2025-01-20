#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
int  reverse(char *, int, int);
int  word_print(char *, int, int);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    // Handles case of 0 len
    if (len == 0) {
        printf("len must be greater than 0");
        return -2;
    }

    int user_str_len = 0;
    bool whitespace_encountered = true;
    
    while (*user_str != '\0') {
        if (user_str_len > len) {
            printf("The supplied string is too large for the buffer.");
            return -1;
        }
        if (*user_str == ' ' || *user_str == '\t') {
            if (whitespace_encountered == true) {
                user_str++;
            } else {
                whitespace_encountered = true;
                *buff = ' ';

                buff++;
                user_str++;
                user_str_len++;
            }
        } else {
            whitespace_encountered = false;
            *buff = *user_str;
            
            buff++;
            user_str++;
            user_str_len++;
        }
    }

    if (user_str_len > 0 && *(buff - 1) == ' ') { // Deals with trailing whitespace
        *(buff - 1) = '.';
    }

    memset(buff, '.', len - user_str_len*sizeof(char));

    return user_str_len;
}

void print_str(char *buff, int len) {
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    print_str(buff, len); //Moved this code to use it elsewhere
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    if (str_len > len) {
        printf("The supplied string is too large for the buffer.\n");
        return -1;
    }

    int spaces_encountered = 0;
    for (int i = 0; i < str_len; i++) {
        if (*buff == ' ') {
            spaces_encountered++;
        }
        buff++;
    }

    return spaces_encountered + 1;
}

int reverse(char *buff, int len, int str_len) {
    if (str_len > len) {
        printf("The supplied string is too large for the buffer.\n");
        return -1;
    }

    char* start = buff;
    char* end = buff + str_len - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }

    return 0;
}

int word_print(char* buff, int len, int str_len) {
    if (str_len > len) {
        printf("The supplied string is too large for the buffer.\n");
        return -1;
    }

    printf("Word Print\n");
    printf("----------\n");

    char *ptr = buff;

    int word_count = 1;
    int char_count = 0;

    for (int i = 0; i < str_len + 1; i++) {
        if (*buff == ' ' || *buff == '.' || *buff == '\0')  {
            printf("%d. %.*s (%d)\n", word_count, char_count, ptr, char_count);
            ptr+=char_count + 1;
            char_count=0;
            word_count++;
        } else {
            char_count++;
        }
        buff++;
    }
    printf("\n");

    return word_count - 1; // Since we overcounted at the end
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      If argc is not less than 2, than the right statement will not be evaluated,
    //      known as a short circuit. So we don't have to worry about argv[1] not existing
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      This if statement checks if the user passed less than 3,
    //      the minimum arguments, to notify the user of proper use. argv[0]
    //      is the name of the executable (./stringfun)
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    if (opt == 'x' && argc != 5) {
        printf("Needs 5 arguments, string to find and string to replace");
        exit(1);
    }
    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = (char*) malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Malloc failure");
        exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len); 
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'r':
            rc = reverse(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error reversing strings, rc = %d", rc);
                exit(2);
            }
            printf("Reversed String: ");
            print_str(buff, user_str_len);
            break;
        case 'w':
            rc = word_print(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error printing words, rc = %d", rc);
                exit(2);
            }
            printf("Number of words returned: %d\n", rc);
            break;
        case 'x':
            printf("Not Implemented!\n");
            exit(2);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          Providing both the pointer and the length is a good idea,
//          as it passes responsibility onto the user that we don't
//          access incorrect memory. While in this program, the buff
//          variable will have 50 bytes it would not be good practice
//          to assume so. This size could also be modified in the future.