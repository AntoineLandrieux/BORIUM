#include <STD/stdlib.h>
#include <STD/stdarg.h>

#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

#include <kernel.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Runtime.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

static MEM FUNCTION = NULL;

/**
 * @brief Executes code from a tree
 *
 * @param tree
 * @return char*
 */
static char *Runtime(AST tree);

/**
 * @brief Set Interpreter variables
 *
 * @param file
 */
static void InterpreterVar(char *file)
{
    MEMORY = Mem();

    /* SOARE version */
    MemPush(MEMORY, "__SOARE__", strdup(SOARE_VERSION));
    /* Current file */
    MemPush(MEMORY, "__FILE__", strdup(file));
    /* Path to SOARE executable */
    MemPush(MEMORY, "__ENVIRONMENT__", strdup(GetEnvironment()));
    /* Errors */
    MemPush(MEMORY, "__ERROR__", strdup("NoError"));
    /* Build date */
    MemPush(MEMORY, "__BUILD__", strdup(__DATE__));
    /* Current OS */
    MemPush(MEMORY, "__PLATFORM__", strdup(__PLATFORM__));
}

/**
 * @brief Execute a function
 *
 * @param tree
 * @return char*
 */
char *RunFunction(AST tree)
{
    // Get the memory
    MEM get = MemGet(MEMORY, tree->value);

    // Memory not found
    if (!get)
        return LeaveException(UndefinedReference, tree->value, tree->file);

    // Memory is not a function
    if (!get->body)
        return LeaveException(ObjectIsNotCallable, tree->value, tree->file);

    // Arguments
    AST ptr = get->body->child;
    AST src = tree->child;
    MEM memf = Mem();

    AST func = NULL;

    while (ptr)
    {
        // Execute MEMORY (no more argument required)
        if (ptr->type == NODE_BODY)
        {
            FUNCTION = memf;
            return Runtime(ptr);
        }

        // Not enough argument
        if (!src)
        {
            MemFree(memf);
            memf = NULL;
            return LeaveException(UndefinedReference, ptr->value, tree->file);
        }

        get = NULL;

        // If it is a reference to a memory
        if (src->type == NODE_MEMGET)
            // If memory exists
            if ((get = MemGet(MEMORY, src->value)))
                // If memory is a function
                if ((func = get->body))
                    // Add this function in argument
                    get = MemPushf(memf, ptr->value, func);

        // If argument is not a function
        if (!get || !func)
            MemPush(memf, ptr->value, Eval(src));

        // Next argument
        src = src->sibling;
        ptr = ptr->sibling;
    }

    return NULL;
}

static char broken = 0;

/**
 * @brief Executes code from a tree
 *
 * @param tree
 * @return char*
 */
static char *Runtime(AST tree)
{
    if (!tree)
        return NULL;

    AST root = tree;
    AST tmp = NULL;
    MEM get = NULL;

    char *returned = NULL;
    unsigned char error = 0;

    broken = 0;

    MemJoin(MEMORY, FUNCTION);
    FUNCTION = NULL;

    for (AST curr = root->child; curr && !ErrorLevel(); curr = curr->sibling)
    {
        switch (curr->type)
        {
            /**
             *
             * BORIUM EXECUTABLE
             *
             */

        case NODE_BORIUM_CLEAR:
            SCREEN_CLEAR();
            break;

        case NODE_BORIUM_COLOR:
            if ((returned = Eval(curr->child)))
                SET_LOCAL_COLOR(atoi(returned));
            break;

        case NODE_BORIUM_CURSOR:
            if ((returned = Eval(curr->child)))
                SET_CURSOR(atoi(returned));
            break;

        case NODE_BORIUM_EDITOR:
            EDITOR();
            break;

        case NODE_BORIUM_GETC:
            if ((get = MemGet(MEMORY, curr->value)))
            {
                if (!(returned = malloc(2)))
                    return __SOARE_OUT_OF_MEMORY();
                returned[0] = GETC();
                returned[1] = 0;
                MemSet(get, returned);
                break;
            }

            return LeaveException(UndefinedReference, curr->value, curr->file);

        case NODE_BORIUM_HELP:
            PUTS(
                //
                "\n HELP \n"
                " \t clear   -> Clear screen \n"
                " \t color   -> Text color \n"
                " \t cursor  -> Set cursor location \n"
                " \t editor  -> Text editor \n"
                " \t getc    -> Get char \n"
                " \t help    -> Show commands \n"
                " \t license -> Show license \n"
                " \t pause   -> Interrupts the execution \n"
                " \t setup   -> Change BORIUM settings \n"
                " \t sleep   -> Pause for a while \n"
                //
            );
            break;

        case NODE_BORIUM_LICENSE:
            PUTS(
                //
                "\n"
                "BORIUM 2025 Antoine LANDRIEUX (MIT LICENSE)\n"
                "<https://github.com/AntoineLandrieux/BORIUM>\n"
                "SOARE 2024-2025 Antoine LANDRIEUX (MIT LICENSE)\n"
                "<https://github.com/AntoineLandrieux/SOARE>\n"
                "\n"
                //
            );
            break;

        case NODE_BORIUM_PAUSE:
            PUTS("\nPress any key to continue...\n");
            GETC();
            break;

        case NODE_BORIUM_SETUP:
            SETUP();
            break;

        case NODE_BORIUM_SLEEP:
            if ((returned = Eval(curr->child)))
                SLEEP(atoi(returned) * 1000);
            break;

        /**
         *
         * DEFAULT SOARE EXECUTABLE
         *
         */

        // Execute shell code
        case NODE_SHELL:

            returned = Eval(curr->child);
            system(returned);
            free(returned);
            break;

        // Store function into MEMORY
        case NODE_FUNCTION:

            MemPushf(MEMORY, curr->value, curr);
            break;

        // User input
        case NODE_INPUT:

            if ((get = MemGet(MEMORY, curr->value)))
            {
                char input[__SOARE_MAX_INPUT__] = {0};
                soare_input(input);
                MemSet(get, strdup(input));
                break;
            }

            return LeaveException(UndefinedReference, curr->value, curr->file);

        // Call a function
        case NODE_CALL:

            free(RunFunction(curr));
            break;

        // Push a new variable into MEMORY
        case NODE_MEMNEW:

            MemPush(MEMORY, curr->value, Eval(curr->child));
            break;

        // Set a value to a variable
        case NODE_MEMSET:

            if (!(get = MemGet(MEMORY, curr->value)))
                return LeaveException(UndefinedReference, curr->value, curr->file);

            if (get->body)
                return LeaveException(VariableDefinedAsFunction, curr->value, curr->file);

            MemSet(get, Eval(curr->child));
            break;

        // Condition MEMORY (if)
        case NODE_CONDITION:

            returned = Eval(curr->child);
            tmp = curr->child;

            while (returned)
            {
                if (strcmp(returned, "0"))
                {
                    free(returned);

                    if ((returned = Runtime(tmp->sibling)) || broken)
                        return returned;

                    break;
                }

                free(returned);

                if (!(tmp = tmp->sibling->sibling))
                    break;

                returned = Eval(tmp);
            }
            break;

        // Repetition MEMORY (while)
        case NODE_REPETITION:

            while ((returned = Eval(curr->child)))
            {
                if (!strcmp(returned, "0") || ErrorLevel() || broken)
                    break;

                free(returned);

                if ((returned = Runtime(curr->child->sibling)))
                    return returned;
            }

            free(returned);
            break;

        // try/iferror MEMORY
        case NODE_TRY:

            error = AsIgnoredException();
            IgnoreException(1);
            returned = Runtime(curr->child);
            IgnoreException(error);

            if (ErrorLevel() && !broken)
            {
                free(returned);
                ClearException();
                returned = Runtime(curr->child->sibling);
            }

            if (!returned && !broken)
                break;

            return returned;

        // Print
        case NODE_OUTPUT:

            if ((returned = Eval(curr->child)))
            {
                soare_write(returned);
                free(returned);
            }
            break;

        // Break loop
        case NODE_BREAK:

            broken = 1;
            return NULL;

        // Return
        case NODE_RETURN:

            return Eval(curr->child);

        // Leave new exception
        case NODE_RAISE:

            return LeaveException(RaiseException, curr->value, curr->file);

        default:

            break;
        }
    }

    // Quit
    return NULL;
}

/**
 * @brief Execute the code from a string
 *
 * @param rawcode
 */
int Execute(char *__restrict__ file, char *__restrict__ rawcode)
{
    if (!MEMORY)
        // Set default vars..
        InterpreterVar(file);

    // Clear interpreter exception
    ClearException();

    // Interpretation step 1: Tokenizer
    Tokens *tokens = Tokenizer(file, rawcode);
    // Interpretation step 2: Parser
    AST ast = Parse(tokens);

#ifdef __SOARE_DEBUG
    // DEBUG: print tokens and trees
    TreeLog(ast);
    TokensLog(tokens);
#endif

    // Free tokens
    TokensFree(tokens);

    // Interpretation step 3: Runtime
    free(Runtime(ast));

    // Free AST
    TreeFree(ast);
    // Free MEMORY
    MemFree(MEMORY);

    // Set MEMORY to NULL
    MEMORY = NULL;

    // Return error level
    // 0: EXIT_SUCCESS
    // 1: EXIT_FAILURE
    return (int)ErrorLevel();
}
