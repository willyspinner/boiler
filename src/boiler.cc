#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include "configfile.h"
#include "printutils.h"
#include "input.h"

const int EXIT_BDIR_NOT_FOUND = 4;
const int EXIT_NOT_FOUND = 3;
const int EXIT_SYSCALL_FAILURE = 2;
const int EXIT_GEN_FAILURE = 1; // general failure
const int EXIT_BAD_ARG = 1;
const int EXIT_OK = 0;

int main (int argc, char* argv[]) {
    char* BOILERDIR = getenv("BOILERDIR");
    if (BOILERDIR == NULL) {
        printutils::print_error("Please declare the environment variable BOILERDIR. This is where we will save all the boilerplates.\n");
        return EXIT_BDIR_NOT_FOUND;
    }
    if (argc == 1) {
        printutils::print_usage();
        return EXIT_BAD_ARG;
    }
    if (argc < 2) {
        if(!printutils::print_command_usage(argv[1])) {
            printutils::print_usage();
        }
        return EXIT_BAD_ARG;
    }
    char boilerconfig_json_path[strlen(BOILERDIR) + strlen("/boilerconfig.json") + 1];
    sprintf(boilerconfig_json_path, "%s/boilerconfig.json", BOILERDIR);

    ConfigFile cfg_file;
    try {
        cfg_file = ConfigFile(boilerconfig_json_path);
    } catch(std::runtime_error& er) {
        printutils::print_error("read boilerconfig.json: %s.\n", strerror(errno));
        if ( errno == ENOENT) {
            printf("boilerconfig.json doesn't exist. Creating a new boiler.conf.....");
            // create a new one.
            cfg_file = ConfigFile(boilerconfig_json_path, true);
            cfg_file.save_contents();
            printf("done. \n");

        } else if ( errno == EFTYPE) {
            //this errno value means that the file exists, is writeable, but has bad format.
            // our constructor sets this as errno.
            bool can_overwrite = input::prompt_bool_answer("I detected a bad/empty boilerconfig.json file. Can I overwrite it?");
            if (!can_overwrite) {
                printutils::print_error("Please check your boilerconfig.json and fix it. \n");
                return EXIT_GEN_FAILURE;
            }
            printf("OK. Overwriting...\n");
            cfg_file.save_contents();
            return EXIT_OK;
        } else {
            return EXIT_SYSCALL_FAILURE;
        }
    }

    char* boilerplate_name = argv[2];
    char* command_str = argv[1];
    int command_str_len = strlen(command_str);
    for( int i = 0; i < command_str_len; ++i) {
        command_str[i] = tolower(command_str[i]);
    }

    nlohmann::json* boilerplates = cfg_file.get_json();
    if (boilerplates == NULL) {
        //TODO: 
    }
    boilerplates = &((*boilerplates)["boilerplates"]);

    if (strcmp(command_str, "list") == 0 || strcmp(command_str, "ls") == 0) {
        printutils::print_header("list of boilerplates: \n");
        int list = 0;
        for(auto& boilerplate: *boilerplates) {
            printutils::print_boilerplate_entry(
                boilerplate["name"].get<std::string>().c_str(),
                boilerplate["boilerpath"].get<std::string>().c_str(),
                boilerplate["description"].get<std::string>().c_str()
            );
            list++;
        }
        if (list == 0) {
            printf("none.\n");
        }
        return EXIT_OK;

    } else if (strcmp(command_str, "show") == 0) {
        for(auto& boilerplate: *boilerplates) {
            // NOTE: the following is essential. 'get' 's pointer changes itself, so we have to strcpy.
            const char * temp_name = boilerplate["name"].get<std::string>().c_str();
            char name [strlen(temp_name)];
            strcpy(name, temp_name);
            const char * temp_bpath = boilerplate["boilerpath"].get<std::string>().c_str();
            char boilerpath [strlen(temp_bpath)];
            strcpy(boilerpath, temp_bpath);

            if (strcmp(boilerplate_name, name) == 0) {
                printf("yep\n");
                char whole_path[strlen(boilerpath) + strlen(BOILERDIR) + strlen("boilerplates") + 1];
                sprintf(whole_path, "%s/%s/%s", BOILERDIR, "boilerplates",  boilerpath);
                int boiler_file_fd;
                if ((boiler_file_fd = open(whole_path, O_RDONLY)) == -1) {
                    printutils::print_error("couldn't open boilerplate file %s for reading: %s", whole_path, strerror(errno));
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
        printutils::print_error("boilerplate \"%s\" not found.\n", boilerplate_name);
        return EXIT_NOT_FOUND;

    } else if (strcmp(command_str, "edit") == 0) {
        for(auto& boilerplate: *boilerplates) {
            const char * temp_name = boilerplate["name"].get<std::string>().c_str();
            char name [strlen(temp_name)];
            strcpy(name, temp_name);
            const char * temp_bpath = boilerplate["boilerpath"].get<std::string>().c_str();
            char boilerpath [strlen(temp_bpath)];
            strcpy(boilerpath, temp_bpath);
            if (strcmp(boilerplate_name, name) == 0) {
                char whole_path[strlen(boilerpath) + strlen(BOILERDIR) + strlen("boilerplates") + 1];
                sprintf(whole_path, "%s/%s/%s", BOILERDIR, "boilerplates",  boilerpath);
                int denied = access(whole_path, R_OK | W_OK);
                if (denied) {
                    printutils::print_error("Error in accessing %s: %s",whole_path, strerror(errno));
                }
                char* visual = getenv("VISUAL");
                if (visual == NULL) {
                    visual = input::prompt_string_answer("VISUAL is not defined. Please input your desired text editor to edit the boilerplate: ");
                }
                execlp(visual, visual, whole_path, NULL);
                printutils::print_error("Error in editing with %s: %s", visual, strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            }
        }
            printutils::print_error("boilerplate \"%s\" not found.\n", boilerplate_name);
            return EXIT_NOT_FOUND;
    } else {
        printutils::print_error("unrecognised command \"%s\".\n", command_str);
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
