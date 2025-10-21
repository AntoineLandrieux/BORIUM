#ifndef __SPEAKER_H__
#define __SPEAKER_H__ 0x1

/* #pragma once */

#include "io.h"

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <speaker.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 * <https://github.com/AntoineLandrieux/x86driver/>
 *
 */

#define SPEAKER_PORT 0x61

/**
 * @brief Musical note type
 *
 */
typedef enum notes_t
{

    // Naturals

    // A (la)
    NOTE_A,
    // B (si)
    NOTE_B,
    // C (do)
    NOTE_C,
    // D (re)
    NOTE_D,
    // E (mi)
    NOTE_E,
    // F (fa)
    NOTE_F,
    // G (sol)
    NOTE_G,

    // Sharps

    // A# (la#)
    NOTE_As,
    // C# (do#)
    NOTE_Cs,
    // D# (re#)
    NOTE_Ds,
    // F# (fa#)
    NOTE_Fs,
    // G# (sol#)
    NOTE_Gs

} note_t;

/**
 * @brief Enable the PC speaker
 *
 */
void ENABLE_SPEAKER();

/**
 * @brief Disable the PC speaker
 *
 */
void DISABLE_SPEAKER();

/**
 * @brief Play a sound at the specified frequency using the PC speaker
 *
 * @param frequency Frequency in Hz to play
 */
void PLAY_FREQUENCY(unsigned int frequency);

/**
 * @brief Convert a musical note to its corresponding frequency
 *
 * @param note Musical note (note_t)
 * @return Frequency in Hz
 */
unsigned int NOTE_TO_FREQUENCY(note_t note);

/**
 * @brief Play a musical note for a specified duration
 *
 * @param note Musical note (note_t)
 * @param hight Hight
 * @param duration Duration in milliseconds
 */
void PLAY_NOTE(note_t note, unsigned char hight, unsigned int duration);

/**
 * @brief Terminal beep
 *
 */
void TERMINAL_BEEP(void);

#endif /* __SPEAKER_H__ */