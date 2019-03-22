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
#include <streambuf>
#include <fstream>

ConfigFile::~ConfigFile() {

}
ConfigFile::ConfigFile() {

}

ConfigFile::ConfigFile(char* boiler_conf_filepath, bool is_first_time) throw (std::runtime_error) {
    if( is_first_time ) {
        m_configfile_json = {
            { "boilerplates",nlohmann::json::array({})}
        };
        return;
    }
    // open boiler_conf_filepath.
    m_boiler_conf_filepath = boiler_conf_filepath;
    std::ifstream t(m_boiler_conf_filepath);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    m_configfile_json = nlohmann::json::parse(str, nullptr, false);
    if (m_configfile_json.is_discarded()) {
        errno =EFTYPE;
        throw std::runtime_error("Invalid json.");
    }
}


bool ConfigFile::add_boiler_item (const boiler_item& bi) {
    // first, check if already present.
    auto& boilerplates = m_configfile_json["boilerplates"];
    for(auto& boilerplate: boilerplates) {
        if(boilerplate["name"].get<std::string>().compare(std::string(bi.name)) == 0) {
            //already exists.
            return false;
        }
    }
    m_configfile_json["boilerplates"].push_back({
        {"name", bi.name},
        {"description", bi.description},
        {"boilerpath", bi.boilerpath},
    });
    return true;
}
// if found and able to be removed, return true. Else false.
bool ConfigFile::remove_boiler_item (std::string& boilerplate_name) {
    auto& boilerplates = m_configfile_json["boilerplates"];
    int pos = 0;
    for(auto& boilerplate: boilerplates) {
        if(boilerplate["name"].get<std::string>().compare(boilerplate_name) == 0) {
            //already exists.
            boilerplates.erase(pos);
            return true;
        }
        pos++;
    }
    return false;
}

bool ConfigFile::save_contents() { // save to BOILERDIR/boilerconfig.json
   // stringify json and save the file. 
   char* json_str = (char* ) m_configfile_json.dump().c_str();
   fprintf(stderr, "DEBUG: dumping json: %s\n", json_str);
   int json_str_len = strlen(json_str);

    // put stuff to swapfile first.
    char temp_filepath [strlen(m_boiler_conf_filepath) + strlen(".swp") + 1];
    sprintf(temp_filepath, "%s.swp", m_boiler_conf_filepath);
    int temp_fd;
    if((temp_fd = open(temp_filepath, O_WRONLY | O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR | S_IRGRP |S_IROTH )) == -1) {
        return false;
    }

    if(write(temp_fd, json_str, json_str_len) == -1) {
        close(temp_fd);
        unlink(temp_filepath);
        // could not write/
        return false;
    }
    close(temp_fd);
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