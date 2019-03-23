#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>
#include <iostream>
#include <stdexcept>
#include <sstream>      // std::ostringstream
#include "nlohmann_json/single_include/nlohmann/json.hpp"

#include "configfile.h"
#include "printutils.h"
#include "input.h"
#include "errors.h"

const int EXIT_BDIR_NOT_FOUND = 4;
const int EXIT_NOT_FOUND = 3;
const int EXIT_SYSCALL_FAILURE = 2;
const int EXIT_GEN_FAILURE = 1; // general failure
const int EXIT_BAD_ARG = 1;
const int EXIT_OK = 0;

static struct option long_options[] = {
{"name", required_argument, NULL, 'n'},
{"description", required_argument, NULL, 'd'},
{"soft", no_argument, NULL, 's'},
{"hard", no_argument, NULL, 'h'},
{"cached", no_argument, NULL, 'c'},
{0, 0, 0, 0}
};

std::string get_boilerplate_path(const char* boilerdir, const char * relative_path) {
    //relative_path is relative to BOILERDIR/boilerplates
    std::ostringstream oss(std::ostringstream::ate); 
    oss << boilerdir << "/boilerplates/" << relative_path;
    return oss.str();
}
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

    char boilerconfig_json_path[strlen(BOILERDIR) + strlen("/boilerconfig.json") + 1];
    sprintf(boilerconfig_json_path, "%s/boilerconfig.json", BOILERDIR);

    ConfigFile cfg_file;
    try {
        cfg_file = ConfigFile(boilerconfig_json_path);
    } catch(std::runtime_error& er) {
        if ( errno == ENOENT) {
            printutils::print_error("read boilerconfig.json: %s.\n", strerror(errno));
            printf("boilerconfig.json doesn't exist. Creating a new boilerconfig.json.....");
            // create a new one.
            cfg_file = ConfigFile(boilerconfig_json_path, true);
            bool save_ok = cfg_file.save_contents();
            if(!save_ok) {
                printutils::print_error("\nCould not save boilerconfig.json. %s", strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            }
            printf("done. \n");
        } else if ( cfg_errno == EINVALIDJSON) {
            printutils::print_error("invalid boilerconfig.json structure. Please fix it.\n");
            return EXIT_GEN_FAILURE;
        } else if ( cfg_errno == EEMPTY) {
            bool can_overwrite = input::prompt_bool_answer("I detected an empty boilerconfig.json file. Can I overwrite it?");
            if (!can_overwrite) {
                printutils::print_error("Please fill up the boilerconfig.json file. \n");
                return EXIT_GEN_FAILURE;
            }
            printf("OK. Overwriting...\n");
            cfg_file = ConfigFile(boilerconfig_json_path, true);
            bool overwrite_ok = cfg_file.save_contents();
            if(!overwrite_ok) {
                printutils::print_error("Failed to overwrite: %s. \n", strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            }
        } else {
            return EXIT_SYSCALL_FAILURE;
        }
        return EXIT_OK;
    }

    int c, counter;
    std::string new_name = "";
    char link_mode = '\0';
    std::string new_description = "";
    bool is_cached = false;
    //NOTE: we are doing this because getopt_long shuffles argv.
    char* argv_temp[argc];
    for(int i = 0; i < argc; ++i) {
        argv_temp[i] = argv[i];
    }
    while((c = getopt_long(argc, argv_temp, "d:n:hsc", long_options, &counter)) != -1) {
        switch (c) {
            case 's': case 'h':
                link_mode = c;
                break;
            case 'n':
                new_name = std::string(optarg);
                break;
            case 'd':
                new_description = std::string(optarg);
                break;
            case 'c':
                is_cached  = true;
                break;
            case '?':
                return EXIT_BAD_ARG;
            case ':':
                return EXIT_BAD_ARG;
        }
    }
    //TODO: can't use argv anymore..
    char* command_str = argv[1];
    int command_str_len = strlen(command_str);
    for( int i = 0; i < command_str_len; ++i) {
        command_str[i] = tolower(command_str[i]);
    }


    //TODO: is this the best way to get the json data? Nah. it's not.
    // please just make an iterator?
    nlohmann::json* cfg_json = cfg_file.get_json();
    if (cfg_json == NULL) {
        printutils::print_error("get_json() fatal failure. Exiting..\n");
        return EXIT_SYSCALL_FAILURE;
    }

    auto boilerplates = ((*cfg_json)["boilerplates"]);

    if (strcmp(command_str, "list") == 0 || strcmp(command_str, "ls") == 0) {
        printutils::print_header("list of boilerplates: \n");
        int list = 0;
        std::string name, boilerpath, description;
        for(auto& boilerplate: boilerplates) {
            const boiler_item bti {
                std::string(boilerplate["name"].get<std::string>()),
                std::string(boilerplate["description"].get<std::string>()),
                std::string(boilerplate["boilerpath"].get<std::string>())
            };
            printutils::print_boilerplate_entry(bti);
            list++;
        }
        if (list == 0) {
            printf("none.\n");
        }
        return EXIT_OK;

    } else if (strcmp(command_str, "show") == 0) {
        if(argc < 3) {
            printutils::print_command_usage("show");
            return EXIT_BAD_ARG;
        }
        //TODO: can't use argv anymore..
        char* boilerplate_name = argv[2];
        for(auto& boilerplate: boilerplates) {
            std::string name = boilerplate["name"].get<std::string>();
            std::string boilerpath = boilerplate["boilerpath"].get<std::string>();
            if (strcmp(boilerplate_name, name.c_str()) == 0) {
                std::string whole_path = get_boilerplate_path(BOILERDIR, boilerpath.c_str());
                int boiler_file_fd;
                if ((boiler_file_fd = open(whole_path.c_str(), O_RDONLY)) == -1) {
                    printutils::print_error("couldn't open boilerplate file %s for reading: %s", whole_path.c_str(), strerror(errno));
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
        if(argc < 3) {
            printutils::print_command_usage("edit");
            return EXIT_BAD_ARG;
        }
        //TODO: can't use argv anymore..
        char* boilerplate_name = argv[2];
        for(auto& boilerplate: boilerplates) {
            std::string name = std::string(boilerplate["name"].get<std::string>());
            std::string boilerpath= std::string(boilerplate["boilerpath"].get<std::string>());
            if (strcmp(boilerplate_name, name.c_str()) == 0) {
                std::string whole_path = get_boilerplate_path(BOILERDIR, boilerpath.c_str());
                int denied = access(whole_path.c_str(), R_OK | W_OK);
                if (denied) {
                    printutils::print_error("Error in accessing %s: %s",whole_path.c_str(), strerror(errno));
                    return EXIT_GEN_FAILURE;
                }
                //std::string visual(getenv("VISUAL"));
                std::string visual;
                char *visual_cstr = getenv("VISUAL");
                if (visual_cstr == NULL) {
                    visual = input::prompt_string_answer("VISUAL is not defined. Please input your desired text editor to edit the boilerplate: ");
                } else {
                    visual = std::string(visual_cstr);
                }
                execlp(visual.c_str(), visual.c_str(), whole_path.c_str(), NULL);
                printutils::print_error("Error: program %s %s", visual.c_str(), strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            }
        }
            printutils::print_error("boilerplate \"%s\" not found.\n", boilerplate_name);
            return EXIT_NOT_FOUND;
    } else if (strcmp(command_str, "install") == 0 || strcmp(command_str, "i") == 0 || strcmp(command_str, "add" ) == 0) {
        if (argc < 3) {
            printutils::print_command_usage("install");
            return EXIT_BAD_ARG;
        }
    //TODO: can't use argv anymore..
        char* boilerplate_file = argv[2]; // bounds checking ok?
        int denied = access(boilerplate_file, R_OK);
        if (denied) {
            printutils::print_error("Error in accessing %s: %s", boilerplate_file, strerror(errno));
        }
        if (new_name.empty()) {
            new_name = input::prompt_string_answer("Please input new name of the boilerplate: ");
        }
        if (new_description.empty()) {
            new_description = input::prompt_string_answer("Please input description of the boilerplate: ");
        }
        std::string b_filename(basename(boilerplate_file));
        std::string destination = get_boilerplate_path(BOILERDIR, b_filename.c_str());

        const boiler_item bti = {new_name, new_description, b_filename};
        bool no_duplicates = cfg_file.add_boiler_item(bti);
        if(!no_duplicates) {
            printutils::print_error("Error: boilerplate with name \"%s\" already exists. \n", new_name.c_str());
            return EXIT_GEN_FAILURE;
        }
        printf("copying %s to %s/boilerplates...\n", boilerplate_file, BOILERDIR);
        if(link_mode == 's') {
            printf("symlinking... \n");
            std::string boilerplate_filepath;
            if( argv[2][0] == '/') {
                // absolute path
                boilerplate_filepath = std::string(argv[2]);
            } else {
                // relative to current cwd.
                char* cwd = getcwd(NULL, 0);
                std::ostringstream oss_bpf(std::ostringstream::ate); 
                oss_bpf<< cwd<< "/" << boilerplate_file;
                boilerplate_filepath = std::string(oss_bpf.str());
                free(cwd);
            }
            if (symlink(boilerplate_filepath.c_str(), destination.c_str()) == -1) {
                printutils::print_error("Error in symlinking %s: %s\n", boilerplate_file, strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            } 
        }else if (link_mode == 'h') {
            //TODO: BUG: if we hard link to boilerconfig.json itself, it doesn't become a hardlinkb ecause we overwrite it. Why? It's really wierd.
            printf("hardlinking.. .\n");
            if (link(boilerplate_file, destination.c_str()) == -1) {
                printutils::print_error("Error in hard-linking %s: %s\n", boilerplate_file, strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            } 
        } else {
            //SUGGESTION: maybe wrap this reading code somewhere.. in a utils file?
            int destination_fd, boilerplate_fd;
            if ((boilerplate_fd = open(boilerplate_file, O_RDONLY)) == -1) {
                printutils::print_error("couldn't open boilerplate source file %s for reading: %s", boilerplate_file, strerror(errno));
                return EXIT_SYSCALL_FAILURE;
            }
            if ((destination_fd= open(destination.c_str(), O_WRONLY| O_CREAT| O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP| S_IROTH)) == -1) {
                printutils::print_error("couldn't open destination file %s for writing: %s", destination.c_str(), strerror(errno));
                close(boilerplate_fd);
                return EXIT_SYSCALL_FAILURE;
            }
            char read_buf[4096];
            int res;
            while((res = read(boilerplate_fd, read_buf, 4096)) != 0) {
                if (res == -1) {
                    close(boilerplate_fd);
                    close(destination_fd);
                    printutils::print_error("boilerplate file read error: %s", strerror(errno));
                    return EXIT_SYSCALL_FAILURE;
                }
                if(write(destination_fd, read_buf, 4096) == -1) {
                    close(boilerplate_fd);
                    close(destination_fd);
                    printutils::print_error("destinaiton file write error: %s", strerror(errno));
                    return EXIT_SYSCALL_FAILURE;
                }
            }
            close(boilerplate_fd);
            close(destination_fd);
        }
        
        //now , save the contents.
        bool contents_saved = cfg_file.save_contents();
        if (!contents_saved) {
            unlink(destination.c_str());
            printutils::print_error("Error in saving %s: %s\n", boilerplate_file, strerror(errno));
            return EXIT_SYSCALL_FAILURE;
        }
        printf("boilerplate \"%s\" saved!\n", new_name.c_str());
        
    } else if (strcmp(command_str, "uninstall") == 0 || strcmp(command_str, "r") == 0 || strcmp(command_str, "remove" ) == 0 || strcmp(command_str, "rm") == 0) {
        if(argc < 3) {
            printutils::print_command_usage("uninstall");
            return EXIT_BAD_ARG;
        }
        //TODO: can't use argv anymore..
        char* boilerplate_name = argv[2];
        std::string name, boilerpath;
        for(auto& boilerplate: boilerplates) {
            name = std::string(boilerplate["name"].get<std::string>());
            if (strcmp(boilerplate_name, name.c_str()) == 0) {
                bool success = cfg_file.remove_boiler_item(name);
                if(success) {
                    boilerpath = std::string(boilerplate["boilerpath"].get<std::string>());
                    std::string whole_path = get_boilerplate_path(BOILERDIR, boilerpath.c_str());
                    if(!is_cached) {
                        unlink(whole_path.c_str());
                    }
                    bool save_ok = cfg_file.save_contents(); 
                    if(!save_ok) {
                        printutils::print_error("Could not update boilerconfig.json. %s", strerror(errno));
                        return EXIT_SYSCALL_FAILURE;
                    }
                    printf("Successfully uninstalled %s\n", name.c_str());
                    return EXIT_OK;
                } else {
                    printutils::print_error("cannot remove boilerplate %s from configfile. Not found in boilerconfig.json.", name.c_str());
                    return EXIT_GEN_FAILURE;
                }
            }
        }
        printutils::print_error("boilerplate \"%s\" not found.\n", boilerplate_name);
        return EXIT_NOT_FOUND;
    } else {
        printutils::print_error("unrecognized command \"%s\".\n", command_str);
        return EXIT_BAD_ARG;
    }
    return EXIT_OK;
}