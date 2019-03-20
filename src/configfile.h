#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <iostream>
#include <vector>
#include <stdexcept>
namespace configfile {
    typedef struct {
        char* name; // name of boilerplate item.
        char* description; // description of the boilerplate.
        char* boilerpath; // location relative to $BOILERDIR/boilerplates.
        bool is_single_file; // either a directory, or single file.
    } boiler_item;

    class ConfigFile {
        public:
            // constructors.
            ConfigFile(); // new configfile
            ConfigFile(char* boiler_conf_file) throw(std::runtime_error); // parse a boiler config file (default in BOIERPLATESDIR/boiler.conf)
            // ^ should throw exception if not possible.
            ~ConfigFile(); // don't forget to call this!

            std::vector<boiler_item>* get_boiler_list() const;
            bool save_contents(char* boiler_conf_file);
        private:
            // list of boilerplates.
            std::vector<boiler_item> boiler_list;
    };
}

#endif

