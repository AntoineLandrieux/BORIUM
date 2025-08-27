#ifndef __SOARE__
#define __SOARE__ 0x1

/* #pragma once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <SOARE.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

/* SOARE major */
#define SOARE_MAJOR 1
/* SOARE minor */
#define SOARE_MINOR 1
/* SOARE patch */
#define SOARE_PATCH 1

/**
 * SOARE version
 * 
 * R: release
 * A: alpha
 * B: beta
 * S: snapshot
 * T: testing
 * 
 */
#define SOARE_VERSION "Rv1.1.1"

/* SOARE max input */
#define __SOARE_MAX_INPUT__ 70

/* Output */
#define soare_write PUTS
/* Input */
#define soare_input(__buffer) GETS(__buffer, __SOARE_MAX_INPUT__);

#ifdef __cplusplus
/**
 * SOARE namespace
 */
namespace SOARE
{
    extern "C"
    {
#endif /* __cplusplus */

        /**
         * @brief Structure of a document
         */
        typedef struct Document
        {

            // File
            char *file;

            // Line
            unsigned long long ln;
            // Column
            unsigned long long col;

        } Document;

#include "utils/keywords.h"
#include "utils/platform.h"

#include "core/error.h"
#include "core/tokenizer.h"
#include "core/parser.h"
#include "core/memory.h"
#include "core/math.h"
#include "core/runtime.h"

#ifdef __cplusplus
    }
}
#endif /* __cplusplus */

#endif /* __SOARE__ */