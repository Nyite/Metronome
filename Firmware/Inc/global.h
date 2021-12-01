#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//#define SPECTRUM_DEBUG

/* Bus clock speeds */
#define APB1CLOCK 	16000000
#define APB2CLOCK 	16000000

#define UART_BAUD	115200			/* UART BOUD rate */

/* Default metronome values */
#define DEFAULT_TEMPO	120			/* Default metronome bpm */
/**
  * @note	 BAr length is set in musical proportion i.e.
  * 					ONE_FORTH*4  as 4/4
  * 				   ONE_EIGHTH*8  as 8/8
  * 				ONE_SIXTEENTH*16 as 16/16
  *				ONE_THIRTY_SECOND*32 as 32/32
  */
#define DEFAULT_BAR_LEN	ONE_FORTH*4	/* Default metronome bar length (4/4) */
#define DEFAULT_RHYTHM	0xF0000000	/* Default metronome rhythm */

#define SAMPLE_FREQ		40000		/* ADC sample frequency is Hz */
#define BEEP_FREQ		440			/* Sound indication frequency is Hz */

#define FFT_RESOLUTION	2048		/* FFT sample pool [power of 2] */
#define NOISE_FLOOR		90			/* Noise floor offset [dB] / default value is 90dB */
#define TRANSIENT_THR	12			/* Transient volume change detection threshold [dB] */
#ifndef SPECTRUM_DEBUG
	#define LOW_PASS_CUTOFF_FREQ 1000	/* Low pass filter cutoff frequency [Hz] */
#else
	#define LOW_PASS_CUTOFF_FREQ SAMPLE_FREQ
#endif

/* Note length code for note replay and UART transmission */
enum NOTE_LENGTH
{
	ONE_FORTH 			= 1,
	ONE_EIGHTH 			= 2,
	ONE_SIXTEENTH	 	= 4,
	ONE_THIRTY_SECOND 	= 8,
};

#endif /* _GLOBAL_H_ */
