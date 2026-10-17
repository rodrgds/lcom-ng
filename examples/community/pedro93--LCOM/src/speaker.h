#ifndef __SPEAKER_H
#define __SPEAKER_H

//the letter represents the note
//the number represents the string of the guitar

#define D2 146.83 //5? fret
#define A3 220    //7? fret
#define C4 261.63 //5? fret
#define D4 293.66 //7? fret
#define E5 329.23 //5? fret
#define F5 349.23 //6? fret
#define G5 392    //8? fret
#define A6 440    //5? fret
#define As6 466.16//6? fret
#define F6 349.23 //1? fret
#define D5 293.66 //3? fret
#define A4 220    //2? fret
#define As3 233.08//8? fret
#define F4 349.23 //10?fret
#define A5 440    //10?fret
#define G5 392    //8? fret
#define E4 329.63 //9? fret
#define G4 196    //0? fret
#define As5 466.16//11? fret
#define F3 130.81 //3? fret
#define E3 123.47 //2? fret
#define D3 146.83 //0? fret
#define C5 523.25 //13? fret

/** @defgroup Speaker Speaker
 * @{
 *
 * Functions that control the speaker output
 */

/**
 * @brief Enables/disables speaker
 *
 * Enables/disables speaker, by both enabling/disabling Timer 2 and its output gate
 *
 * @param on If zero disable speaker, otherwise enable it
 * @return Return 0 upon success and non-zero otherwise
 */
int speaker_ctrl(unsigned char on);

/**
 * @brief Programs speaker to generate tone for the given time interval
 *
 * @param freq Frequency of tone to generate in Hz
 * @param time Duration of tone in s
 * @return Return 0 upon success and non-zero otherwise
 */
int play_note(unsigned long freq, unsigned long time);

/**@}*/

#endif /* __SPEAKER_H */
