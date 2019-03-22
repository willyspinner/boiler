#include <iostream>
#include <vector>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "configfile.h"
#include <stdio.h>
#include <string>
#include <errno.h>

ConfigFile::~ConfigFile() {

}
ConfigFile::ConfigFile() {

}

ConfigFile::ConfigFile(char* boiler_conf_filepath, bool is_first_time) throw (std::runtime_error) {
    if( is_first_time ) {
        m_configfile_json = {
            { "boilerplates", {}}
        };
        return;
    }
    // open boiler_conf_filepath.
    int conf_fd;
    m_boiler_conf_filepath = boiler_conf_filepath;
    if((conf_fd = open(m_boiler_conf_filepath, O_RDONLY)) == -1) {
        printf("not found ting\n");
        throw std::runtime_error(strerror(errno));
    }
    struct stat st;
    if(stat(m_boiler_conf_filepath, &st) == -1) {
        throw std::runtime_error(strerror(errno));
    }
    size_t size = st.st_size;
    char configfile_str[st.st_size];
    if(read(conf_fd, configfile_str, size) == -1) {
        throw std::runtime_error(strerror(errno));
    }
    close(conf_fd);
    m_configfile_json = nlohmann::json::parse(configfile_str, nullptr, false);
    if (m_configfile_json.is_discarded()) {
        errno =EFTYPE;
        throw std::runtime_error("Invalid json.");
    }
}


void ConfigFile::add_boiler_item (boiler_item& bi) {
    m_configfile_json["boilerplates"].push_back({
        {"name", bi.name},
        {"description", bi.description},
        {"boilerpath", bi.boilerpath},
    });
}

bool ConfigFile::save_contents() { // save to BOILERDIR/boilerconfig.json
   // stringify json and save the file. 
   char* json_str = (char* ) m_configfile_json.dump().c_str();
   int json_str_len = strlen(json_str);

    // put stuff to swapfile first.
    char temp_filepath [strlen(m_boiler_conf_filepath) + strlen(".swp") + 1];
    sprintf(temp_filepath, "%s.swp", m_boiler_conf_filepath);
    int temp_fd;
    if((temp_fd = open(temp_filepath, O_WRONLY | O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR | S_IRGRP |S_IROTH )) == -1) {
        return false;
    }

    if(write(temp_fd, json_str, json_str_len) == -1) {
        unlink(temp_filepath);
        // could not write/
        return false;
    }
    // below is atomic, so it is fine.
   if(rename(temp_filepath, m_boiler_conf_filepath) == -1) {
        unlink(temp_filepath);
        return false;
   }
   return true;
}
nlohmann::json* ConfigFile::get_json() {
    return &m_configfile_json;
}