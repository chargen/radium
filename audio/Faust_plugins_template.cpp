
/*
#include "/usr/share/faust/audio/dsp.h"
#include "/usr/share/faust/gui/UI.h"
*/

// We use faust1 here.

struct Meta
{
    void declare (const char* key, const char* value) { }
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"


#include <faust/dsp/dsp.h>


#if 0 //CREATE_NAME==create_zita_rev_plugin

  #include "mfaustqt1.cpp"

#else

  #include "../common/nsmtracker.h"
  #include "../Qt/FocusSniffers.h"
  #include <faust/gui/faustqt.h>

#endif

#pragma GCC diagnostic pop

#include "Faust_plugins_template1.cpp"


/******************************************************************************
*******************************************************************************

							       VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/

<<includeIntrinsic>>

/******************************************************************************
*******************************************************************************

			ABSTRACT USER INTERFACE

*******************************************************************************
*******************************************************************************/

//----------------------------------------------------------------------------
//  FAUST generated signal processor
//----------------------------------------------------------------------------

<<includeclass>>


#include "Faust_plugins_template2.cpp"

