#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

const int EXIT_BAD_ARG = 1;
const int EXIT_OK = 0;

static struct option long_options[] = {
    {"info", required_argument, NULL, 'i'},
    {"script", required_argument, NULL, 's'},
    {"id", required_argument, NULL, 'i'},
    {0, 0, 0, 0}
};

int main (int argc, char* argv[]) {
    int c, counter;
    char* boilerplate_install_path = NULL;
    if (argc == 1) {
        // print usage here.
    
    }
    if (argc < 2) {
        //TODO: error. report and print usage here.
        return EXIT_BAD_ARG;
    }
    char* command_str = argv[1];
    int command_str_len = strlen(command_str);
    for( int i = 0; i < command_str_len; ++i)
        command_str[i] = tolower(command_str[i]);

    if(strcmp(command_str, "install") == 0) {
    } else if(strcmp(command_str, "uninstall") == 0) {
    } else if(strcmp(command_str, "list") == 0) {
        // list all the stuff here.
    } else {
        // print error.
    }

    while((c = getopt_long(argc, argv, "l:p:s:h:i:", long_options, &counter)) != -1) {
        switch (c) {
            case 'h':
                break;
            case 'i':
                break;
            case 'l':
                break;
            case 'p':
                break;
            case 's': 
                break;
            case '?':
                return EXIT_BAD_ARG;
            case ':':
                /* missing option argument */
                return EXIT_BAD_ARG;
        }
    }
}
