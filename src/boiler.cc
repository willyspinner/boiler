#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>

#include "configfile.h"
#include "printutils.h"
#include "input.h"

const int EXIT_NOT_FOUND = 3;
const int EXIT_SYSCALL_FAILURE = 2;
const int EXIT_BAD_ARG = 1;
const int EXIT_OK = 0;

int main (int argc, char* argv[]) {
    if (argc == 1) {
        printutils::print_usage();
    }
    if (argc < 2) {
        if(!printutils::print_command_usage(argv[1])) {
            printutils::print_usage();
        }
        return EXIT_BAD_ARG;
    }
    char* BOILERDIR = getenv("BOILERDIR");
    if (BOILERDIR == NULL) {
        printutils::print_error("Please declare the environment variable BOILERDIR. This is where we will save all the boilerplates.\n");
        return EXIT_OK;
    }
    char boiler_conf_path[strlen(BOILERDIR) + strlen("/boiler.conf") + 1];
    sprintf(boiler_conf_path, "%s/boiler.conf", BOILERDIR);

    configfile::ConfigFile cfg_file;
    try {
        cfg_file = configfile::ConfigFile(boiler_conf_path);
    } catch(std::runtime_error* er) {
        printutils::print_error("Cannot read configfile: %s", strerror(errno));
        return EXIT_SYSCALL_FAILURE;
    }

    char* boilerplate_name = argv[2];
    char* command_str = argv[1];
    int command_str_len = strlen(command_str);
    for( int i = 0; i < command_str_len; ++i) {
        command_str[i] = tolower(command_str[i]);
    }

    std::vector<configfile::boiler_item>* boilerplates = cfg_file.get_boiler_list();
    if (strcmp(command_str, "list") == 0 || strcmp(command_str, "ls") == 0) {
        for(auto& boilerplate: *boilerplates) {
            printutils::print_boilerplate_entry(boilerplate);
        }
        return EXIT_OK;

    } else if (strcmp(command_str, "show") == 0) {
        for(auto& boilerplate: *boilerplates) {
            if (strcmp(boilerplate_name, boilerplate.name) == 0) {
                char whole_path[strlen(boilerplate.boilerpath) + strlen(BOILERDIR) + 1];
                sprintf(whole_path, "%s/%s", BOILERDIR, boilerplate.boilerpath);
                int boiler_file_fd = open(whole_path, O_RDONLY);
                if (boiler_file_fd == -1) {
                    printutils::print_error("couldn't open boilerplate file for reading: %s", strerror(errno));
                    return EXIT_SYSCALL_FAILURE;
                }
                char read_buf[4096];
                int res;
                while((res = read(boiler_file_fd, read_buf, 4096)) != 0) {
                    if (res == -1) {
                        close(boiler_file_fd);
                        printutils::print_error("boilerplate file read error: %s", strerror(errno));
                        return EXIT_SYSCALL_FAILURE;
                    }
                    printf("%s", read_buf);
                }
                close(boiler_file_fd);
                return EXIT_OK;
            }
        }
        printutils::print_error("boilerplate %s not found.\n", boilerplate_name);
        return EXIT_NOT_FOUND;

    } else if (strcmp(command_str, "edit") == 0) {
        for(auto& boilerplate: *boilerplates) {
            if (strcmp(boilerplate_name, boilerplate.name) == 0) {
                char whole_path[strlen(boilerplate.boilerpath) + strlen(BOILERDIR) + 1];
                sprintf(whole_path, "%s/%s", BOILERDIR, boilerplate.boilerpath);
                int denied = access(whole_path, R_OK | W_OK);
                if (denied) {
                    printutils::print_error("Error in accessing %s: %s",whole_path, strerror(errno));
                }
                char* visual = getenv("VISUAL");
                if (visual == NULL) {
                    visual = input::prompt_string_answer("VISUAL is not defined. Please input your desired text editor to edit the boilerplate.");
                }
                char* execvp_argv[2] = {whole_path, NULL};
                execvp(visual, execvp_argv);
                printutils::print_error("Error in editing with %s: %s", visual, strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            }
        }
            printutils::print_error("boilerplate %s not found.\n", boilerplate_name);
            return EXIT_NOT_FOUND;

    } else {
        printutils::print_error("unrecognised command %s. ", command_str);
        return EXIT_BAD_ARG;
    }
    // TODO: the below will be done later.
    /*
    static struct option long_options[] = {
    {"info", required_argument, NULL, 'i'},
    {0, 0, 0, 0}
    };
    int c, counter;
    while((c = getopt_long(argc, argv, "iul", long_options, &counter)) != -1) {
        switch (c) {
            case '?':
                return EXIT_BAD_ARG;
            case ':':
                return EXIT_BAD_ARG;
        }
    }
     */
    //} else if (strcmp(command_str, "install") == 0) {
    //} else if (strcmp(command_str, "uninstall") == 0) {
}
