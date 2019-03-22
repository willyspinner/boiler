#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <iostream>
#include <vector>
#include <stdexcept>
#include <nlohmann/json.hpp>

typedef struct {
    char* name; // name of boilerplate item.
    char* description; // description of the boilerplate.
    char* boilerpath; // location relative to $BOILERDIR/boilerplates.
} boiler_item;

class ConfigFile {
    public:
        // constructors.
        ConfigFile();
        ConfigFile(char* boiler_conf_filepath, bool is_first_time = false) throw(std::runtime_error); // parse a boiler config file, or create new one if first time.
        // ^ should throw exception if not possible.
        ~ConfigFile(); // don't forget to call this!

        void add_boiler_item (boiler_item& bi);
        nlohmann::json* get_json() ;
        bool save_contents(); // save to BOILERDIR/boiler.conf
    private:
        char* m_boiler_conf_filepath;
        nlohmann::json m_configfile_json;

};

#endif

