/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrOptions__
#define __msrOptions__

#include "msrBasicTypes.h"

#include "optionsHandling.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class EXP msrOptions : public msrOptionsGroup
{
  public:

    static SMARTP<msrOptions> create (
      S_msrOptionsHandler optionsHandler);
      
    SMARTP<msrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printMsrOptionsHelp ();

    void                  printMsrOptionsValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptions (
      S_msrOptionsHandler optionsHandler);
  
    virtual ~msrOptions ();
 
    // set and get
    // ------------------------------------------------------

    bool                  setMsrQuarterTonesPitchesLanguage (
                            string language);    
 
  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkMsrOptionsConsistency () const;

  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceMsr;
    
    bool                  fTraceMsrVisitors;
    
    bool                  fDisplayMsr;
    
    bool                  fDisplayMsrSummary;
    
    // languages
    // --------------------------------------
    
    msrQuarterTonesPitchesLanguage
                          fMsrQuarterTonesPitchesLanguage;
    
    // parts
    // --------------------------------------
    
    map<string, string>   fPartsRenamingMap;

    // voices
    // --------------------------------------
    
    bool                  fCreateVoicesStaffRelativeNumbers;
    
    bool                  fShowSilentVoices;
    bool                  fKeepSilentVoices;

    // notes
    // --------------------------------------
    
    bool                  fDelayRestsDynamics;
    bool                  fDelayRestsWords; // JMI
    bool                  fDelayRestsSlurs; // JMI
    bool                  fDelayRestsLigatures; // JMI
    bool                  fDelayRestsWedges; // JMI

    // lyrics
    // --------------------------------------
    
    bool                  fShowMsrStanzas;
    bool                  fKeepMuteStanzas;
    
    // harmonies
    // --------------------------------------
    
    bool                  fShowHarmonyVoices;
    bool                  fKeepEmptyHarmonyVoices;
    
    // figured bass
    // --------------------------------------
    
    bool                  fShowFiguredBassVoices;
    bool                  fKeepEmptyFiguredBassVoices;
};
typedef SMARTP<msrOptions> S_msrOptions;
EXP ostream& operator<< (ostream& os, const S_msrOptions& elt);

extern S_msrOptions gMsrOptions;
extern S_msrOptions gMsrOptionsUserChoices;
extern S_msrOptions gMsrOptionsWithDetailedTrace;


/*! @} */

}


#endif
