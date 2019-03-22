#ifndef PRINTUTILS_H 
#define PRINTUTILS_H
#include "configfile.h"

namespace printutils {

    // print each boilerplate entry. name, location, description.
    void print_boilerplate_entry(const boiler_item& bti);

    void print_usage();

    // checks if is an actual command. If it is, print usage of it, and return true.
    bool print_command_usage(char const* cmd);

    // prints error to stderrr.
    void print_error(char const* fmt, ...);

    void print_prompt(char const* fmt, ...);

    void print_header(char const* fmt, ...);

}

#endif
