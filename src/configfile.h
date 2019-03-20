#ifndef JSONPARSER_H
#define JSONPARSER_H
/* TODO: make a json structure that we can parse our json file to. */
typedef struct {
    char* name;
    char* location;
} boiler_item;

class ConfigFile {
    public: 
        // constructors.
        ConfigFile(); // new configfile
        ConfigFile(char* boiler_conf_file); // parse a boiler config file (default in BOIERPLATESDIR/boiler.conf)
        ~ConfigFile();

        // list of boilerplates.
        std::vector<boiler_item*> boiler_list;
        bool save_contents(char* boiler_conf_file);
}

#endif

