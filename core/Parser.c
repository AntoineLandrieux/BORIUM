#include <STD/stdlib.h>
#include <STD/stdarg.h>

#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Parser.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

/**
 *  Example of AST (Abstract Syntax Tree) :
 *
 *
 *          (node A)
 *          /
 *      [child]
 *        /
 *    (node B)-[sibling]-(node C)-[sibling]-(node D)
 *
 * Vocabulary:
 *
 * - NULL : Non-existent node
 *
 *  ---
 *
 * - parent : Direct parent of the node
 *
 * Example :
 *
 *  (node A) is the parent of (node B)
 *  (node A) is the parent of (node C)
 *  (node A) is the parent of (node D)
 *  NULL is the parent of (node A)
 *
 *  ---
 *
 * - child : Direct child of the node
 *
 * Example :
 *
 *  (node B) is the child of (node A)
 *  (node A) is the child of NULL
 *  (node C) is the child of NULL
 *  (node D) is the child of NULL
 *
 *  - sibling : Direct sibling of the node
 *
 * Example :
 *
 *  NULL is the sibling of (node A)
 *  NULL is the sibling of (node B)
 *  (node B) is the sibling of (node C)
 *  (node C) is the sibling of (node D)
 *  NULL is the sibling of (node D)
 *
 */

/**
 * @brief Create a new node
 *
 * @param value
 * @param type
 * @param file
 * @return Node*
 */
Node *Branch(char *value, node_type type, Document file)
{
    Node *branch = (Node *)malloc(sizeof(Node));

    if (!branch)
        return __SOARE_OUT_OF_MEMORY();

    branch->value = !value ? NULL : strdup(value);
    branch->type = type;
    branch->file = file;
    branch->parent = NULL;
    branch->child = NULL;
    branch->sibling = NULL;

    return branch;
}

/**
 * @brief Add a sibling branch
 *
 * @param source
 * @param element
 * @return AST
 */
AST BranchJuxtapose(Node *source, AST element)
{
    if (!source)
        return element;

    if (!element)
        return source;

    /**
     *
     *  source:
     *
     *    (a)
     *    /
     *  (b)-(c)
     *
     * element:
     *
     *    (d)
     *    /
     *  (e)-(f)
     *
     * returns:
     *
     *     (a)-(d)
     *    /     \
     *  (b)-(c) (e)-(f)
     *
     */

    AST tmp = source->sibling;
    source->sibling = element;

    for (; element->sibling; element = element->sibling)
    {
        element->parent = source->parent;
    }

    element->parent = source->parent;
    element->sibling = tmp;

    return source;
}

/**
 * @brief Join 2 branches
 *
 * @param parent
 * @param child
 * @return AST
 */
AST BranchJoin(Node *parent, Node *child)
{
    if (!child)
        return parent;

    if (!parent)
        return child;

    /**
     *
     *  parent:
     *
     *    (a)
     *    /
     *  (b)-(c)
     *
     * child:
     *
     *    (d)
     *    /
     *  (e)-(f)
     *
     * returns:
     *
     *     (a)
     *    /
     * (b)-(c)-(d)
     *         /
     *       (e)-(f)
     *
     */

    if (!parent->child)
    {
        parent->child = child;
    }

    else
    {
        Node *tmp = parent->child;

        while (tmp->sibling)
        {
            tmp = tmp->sibling;
        }

        tmp->sibling = child;
    }

    child->parent = parent;
    return parent;
}

/**
 * @brief Display a tree
 *
 * @param tree
 */
void TreeLog(AST tree)
{
    if (!tree)
        return;

    /**
     *
     * Example:
     *
     * [BRANCH] [(null):00000:00000, 00, "root"]
     * [BRANCH] [(null):00000:00000, 00, "root"]       [test.soare:00002:00001, 05, "write"]
     * [BRANCH] [test.soare:00002:00001, 05, "write"]  [test.soare:00002:00007, 07, "Hello World!"]
     *
     */

    if (tree->parent)
    {
        soare_write(tree->parent->value);
        soare_write(" -> ");
    }

    soare_write(tree->value);
    soare_write("\n");

    TreeLog(tree->child);
    TreeLog(tree->sibling);
}

/**
 * @brief Frees the memory allocated by a tree
 *
 * @param tree
 */
void TreeFree(AST tree)
{
    if (!tree)
        __asm__("nop");

    // TreeFree(tree->child);
    // TreeFree(tree->sibling);
    // free(tree->value);
    // free(tree);
}

/**
 * @brief Turns a sequence of tokens into a tree (AST)
 *
 * @param tokens
 * @return AST
 */
AST Parse(Tokens *tokens)
{
    Node *root = Branch(NULL, NODE_ROOT, EmptyDocument());
    Node *curr = root;

    while (tokens)
    {
        if (tokens->type == TKN_EOF)
            break;

        Tokens *old = tokens;
        tokens = tokens->next;

        switch (old->type)
        {
        case TKN_SEMICOLON:
            break;

        case TKN_KEYWORD:

            /**
             *
             *  CUSTOM SOARE KEYWORD FOR BORIUM
             *
             */

            if (!strcmp(old->value, KEYWORD_BORIUM_CLEAR))
            {
                BranchJoin(curr, Branch(NULL, NODE_BORIUM_CLEAR, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_COLOR))
            {
                BranchJoin(
                    curr,
                    BranchJoin(
                        Branch(NULL, NODE_BORIUM_COLOR, old->file),
                        ParseExpr(&tokens, 0xFF)));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_CURSOR))
            {
                BranchJoin(
                    curr,
                    BranchJoin(
                        Branch(NULL, NODE_BORIUM_CURSOR, old->file),
                        ParseExpr(&tokens, 0xFF)));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_EDITOR))
            {
                BranchJoin(curr, Branch(NULL, NODE_BORIUM_EDITOR, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_GETC))
            {
                if (tokens->type != TKN_NAME)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                BranchJoin(curr, Branch(tokens->value, NODE_BORIUM_GETC, old->file));
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_HELP))
            {
                BranchJoin(curr, Branch(NULL, NODE_BORIUM_HELP, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_LICENSE))
            {
                BranchJoin(curr, Branch(NULL, NODE_BORIUM_LICENSE, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_PAUSE))
            {
                BranchJoin(curr, Branch(NULL, NODE_BORIUM_PAUSE, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_SETUP))
            {
                BranchJoin(curr, Branch(NULL, NODE_BORIUM_SETUP, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_BORIUM_SLEEP))
            {
                BranchJoin(
                    curr,
                    BranchJoin(
                        Branch(NULL, NODE_BORIUM_SLEEP, old->file),
                        ParseExpr(&tokens, 0xFF)));
            }

            /**
             *
             * DEFAULT SOARE KEYWORD
             *
             */

            else if (*(old->value) == '$')
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                /**
                 *
                 *      \
                 *      (SHELL)
                 *         |
                 *       (code)
                 *
                 */

                BranchJoin(
                    //
                    curr,
                    BranchJoin(
                        //
                        Branch(old->value, NODE_SHELL, old->file),
                        content
                        //
                        )
                    //
                );
            }

            else if (!strcmp(old->value, KEYWORD_FN))
            {
                if (!TokensFollowPattern(tokens, 2, TKN_NAME, TKN_PARENL))
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                /**
                 *
                 *       \
                 *       (function)
                 *      /
                 *  (arg1)-(arg2)-(body)
                 *                   \...
                 *
                 */

                AST function = Branch(tokens->value, NODE_FUNCTION, old->file);
                TokenNext(&tokens, 2);
                BranchJoin(curr, function);

                while (1)
                {
                    if (tokens->type == TKN_PARENR)
                        break;

                    if (tokens->type != TKN_NAME)
                    {
                        TreeFree(root);
                        return LeaveException(SyntaxError, tokens->value, tokens->file);
                    }

                    BranchJoin(
                        //
                        function,
                        Branch(tokens->value, NODE_MEMSET, tokens->file)
                        //
                    );

                    TokenNext(&tokens, 1 + (tokens->next->type == TKN_SEMICOLON));
                }

                AST body = Branch(NULL, NODE_BODY, old->file);
                BranchJoin(function, body);
                tokens = tokens->next;
                curr = body;
            }

            else if (!strcmp(old->value, KEYWORD_BREAK))
            {
                /**
                 *
                 *     \
                 *     (break)
                 *
                 */
                BranchJoin(curr, Branch(old->value, NODE_BREAK, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_LET))
            {
                if (!TokensFollowPattern(tokens, 2, TKN_NAME, TKN_ASSIGN))
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                TokenNext(&tokens, 2);
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                /**
                 *
                 *     \
                 *     (varname)
                 *         |
                 *      (value)
                 */

                BranchJoin(
                    //
                    curr,
                    BranchJoin(
                        //
                        Branch(old->next->value, NODE_MEMNEW, old->file),
                        content
                        //
                        )
                    //
                );
            }

            else if (!strcmp(old->value, KEYWORD_RETURN))
            {

                /**
                 *
                 *     \
                 *     (return)
                 *         |
                 *      (value)
                 */

                BranchJoin(curr, BranchJoin(Branch(old->value, NODE_RETURN, old->file), ParseExpr(&tokens, 0xF)));
            }

            else if (!strcmp(old->value, KEYWORD_INPUT))
            {
                if (tokens->type != TKN_NAME)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                /**
                 *
                 *      \
                 *      (input)
                 *         |
                 *     (varname)
                 *
                 */

                BranchJoin(curr, Branch(tokens->value, NODE_INPUT, old->file));
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_LOADIMPORT) || !strcmp(old->value, KEYWORD_RAISE))
            {
                if (tokens->type != TKN_STRING)
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                /**
                 *
                 *      \
                 *    (loadimport)
                 *         |
                 *       (path)
                 *
                 */

                BranchJoin(curr, Branch(tokens->value, strcmp(old->value, KEYWORD_RAISE) ? NODE_IMPORT : NODE_RAISE, old->file));
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_TRY))
            {
                Node *try = Branch(old->value, NODE_TRY, old->file);
                BranchJoin(try, Branch(NULL, NODE_BODY, old->file));
                BranchJoin(curr, try);

                /**
                 *
                 *      \
                 *     (try)
                 *       |...
                 *
                 */

                curr = try->child;
            }

            else if (!strcmp(old->value, KEYWORD_IFERROR))
            {
                if (curr == root || curr->parent->type != NODE_TRY || curr->type == NODE_IFERROR)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                /**
                 *
                 *      /
                 *  (try)-(iferror)
                 *    |...    |...
                 *
                 */

                Node *iferror = Branch(old->value, NODE_IFERROR, old->file);
                BranchJoin(curr->parent, iferror);
                curr = iferror;
            }

            else if (!strcmp(old->value, KEYWORD_IF) || !strcmp(old->value, KEYWORD_WHILE))
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                if (tokens->type != TKN_KEYWORD || strcmp(tokens->value, "do"))
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                /**
                 *
                 *
                 *           (while/if)
                 *           /
                 *   (condition)-(body)
                 * .../             \...
                 *
                 */

                AST statement = Branch(old->value, strcmp(old->value, KEYWORD_IF) ? NODE_REPETITION : NODE_CONDITION, old->file);
                AST body = Branch(NULL, NODE_BODY, old->file);

                BranchJoin(statement, content);
                BranchJoin(statement, body);
                BranchJoin(curr, statement);

                curr = body;
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_OR))
            {
                if (curr->parent->type != NODE_CONDITION)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                if (tokens->type != TKN_KEYWORD || strcmp(tokens->value, "do"))
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                /**
                 *
                 *                /
                 *             (if)---------------(or)
                 *             /                  /
                 *   (condition)--(body) (condition)--(body)
                 *        |...       |...     |...       |...
                 *
                 */

                AST body = Branch(NULL, NODE_BODY, old->file);

                BranchJoin(curr->parent, content);
                BranchJoin(curr->parent, body);

                curr = body;
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_ELSE))
            {
                if (curr->parent->type != NODE_CONDITION)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                /**
                 *
                 *                /
                 *             (if)--------(else)
                 *             /            /
                 *   (condition)--(body)  (1)--(body)
                 *                   |...        |...
                 *
                 */

                AST body = Branch(NULL, NODE_BODY, old->file);

                BranchJoin(curr->parent, Branch("1", NODE_VALUE, old->file));
                BranchJoin(curr->parent, body);

                curr = body;
            }

            else if (!strcmp(old->value, KEYWORD_WRITE))
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                /**
                 *
                 *      \
                 *      (write)
                 *         |
                 *      (value)
                 *
                 */

                BranchJoin(curr, BranchJoin(Branch(old->value, NODE_OUTPUT, old->file), content));
            }

            else if (!strcmp(old->value, KEYWORD_END))
            {
                if (curr == root)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                curr = curr->parent->parent;
            }

            break;

        case TKN_NAME:

            if (tokens->type != TKN_PARENL)
            {
                if (tokens->type != TKN_ASSIGN)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                tokens = tokens->next;
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                {
                    TreeFree(root);
                    return LeaveException(ValueError, old->value, old->file);
                }

                /**
                 *
                 *      \
                 *     (varname)
                 *         |
                 *      (value)
                 *
                 */

                BranchJoin(
                    //
                    curr,
                    BranchJoin(
                        //
                        Branch(old->value, NODE_MEMSET, old->file),
                        content
                        //
                        )
                    //
                );
                break;
            }

            tokens = old;
            AST call = ParseValue(&tokens);

            if (!call)
            {
                TreeFree(root);
                return LeaveException(ValueError, old->value, old->file);
            }

            /**
             *
             *      \
             *    (function name)
             *      /
             *  (arg1)-(arg2)-(arg...)
             *
             */

            BranchJoin(curr, call);
            break;

        default:
            TreeFree(root);
            return LeaveException(UnexpectedNear, old->value, old->file);
        }
    }

    if (ErrorLevel())
    {
        TreeFree(root);
        return NULL;
    }

    return (AST)root;
}
