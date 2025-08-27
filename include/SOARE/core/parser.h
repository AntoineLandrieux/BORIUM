#ifndef __SOARE_PARSER_H__
#define __SOARE_PARSER_H__ 0x1

/* #pragma once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <parser.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

/**
 * @brief List the different types of nodes
 */
typedef enum node_type
{

    /**
     *
     * SOARE
     *
     */

    NODE_ROOT,
    NODE_TRY,
    NODE_BODY,
    NODE_RAISE,
    NODE_ARRAY,
    NODE_OUTPUT,
    NODE_VALUE,
    NODE_IMPORT,
    NODE_CALL,
    NODE_TYPE,
    NODE_INPUT,
    NODE_FUNCTION,
    NODE_MEMNEW,
    NODE_MEMGET,
    NODE_MEMSET,
    NODE_IFERROR,
    NODE_OPERATOR,
    NODE_CONDITION,
    NODE_REPETITION,
    NODE_BREAK,
    NODE_SHELL,
    NODE_RETURN,

    /**
     *
     * BORIUM
     *
     */

    NODE_BORIUM_CLEAR,
    NODE_BORIUM_COLOR,
    NODE_BORIUM_CURSOR,
    NODE_BORIUM_EDITOR,
    NODE_BORIUM_GETC,
    NODE_BORIUM_HELP,
    NODE_BORIUM_LICENSE,
    NODE_BORIUM_PAUSE,
    NODE_BORIUM_SETUP,
    NODE_BORIUM_SLEEP

} node_type;

/**
 * @brief Structure of a node
 */
typedef struct node
{

    // Value
    char *value;
    // Type
    node_type type;

    // Document
    Document file;

    // Node Parent
    struct node *parent;
    // Node Child
    struct node *child;
    // Node Sibling
    struct node *sibling;

} Node, *AST;

/**
 * @brief Create a new node
 *
 * @param value
 * @param type
 * @param file
 * @return Node*
 */
Node *Branch(char *value, node_type type, Document file);

/**
 * @brief Add a sibling branch
 *
 * @param source
 * @param element
 * @return AST
 */
AST BranchJuxtapose(Node *source, AST element);

/**
 * @brief Join 2 branches
 *
 * @param parent
 * @param child
 * @return AST
 */
AST BranchJoin(Node *parent, Node *child);

/**
 * @brief Frees the memory allocated by a tree
 *
 * @param tree
 */
void TreeFree(AST tree);

/**
 * @brief Display a tree
 *
 * @param tree
 */
void TreeLog(AST tree);

/**
 * @brief Turns a sequence of tokens into a tree (AST)
 *
 * @param tokens
 * @return AST
 */
AST Parse(Tokens *tokens);

#endif /* __SOARE_PARSER_H__ */