#ifndef __SOARE_TOKENIZER_H__
#define __SOARE_TOKENIZER_H__ 0x1

/* #pragma once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <tokenizer.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

/**
 * @brief List the different types of tokens
 */
typedef enum token_type
{

    TKN_EOF,
    TKN_NAME,
    TKN_NUMBER,
    TKN_STRING,
    TKN_PARENL,
    TKN_PARENR,
    TKN_ARRAYL,
    TKN_ARRAYR,
    TKN_ASSIGN,
    TKN_KEYWORD,
    TKN_OPERATOR,
    TKN_SEMICOLON

} token_type;

/**
 * @brief Structure of a token
 */
typedef struct Tokens
{

    // Value
    char *value;
    // Type
    token_type type;

    // Document
    Document file;

    // Next
    struct Tokens *next;

} Tokens;

/**
 * @brief Set the Environment object
 *
 * @param path
 */
void SetEnvironment(char *path);

/**
 * @brief Get the Environment object
 *
 * @return char*
 */
char *GetEnvironment(void);

/**
 * @brief Return an empty document
 *
 * @return Document
 */
Document EmptyDocument(void);

/**
 * @brief Create a new token
 *
 * @param filename
 * @param value
 * @param type
 * @return Tokens*
 */
Tokens *Token(char *__restrict__ filename, char *__restrict__ value, token_type type);

/**
 * @brief Move on to the next token
 *
 * @param tokens
 * @param step
 */
void TokenNext(Tokens **tokens, unsigned int step);

/**
 * @brief Check if a sequence of tokens corresponds with a sequence of token types
 *
 * @param tokens
 * @param iteration
 * @param ...
 * @return unsigned char
 */
unsigned char TokensFollowPattern(Tokens *tokens, unsigned int iteration, ...);

/**
 * @brief Free the memory allocated by the tokens
 *
 * @param token
 */
void TokensFree(Tokens *token);

/**
 * @brief Display the tokens
 *
 * @param token
 */
void TokensLog(Tokens *token);

/**
 * @brief Transform a string into a sequence of tokens
 *
 * @param filename
 * @param text
 * @return Tokens*
 */
Tokens *Tokenizer(char *__restrict__ filename, char *__restrict__ text);

#endif /* __SOARE_TOKENIZER_H__ */