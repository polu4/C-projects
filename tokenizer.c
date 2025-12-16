/* 
Author: Luke Poley adapted from  Jeff Ondich's tokenizer.h
2 October 2025 
tokenizer.c returns the number of tokens in s, where tokens are substrings separated by delimiter. Adjacent delimiters produce empty tokens.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_token_count(const char *s, char delimiter) {
    if (s == NULL) {
        return 0;
    }

    if (s[0] == '\0') {
        return 0;
    }

    int count = 1;  // at least one token if string is non-empty

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == delimiter) {
            count++;
        }
    }

    return count;
}

/*
    Separates s into tokens. Allocates memory for the token array
    and for each token.
*/
char **get_tokens(const char *s, char delimiter, int *token_count) {
    if (s == NULL || token_count == NULL) {
        return NULL;
    }

    *token_count = get_token_count(s, delimiter);

    char **tokens = malloc((*token_count) * sizeof(char *));
    if (tokens == NULL) {
        return NULL;
    }

    int token_index = 0;
    const char *start = s;

    while (token_index < *token_count) {
        const char *end = start;

        while (*end != '\0' && *end != delimiter) {
            end++;
        }

        size_t len = end - start;
        tokens[token_index] = malloc(len + 1);

        if (tokens[token_index] == NULL) {
            // clean up previously allocated memory
            for (int i = 0; i < token_index; i++) {
                free(tokens[i]);
            }
            free(tokens);
            return NULL;
        }

        strncpy(tokens[token_index], start, len);
        tokens[token_index][len] = '\0';

        token_index++;
        start = end + 1;  // skip delimiter
    }

    return tokens;
}

/*
    Frees all memory allocated by get_tokens.
*/
void free_tokens(char **tokens, int token_count) {
    if (tokens == NULL) {
        return;
    }

    for (int i = 0; i < token_count; i++) {
        if (tokens[i] != NULL) {
            free(tokens[i]);
        }
    }

    free(tokens);
}

/*
    Prints tokens for debugging.
*/
void print_tokens(char **tokens, int token_count) {
    if (tokens == NULL || token_count <= 0) {
        return;
    }

    for (int i = 0; i < token_count; i++) {
        printf("%s\n", tokens[i]);
    }
}
