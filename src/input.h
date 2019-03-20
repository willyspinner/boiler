#ifndef INPUT_H
#define INPUT_H

/*
 * These methods prompt the user for an answer. The prompt_bool_answer won't stop until a correct answer is given (yes,no, y, n, ok, nah, etc.)
 */
namespace input {
    bool prompt_bool_answer(char const* question); // e.g. are you sure?

    /* won't accept a "" as an answer. NOTE: this will dynamically allocate the string. */
    char* prompt_string_answer(char const* question);
}
#endif
