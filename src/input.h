#ifndef INPUT_H
#define INPUT_H

/*
 * These methods prompt the user for an answer. The prompt_bool_answer won't stop until a correct answer is given (yes,no, y, n, ok, nah, etc.)
 */
bool prompt_bool_answer(char* question); // e.g. are you sure?

/* won't accept a "" as an answer. */
void prompt_string_answer(char* question, char* answer, size_t max_len); // e.g. are you sure?
#endif
