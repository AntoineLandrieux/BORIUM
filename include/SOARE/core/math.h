#ifndef __SOARE_MATH_H__
#define __SOARE_MATH_H__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <math.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

/**
 * @brief Duplicate a string
 *
 * @param value
 * @return char*
 */
char *vardup(char *string);

/**
 * @brief Return the value as a node
 * @author Antoine LANDRIEUX
 *
 * @param tokens
 * @return AST
 */
AST ParseValue(Tokens **tokens);

/**
 * @brief Build a math tree
 * @author Antoine LANDRIEUX
 *
 * @param tokens
 * @param priority 
 * @return AST
 */
AST ParseExpr(Tokens **tokens, unsigned char priority);

/**
 * @brief Evaluates the mathematical expression of a tree
 *
 * @param tree
 * @return char*
 */
char *Math(AST tree);

#endif /* __SOARE_MATH_H__ */