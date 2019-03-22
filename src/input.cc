#include "input.h"
#include "printutils.h"
#include <iostream>
#include <string>

bool input::prompt_bool_answer(char const* question) {
    while(true) {
        printutils::print_prompt("%s (yes/no): ", question);
        std::string answer;
        std::getline(std::cin, answer);
        if (answer.compare("yes") == 0 || answer.compare("y")  == 0) {
            return true;
        } else if (answer.compare("no") == 0 || answer.compare("n")  == 0) {
            return false;
        }
        printutils::print_error("Bad answer. Please try again (yes/no): ");
    }
}

/* won't accept a "" as an answer. */
std::string input::prompt_string_answer(char const* question) {
        printutils::print_prompt("%s", question);
        std::string answer;
        std::getline(std::cin, answer);
        return answer;
}