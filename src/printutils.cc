#include "printutils.h"
#include <iostream>
#include "configfile.h"
/* FOREGROUND */
#define RESET  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define UNDL "\x1B[4m"
#define BOLD "\x1B[1m"
// print each boilerplate entry. name, location, description.
void printutils::print_boilerplate_entry(const boiler_item& bti) {
    std::cout << UNDL << BOLD<< KCYN<< "- "<<bti.name << RESET <<std::endl;
    std::cout << BOLD << ("Location: ")<< RESET << bti.boilerpath<<std::endl;
    std::cout << bti.description<<std::endl;
    std::cout << std::endl;
}

void printutils::print_usage() {
    printf("print boiler usage here\n");
    //TODO: print usage.

}

// checks if is an actual command. If it is, print usage of it, and return true.
bool printutils:: print_command_usage(char const* cmd) {
    //TODO: print command usage.
        printutils::print_header("%s command:\n", cmd);
    if (strcmp(cmd, "show") == 0 ) {
        printf("USAGE: boiler show BOILERPLATE_NAME\n");
    } else if (strcmp(cmd, "install") == 0 ){
        printf("USAGE: boiler install PATH/TO/BOILERPLATE [-n BOILERPLATE_NAME] [ -d BOILERPLATE_DESCRIPTION] [-s,--soft or -h, --hard]\n");
    } else if (strcmp(cmd, "uninstall") == 0 ){
        printf("USAGE: boiler uninstall BOILERPLATE_NAME [-x] \n");
    } else if (strcmp(cmd, "edit") == 0 ){
        printf("USAGE: boiler edit BOILERPLATE_NAME\n");
    }     
    return true;
}
 

// prints error to stderrr.
void printutils::print_error(char const* fmt, ...) {
     va_list argList;
    va_start(argList, fmt);

    printf(KRED);
    vprintf(fmt,argList);
    printf(RESET);

    va_end(argList);
}

void printutils::print_prompt(char const * fmt, ...){
     va_list argList;
    va_start(argList, fmt);

    printf("%s%s", BOLD, KWHT);
    vprintf(fmt,argList);
    printf(RESET);

    va_end(argList);

}
void printutils::print_header(char const * fmt, ...){
     va_list argList;
    va_start(argList, fmt);

    printf("%s%s", BOLD, KCYN);
    vprintf(fmt,argList);
    printf(RESET);

    va_end(argList);

}