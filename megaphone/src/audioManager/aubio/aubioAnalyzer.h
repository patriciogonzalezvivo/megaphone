#ifndef _AUBIO_SOUND_PROCESSOR
#define _AUBIO_SOUND_PROCESSOR

#include "aubio.h"
#include "ofMain.h"
//#include "ofAddons.h"

#define uint_t unsigned int
class aubioAnalyzer{
	
	public:
		
		void setup();
		void processAudio(float * buffer, int bufferSize);		
		
		~aubioAnalyzer();
	
		float pitch;
		float amplitude;
		
		// ------------------------------------------------------------------
		// internal aubio variables

		uint_t win_s;							/* window size */
		uint_t hop_s;							/* hop size */
		uint_t samplerate;						/* samplerate */
		uint_t channels;						/* number of channel */
		
		aubio_pitchdetection_mode mode;
		aubio_pitchdetection_type type;

		fvec_t * in;
		aubio_pitchdetection_t * pitch_output ;
	
    
    
        aubio_pvoc_t*           mPhaseVocoder;
        aubio_onsetdetection_t*  mOnsetDetector;
        aubio_pickpeak_t*           mPeakPickingParms;
        
        fvec_t*                     mCurrentFrame;
        fvec_t*                     mInputAudio;
        fvec_t*                     mOnsetBuffer;
        cvec_t*                     mSpectrum;
        aubio_onsetdetection_type  mOnsetType;
        

		// ------------------------------------------------------------------

};

#endif	

