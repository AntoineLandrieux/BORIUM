#include <DRIVER/speaker.h>

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <speaker.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 * <https://github.com/AntoineLandrieux/x86driver/>
 *
 */

static unsigned char SPEAKER_STATUS = 0;

/**
 * @brief Frequency table for musical notes (A, B, C, D, E, F, G and their sharps/flats)
 *
 * NOTE_FREQUENCY[note] returns the frequency in Hz for the given note_t value
 */
static unsigned int NOTE_FREQUENCY[12] = {
    //
    440, // A
    493, // B
    261, // C
    294, // D
    329, // E
    349, // F
    392, // G
    466, // A# / Bb
    277, // C# / Db
    311, // D# / Eb
    370, // F# / Gb
    415  // G# / Ab
    //
};

/**
 * @brief Enable the PC speaker
 *
 */
void ENABLE_SPEAKER()
{
    if (!SPEAKER_STATUS)
        OUTB(SPEAKER_PORT, INB(SPEAKER_PORT) | 0x03);
    SPEAKER_STATUS = 1;
}

/**
 * @brief Disable the PC speaker
 *
 */
void DISABLE_SPEAKER()
{
    if (SPEAKER_STATUS)
        OUTB(SPEAKER_PORT, INB(SPEAKER_PORT) & 0xFC);
    SPEAKER_STATUS = 0;
}

/**
 * @brief Play a sound at the specified frequency using the PC speaker
 *
 * @param frequency Frequency in Hz to play
 */
void PLAY_FREQUENCY(unsigned int frequency)
{
    if (!frequency)
        return;

    unsigned int divisor = 1193180 / frequency;

    OUTB(0x43, 0xB6);
    OUTB(0x42, (unsigned char)(divisor));
    OUTB(0x42, (unsigned char)(divisor >> 8));

    ENABLE_SPEAKER();
}

/**
 * @brief Convert a musical note to its corresponding frequency
 *
 * @param note Musical note (note_t)
 * @return Frequency in Hz
 */
unsigned int NOTE_TO_FREQUENCY(note_t note)
{
    return NOTE_FREQUENCY[note];
}

/**
 * @brief Play a musical note for a specified duration
 *
 * @param note Musical note (note_t)
 * @param hight Hight
 * @param duration Duration in milliseconds
 */
void PLAY_NOTE(note_t note, unsigned char hight, unsigned int duration)
{
    PLAY_FREQUENCY(NOTE_FREQUENCY[note] * hight);
    SLEEP(duration);
    DISABLE_SPEAKER();
}

/**
 * @brief Terminal beep
 *
 */
void TERMINAL_BEEP(void)
{
    PLAY_FREQUENCY(1000);
    SLEEP(100);
    DISABLE_SPEAKER();
}
