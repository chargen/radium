//-----------------------------------------------------
// name: "Brass"
// author: "Romain Michon (rmichon@ccrma.stanford.edu)"
// copyright: "Romain Michon"
// version: "1.0"
//
// Code generated with Faust 0.9.55 (http://faust.grame.fr)
//-----------------------------------------------------
/* link with  */
#include <math.h>
#include <cmath>
template <int N> inline float faustpower(float x) 		{ return powf(x,N); } 
template <int N> inline double faustpower(double x) 	{ return pow(x,N); }
template <int N> inline int faustpower(int x) 			{ return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline int faustpower<0>(int x) 		{ return 1; }
template <> 	 inline int faustpower<1>(int x) 		{ return x; }
#include <math.h>
#include <string>

/*
#include "/usr/share/faust/audio/dsp.h"
#include "/usr/share/faust/gui/UI.h"
*/

#include "/home/kjetil/faudiostream/architecture/faust/audio/dsp.h"
#include "/home/kjetil/faudiostream/architecture/faust/gui/UI.h"

struct Meta
{
    void declare (const char* key, const char* value) { }
};

static float scale(float x, float x1, float x2, float y1, float y2){
  return y1 + ( ((x-x1)*(y2-y1))
                /
                (x2-x1)
                );
}

#if 0
static float linear2db(float val){
  if(val<=0.0f)
    return 0.0f;

  float db = 20*log10(val);
  if(db<-70)
    return 0.0f;
  else if(db>40)
    return 1.0f;
  else
    return scale(db,-70,40,0,1);
}
#endif

// input is between 0 and 1.
// output is between 0 and 1.
static float velocity2gain(float val){
  if(val<=0.0f)
    return 0.0f;
  else if(val>=1.0f)
    return 1.0f;
  else
    return powf(10, scale(val,0.0, 1.0 ,-40, 20) / 20.0f) / 10.0f;
}


static double midi_to_hz(float midi){
  if(midi<=0)
    return 0;
  else
    //  return 1;
  return 8.17579891564*(expf(.0577622650*midi));
}

#if 0
// For some reason, it won't compile with the usual min/max macros.
template<typename T> static inline T min(T a,T b){return a<b ? a : b;}
template<typename T> static inline T max(T a,T b){return a>b ? a : b;}
static inline float min(float a,int b){return a<b ? a : b;}
static inline float max(float a,int b){return a>b ? a : b;}
static inline float min(int a,float b){return a<b ? a : b;}
static inline float max(int a,float b){return a>b ? a : b;}
#endif

inline int 	max (unsigned int a, unsigned int b) { return (a>b) ? a : b; }
inline int 	max (int a, int b)		{ return (a>b) ? a : b; }

inline long 	max (long a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (int a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (long a, int b) 		{ return (a>b) ? a : b; }

inline float 	max (float a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (int a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, int b) 		{ return (a>b) ? a : b; }
inline float 	max (long a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, long b) 		{ return (a>b) ? a : b; }

inline double 	max (double a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (int a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, int b) 		{ return (a>b) ? a : b; }
inline double 	max (long a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, long b) 		{ return (a>b) ? a : b; }
inline double 	max (float a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (double a, float b) 	{ return (a>b) ? a : b; }


inline int	min (int a, int b)		{ return (a<b) ? a : b; }

inline long 	min (long a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (int a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (long a, int b) 		{ return (a<b) ? a : b; }

inline float 	min (float a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (int a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, int b) 		{ return (a<b) ? a : b; }
inline float 	min (long a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, long b) 		{ return (a<b) ? a : b; }

inline double 	min (double a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (int a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, int b) 		{ return (a<b) ? a : b; }
inline double 	min (long a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, long b) 		{ return (a<b) ? a : b; }
inline double 	min (float a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (double a, float b) 	{ return (a<b) ? a : b; }


/******************************************************************************
*******************************************************************************

							       VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/


/******************************************************************************
*******************************************************************************

			ABSTRACT USER INTERFACE

*******************************************************************************
*******************************************************************************/

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

typedef long double quad;

#ifndef FAUSTCLASS 
#define FAUSTCLASS Brass_dsp
#endif

class Brass_dsp : public dsp {
  private:
	class SIG0 {
	  private:
		int 	fSamplingFreq;
		int 	iRec4[2];
	  public:
		int getNumInputs() 	{ return 0; }
		int getNumOutputs() 	{ return 1; }
		void init(int samplingFreq) {
			fSamplingFreq = samplingFreq;
			for (int i=0; i<2; i++) iRec4[i] = 0;
		}
		void fill (int count, float output[]) {
			for (int i=0; i<count; i++) {
				iRec4[0] = (1 + iRec4[1]);
				output[i] = sinf((9.587379924285257e-05f * float((iRec4[0] - 1))));
				// post processing
				iRec4[1] = iRec4[0];
			}
		}
	};


	FAUSTFLOAT 	fentry0;
	float 	fRec0[2];
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT 	fentry1;
	int 	iConst0;
	float 	fConst1;
	static float 	ftbl0[65536];
	FAUSTFLOAT 	fslider1;
	float 	fRec6[2];
	FAUSTFLOAT 	fentry2;
	float 	fConst2;
	float 	fRec5[2];
	FAUSTFLOAT 	fbutton0;
	int 	iRec7[2];
	FAUSTFLOAT 	fslider2;
	FAUSTFLOAT 	fslider3;
	float 	fRec8[2];
	FAUSTFLOAT 	fslider4;
	float 	fRec9[2];
	int 	iConst3;
	FAUSTFLOAT 	fslider5;
	float 	fVec0[2];
	float 	fRec15[2];
	float 	fRec14[2];
	float 	fRec13[2];
	float 	fRec12[2];
	float 	fRec11[2];
	float 	fRec10[2];
	float 	fRec21[2];
	float 	fRec20[2];
	float 	fRec19[2];
	float 	fRec18[2];
	float 	fRec17[2];
	float 	fRec16[2];
	FAUSTFLOAT 	fslider6;
	float 	fRec22[2];
	int 	iRec23[2];
	int 	iRec24[2];
	FAUSTFLOAT 	fslider7;
	FAUSTFLOAT 	fslider8;
	FAUSTFLOAT 	fslider9;
	float 	fRec25[2];
	FAUSTFLOAT 	fslider10;
	int 	iRec26[2];
	FAUSTFLOAT 	fslider11;
	FAUSTFLOAT 	fslider12;
	float 	fRec27[2];
	FAUSTFLOAT 	fslider13;
	float 	fRec3[3];
	float 	fVec1[2];
	float 	fRec2[2];
	int 	IOTA;
	float 	fRec1[8192];
	float 	fVec2[4096];
	FAUSTFLOAT 	fslider14;
	FAUSTFLOAT 	fslider15;
	float 	fConst4;
  public:
	static void metadata(Meta* m) 	{ 
		m->declare("name", "Brass");
		m->declare("description", "WaveGuide Brass instrument from STK");
		m->declare("author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("copyright", "Romain Michon");
		m->declare("version", "1.0");
		m->declare("licence", "STK-4.3");
		m->declare("reference", "https://ccrma.stanford.edu/~jos/pasp/Brasses.html");
		m->declare("music.lib/name", "Music Library");
		m->declare("music.lib/author", "GRAME");
		m->declare("music.lib/copyright", "GRAME");
		m->declare("music.lib/version", "1.0");
		m->declare("music.lib/license", "LGPL with exception");
		m->declare("math.lib/name", "Math Library");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/version", "1.0");
		m->declare("math.lib/license", "LGPL with exception");
		m->declare("instrument.lib/name", "Faust-STK Tools Library");
		m->declare("instrument.lib/author", "Romain Michon (rmichon@ccrma.stanford.edu)");
		m->declare("instrument.lib/copyright", "Romain Michon");
		m->declare("instrument.lib/version", "1.0");
		m->declare("instrument.lib/licence", "STK-4.3");
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("filter.lib/copyright", "Julius O. Smith III");
		m->declare("filter.lib/version", "1.29");
		m->declare("filter.lib/license", "STK-4.3");
		m->declare("filter.lib/reference", "https://ccrma.stanford.edu/~jos/filters/");
		m->declare("effect.lib/name", "Faust Audio Effect Library");
		m->declare("effect.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("effect.lib/copyright", "Julius O. Smith III");
		m->declare("effect.lib/version", "1.33");
		m->declare("effect.lib/license", "STK-4.3");
	}

	virtual int getNumInputs() 	{ return 0; }
	virtual int getNumOutputs() 	{ return 2; }
	static void classInit(int samplingFreq) {
		SIG0 sig0;
		sig0.init(samplingFreq);
		sig0.fill(65536,ftbl0);
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fentry0 = 1.0f;
		for (int i=0; i<2; i++) fRec0[i] = 0;
		fslider0 = 0.78f;
		fentry1 = 4.4e+02f;
		iConst0 = min(192000, max(1, fSamplingFreq));
		fConst1 = (6.283185307179586f / float(iConst0));
		fslider1 = 2.2e+02f;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		fentry2 = 0.0f;
		fConst2 = (1.0f / float(iConst0));
		for (int i=0; i<2; i++) fRec5[i] = 0;
		fbutton0 = 0.0;
		for (int i=0; i<2; i++) iRec7[i] = 0;
		fslider2 = 0.07f;
		fslider3 = 0.1f;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		fslider4 = 0.0f;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		iConst3 = (2 * iConst0);
		fslider5 = 0.041f;
		for (int i=0; i<2; i++) fVec0[i] = 0;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		for (int i=0; i<2; i++) fRec14[i] = 0;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		for (int i=0; i<2; i++) fRec10[i] = 0;
		for (int i=0; i<2; i++) fRec21[i] = 0;
		for (int i=0; i<2; i++) fRec20[i] = 0;
		for (int i=0; i<2; i++) fRec19[i] = 0;
		for (int i=0; i<2; i++) fRec18[i] = 0;
		for (int i=0; i<2; i++) fRec17[i] = 0;
		for (int i=0; i<2; i++) fRec16[i] = 0;
		fslider6 = 6.0f;
		for (int i=0; i<2; i++) fRec22[i] = 0;
		for (int i=0; i<2; i++) iRec23[i] = 0;
		for (int i=0; i<2; i++) iRec24[i] = 0;
		fslider7 = 0.1f;
		fslider8 = 0.05f;
		fslider9 = 0.5f;
		for (int i=0; i<2; i++) fRec25[i] = 0;
		fslider10 = 0.05f;
		for (int i=0; i<2; i++) iRec26[i] = 0;
		fslider11 = 0.001f;
		fslider12 = 0.005f;
		for (int i=0; i<2; i++) fRec27[i] = 0;
		fslider13 = 1.0f;
		for (int i=0; i<3; i++) fRec3[i] = 0;
		for (int i=0; i<2; i++) fVec1[i] = 0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		IOTA = 0;
		for (int i=0; i<8192; i++) fRec1[i] = 0;
		for (int i=0; i<4096; i++) fVec2[i] = 0;
		fslider14 = 0.6f;
		fslider15 = 0.5f;
		fConst4 = (0.5f * iConst0);
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(UI* interface) {
		interface->openVerticalBox("brass");
		interface->openHorizontalBox("Basic_Parameters");
		interface->declare(&fentry1, "1", "");
		interface->declare(&fentry1, "tooltip", "Tone frequency");
		interface->declare(&fentry1, "unit", "Hz");
		interface->addNumEntry("freq", &fentry1, 4.4e+02f, 2e+01f, 2e+04f, 1.0f);
		interface->declare(&fentry0, "1", "");
		interface->declare(&fentry0, "tooltip", "Gain (value between 0 and 1)");
		interface->addNumEntry("gain", &fentry0, 1.0f, 0.0f, 1.0f, 0.01f);
		interface->declare(&fbutton0, "1", "");
		interface->declare(&fbutton0, "tooltip", "noteOn = 1, noteOff = 0");
		interface->addButton("gate", &fbutton0);
		interface->closeBox();
		interface->openHorizontalBox("Envelopes_and_Vibrato");
		interface->openVerticalBox("Envelope_Parameters");
		interface->declare(&fslider12, "5", "");
		interface->declare(&fslider12, "tooltip", "Envelope attack duration");
		interface->declare(&fslider12, "unit", "s");
		interface->addHorizontalSlider("Envelope_Attack", &fslider12, 0.005f, 0.0f, 2.0f, 0.01f);
		interface->declare(&fslider11, "5", "");
		interface->declare(&fslider11, "tooltip", "Envelope decay duration");
		interface->declare(&fslider11, "unit", "s");
		interface->addHorizontalSlider("Envelope_Decay", &fslider11, 0.001f, 0.0f, 2.0f, 0.01f);
		interface->declare(&fslider2, "5", "");
		interface->declare(&fslider2, "tooltip", "Envelope release duration");
		interface->declare(&fslider2, "unit", "s");
		interface->addHorizontalSlider("Envelope_Release", &fslider2, 0.07f, 0.0f, 2.0f, 0.01f);
		interface->closeBox();
		interface->openVerticalBox("Vibrato_Parameters");
		interface->declare(&fslider9, "4", "");
		interface->declare(&fslider9, "tooltip", "Vibrato attack duration");
		interface->declare(&fslider9, "unit", "s");
		interface->addHorizontalSlider("Vibrato_Attack", &fslider9, 0.5f, 0.0f, 2.0f, 0.01f);
		interface->declare(&fslider8, "4", "");
		interface->declare(&fslider8, "tooltip", "Vibrato silence duration before attack");
		interface->declare(&fslider8, "unit", "s");
		interface->addHorizontalSlider("Vibrato_Begin", &fslider8, 0.05f, 0.0f, 2.0f, 0.01f);
		interface->declare(&fslider6, "4", "");
		interface->declare(&fslider6, "unit", "Hz");
		interface->addHorizontalSlider("Vibrato_Freq", &fslider6, 6.0f, 1.0f, 15.0f, 0.1f);
		interface->declare(&fslider10, "4", "");
		interface->declare(&fslider10, "tooltip", "A value between 0 and 1");
		interface->addHorizontalSlider("Vibrato_Gain", &fslider10, 0.05f, 0.0f, 1.0f, 0.01f);
		interface->declare(&fslider7, "4", "");
		interface->declare(&fslider7, "tooltip", "Vibrato release duration");
		interface->declare(&fslider7, "unit", "s");
		interface->addHorizontalSlider("Vibrato_Release", &fslider7, 0.1f, 0.0f, 2.0f, 0.01f);
		interface->closeBox();
		interface->closeBox();
		interface->openHorizontalBox("Physical_and_Nonlinearity");
		interface->openVerticalBox("Nonlinear_Filter_Parameters");
		interface->declare(&fslider1, "3", "");
		interface->declare(&fslider1, "tooltip", "Frequency of the sine wave for the modulation of theta (works if Modulation Type=3)");
		interface->declare(&fslider1, "unit", "Hz");
		interface->addHorizontalSlider("Modulation_Frequency", &fslider1, 2.2e+02f, 2e+01f, 1e+03f, 0.1f);
		interface->declare(&fentry2, "3", "");
		interface->declare(&fentry2, "tooltip", "0=theta is modulated by the incoming signal; 1=theta is modulated by the averaged incoming signal; 2=theta is modulated by the squared incoming signal; 3=theta is modulated by a sine wave of frequency freqMod; 4=theta is modulated by a sine wave of frequency freq;");
		interface->addNumEntry("Modulation_Type", &fentry2, 0.0f, 0.0f, 4.0f, 1.0f);
		interface->declare(&fslider4, "3", "");
		interface->declare(&fslider4, "tooltip", "Nonlinearity factor (value between 0 and 1)");
		interface->addHorizontalSlider("Nonlinearity", &fslider4, 0.0f, 0.0f, 1.0f, 0.01f);
		interface->declare(&fslider3, "3", "");
		interface->declare(&fslider3, "Attack duration of the nonlinearity", "");
		interface->declare(&fslider3, "unit", "s");
		interface->addHorizontalSlider("Nonlinearity_Attack", &fslider3, 0.1f, 0.0f, 2.0f, 0.01f);
		interface->closeBox();
		interface->openVerticalBox("Physical_Parameters");
		interface->declare(&fslider0, "2", "");
		interface->declare(&fslider0, "tooltip", "A value between 0 and 1");
		interface->addHorizontalSlider("Lip_Tension", &fslider0, 0.78f, 0.01f, 1.0f, 0.001f);
		interface->declare(&fslider13, "2", "");
		interface->declare(&fslider13, "tooltip", "A value between 0 and 1");
		interface->addHorizontalSlider("Pressure", &fslider13, 1.0f, 0.01f, 1.0f, 0.01f);
		interface->declare(&fslider5, "2", "");
		interface->declare(&fslider5, "tooltip", "A value between 0 and 1");
		interface->addHorizontalSlider("Slide_Length", &fslider5, 0.041f, 0.01f, 1.0f, 0.001f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("Spat");
		interface->addHorizontalSlider("pan angle", &fslider14, 0.6f, 0.0f, 1.0f, 0.01f);
		interface->addHorizontalSlider("spatial width", &fslider15, 0.5f, 0.0f, 1.0f, 0.01f);
		interface->closeBox();
		interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fSlow0 = (0.0010000000000000009f * fentry0);
		float 	fSlow1 = fentry1;
		float 	fSlow2 = (0 - (1.994f * cosf((fConst1 * (fSlow1 * powf(4,((2 * fslider0) - 1)))))));
		float 	fSlow3 = (0.0010000000000000009f * fslider1);
		float 	fSlow4 = fentry2;
		int 	iSlow5 = (fSlow4 != 4);
		float 	fSlow6 = (fSlow1 * (fSlow4 == 4));
		float 	fSlow7 = fbutton0;
		int 	iSlow8 = (fSlow7 > 0);
		int 	iSlow9 = (fSlow7 <= 0);
		float 	fSlow10 = fslider2;
		float 	fSlow11 = (1 - (1.0f / powf(1e+05f,(1.0f / ((fSlow10 == 0.0f) + (iConst0 * fSlow10))))));
		float 	fSlow12 = fslider3;
		float 	fSlow13 = (1.0f / ((fSlow12 == 0.0f) + (iConst0 * fSlow12)));
		float 	fSlow14 = (0.0010000000000000009f * fslider4);
		float 	fSlow15 = ((0.5f + fslider5) * (3 + (float(iConst3) / fSlow1)));
		int 	iSlow16 = int(fSlow15);
		int 	iSlow17 = int((1 + int((iSlow16 & 4095))));
		int 	iSlow18 = (1 + iSlow16);
		float 	fSlow19 = (iSlow18 - fSlow15);
		int 	iSlow20 = int((1 + int((int(iSlow18) & 4095))));
		float 	fSlow21 = (fSlow15 - iSlow16);
		int 	iSlow22 = (fSlow4 >= 3);
		float 	fSlow23 = (3.141592653589793f * (fSlow4 == 2));
		float 	fSlow24 = (3.141592653589793f * (fSlow4 == 0));
		float 	fSlow25 = (1.5707963267948966f * (fSlow4 == 1));
		int 	iSlow26 = (fSlow4 < 3);
		float 	fSlow27 = (fConst2 * fslider6);
		float 	fSlow28 = fslider7;
		float 	fSlow29 = (1 - (1.0f / powf(1e+05f,(1.0f / ((fSlow28 == 0.0f) + (iConst0 * fSlow28))))));
		float 	fSlow30 = fslider8;
		float 	fSlow31 = (iConst0 * fSlow30);
		float 	fSlow32 = ((fSlow30 == 0.0f) + fSlow31);
		float 	fSlow33 = fslider9;
		float 	fSlow34 = (1.0f / ((fSlow33 == 0.0f) + (iConst0 * fSlow33)));
		float 	fSlow35 = fslider10;
		float 	fSlow36 = fslider11;
		float 	fSlow37 = (1 - powf(1e+02f,(1.0f / ((fSlow36 == 0.0f) + (iConst0 * fSlow36)))));
		float 	fSlow38 = fslider12;
		float 	fSlow39 = (1.0f / ((fSlow38 == 0.0f) + (iConst0 * fSlow38)));
		float 	fSlow40 = fslider13;
		float 	fSlow41 = fslider14;
		float 	fSlow42 = (4 * (1.0f - fSlow41));
		int 	iSlow43 = int((int((fConst4 * (fslider15 / fSlow1))) & 4095));
		float 	fSlow44 = (4 * fSlow41);
		FAUSTFLOAT* output0 = output[0];
		FAUSTFLOAT* output1 = output[1];
		for (int i=0; i<count; i++) {
			fRec0[0] = (fSlow0 + (0.999f * fRec0[1]));
			fRec6[0] = (fSlow3 + (0.999f * fRec6[1]));
			float fTemp0 = ((fConst2 * (fSlow6 + (iSlow5 * fRec6[0]))) + fRec5[1]);
			fRec5[0] = (fTemp0 - floorf(fTemp0));
			iRec7[0] = (iSlow8 & (iRec7[1] | (fRec8[1] >= 1)));
			int iTemp1 = (iSlow9 & (fRec8[1] > 0));
			fRec8[0] = (((iTemp1 == 0) | (fRec8[1] >= 1e-06f)) * ((fSlow13 * (((iRec7[1] == 0) & iSlow8) & (fRec8[1] < 1))) + (fRec8[1] * (1 - (fSlow11 * iTemp1)))));
			fRec9[0] = (fSlow14 + (0.999f * fRec9[1]));
			float fTemp2 = (fRec9[0] * fRec8[0]);
			float fTemp3 = (3.141592653589793f * (fTemp2 * ftbl0[int((65536.0f * fRec5[0]))]));
			float fTemp4 = sinf(fTemp3);
			float fTemp5 = ((fSlow21 * fRec1[(IOTA-iSlow20)&8191]) + (fSlow19 * fRec1[(IOTA-iSlow17)&8191]));
			fVec0[0] = fTemp5;
			float fTemp6 = (0 - fTemp4);
			float fTemp7 = cosf(fTemp3);
			float fTemp8 = ((fVec0[0] * fTemp7) + (fTemp6 * fRec10[1]));
			float fTemp9 = ((fTemp7 * fTemp8) + (fTemp6 * fRec11[1]));
			float fTemp10 = ((fTemp7 * fTemp9) + (fTemp6 * fRec12[1]));
			float fTemp11 = ((fTemp7 * fTemp10) + (fTemp6 * fRec13[1]));
			float fTemp12 = ((fTemp7 * fTemp11) + (fTemp6 * fRec14[1]));
			fRec15[0] = ((fTemp7 * fTemp12) + (fTemp6 * fRec15[1]));
			fRec14[0] = ((fTemp7 * fRec15[1]) + (fTemp4 * fTemp12));
			fRec13[0] = ((fTemp7 * fRec14[1]) + (fTemp4 * fTemp11));
			fRec12[0] = ((fTemp7 * fRec13[1]) + (fTemp4 * fTemp10));
			fRec11[0] = ((fTemp7 * fRec12[1]) + (fTemp4 * fTemp9));
			fRec10[0] = ((fTemp7 * fRec11[1]) + (fTemp4 * fTemp8));
			float fTemp13 = (fTemp2 * (((fSlow25 * (fVec0[0] + fVec0[1])) + (fSlow24 * fVec0[0])) + (fSlow23 * faustpower<2>(fVec0[0]))));
			float fTemp14 = sinf(fTemp13);
			float fTemp15 = (0 - fTemp14);
			float fTemp16 = cosf(fTemp13);
			float fTemp17 = ((fVec0[0] * fTemp16) + (fTemp15 * fRec16[1]));
			float fTemp18 = ((fTemp16 * fTemp17) + (fTemp15 * fRec17[1]));
			float fTemp19 = ((fTemp16 * fTemp18) + (fTemp15 * fRec18[1]));
			float fTemp20 = ((fTemp16 * fTemp19) + (fTemp15 * fRec19[1]));
			float fTemp21 = ((fTemp16 * fTemp20) + (fTemp15 * fRec20[1]));
			fRec21[0] = ((fTemp16 * fTemp21) + (fTemp15 * fRec21[1]));
			fRec20[0] = ((fTemp16 * fRec21[1]) + (fTemp14 * fTemp21));
			fRec19[0] = ((fTemp16 * fRec20[1]) + (fTemp14 * fTemp20));
			fRec18[0] = ((fTemp16 * fRec19[1]) + (fTemp14 * fTemp19));
			fRec17[0] = ((fTemp16 * fRec18[1]) + (fTemp14 * fTemp18));
			fRec16[0] = ((fTemp16 * fRec17[1]) + (fTemp14 * fTemp17));
			float fTemp22 = ((iSlow26 * (((1 - fRec9[0]) * fVec0[0]) + (fRec9[0] * ((fTemp16 * fRec16[1]) + (fVec0[0] * fTemp14))))) + (iSlow22 * ((fTemp7 * fRec10[1]) + (fVec0[0] * fTemp4))));
			float fTemp23 = (fSlow27 + fRec22[1]);
			fRec22[0] = (fTemp23 - floorf(fTemp23));
			iRec23[0] = (iSlow8 & (iRec23[1] | (fRec25[1] >= 1)));
			iRec24[0] = (iSlow8 * (1 + iRec24[1]));
			int iTemp24 = (iSlow9 & (fRec25[1] > 0));
			fRec25[0] = (((iTemp24 == 0) | (fRec25[1] >= 1e-06f)) * ((fSlow34 * ((1 - (iRec24[1] < fSlow32)) * ((((iRec23[1] == 0) & iSlow8) & (fRec25[1] < 1)) & (iRec24[1] > fSlow31)))) + (fRec25[1] * (1 - (fSlow29 * iTemp24)))));
			iRec26[0] = (iSlow8 & (iRec26[1] | (fRec27[1] >= 1)));
			int iTemp25 = (iSlow9 & (fRec27[1] > 0));
			fRec27[0] = (((iTemp25 == 0) | (fRec27[1] >= 1e-06f)) * ((fSlow39 * (((iRec26[1] == 0) & iSlow8) & (fRec27[1] < 1))) + (fRec27[1] * ((1 - (fSlow37 * (iRec26[1] & (fRec27[1] > 100)))) - (fSlow11 * iTemp25)))));
			float fTemp26 = ((fSlow40 * fRec27[0]) + (fSlow35 * (fRec25[0] * ftbl0[int((65536.0f * fRec22[0]))])));
			fRec3[0] = ((0.03f * ((0.3f * fTemp26) - (0.85f * fTemp22))) - ((0.994009f * fRec3[2]) + (fSlow2 * fRec3[1])));
			float fTemp27 = faustpower<2>(fRec3[0]);
			float fTemp28 = ((fTemp27 * (fTemp27 <= 1)) + (fTemp27 > 1));
			float fTemp29 = ((0.85f * (fTemp22 * (0 - (fTemp28 - 1)))) + (0.3f * (fTemp26 * fTemp28)));
			fVec1[0] = fTemp29;
			fRec2[0] = ((fVec1[0] + (0.995f * fRec2[1])) - fVec1[1]);
			fRec1[IOTA&8191] = fRec2[0];
			float fTemp30 = (fRec1[(IOTA-0)&8191] * fRec0[0]);
			fVec2[IOTA&4095] = fTemp30;
			output0[i] = (FAUSTFLOAT)(fSlow42 * fVec2[IOTA&4095]);
			output1[i] = (FAUSTFLOAT)(fSlow44 * fVec2[(IOTA-iSlow43)&4095]);
			// post processing
			IOTA = IOTA+1;
			fRec2[1] = fRec2[0];
			fVec1[1] = fVec1[0];
			fRec3[2] = fRec3[1]; fRec3[1] = fRec3[0];
			fRec27[1] = fRec27[0];
			iRec26[1] = iRec26[0];
			fRec25[1] = fRec25[0];
			iRec24[1] = iRec24[0];
			iRec23[1] = iRec23[0];
			fRec22[1] = fRec22[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fVec0[1] = fVec0[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			iRec7[1] = iRec7[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec0[1] = fRec0[0];
		}
	}
};


float 	Brass_dsp::ftbl0[65536];



#include "../common/nsmtracker.h"
#include "SoundPlugin.h"
#include "SoundPlugin_proc.h"

#include "Mixer_proc.h"
#include "SoundPluginRegistry_proc.h"

namespace{

class MyUI : public UI
{

 public:

  MyUI() 
    : _gate_control(NULL)
    , _freq_control(NULL)
    , _gain_control(NULL)
    , _num_effects(0)
    , _effect_tooltip("")
    , _curr_box_name(NULL)
  { }

  ~MyUI() {	}

  float *_gate_control;
  float *_freq_control;
  float *_gain_control;

  struct Controller{
    float* control_port;

    float min_value;
    float default_value;
    float max_value;

    std::string name;
    int type;

    const char *tooltip;
    const char *unit;

    Controller(float *control_port)
      : control_port(control_port)
      , min_value(0.0f)
      , default_value(0.5f)
      , max_value(1.0f)
      , name("<no name>")
      , type(EFFECT_FORMAT_FLOAT)
      , tooltip("")
      , unit("")
    {}
  };

  std::vector<Controller> _controllers;

  int get_controller_num(float *control_port){
    for(unsigned int i=0;i<_controllers.size();i++){
      if(control_port == _controllers.at(i).control_port)
        return i;
    }

    Controller controller(control_port);

    _controllers.push_back(controller);
    _num_effects++;

    return _controllers.size()-1;
  }

  bool is_instrument(){
    if(_gate_control!=NULL && _freq_control!=NULL && _gain_control!=NULL)
      return true;
    else
      return false;
  }

  // Remove gain/gate/freq sliders for instruments.
  void remove_instrument_notecontrol_effects(){
    if(is_instrument()){
      _controllers.erase(_controllers.begin() + get_controller_num(_gate_control));
      _controllers.erase(_controllers.begin() + get_controller_num(_freq_control));
      _controllers.erase(_controllers.begin() + get_controller_num(_gain_control));
      _num_effects -= 3;
    }
  }

  // We don't use passive items. (it would have been nice to have a passive effect telling when an instrument is finished playing)
  void remove_last_item(){
    _controllers.pop_back();
    _num_effects--;
  }
  

  int _num_effects;

  const char *_effect_tooltip;

  const char* _curr_box_name;

  // -- widget's layouts
  
  void openFrameBox(const char* label) {_curr_box_name = label;}
  void openTabBox(const char* label) {_curr_box_name = label;}
  void openHorizontalBox(const char* label) {_curr_box_name = label;}
  void openVerticalBox(const char* label) {_curr_box_name = label;}
  void closeBox() {_curr_box_name = NULL;}
  
  // -- active widgets

  void addEffect(const char *name, float* control_port, int type, float min_value, float default_value, float max_value){
    int effect_num = get_controller_num(control_port);

    Controller *controller = &_controllers.at(effect_num);

    if(_curr_box_name != NULL && strlen(_curr_box_name) < 10){
      controller->name = std::string(_curr_box_name) + ": " + name;
    }else{
      controller->name = name;
    }
    controller->type = type;
    controller->min_value = min_value;
    controller->default_value = default_value;
    controller->max_value = max_value;

    if(!strcmp(name,"gate"))
      _gate_control = control_port;

    if(!strcmp(name,"freq"))
      _freq_control = control_port;

    if(!strcmp(name,"gain"))
      _gain_control = control_port;
  }

  void addButton(const char* label, float* zone) {
    addEffect(label, zone, EFFECT_FORMAT_BOOL, 0, 0, 1);
  }
  void addToggleButton(const char* label, float* zone) {
    addEffect(label, zone, EFFECT_FORMAT_BOOL, 0, 0, 1);
  }
  void addCheckButton(const char* label, float* zone) {
    addEffect(label, zone, EFFECT_FORMAT_BOOL, 0, 0, 1);
  }
  void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone, EFFECT_FORMAT_FLOAT, min, init, max);
  }
  void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone, EFFECT_FORMAT_FLOAT, min, init, max);
  }
  void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone, EFFECT_FORMAT_FLOAT, min, init, max); // The INT effect format might not work. Need to go through the code first.
  }
  
  // -- passive widgets

  void addNumDisplay(const char* label, float* zone, int precision) {remove_last_item();}
  void addTextDisplay(const char* label, float* zone, const char* names[], float min, float max) {remove_last_item();}
  void addHorizontalBargraph(const char* label, float* zone, float min, float max) {remove_last_item();}
  void addVerticalBargraph(const char* label, float* zone, float min, float max) {remove_last_item();}
  
  // -- metadata declarations
  
  void declare(float* control_port, const char* key, const char* value) {
    if(control_port==NULL){
      if(!strcmp(key,"tooltip"))
        _effect_tooltip = value;
    } else {
      int effect_num = get_controller_num(control_port);
      Controller *controller = &_controllers.at(effect_num);
      if(!strcmp(key,"tooltip"))
        controller->tooltip = value;
      else if(!strcmp(key,"unit"))
        controller->unit = value;
    }
  }
};


#define MAX_POLYPHONY 32

struct Voice{
  struct Voice *prev;
  struct Voice *next;
  dsp *dsp_instance;
  MyUI myUI;
  int note_num;

  Voice()
    : prev(NULL)
    , next(NULL)
    , dsp_instance(NULL)
    , note_num(0)
  { }
};

struct Data{
  Voice *voices_playing; // not used by effects
  Voice *voices_not_playing; // not used by effects
  Voice voices[MAX_POLYPHONY];   // Only voices[0] is used by effects.
  Data()
    : voices_playing(NULL)
    , voices_not_playing(NULL)
  {}
};

} // end anonymous namespace

static void RT_add_voice(Voice **root, Voice *voice){
  voice->next = *root;
  if(*root!=NULL)
    (*root)->prev = voice;
  *root = voice;
  voice->prev = NULL;
}

static void RT_remove_voice(Voice **root, Voice *voice){
  if(voice->prev!=NULL)
    voice->prev->next = voice->next;
  else
    *root=voice->next;

  if(voice->next!=NULL)
    voice->next->prev = voice->prev;
}

static bool RT_is_silent(float *sound, int num_frames){
  for(int i=0;i<num_frames;i++)
    if(sound[i]>0.02f)
      return false;
  return true;
}

enum VoiceOp{
  VOICE_KEEP,
  VOICE_REMOVE
};

static VoiceOp RT_play_voice(Voice *voice, int num_frames, float **inputs, float **outputs){  
  voice->dsp_instance->compute(num_frames, inputs, outputs);
  if( *(voice->myUI._gate_control)==0.0f && RT_is_silent(outputs[0],num_frames))
    return VOICE_REMOVE;
  else
    return VOICE_KEEP;
}

static void RT_process_instrument(SoundPlugin *plugin, int64_t time, int num_frames, float **inputs, float **outputs){
  Data *data = (Data*)plugin->data;
  int num_outputs = plugin->type->num_outputs;

  for(int i=0;i<num_outputs;i++)
    memset(outputs[i],0,num_frames*sizeof(float));

  float *tempsounds[num_outputs];
  float tempdata[num_outputs][num_frames];
  for(int i=0;i<num_outputs;i++)
    tempsounds[i] = &tempdata[i][0];

  Voice *voice = data->voices_playing;
  
  while(voice!=NULL){
    Voice *next = voice->next;

    if(RT_play_voice(voice,num_frames,inputs,tempsounds)==VOICE_REMOVE){
      RT_remove_voice(&data->voices_playing, voice);
      RT_add_voice(&data->voices_not_playing, voice);
    }

    for(int ch=0;ch<num_outputs;ch++){
      float *source = tempsounds[ch];
      float *target = outputs[ch];
      for(int i=0;i<num_frames;i++)
        target[i] += source[i];
    }

    voice=next;
  }
}

static void play_note(struct SoundPlugin *plugin, int64_t time, int note_num, float volume){
  Data *data = (Data*)plugin->data;

  //printf("Playing %d\n",note_num);

  Voice *voice = data->voices_not_playing;

  if(voice==NULL){
    printf("no more free voices\n");
    return;
  }

  RT_remove_voice(&data->voices_not_playing, voice);
  RT_add_voice(&data->voices_playing, voice);

  *(voice->myUI._gate_control) = 1.0f;
  *(voice->myUI._freq_control) = midi_to_hz(note_num);
  *(voice->myUI._gain_control) = velocity2gain(volume);

  voice->note_num = note_num;
  
}

static void set_note_volume(struct SoundPlugin *plugin, int64_t time, int note_num, float volume){
  Data *data = (Data*)plugin->data;
  Voice *voice = data->voices_playing;
  //printf("Setting volume %f / %f\n",volume,velocity2gain(volume));
  while(voice!=NULL){
    if(voice->note_num==note_num)
      *(voice->myUI._gain_control) = velocity2gain(volume);
    voice=voice->next;
  }
}

static void stop_note(struct SoundPlugin *plugin, int64_t time, int note_num, float volume){
  Data *data = (Data*)plugin->data;
  Voice *voice = data->voices_playing;
  while(voice!=NULL){
    if(voice->note_num==note_num)
      *(voice->myUI._gate_control) = 0.0f;
    voice=voice->next;
  }
}

static void RT_process_effect(SoundPlugin *plugin, int64_t time, int num_frames, float **inputs, float **outputs){
  //SoundPluginType *type = plugin->type;
  Data *data = (Data*)plugin->data;

  data->voices[0].dsp_instance->compute(num_frames, inputs, outputs);
  //printf("in00: %f, in10: %f\n",inputs[0][0],inputs[1][0]);
  //printf("out00: %f, out10: %f\n",outputs[0][0],outputs[1][0]);
}

static void *create_effect_plugin_data(const SoundPluginType *plugin_type, struct SoundPlugin *plugin, float samplerate, int blocksize){
  Data *data = new Data;
  Voice *voice = &data->voices[0];
  voice->dsp_instance = new CLASSNAME;
  printf("Creating %s / %s. samplerate: %d\n",plugin_type->type_name,plugin_type->name,(int)samplerate);
  voice->dsp_instance->init(samplerate);
  voice->dsp_instance->buildUserInterface(&voice->myUI);
  return data;
}

static void *create_instrument_plugin_data(const SoundPluginType *plugin_type, struct SoundPlugin *plugin, float samplerate, int blocksize){
  Data *data = new Data;

  for(int i=0;i<MAX_POLYPHONY;i++){
    Voice *voice = &data->voices[i];
    voice->dsp_instance = new CLASSNAME;
    voice->dsp_instance->init(samplerate);
    voice->dsp_instance->buildUserInterface(&voice->myUI);
    voice->myUI.remove_instrument_notecontrol_effects();

    RT_add_voice(&data->voices_not_playing, voice);
  }


  return data;
}

static void cleanup_plugin_data(SoundPlugin *plugin){
  Data *data = (Data*)plugin->data;

  for(int i=0;i<MAX_POLYPHONY;i++){
    Voice *voice = &data->voices[i];
    if(voice->dsp_instance==NULL) // an effect
      break;
    else
      delete voice->dsp_instance;
  }

  delete data;
}

static const char *get_effect_name(const struct SoundPluginType *plugin_type, int effect_num){
  Data *data = (Data*)plugin_type->data;
  Voice *voice = &data->voices[0];
  MyUI::Controller *controller = &voice->myUI._controllers.at(effect_num);
  return controller->name.c_str();
}

static void set_effect_value(struct SoundPlugin *plugin, int64_t time, int effect_num, float value, enum ValueFormat value_format){
  Data *data = (Data*)plugin->data;
  float scaled_value;

  if(value_format==PLUGIN_FORMAT_SCALED){
#ifdef DONT_NORMALIZE_EFFECT_VALUES
    scaled_value = value;
#else
    MyUI::Controller *controller = &data->voices[0].myUI._controllers.at(effect_num);
    float min = controller->min_value;
    float max = controller->max_value;
    float scaled_value = scale(value,0,1,min,max);
#endif
  }else{
    scaled_value = value;
  }

  printf("Setting effect %d to %f. input: %f\n",effect_num,scaled_value,value);

  for(int i=0;i<MAX_POLYPHONY;i++){
    Voice *voice = &data->voices[i];
    if(voice->dsp_instance==NULL) // an effect
      break;
    MyUI::Controller *controller = &voice->myUI._controllers.at(effect_num);
    *(controller->control_port) = scaled_value;
  }
}

static float get_effect_value(struct SoundPlugin *plugin, int effect_num, enum ValueFormat value_format){
  Data *data = (Data*)plugin->data;
  Voice *voice = &data->voices[0];
  MyUI::Controller *controller = &voice->myUI._controllers.at(effect_num);

  if(value_format==PLUGIN_FORMAT_SCALED){
#ifdef DONT_NORMALIZE_EFFECT_VALUES
    return *(controller->control_port);
#else
    float min = controller->min_value;
    float max = controller->max_value;
    return scale(*(controller->control_port),min,max,0.0f,1.0f);
#endif
  }else{
    return *(controller->control_port);
  }
}

static void get_display_value_string(struct SoundPlugin *plugin, int effect_num, char *buffer, int buffersize){
  Data *data = (Data*)plugin->data;
  Voice *voice = &data->voices[0];
  MyUI::Controller *controller = &voice->myUI._controllers.at(effect_num);

  snprintf(buffer,buffersize-1,"%f %s",*(controller->control_port), controller->unit);
}

static const char *get_effect_description(const struct SoundPluginType *plugin_type, int effect_num){
  Data *data = (Data*)plugin_type->data;
  Voice *voice = &data->voices[0];
  MyUI::Controller *controller = &voice->myUI._controllers.at(effect_num);

  return controller->tooltip;
}

static int get_effect_num(const struct SoundPluginType *plugin_type, const char *effect_name){
  Data *data = (Data*)plugin_type->data;
  Voice *voice = &data->voices[0];
  for(unsigned int i=0;i<voice->myUI._controllers.size();i++){
    MyUI::Controller *controller = &voice->myUI._controllers.at(i);
    if(!strcmp(controller->name.c_str(),effect_name))
      return i;
  }
  RError("Couldn't find effect name \"%s\" in plugin %s/%s\n",plugin_type->type_name,plugin_type->name);
  return 0;
}

static void fill_type(SoundPluginType *type){
 type->type_name                = "Faust";
 type->note_handling_is_RT      = false;
 type->get_effect_format        = NULL;
 type->get_effect_name          = get_effect_name;
 type->effect_is_RT             = NULL;
 type->cleanup_plugin_data      = cleanup_plugin_data;

 type->play_note       = play_note;
 type->set_note_volume = set_note_volume;
 type->stop_note       = stop_note;

 type->set_effect_value         = set_effect_value;
 type->get_effect_value         = get_effect_value;
 type->get_display_value_string = get_display_value_string;
 type->get_effect_description   = get_effect_description;

 type->get_effect_num = get_effect_num;

 type->data                     = NULL;
};

static SoundPluginType faust_type = {0};

void CREATE_NAME (void){
  fill_type(&faust_type);

  Data *data = (Data*)create_effect_plugin_data(&faust_type, NULL, MIXER_get_sample_rate(), MIXER_get_buffer_size());
  faust_type.data = data;

  faust_type.name = DSP_NAME;

  faust_type.num_inputs = data->voices[0].dsp_instance->getNumInputs();
  faust_type.num_outputs = data->voices[0].dsp_instance->getNumOutputs();

  if(data->voices[0].myUI.is_instrument()){

    faust_type.is_instrument      = true;

    faust_type.RT_process         = RT_process_instrument;
    faust_type.create_plugin_data = create_instrument_plugin_data;

    data->voices[0].myUI.remove_instrument_notecontrol_effects();

  }else{

    faust_type.is_instrument      = false;

    faust_type.RT_process         = RT_process_effect;
    faust_type.create_plugin_data = create_effect_plugin_data;

    faust_type.play_note          = NULL;
    faust_type.set_note_volume    = NULL;
    faust_type.stop_note          = NULL;

  }

  faust_type.num_effects = data->voices[0].myUI._num_effects;

  PR_add_plugin_type(&faust_type);
}
