/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// global variables


//_______________________________________________________________________________
void initializeStringToMsrNoteNamesLanguage ()
{
  gMsrNoteNamesLanguageMap["dutch"] =     kNederlands;
  gMsrNoteNamesLanguageMap["catalan"] =   kCatalan;
  gMsrNoteNamesLanguageMap["deutsch"] =   kDeutsch;
  gMsrNoteNamesLanguageMap["english"] =   kEnglish;
  gMsrNoteNamesLanguageMap["espanol"] =   kEspanol;
  gMsrNoteNamesLanguageMap["italiano"] =  kItaliano;
  gMsrNoteNamesLanguageMap["francais"] =  kFrancais;
  gMsrNoteNamesLanguageMap["norsk"] =     kNorsk;
  gMsrNoteNamesLanguageMap["portugues"] = kPortugues;
  gMsrNoteNamesLanguageMap["suomi"] =     kSuomi;
  gMsrNoteNamesLanguageMap["svenska"] =   kSvenska;
  gMsrNoteNamesLanguageMap["vlaams"] =    kVlaams;
}

msrNoteNamesLanguage getMsrNoteNamesLanguage (string lang)
{
  return gMsrNoteNamesLanguageMap[lang];
}

//_______________________________________________________________________________
S_msrOptions msrOptions::create ()
{
  msrOptions* o = new msrOptions();
  assert(o!=0);
  return o;
}

msrOptions::msrOptions () {}

msrOptions::~msrOptions () {}

S_msrOptions gMsrOptions;

//______________________________________________________________________________
msrElement::msrElement (
  int           inputLineNumber)
{
  fInputLineNumber = inputLineNumber;  
}

msrElement::~msrElement() {}

void msrElement::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr <<
      "==> msrElement::acceptIn()" << endl;
      
  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr <<
            "==> Launching msrElement::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrElement::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr <<
      "==> msrElement::acceptOut()" << endl;

  if (visitor<S_msrElement>*
    p =
      dynamic_cast<visitor<S_msrElement>*> (v)) {
        S_msrElement elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr <<
            "==> Launching msrElement::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

ostream& operator<< (ostream& os, const S_msrElement& elt)
{
  elt->print (os);
  return os;
}

void msrElement::print (ostream& os)
{
  os << "??? Element ???" << endl;
}

//______________________________________________________________________________
ostream& operator<< (ostream& os, msrNoteData& noteData)
{
  noteData.print (os);
  return os;
}

void msrNoteData::init ()
{
  fStep = '?';
  fStepIsARest = false;;
  fStepIsUnpitched = false;;
  
  fAlter = 0.0;
  fAlteration = kNatural;
  
  fOctave = -1;

  // MusicXML durations are in divisions per quarter note.
  // LilyPond durations are in whole notes,
  // hence the "* 4" multiplications
  
  // the note duration when played
  fDivisions = -17;

  // tuplets member notes need another value for display
  fDisplayDivisions = -19;

  fDotsNumber = 0;
  
  fType = "";

  fNoteIsAGraceNote = false;
  
  fNoteBelongsToAChord = false;
  
  fNoteBelongsToATuplet = false;

  fStaffNumber = 0;
  fVoiceNumber = 0;
}

msrNoteData::msrNoteData ()
{
  init ();
}

void msrNoteData::print (ostream& os)
{
  os <<
    idtr << left <<
      setw(29) <<
      "fStep" << " = " << fStep <<
      endl <<
    idtr << left <<
      setw(29) << "fStepIsARest" << " = " <<
      fStepIsARest <<
      endl <<
    idtr << left <<
      setw(29) << "fAlteration" << " = " <<
      fAlteration <<
      endl <<
    idtr << left <<
      setw(29) << "fOctave" << " = " <<
      fOctave <<
      endl <<
    idtr << left <<
      setw(29) << "fDivisions" << " = " <<
      fDivisions <<
      endl <<
    idtr << left <<
      setw(29) << "fDisplayDivisions" << " = " <<
      fDisplayDivisions <<
      endl <<
    idtr << left <<
      setw(29) << "fDotsNumber" << " = " <<
      fDotsNumber <<
      endl <<
    idtr << left <<
      setw(29) << "fType" << " = " <<
      fType <<
      endl <<
      
    idtr << left <<
      setw(29) << "fNoteBelongsToAChord" << " = " <<
      fNoteBelongsToAChord <<
      endl <<
          
    idtr << left <<
      setw(29) << "fNoteBelongsToATuplet" << " = " <<
      fNoteBelongsToATuplet <<
      endl <<
          
    idtr << left <<
      setw(29) << "fStaffNumber" << " = " <<
      fStaffNumber <<
      endl <<
    idtr <<
      setw(29) << "fVoiceNumber" << " = " <<
      fVoiceNumber <<
      endl;
};


//______________________________________________________________________________
void msrBeatData::print (ostream& os)
{
  os <<
    left << setw(26) << "fBeatUnit = " << fBeatUnit <<  endl <<
    left << setw(26) << "fDots = " << fDots <<  endl;
};

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* o =
    new msrOctaveShift (
      inputLineNumber, octaveShiftKind, octaveShiftSize);
  assert(o!=0);
  return o;
}

msrOctaveShift::msrOctaveShift (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrElement (inputLineNumber)
{
  fOctaveShiftKind = octaveShiftKind;

  fOctaveShiftSize = octaveShiftSize;
}

msrOctaveShift::~msrOctaveShift() {}

void msrOctaveShift::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrOctaveShift::acceptIn()" << endl;
      
  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrOctaveShift::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrOctaveShift::acceptOut()" << endl;

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrOctaveShift::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrOctaveShift::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOctaveShift& elt)
{
  elt->print (os);
  return os;
}

string msrOctaveShift::octaveShiftKindAsString () const
{
  string result;
  
  switch (fOctaveShiftKind) {
    case kOctaveShiftUp:
      result = "up";
      break;
    case kOctaveShiftDown:
      result = "down";
      break;
    case kOctaveShiftStop:
      result = "stop";
      break;
  } // switch

  return result;
}

void msrOctaveShift::print (ostream& os)
{
  idtr++;
  
  os <<
    "OctaveShift" <<
    ", kind: " << octaveShiftKindAsString () <<
    ", size: " << fOctaveShiftSize <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_msrStem msrStem::create (
  int           inputLineNumber,
  msrStemKind   stemKind)
{
  msrStem* o =
    new msrStem (
      inputLineNumber, stemKind);
  assert(o!=0);
  return o;
}

msrStem::msrStem (
  int           inputLineNumber,
  msrStemKind   stemKind)
    : msrElement (inputLineNumber)
{
  fStemKind   = stemKind; 
}

msrStem::~msrStem() {}

void msrStem::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStem::acceptIn()" << endl;
      
  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStem::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrStem::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStem::acceptOut()" << endl;

  if (visitor<S_msrStem>*
    p =
      dynamic_cast<visitor<S_msrStem>*> (v)) {
        S_msrStem elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStem::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrStem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStem& elt)
{
  elt->print (os);
  return os;
}

void msrStem::print (ostream& os)
{
  idtr++;
  
  os <<
    "Stem" " ";

  switch (fStemKind) {
    case k_NoStem:
      os << "none";
      break;
    case kStemUp:
      os << "up";
      break;
    case kStemDown:
      os << "down";
      break;
    case kStemNone:
      os << "none";
      break;
    case kStemDouble:
      os << "double";
      break;
  } // switch
  os <<
    ", line " << fInputLineNumber <<
    endl;

  idtr--;
}

//______________________________________________________________________________
S_msrBeam msrBeam::create (
  int           inputLineNumber,
  int           number,
  msrBeamKind   beamKind)
{
  msrBeam* o =
    new msrBeam (
      inputLineNumber, number, beamKind);
  assert(o!=0);
  return o;
}

msrBeam::msrBeam (
  int           inputLineNumber,
  int           number,
  msrBeamKind   beamKind)
    : msrElement (inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = beamKind; 
}

msrBeam::~msrBeam() {}

void msrBeam::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBeam::acceptIn()" << endl;
      
  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBeam::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBeam::acceptOut()" << endl;

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBeam::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBeam& elt)
{
  elt->print (os);
  return os;
}

void msrBeam::print (ostream& os)
{
  idtr++;
  
  os <<
    "Beam" <<
    " number " << fBeamNumber <<
    ", line " << fInputLineNumber << " ";

  switch (fBeamKind) {
    case kBeginBeam:
      os << "begin";
      break;
    case kContinueBeam:
      os << "continue";
      break;
    case kEndBeam:
      os << "end";
      break;
    case kForwardHookBeam:
      os << "forward";
      break;
    case kBackwardHookBeam:
      os << "backward";
      break;
    case k_NoBeam:
      os << "### none ###";
      break;
  } // switch
  os << endl;

  idtr--;
}

//______________________________________________________________________________
S_msrArticulation msrArticulation::create (
  int                 inputLineNumber,
  msrArticulationKind articulationKind)
{
  msrArticulation* o =
    new msrArticulation (
      inputLineNumber, articulationKind);
  assert (o!=0);
  return o;
}

msrArticulation::msrArticulation (
  int                 inputLineNumber,
  msrArticulationKind articulationKind)
    : msrElement (inputLineNumber)
{
  fArticulationKind = articulationKind;
}

msrArticulation::~msrArticulation() {}

string msrArticulation::articulationKindAsString () const
{
  string result;
  
  switch (fArticulationKind) {
    case msrArticulation::kAccent:
      result = "accent";
      break;
    case msrArticulation::kBreathMark:
      result = "breathMark";
      break;
    case msrArticulation::kCaesura:
      result = "caesura";
      break;
    case msrArticulation::kSpiccato:
      result = "spiccato";
      break;
    case msrArticulation::kStaccato:
      result = "staccato";
      break;
    case msrArticulation::kStaccatissimo:
      result = "staccatissimo";
      break;
    case msrArticulation::kStress:
      result = "stress";
      break;
    case msrArticulation::kUnstress:
      result = "unstress";
      break;
    case msrArticulation::kDetachedLegato:
      result = "detached legato";
      break;
    case msrArticulation::kStrongAccent:
      result = "strong accent";
      break;
    case msrArticulation::kTenuto:
      result = "tenuto";
      break;
    case msrArticulation::kFermata:
      result = "fermata";
      break;
    case msrArticulation::kArpeggiato:
      result = "arpeggiato";
      break;
    case msrArticulation::kDoit:
      result = "doit";
      break;
    case msrArticulation::kFalloff:
      result = "falloff";
      break;
    case msrArticulation::kPlop:
      result = "plop";
      break;
    case msrArticulation::kScoop:
      result = "scoop";
      break;
  } // switch

  return result;
}

void msrArticulation::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrArticulation::acceptIn()" << endl;
      
  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrArticulation::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrArticulation::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrArticulation::acceptOut()" << endl;

  if (visitor<S_msrArticulation>*
    p =
      dynamic_cast<visitor<S_msrArticulation>*> (v)) {
        S_msrArticulation elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrArticulation::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrArticulation::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrArticulation& elt)
{
  elt->print (os);
  return os;
}

void msrArticulation::print (ostream& os)
{
  os <<
    "Articulation" " " <<
    articulationKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrOrnament msrOrnament::create (
  int             inputLineNumber,
  msrOrnamentKind ornamentKind)
{
  msrOrnament* o =
    new msrOrnament (
      inputLineNumber, ornamentKind);
  assert (o!=0);
  return o;
}

msrOrnament::msrOrnament (
  int             inputLineNumber,
  msrOrnamentKind ornamentKind)
    : msrElement (inputLineNumber)
{
  fOrnamentKind = ornamentKind;
}

msrOrnament::~msrOrnament() {}

string msrOrnament::ornamentKindAsString () const
{
  string result;
  
  switch (fOrnamentKind) {
    case msrOrnament::kTrillMark:
      result = "trill";
      break;
    case msrOrnament::kWavyLine:
      result = "wayvy line";
      break;
    case msrOrnament::kTurn:
      result = "turn";
      break;
    case msrOrnament::kInvertedTurn:
      result = "inverted turn";
      break;
    case msrOrnament::kDelayedTurn:
      result = "delayed turn";
      break;
    case msrOrnament::kDelayedInvertedTurn:
      result = "delayed inverted turn";
      break;
    case msrOrnament::kVerticalTurn:
      result = "vertical turn";
      break;
    case msrOrnament::kMordent:
      result = "mordent";
      break;
    case msrOrnament::kInvertedMordent:
      result = "inverted mordent";
      break;
    case msrOrnament::kSchleifer:
      result = "schleifer";
      break;
    case msrOrnament::kShake:
      result = "shake";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentPlacementKindAsString () const
{
  string result;
  
  switch (fOrnamentPlacementKind) {
    case msrOrnament::k_NoPlacementKind:
      result = "none";
      break;
    case msrOrnament::kAbove:
      result = "above";
      break;
    case msrOrnament::kBelow:
      result = "below";
      break;
  } // switch

  return result;
}

string msrOrnament::ornamentAccidentalMarkKindAsString () const
{
  string result;
  
  switch (fOrnamentAccidentalMarkKind) {
    case msrOrnament::kNatural:
      result = "natural";
      break;
    case msrOrnament::kSharp:
      result = "sharp";
      break;
    case msrOrnament::kFlat:
      result = "flat";
      break;
  } // switch

  return result;
}

void msrOrnament::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrOrnament::acceptIn()" << endl;
      
  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrOrnament::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrOrnament::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrOrnament::acceptOut()" << endl;

  if (visitor<S_msrOrnament>*
    p =
      dynamic_cast<visitor<S_msrOrnament>*> (v)) {
        S_msrOrnament elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrOrnament::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrOrnament::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOrnament& elt)
{
  elt->print (os);
  return os;
}

void msrOrnament::print (ostream& os)
{
  os <<
    "Ornament" " " <<
    ornamentKindAsString () <<
    ", line " << fInputLineNumber <<
    ", placement" << " = " << ornamentPlacementKindAsString () <<
    ", accidental mark" << " = " << ornamentAccidentalMarkKindAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrRehearsal msrRehearsal::create (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
{
  msrRehearsal* o =
    new msrRehearsal (
      inputLineNumber, rehearsalKind, rehearsalText);
  assert (o!=0);
  return o;
}

msrRehearsal::msrRehearsal (
  int              inputLineNumber,
  msrRehearsalKind rehearsalKind,
  string           rehearsalText)
    : msrElement (inputLineNumber)
{
  fRehearsalKind = rehearsalKind;

  fRehearsalText = rehearsalText;
}

msrRehearsal::~msrRehearsal() {}

void msrRehearsal::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRehearsal::acceptIn()" << endl;
      
  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRehearsal::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrRehearsal::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRehearsal::acceptOut()" << endl;

  if (visitor<S_msrRehearsal>*
    p =
      dynamic_cast<visitor<S_msrRehearsal>*> (v)) {
        S_msrRehearsal elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRehearsal::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrRehearsal::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrRehearsal& elt)
{
  elt->print (os);
  return os;
}

void msrRehearsal::print (ostream& os)
{
  os <<
    "Rehearsal" << " " << fRehearsalText <<
    " kind: ";

  switch (fRehearsalKind) {
    case kNone:
      os << "none";
      break;
    case kRectangle:
      os << "rectangle";
      break;
    case kOval:
      os << "oval";
      break;
    case kCircle:
      os << "circle";
      break;
    case kBracket:
      os << "bracket";
      break;
    case kTriangle:
      os << "triangle";
      break;
    case kDiamond:
      os << "diamond";
      break;
  } // switch
  
  os << endl;
}

//______________________________________________________________________________
S_msrDynamics msrDynamics::create (
  int             inputLineNumber,
  msrDynamicsKind dynamicsKind)
{
  msrDynamics* o =
    new msrDynamics (
      inputLineNumber, dynamicsKind);
    assert(o!=0);
  return o;
}

msrDynamics::msrDynamics (
  int             inputLineNumber,
  msrDynamicsKind dynamicsKind)
    : msrElement (inputLineNumber)
{
  fDynamicsKind = dynamicsKind; 
}
msrDynamics::~msrDynamics() {}

string msrDynamics::dynamicsKindAsString ()
{
  stringstream s;
  
  switch (fDynamicsKind) {
    case kF:
      s << "f";
      break;
    case kFF:
      s << "ff"; break;
    case kFFF:
      s << "fff"; break;
    case kFFFF:
      s << "ffff"; break;
    case kFFFFF:
      s << "fffff"; break;
    case kFFFFFF:
      s << "ffffff"; break;

    case kP:
      s << "p"; break;
    case kPP:
      s << "pp"; break;
    case kPPP:
      s << "ppp"; break;
    case kPPPP:
      s << "pppp"; break;
    case kPPPPP:
      s << "ppppp"; break;
    case kPPPPPP:
      s << "pppppp"; break;

    case kMF:
      s << "mf"; break;
    case kMP:
      s << "mp"; break;
    case kFP:
      s << "fp"; break;
    case kFZ:
      s << "fz"; break;
    case kRF:
      s << "rf"; break;
    case kSF:
      s << "sf"; break;

    case kRFZ:
      s << "rfz"; break;
    case kSFZ:
      s << "sfz"; break;
    case kSFP:
      s << "sfp"; break;
    case kSFPP:
      s << "sfpp"; break;
    case kSFFZ:
      s << "sffz"; break;

    default:
      {
      stringstream s;
      
      s << "Dynamics " << fDynamicsKind << " is unknown";
      
      msrMusicXMLError (
        fInputLineNumber,
        s.str());
      }
  } // switch
  
  return s.str();
}

void msrDynamics::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrDynamics::acceptIn()" << endl;
      
  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrDynamics::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrDynamics::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrDynamics::acceptOut()" << endl;

  if (visitor<S_msrDynamics>*
    p =
      dynamic_cast<visitor<S_msrDynamics>*> (v)) {
        S_msrDynamics elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrDynamics::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrDynamics::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDynamics& elt)
{
  elt->print (os);
  return os;
}

void msrDynamics::print (ostream& os)
{
  os <<
    "Dynamics" " " << dynamicsKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrWedge msrWedge::create (
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
{
  msrWedge* o =
    new msrWedge (
      inputLineNumber, wedgeKind);
  assert(o!=0);
  return o;
}

msrWedge::msrWedge (
  int           inputLineNumber,
  msrWedgeKind  wedgeKind)
    : msrElement (inputLineNumber)
{
  fWedgeKind = wedgeKind; 
}
msrWedge::~msrWedge() {}

string msrWedge::wedgeKindAsString ()
{
  stringstream s;
  
  switch (fWedgeKind) {
    case msrWedge::kCrescendoWedge:
      s << "crescendo";
      break;
    case msrWedge::kDecrescendoWedge:
      s << "decrescendo";
      break;
    case msrWedge::kStopWedge:
      s << "stop";
      break;
  } // switch
    
  return s.str();
}

void msrWedge::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWedge::acceptIn()" << endl;
      
  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWedge::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrWedge::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWedge::acceptOut()" << endl;

  if (visitor<S_msrWedge>*
    p =
      dynamic_cast<visitor<S_msrWedge>*> (v)) {
        S_msrWedge elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWedge::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrWedge::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWedge& elt)
{
  elt->print (os);
  return os;
}

void msrWedge::print (ostream& os)
{
  os <<
    "Wedge" << " " << wedgeKindAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrTie msrTie::create (
  int           inputLineNumber,
  msrTieKind    tieKind)
{
  msrTie* o =
    new msrTie (
      inputLineNumber, tieKind);
  assert(o!=0);
  return o;
}

msrTie::msrTie (
  int           inputLineNumber,
  msrTieKind    tieKind)
    : msrElement (inputLineNumber)
{
  fTieKind = tieKind; 
}
msrTie::~msrTie() {}

string msrTie::tieKindAsString (msrTieKind tieKind)
{
  stringstream s;
  
  switch (tieKind) {
    case kStartTie:
      s << "start";
      break;
    case kContinueTie:
      s << "continue";
      break;
    case kStopTie:
      s << "stop";
      break;
    default:
      s << "Tie" << tieKind << "???";
  } // switch
    
  return s.str();
}

void msrTie::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTie::acceptIn()" << endl;
      
  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTie::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTie::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTie::acceptOut()" << endl;

  if (visitor<S_msrTie>*
    p =
      dynamic_cast<visitor<S_msrTie>*> (v)) {
        S_msrTie elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTie::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTie::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTie& elt)
{
  elt->print (os);
  return os;
}

void msrTie::print (ostream& os)
{
  os <<
    "Tie" << " " << tieKindAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrSlur msrSlur::create (
  int           inputLineNumber,
  int           slurNumber,
  msrSlurKind   slurKind)
{
  msrSlur* o =
    new msrSlur (
      inputLineNumber, slurNumber, slurKind);
  assert(o!=0);
  return o;
}

msrSlur::msrSlur (
  int           inputLineNumber,
  int           slurNumber,
  msrSlurKind   slurKind)
    : msrElement (inputLineNumber)
{
  fSlurNumber = slurNumber;
  fSlurKind   = slurKind; 
}
msrSlur::~msrSlur() {}

string msrSlur::slurKindAsString ()
{
  stringstream s;
  
  switch (fSlurKind) {
    case msrSlur::kStartSlur:
      s << "start";
      break;
    case msrSlur::kContinueSlur:
      s << "continue";
      break;
    case msrSlur::kStopSlur:
      s << "stop";
      break;
    default:
      s << "Slur" << fSlurKind << "???";
  } // switch
    
  return s.str();
}

void msrSlur::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrSlur::acceptIn()" << endl;
      
  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrSlur::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrSlur::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrSlur::acceptOut()" << endl;

  if (visitor<S_msrSlur>*
    p =
      dynamic_cast<visitor<S_msrSlur>*> (v)) {
        S_msrSlur elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrSlur::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrSlur::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSlur& elt)
{
  elt->print (os);
  return os;
}

void msrSlur::print (ostream& os)
{
  os <<
    "Slur" " " << slurKindAsString () <<
    ", line " << fInputLineNumber << " " <<
    endl;
}

//______________________________________________________________________________
S_msrGracenotes msrGracenotes::create (
  int             inputLineNumber,
  bool            slashed,
  S_msrVoice      gracenotesVoiceUplink)
{
  msrGracenotes* o =
    new msrGracenotes (
      inputLineNumber,
      slashed, gracenotesVoiceUplink);
  assert(o!=0);
  return o;
}

msrGracenotes::msrGracenotes (
  int             inputLineNumber,
  bool            slashed,
  S_msrVoice      gracenotesVoiceUplink)
    : msrElement (inputLineNumber)
{
  fGracenotesIsSlashed = slashed;

  fGracenotesVoiceUplink =
    gracenotesVoiceUplink;

  // create the voice chunk that will receive the notes
  fGracenotesVoicechunk =
    msrVoicechunk::create (
      fInputLineNumber,
      gracenotesVoiceUplink);
}

msrGracenotes::~msrGracenotes() {}

S_msrGracenotes msrGracenotes::createGracenotesBareClone (
  S_msrVoice voiceClone)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of grace notes" <<
      endl;
  
  S_msrGracenotes
    clone =
      msrGracenotes::create (
        fInputLineNumber,
        fGracenotesIsSlashed,
        voiceClone);
  
  return clone;
}

void msrGracenotes::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrGracenotes::acceptIn()" << endl;
      
  if (visitor<S_msrGracenotes>*
    p =
      dynamic_cast<visitor<S_msrGracenotes>*> (v)) {
        S_msrGracenotes elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrGracenotes::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrGracenotes::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrGracenotes::acceptOut()" << endl;

  if (visitor<S_msrGracenotes>*
    p =
      dynamic_cast<visitor<S_msrGracenotes>*> (v)) {
        S_msrGracenotes elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrGracenotes::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrGracenotes::browseData (basevisitor* v)
{
  // browse the voicechunk
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fGracenotesVoicechunk);
}

ostream& operator<< (ostream& os, const S_msrGracenotes& elt)
{
  elt->print (os);
  return os;
}

void msrGracenotes::print (ostream& os)
{
  os <<
    "Gracenotes" <<
    ", line " << fInputLineNumber <<
    ", slashed: " <<
    string (
      fGracenotesIsSlashed
        ? "yes"
        : "no") <<
    endl;
  
  idtr++;
  
  os << fGracenotesVoicechunk;
      
  idtr--;
}

//______________________________________________________________________________
S_msrNote msrNote::createFromNoteData (
  int           inputLineNumber,
  msrNoteData & noteData)
{  
  msrNote * o =
    new msrNote (
      inputLineNumber, noteData);
  assert(o!=0); 
  return o;
}

S_msrNote msrNote::createSkipNote (
  int           inputLineNumber,
  int           divisions,
  int           divisionsPerWholeNote,
  int           staffNumber,
  int           externalVoiceNumber)
{
  msrNoteData noteData;

  noteData.fStep = 's';
  noteData.fStepIsARest = true;
  
  noteData.fDivisions = divisions;
  noteData.fDisplayDivisions = divisions;
    
  noteData.fStaffNumber = staffNumber;
  noteData.fVoiceNumber = externalVoiceNumber;

  msrNote * o =
    new msrNote (
      inputLineNumber, noteData);
  assert(o!=0);

  // set rest's note kind
  o->fNoteKind = kSkipNote;
  
  // set rest's divisions per whole note
  o->fNoteDivisionsPerWholeNote = divisionsPerWholeNote;
  
  return o;
}    

S_msrNote msrNote::createNoteBareClone ()
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of note " <<
      noteAsString () <<
      endl;

  S_msrNote
    clone =
      msrNote::createFromNoteData (
        fInputLineNumber,
        fNoteData);

  clone->fNoteKind = fNoteKind;
  
  clone->fNoteOctaveShift = fNoteOctaveShift;
  
  clone->fNoteStem = fNoteStem;  

  clone->fNoteDivisionsPerWholeNote = fNoteDivisionsPerWholeNote;
  
  return clone;
}

msrNote::msrNote (
  int           inputLineNumber,
  msrNoteData&  noteData)
  :
    msrElement (inputLineNumber),
    fNoteData (noteData)
{
//  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
  if (gGeneralOptions->fDebugDebug) {
    cerr << idtr <<
      "==> fNoteData contains:" <<
      endl;

    idtr++;
    
    cerr <<
      fNoteData;

    idtr--;

    cerr <<
      idtr <<
        "<==" <<
        endl << endl;
  }

  fNoteKind = k_NoNoteKind;

  if (
    fNoteData.fStep < 'A'
      ||
    fNoteData.fStep > 'G') {
    if (! fNoteData.fStepIsARest) {
      stringstream s;
      
      s <<
        "step value " << fNoteData.fStep <<
        " is not a letter from A to G";
        
    msrMusicXMLError (
 // JMI   msrMusicXMLWarning (
      fInputLineNumber,
      s.str());
    }
  }

//  cerr << "=== xmlPart2MsrVisitor::visitStart ( S_step& elt ) " << fCurrentStep << endl;
// JMI

  switch (fNoteData.fStep) {
    case 'A': fNoteData.fDiatonicPitch = msrNoteData::kA; break;
    case 'B': fNoteData.fDiatonicPitch = msrNoteData::kB; break;
    case 'C': fNoteData.fDiatonicPitch = msrNoteData::kC; break;
    case 'D': fNoteData.fDiatonicPitch = msrNoteData::kD; break;
    case 'E': fNoteData.fDiatonicPitch = msrNoteData::kE; break;
    case 'F': fNoteData.fDiatonicPitch = msrNoteData::kF; break;
    case 'G': fNoteData.fDiatonicPitch = msrNoteData::kG; break;
    default: {}
  } // switch

  // how many quater tones from A?s
  int noteQuatertonesFromA;
  
  switch (fNoteData.fStep) {
    case 'A': noteQuatertonesFromA =  0; break;
    case 'B': noteQuatertonesFromA =  4; break;
    case 'C': noteQuatertonesFromA =  6; break;
    case 'D': noteQuatertonesFromA = 10; break;
    case 'E': noteQuatertonesFromA = 14; break;
    case 'F': noteQuatertonesFromA = 16; break;
    case 'G': noteQuatertonesFromA = 20; break;
    default: {}    
  } // switch

//   if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
  if (gGeneralOptions->fDebugDebug) {
    cerr << idtr <<
      "--> fNoteData.fAlter = " <<
      fNoteData.fAlter <<
      endl;
  }

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones.

  The following table lists note names for quarter-tone accidentals
  in various languages; here the pre- fixes semi- and sesqui-
  respectively mean ‘half’ and ‘one and a half’.
  
  Languages that do not appear in this table do not provide special note names yet.
  Language
              semi-sharp semi-flat sesqui-sharp sesqui-flat
                 +0.5      -0.5        +1.5       -1.5
    nederlands   -ih       -eh        -isih       -eseh

  We use dutch pitches names for the enumeration below.
  The following is a series of Cs with increasing pitches:
    \relative c'' { ceseh ces ceh c cih cis cisih }
*/

  if      (fNoteData.fAlter == 0 ) {
    fNoteData.fAlteration = msrNoteData::kNatural;
  }
  
  else if (fNoteData.fAlter == -1 ) {
    fNoteData.fAlteration = msrNoteData::kFlat;
    noteQuatertonesFromA -= 2;
    if (noteQuatertonesFromA < 0)
      noteQuatertonesFromA += 24; // it is below A
  }
  
  else if (fNoteData.fAlter == 1 ) {
    fNoteData.fAlteration = msrNoteData::kSharp;
    noteQuatertonesFromA += 2;
  }
  
  else if (fNoteData.fAlter == -0.5 ) {
    fNoteData.fAlteration = msrNoteData::kSemiFlat;
    noteQuatertonesFromA -= 1;
    if (noteQuatertonesFromA < 0)
      noteQuatertonesFromA += 24; // it is below A
  }
  
  else if (fNoteData.fAlter == +0.5 ) {
    fNoteData.fAlteration = msrNoteData::kSemiSharp;
    noteQuatertonesFromA += 1;
  }
  
  else if (fNoteData.fAlter == -1.5 ) {
    fNoteData.fAlteration = msrNoteData::kSesquiFlat;
    noteQuatertonesFromA -= 3;
    if (noteQuatertonesFromA < 0)
      noteQuatertonesFromA += 24; // it is below A
  }
  
  else if (fNoteData.fAlter == +1.5 ) {
    fNoteData.fAlteration = msrNoteData::kSesquiSharp;
    noteQuatertonesFromA += 3;
  }
  
  else if (fNoteData.fAlter == -2 ) {
    fNoteData.fAlteration = msrNoteData::kSesquiFlat;
    noteQuatertonesFromA -= 3;
    if (noteQuatertonesFromA < 0)
      noteQuatertonesFromA += 24; // it is below A
  }
  
  else if (fNoteData.fAlter == +2 ) {
    fNoteData.fAlteration = msrNoteData::kSesquiSharp;
    noteQuatertonesFromA += 3;
  }
  
  else {
    stringstream s;
    
    s <<
      " alter " << fNoteData.fAlter <<
      " should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      fInputLineNumber,
      s.str());
  }

  fNoteOccupiesAFullMeasure = false;
  
  fNoteHasATrill = false;
  
/* JMI
  if (gGeneralOptions->fDebugDebug)
    cerr <<
      "--> noteQuatertonesFromA = " << noteQuatertonesFromA <<
      endl;
*/
}

msrNote::~msrNote()
{}

int msrNote::getNoteMeasureNumber () const
{
  return
    fNoteMeasureUplink
      ? fNoteMeasureUplink->getMeasureNumber ()
      : -94;
}

void msrNote::setNoteBelongsToAChord () {
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> note " << noteAsString () <<
      ", line " << fInputLineNumber <<
      ", is set to belong to a chord" << endl;

  fNoteData.fNoteBelongsToAChord = true;
  fNoteKind = msrNote::kChordMemberNote;
}

void msrNote::applyTupletMemberDisplayFactor (
  int actualNotes, int normalNotes)
{
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> applying tuplet display factor " <<
      actualNotes << "/" << normalNotes <<
      " to note " << this <<
      ", line " << fInputLineNumber <<
      endl;

  fNoteData.fDisplayDivisions =
    fNoteData.fDisplayDivisions
      *
    actualNotes
      /
    normalNotes;
}

void msrNote::addBeamToNote (S_msrBeam beam)
{
  fNoteBeams.push_back (beam);
}

void msrNote::addArticulationToNote (S_msrArticulation art)
{
  fNoteArticulations.push_back (art);
}

void msrNote::addOrnamentToNote (S_msrOrnament art)
{
  fNoteOrnaments.push_back (art);

  if (art->getOrnamentKind () == msrOrnament::kTrillMark)
    fNoteHasATrill = true;
}

void msrNote::addDynamicsToNote (S_msrDynamics dynamics)
{
  fNoteDynamics.push_back (dynamics);
}

void msrNote::addWordsToNote (S_msrWords words)
{
  fNoteWords.push_back (words);
}

void msrNote::addSlurToNote (S_msrSlur slur)
{
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "% --> adding slur " << slur << " to note " << noteAsString ()
       << endl;

  if (fNoteSlurs.size ()) {
    if (
      fNoteSlurs.back ()->getSlurKind () == msrSlur::kStartSlur
        &&
      slur->getSlurKind () == msrSlur::kStopSlur
        &&
      fNoteSlurs.back ()->getSlurNumber () == slur->getSlurNumber ()
      ) {
      // it may happen that a given note has a 'slur start'
      // and a 'slur stop' in sequence, ignore both

      stringstream s;
      
      s <<
        "a 'slur start' is immediately followed by a 'slur stop'" <<
        endl <<
        "with the same number, ignoring both of them at line " <<
        slur->getInputLineNumber ();
        
      msrMusicXMLWarning (
        slur->getInputLineNumber (),
        s.str());
        
      // rmeove 'slur start'
      fNoteSlurs.pop_back ();

      // don't register 'slur stop'
    }

    else
      fNoteSlurs.push_back (slur);
  }

  else
    fNoteSlurs.push_back (slur);
}

void msrNote::addWedgeToNote (S_msrWedge wedge)
{
  fNoteWedges.push_back (wedge);
}

S_msrDynamics msrNote::removeFirstDynamics () // JMI
{
  S_msrDynamics dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();
  return dyn;
}

S_msrWedge msrNote::removeFirstWedge () // JMI
{
  S_msrWedge wedge = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wedge;
}

void msrNote::acceptIn (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrNote::acceptIn()" << endl;
      
  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrNote::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrNote::acceptOut()" << endl;

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrNote::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrNote::browseData (basevisitor* v)
{
  if (fNoteOctaveShift) {
    // browse the octave shift
    msrBrowser<msrOctaveShift> browser (v);
    browser.browse (*fNoteOctaveShift);
  }

  if (fNoteStem) {
    // browse the stem
    msrBrowser<msrStem> browser (v);
    browser.browse (*fNoteStem);
  }

  // browse the beams if any
  if (fNoteBeams.size()) {
    idtr++;
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin(); i!=fNoteBeams.end(); i++) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the articulations if any
  if (fNoteArticulations.size()) {
    idtr++;
    list<S_msrArticulation>::const_iterator i;
    for (i=fNoteArticulations.begin(); i!=fNoteArticulations.end(); i++) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the ornaments if any
  if (fNoteOrnaments.size()) {
    idtr++;
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin(); i!=fNoteOrnaments.end(); i++) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  if (fNoteTie) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*fNoteTie);
  }

  // browse the dynamics if any
  if (fNoteDynamics.size()) {
    idtr++;
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin(); i!=fNoteDynamics.end(); i++) {
      // browse the dynamics
      msrBrowser<msrDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the words if any
  if (fNoteWords.size()) {
    idtr++;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin(); i!=fNoteWords.end(); i++) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }
  
  // browse the slur if any
  if (fNoteSlurs.size()) {
    idtr++;
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin(); i!=fNoteSlurs.end(); i++) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  // browse the wedges if any
  if (fNoteWedges.size()) {
    idtr++;
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin(); i!=fNoteWedges.end(); i++) {
      // browse the wedge
      msrBrowser<msrWedge> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

/* JMI
  // browse the slur if any
  if (fNoteSlurKind != msrSlur::k_NoSlur) {
    // browse the slur
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  }
  */
}

string msrNote::notePitchAsString () const
{
  stringstream s;
  
  /*
  cerr << "msrNote::notePitchAsString (), isRest = " <<
    fNoteData.fStepIsARest <<
    ", fMsrPitch = " << fMsrPitch << endl;
  */
  
  if (fNoteData.fStepIsARest)

    s << "r"; // JMI R ???

  else if (fNoteData.fStepIsUnpitched)

    s << "unpitched ";

  else {

    switch (fNoteData.fDiatonicPitch) {
      case msrNoteData::kA: s << "a"; break;
      case msrNoteData::kB: s << "b"; break;
      case msrNoteData::kC: s << "c"; break;
      case msrNoteData::kD: s << "d"; break;
      case msrNoteData::kE: s << "e"; break;
      case msrNoteData::kF: s << "f"; break;
      case msrNoteData::kG: s << "g"; break;
      default: s << "?";
    } // switch

    /*
    The alter element represents chromatic alteration
    in number of semitones (e.g., -1 for flat, 1 for sharp).
    Decimal values like 0.5 (quarter tone sharp) are used for microtones.
  
    The following table lists note names for quarter-tone accidentals
    in various languages; here the pre- fixes semi- and sesqui-
    respectively mean ‘half’ and ‘one and a half’.
    
    Languages that do not appear in this table do not provide special note names yet.
    Language
                semi-sharp semi-flat sesqui-sharp sesqui-flat
                   +0.5      -0.5        +1.5       -1.5
      nederlands   -ih       -eh        -isih       -eseh
  
    We use dutch pitches names for the enumeration below.
    The following is a series of Cs with increasing pitches:
      \relative c'' { ceseh ces ceh c cih cis cisih }
  */

    switch (fNoteData.fAlteration) {

      case msrNoteData::kDoubleFlat:
        s << "eses";
        break;
        
      case msrNoteData::kSesquiFlat:
        s << "eseh";
        break;
        
      case msrNoteData::kFlat:
        s << "es";
        break;
        
      case msrNoteData::kSemiFlat:
        s << "eh";
        break;
        
      case msrNoteData::kNatural:
        break;
        
      case msrNoteData::kSemiSharp:
        s << "ih";
        break;
        
      case msrNoteData::kSharp:
        s << "is";
        break;
        
      case msrNoteData::kSesquiSharp:
        s << "isih";
        break;
        
      case msrNoteData::kDoubleSharp:
        s << "isis";
        break;      
    } // switch  
  }
  
  return s.str();
}

string msrNote::noteDivisionsAsMSRString () const
{
  string result;
  int    computedNumberOfDots;
  string errorMessage;

  result =
    divisionsAsMSRDuration (
      fNoteData.fDisplayDivisions,
      fNoteDivisionsPerWholeNote,
      fNoteData.fDotsNumber,
      computedNumberOfDots,
      errorMessage,
      false); // 'true' to debug it

  if (errorMessage.size ())
    msrMusicXMLError (
      fInputLineNumber,
      errorMessage);

  return result;
}

string msrNote::noteTypeAsMSRString () const
{
  string result;
  string errorMessage;

  result =
    noteTypeAsMSRDuration (
      fNoteData.fType,
      errorMessage);

  if (errorMessage.size ())
    msrMusicXMLError (
      fInputLineNumber,
      errorMessage);

  return result;
}

string msrNote::noteDiatonicPitchAsString () const
{
  // fNoteData.fStep is a char
  switch (fNoteData.fDiatonicPitch) {
    case msrNoteData::kA: return "A"; break;
    case msrNoteData::kB: return "B"; break;
    case msrNoteData::kC: return "C"; break;
    case msrNoteData::kD: return "D"; break;
    case msrNoteData::kE: return "E"; break;
    case msrNoteData::kF: return "F"; break;
    case msrNoteData::kG: return "G"; break;
    default: return "?";
  } // switch
}

string msrNote::noteAsString () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "k_NoNoteKind ???";
      break;
      
    case msrNote::kStandaloneNote:
      s <<
        "Standalone note" <<
        " " <<
        notePitchAsString () <<
        "[" << fNoteData.fOctave << "]" <<
        ":" <<
        noteDivisionsAsMSRString ();
      break;
      
    case msrNote::kGraceNote:
      s <<
        "Grace note" <<
        " " <<
        notePitchAsString () <<
        "[" << fNoteData.fOctave << "]" <<
        ":" <<
        noteTypeAsMSRString ();
      for (int i = 0; i < fNoteData.fDotsNumber; i++) {
        s << ".";
      } // for
      break;
      
    case msrNote::kRestNote:
      s <<
        "Rest" <<
        ":" <<
        noteDivisionsAsMSRString ();
      break;
      
    case msrNote::kSkipNote:
      s <<
        "Skip" <<
        ":" <<
        noteDivisionsAsMSRString ();
      break;
      
    case msrNote::kChordMemberNote:
      s <<
        "Chord member note" <<
        " " <<
        notePitchAsString () <<
        "[" << fNoteData.fOctave << "]" <<
        ":" <<
        noteDivisionsAsMSRString ();
      break;
      
    case msrNote::kTupletMemberNote:
      s <<
        "Tuplet member note" <<
        " " <<
        notePitchAsString ();

      if (! fNoteData.fStepIsARest)
        s <<
          "[" << fNoteData.fOctave << "]" <<
          ":" <<
          noteDivisionsAsMSRString ();
      break;
  } // switch

/* JMI
  if (fNoteTie) {
    if (fNoteTie->getTieKind () != msrTie::k_NoTie ) {
      s <<
        ", " << fNoteTie->tieKindAsString ();
    }
  }
*/

  return s.str();
}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print (os);
  return os;
}

void msrNote::print (ostream& os)
{
  rational
    position (
      fNotePositionInMeasure,
      fNoteDivisionsPerWholeNote);

  position.rationalise ();
  
  // print the note itself and its position
  os <<
    noteAsString ();

  if (fNoteData.fType.size ())
    os <<
      " (" << fNoteData.fType << ")";
      
  os <<
    ", line " << fInputLineNumber << ", " <<
    "(";
    
  if (fNoteKind == kGraceNote) {
    os <<
      fNoteData.fDisplayDivisions;
  }
  
  else {
    if (
        fNoteData.fDivisions
          ==
        fNoteData.fDisplayDivisions) {
      os <<
        fNoteData.fDivisions;
    }
    else {
      os <<
        fNoteData.fDivisions <<
        "_" <<
        fNoteData.fDisplayDivisions;
    }
  }

  os <<
    "/" <<
    fNoteDivisionsPerWholeNote <<
    ") @"<<
    getNoteMeasureNumber () <<
    ":" <<
    position.getNumerator() <<
    "/" <<
    position.getDenominator() <<
    " (" <<
    fNotePositionInMeasure <<
    "/" <<
    fNoteDivisionsPerWholeNote <<
    ")" <<
    endl;

  // print the octave shift if any
  if (fNoteOctaveShift) {
    idtr++;
    os <<
      idtr << fNoteOctaveShift;
    idtr--;
  }

  // print the stem if any
  if (fNoteStem) {
    idtr++;
    os <<
      idtr << fNoteStem;
    idtr--;
  }
    
  // print the beams if any
  if (fNoteBeams.size()) {
    idtr++;

    list<S_msrBeam>::const_iterator
      iBegin = fNoteBeams.begin(),
      iEnd   = fNoteBeams.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the articulations if any
  if (fNoteArticulations.size()) {
    idtr++;

    list<S_msrArticulation>::const_iterator
      iBegin = fNoteArticulations.begin(),
      iEnd   = fNoteArticulations.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the ornaments if any
  if (fNoteOrnaments.size()) {
    idtr++;

    list<S_msrOrnament>::const_iterator
      iBegin = fNoteOrnaments.begin(),
      iEnd   = fNoteOrnaments.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
        
    idtr--;
  }
  
  // print the tie if any
  if (fNoteTie) {
    idtr++;
    os <<
      idtr << fNoteTie;
    idtr--;
  }
    
  // print the dynamics if any
  if (fNoteDynamics.size()) {
    idtr++;
    
    list<S_msrDynamics>::const_iterator
      iBegin = fNoteDynamics.begin(),
      iEnd   = fNoteDynamics.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr--;
  }

  // print the words if any
  if (fNoteWords.size()) {
    idtr++;
    
    list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin(),
      iEnd   = fNoteWords.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr--;
  }

  // print the slurs if any
  if (fNoteSlurs.size()) {
    idtr++;
    
    list<S_msrSlur>::const_iterator
      iBegin = fNoteSlurs.begin(),
      iEnd   = fNoteSlurs.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
    
    idtr--;
  }
  
  // print the wedges if any
  if (fNoteWedges.size()) {
    idtr++;
    
    list<S_msrWedge>::const_iterator
      iBegin = fNoteWedges.begin(),
      iEnd   = fNoteWedges.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
    
    idtr--;
  }
}

//______________________________________________________________________________
S_msrChord msrChord::create (
  int           inputLineNumber,
  int           chordDivisions)
{
  msrChord* o =
    new msrChord (
      inputLineNumber, chordDivisions);
  assert(o!=0);
  return o;
}

msrChord::msrChord (
  int           inputLineNumber,
  int           chordDivisions)
    : msrElement (inputLineNumber)
{
  fChordDivisions = chordDivisions;
}

msrChord::~msrChord() {}

S_msrChord msrChord::createChordBareClone ()
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of chord" <<
      endl;

  S_msrChord
    clone =
      msrChord::create (
        fInputLineNumber,
        fChordDivisions);

  clone->
    fChordDivisionsPerWholeNote = fChordDivisionsPerWholeNote;

  clone->
    fChordPositionInMeasure = fChordPositionInMeasure;
    
  clone->fChordTie = fChordTie; // JMI

  return clone;
}
    
int msrChord::getChordMeasureNumber () const
{
  return
    fChordMeasureUplink
      ? fChordMeasureUplink->getMeasureNumber ()
      : -9994;
}

void msrChord::addNoteToChord (S_msrNote note)
{
  fChordNotes.push_back (note);
  
  note->setNoteBelongsToAChord ();
  
  // populate position in measure
  note->setNotePositionInMeasure (
    fChordPositionInMeasure);
}

void msrChord::addArticulationToChord (S_msrArticulation art)
{
  msrArticulation::msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't add the same articulation several times
  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin();
    i!=fChordArticulations.end();
    i++) {
      if ((*i)->getArticulationKind () == articulationKind)
        return;
  } // for

//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> adding articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;

  fChordArticulations.push_back (art);
}

void msrChord::addOrnamentToChord (S_msrOrnament orn)
{
  msrOrnament::msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't add the same ornament several times
  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin();
    i!=fChordOrnaments.end();
    i++) {
      if ((*i)->getOrnamentKind () == ornamentKind)
        return;
  } // for

//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> adding ornament '" <<
      orn->ornamentKindAsString () <<
      "' to chord" <<
      endl;

  fChordOrnaments.push_back (orn);
}

void msrChord::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrChord::acceptIn()" << endl;
      
  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrChord::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrChord::acceptOut()" << endl;

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrChord::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrChord::browseData (basevisitor* v)
{
  for (
    vector<S_msrNote>::const_iterator i = fChordNotes.begin();
    i != fChordNotes.end();
    i++ ) {
    // browse chord member note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin();
    i != fChordArticulations.end();
    i++ ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin();
    i != fChordOrnaments.end();
    i++ ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDynamics>::const_iterator i = fChordDynamics.begin();
    i != fChordDynamics.end();
    i++ ) {
    // browse the dynamics
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWords>::const_iterator i = fChordWords.begin();
    i != fChordWords.end();
    i++ ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlur>::const_iterator i = fChordSlurs.begin();
    i != fChordSlurs.end();
    i++ ) {
    // browse the slur
    msrBrowser<msrSlur> browser (v);
    browser.browse (*(*i));
  } // for
  
  for (
    list<S_msrWedge>::const_iterator i = fChordWedges.begin();
    i != fChordWedges.end();
    i++ ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrChord& elt)
{
  elt->print (os);
  return os;
}

string msrChord::chordDivisionsAsMSRString () const
{
  string result;
  int    computedNumberOfDots;
  string errorMessage;

  int inputSourceSuppliedNumberOfDots =
    fChordNotes [0]-> 
      getNoteDotsNumber (); // any chord member note is fine
    
  result =
    divisionsAsMSRDuration (
      fChordDivisions,
      fChordDivisionsPerWholeNote,
      inputSourceSuppliedNumberOfDots,
      computedNumberOfDots,
      errorMessage,
      false); // 'true' to debug it

  if (errorMessage.size ())
    msrMusicXMLError (
      fInputLineNumber,
      errorMessage);

  return result;
}

void msrChord::print (ostream& os)
{
  os <<
    "Chord" <<
    " (" <<
    chordDivisionsAsMSRString () <<
    "/" <<
    fChordDivisionsPerWholeNote <<
    ") @"<<
    getChordMeasureNumber () <<
    ":" <<
    fChordPositionInMeasure <<
    "/" <<
    fChordDivisionsPerWholeNote <<
    endl;

  idtr++;
  
  // print the member notes if any
  if (fChordNotes.size()) {
    vector<S_msrNote>::const_iterator i;
    for (i=fChordNotes.begin(); i!=fChordNotes.end(); i++) {
      os << idtr << (*i);
    } // for

/* JMI   vector<S_msrNote>::const_iterator
      iBegin = fChordNotes.begin(),
      iEnd   = fChordNotes.end(),
      i      = iBegin;
    for ( ; ; ) {
   //   os << (*i)->notePitchAsString (); JMI
      os << idtr << (*i);
      if (++i == iEnd) break;
 //     os << endl;
    } // for
    */
  }
  
  // print the articulations if any
  if (fChordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (i=fChordArticulations.begin(); i!=fChordArticulations.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the ornaments if any
  if (fChordOrnaments.size()) {
    list<S_msrOrnament>::const_iterator i;
    for (i=fChordOrnaments.begin(); i!=fChordOrnaments.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the dynamics if any
  if (fChordDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (i=fChordDynamics.begin(); i!=fChordDynamics.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the words if any
  if (fChordWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (i=fChordWords.begin(); i!=fChordWords.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the slurs if any
  if (fChordSlurs.size()) {
    list<S_msrSlur>::const_iterator i;
    for (i=fChordSlurs.begin(); i!=fChordSlurs.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  // print the wedges if any
  if (fChordWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (i=fChordWedges.begin(); i!=fChordWedges.end(); i++) {
      os << idtr << (*i);
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
S_msrComment msrComment::create (
  int           inputLineNumber,
  string        contents,
  msrGapKind    gapKind)
{
  msrComment* o =
    new msrComment (
      inputLineNumber, contents, gapKind);
  assert(o!=0);
  return o;
}

msrComment::msrComment (
  int           inputLineNumber,
  string        contents,
  msrGapKind    gapKind)
    : msrElement (inputLineNumber)
{
  fContents = contents;
  fGapKind  = gapKind;
}
msrComment::~msrComment() {}

void msrComment::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrComment::acceptIn()" << endl;
      
  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrComment::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrComment::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrComment::acceptOut()" << endl;

  if (visitor<S_msrComment>*
    p =
      dynamic_cast<visitor<S_msrComment>*> (v)) {
        S_msrComment elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrComment::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrComment::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrComment& elt)
{
  elt->print (os);
  return os;
}

void msrComment::print (ostream& os)
{
  os << "Comment" << endl;
  idtr++;
  os << idtr << "% " << fContents << endl;
  if (fGapKind == kGapAfterwards) os << idtr << endl;
  idtr--;
}

//______________________________________________________________________________
S_msrBreak msrBreak::create (
  int           inputLineNumber,
  int           nextBarNumber)
{
  msrBreak* o =
    new msrBreak (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBreak::msrBreak (
  int           inputLineNumber,
  int           nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating a break before measure " << fNextBarNumber <<
      endl;
}

msrBreak::~msrBreak() {}

string msrBreak::breakAsString () const
{
  stringstream s;

  s <<
    "Break" << ", next bar number = " << fNextBarNumber;

  return s.str();
}

void msrBreak::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBreak::acceptIn()" << endl;
      
  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBreak::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBreak::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBreak::acceptOut()" << endl;

  if (visitor<S_msrBreak>*
    p =
      dynamic_cast<visitor<S_msrBreak>*> (v)) {
        S_msrBreak elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBreak::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrBreak::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBreak& elt)
{
  elt->print (os);
  return os;
}

void msrBreak::print (ostream& os)
{
  os <<
    breakAsString () <<
    endl << endl;
}

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  int           inputLineNumber,
  int           nextBarNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarCheck::msrBarCheck (
  int           inputLineNumber,
  int           nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber; 

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating a bar check before measure " << fNextBarNumber <<
      endl;
}
msrBarCheck::~msrBarCheck() {}

string msrBarCheck::barCheckAsString () const
{
  stringstream s;

  s <<
    "BarCheck" << ", next bar number = " << fNextBarNumber;

  return s.str();
}

void msrBarCheck::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarCheck::acceptIn()" << endl;
      
  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarCheck::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarCheck::acceptOut()" << endl;

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarCheck::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrBarCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarCheck& elt)
{
  elt->print (os);
  return os;
}

void msrBarCheck::print (ostream& os)
{
  os <<
    barCheckAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrBarnumberCheck msrBarnumberCheck::create (
  int           inputLineNumber,
  int           nextBarNumber)
{
  msrBarnumberCheck* o =
    new msrBarnumberCheck (
      inputLineNumber, nextBarNumber);
  assert(o!=0);
  return o;
}

msrBarnumberCheck::msrBarnumberCheck (
  int           inputLineNumber,
  int           nextBarNumber)
    : msrElement (inputLineNumber)
{
  fNextBarNumber=nextBarNumber; 
}
msrBarnumberCheck::~msrBarnumberCheck() {}

string msrBarnumberCheck::barnumberCheckAsString () const
{
  stringstream s;

  s <<
    "BarnumberCheck" << ", next bar number = " << fNextBarNumber;

  return s.str();
}

void msrBarnumberCheck::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarnumberCheck::acceptIn()" << endl;
      
  if (visitor<S_msrBarnumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarnumberCheck>*> (v)) {
        S_msrBarnumberCheck elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarnumberCheck::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarnumberCheck::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarnumberCheck::acceptOut()" << endl;

  if (visitor<S_msrBarnumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarnumberCheck>*> (v)) {
        S_msrBarnumberCheck elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarnumberCheck::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrBarnumberCheck::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarnumberCheck& elt)
{
  elt->print (os);
  return os;
}

void msrBarnumberCheck::print (ostream& os)
{
  os <<
    barnumberCheckAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int           inputLineNumber,
  int           number,
  int           actualNotes,
  int           normalNotes,
  int           notePositionInMeasure)
{
  msrTuplet* o =
    new msrTuplet (
      inputLineNumber,
      number, actualNotes, normalNotes, notePositionInMeasure);
  assert(o!=0);
  return o;
}

msrTuplet::msrTuplet (
  int           inputLineNumber,
  int           number,
  int           actualNotes,
  int           normalNotes,
  int           notePositionInMeasure)
    : msrElement (inputLineNumber)
{  
  fTupletNumber = number;
  
  fTupletActualNotes = actualNotes;
  fTupletNormalNotes = normalNotes;
  
/* JMI
  fTupletDivisions =
    firstNote->getNoteDivisions ();
  fTupletDisplayDivisions =
    firstNote->getNoteDisplayDivisions ();
*/

  fTupletPositionInMeasure = notePositionInMeasure;
}

msrTuplet::~msrTuplet() {}

S_msrTuplet msrTuplet::createTupletBareClone ()
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a tuplet" <<
      endl;

  S_msrTuplet
    clone =
      msrTuplet::create (
        fInputLineNumber,
        fTupletNumber,
        fTupletActualNotes,
        fTupletNormalNotes,
        fTupletPositionInMeasure);
  
  return clone;
}

void msrTuplet::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTuplet::acceptIn()" << endl;
      
  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTuplet::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTuplet::acceptOut()" << endl;

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTuplet::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTuplet::browseData (basevisitor* v)
{
 for (
    vector<S_msrElement>::const_iterator i = fTupletElements.begin();
    i != fTupletElements.end();
    i++ ) {
    // browse tuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrTuplet& elt)
{
  elt->print (os);
  return os;
}

void msrTuplet::print (ostream& os)
{
  os <<
    "Tuplet " << fTupletActualNotes << "/" << fTupletNormalNotes <<
    " (" << fTupletDivisions <<
    "/" <<
    fTupletDivisionsPerWholeNote <<
    ") @"<<
    fTupletMeasureNumber <<
    ":" <<
    fTupletPositionInMeasure <<
    "/" <<
    fTupletDivisionsPerWholeNote <<
    endl;
    
  idtr++;

  vector<S_msrElement>::const_iterator i;
  for (i=fTupletElements.begin(); i!=fTupletElements.end(); i++) {
    os << idtr << (*i);
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_msrPageGeometry msrPageGeometry::create (
  int           inputLineNumber)
{
  msrPageGeometry* o =
    new msrPageGeometry (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageGeometry::msrPageGeometry (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperWidth   = -1.0;
  fPaperHeight  = -1.0;
  fTopMargin    = -1.0;
  fBottomMargin = -1.0;
  fLeftMargin   = -1.0;
  fRightMargin  = -1.0;
    
  fBetweenSystemSpace = -1.0;
  fPageTopSpace       = -1.0;

  fMillimeters = -1;
  fTenths      = -1;
}

msrPageGeometry::~msrPageGeometry() {}

float msrPageGeometry::globalStaffSize () const
{
  if (fMillimeters > 0)
    return fMillimeters * 72.27 / 25.4;
  else
    return 20.0; // LilyPond default
}

void msrPageGeometry::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPageGeometry::acceptIn()" << endl;
      
  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPageGeometry::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPageGeometry::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPageGeometry::acceptOut()" << endl;

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPageGeometry::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPageGeometry::browseData (basevisitor* v)
{
  /* JMI
  if (fPaperWidth) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fPaperHeight) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperHeight);
  }

  if (fTopMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fTopMargin);
  }

  if (fBottomMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fLeftMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fRightMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fRightMargin);
  }
  */
}

ostream& operator<< (ostream& os, const S_msrPageGeometry& elt) {
  elt->print (os);
  return os;
}

void msrPageGeometry::print (ostream& os) {
  os << "PageGeometry" << endl;

  bool emptyGeometry = true;
  
  idtr++;
  
  if (fPaperWidth > 0) {
    os <<
      idtr << setw(12) << left << "PaperWidth" << " = " <<
      setprecision(4) << fPaperWidth << " cm" << endl;
    emptyGeometry = false;
  }
  if (fPaperHeight > 0) {
    os <<
      idtr << setw(12) << left << "PaperHeight" << " = " <<
      setprecision(4) << fPaperHeight << " cm" << endl;
    emptyGeometry = false;
  }
  
  if (fTopMargin > 0) {
    os <<
      idtr << setw(12) << left << "TopMargin" << " = " <<
      setprecision(4) << fTopMargin << " cm" << endl;
    emptyGeometry = false;
  }
  if (fBottomMargin > 0) {
    os <<
      idtr << setw(12) << left << "BottomMargin" << " = " <<
      setprecision(4) << fBottomMargin << " cm" << endl;
    emptyGeometry = false;
  }
  if (fLeftMargin > 0) {
    os <<
      idtr << setw(12) << left << "LeftMargin" << " = " <<
      setprecision(4) << fLeftMargin << " cm" << endl;
    emptyGeometry = false;
  }
  if (fRightMargin > 0) {
    os <<
      idtr << setw(12) << left << "RightMargin" << " = " <<
      setprecision(4) << fRightMargin << " cm" << endl;
    emptyGeometry = false;
  }

  if (fMillimeters > 0) {
    os <<
      idtr << setw(12) << left << "Millimeters" << " = " <<
      setprecision(4) << fMillimeters << endl;
    emptyGeometry = false;
  }

  if (fTenths > 0) {
    os <<
      idtr << setw(12) << left << "Tenths" << " = " <<
      setprecision(4) << fTenths << endl;
    emptyGeometry = false;
  }

  if (emptyGeometry)
    os << idtr <<
      " " << "nothing specified" << endl;
  
  os << endl;

  idtr--;
}

/*
  if (fBetweenSystemSpace > 0) {
    os << idtr <<
    * "between-system-space = " <<
    * setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os << idtr <<
    * "page-top-space = " <<
    * setprecision(4) << fPageTopSpace << "\\cm" << endl;
  }
*/


//______________________________________________________________________________

S_msrVarValAssoc msrVarValAssoc::create (
  int           inputLineNumber,
  string        variableName,
  string        value)
{
  msrVarValAssoc* o =
    new msrVarValAssoc(
      inputLineNumber,
      variableName, value);
  assert(o!=0);
  return o;
}

msrVarValAssoc::msrVarValAssoc (
  int           inputLineNumber,
  string        variableName,
  string        value)
    : msrElement (inputLineNumber)
{
  fVariableName    = variableName;
  fVariableValue   = value;
}

msrVarValAssoc::~msrVarValAssoc() {}

void msrVarValAssoc::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVarValAssoc::acceptIn()" << endl;
      
  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVarValAssoc::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVarValAssoc::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVarValAssoc::acceptOut()" << endl;

  if (visitor<S_msrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_msrVarValAssoc>*> (v)) {
        S_msrVarValAssoc elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVarValAssoc::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrVarValAssoc::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrVarValAssoc& elt) {
  elt->print (os);
  return os;
}

void msrVarValAssoc::print (ostream& os)
{
  os << "MSR VarValAssoc" << endl;
  
  idtr++;

  // escape quotes if any
  string variableName;

  for_each (
    fVariableName.begin(),
    fVariableName.end(),
    stringQuoteEscaper (variableName));
    
  string variableValue;

  for_each (
    fVariableValue.begin(),
    fVariableValue.end(),
    stringQuoteEscaper (variableValue));

  // print resulting strings
  os <<
    idtr << "variable name : " <<
    "\"" << variableName << "\"" << endl <<
    idtr << "variable value: " <<
    "\"" << variableValue << "\"" << endl <<
    endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrLayout msrLayout::create (
  int           inputLineNumber)
{
  msrLayout* o =
    new msrLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrLayout::msrLayout (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{}
msrLayout::~msrLayout() {}

void msrLayout::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLayout::acceptIn()" << endl;
      
  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLayout::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLayout::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLayout::acceptOut()" << endl;

  if (visitor<S_msrLayout>*
    p =
      dynamic_cast<visitor<S_msrLayout>*> (v)) {
        S_msrLayout elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLayout::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrLayout::browseData (basevisitor* v)
{
  int n1 = fVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    // browse the lyrics
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fVarValAssocs [i]);
  } // for
}

ostream& operator<< (ostream& os, const S_msrLayout& elt)
{
  elt->print (os);
  return os;
}

void msrLayout::print (ostream& os)
{
  os << "Layout" << endl;

  idtr++;

  int n1 = fVarValAssocs.size();
  
  for (int i = 0; i < n1; i++ ) {
    os << idtr << fVarValAssocs [i];
  } // for

    /* JMI
  int n2 = fMsrSchemeVarValAssocs.size();
  for (int i = 0; i < n2; i++ ) {
    os << idtr << fMsrSchemeVarValAssocs[i];
  } // for
  */
  
  idtr--;
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  int          inputLineNumber,
  msrClefKind  clefKind)
{
  msrClef* o =
    new msrClef (
      inputLineNumber, clefKind);
  assert(o!=0);
  return o;
}

msrClef::msrClef (
  int          inputLineNumber,
  msrClefKind  clefKind)
    : msrElement (inputLineNumber)
{
  fClefKind = clefKind;
}

msrClef::~msrClef() {}

bool msrClef::clefIsATablatureClef () const
{
  switch (fClefKind) {
    case msrClef::kTablature4Clef:
    case msrClef::kTablature5Clef:
    case msrClef::kTablature6Clef:
    case msrClef::kTablature7Clef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::clefIsAPercussionClef () const
{
  switch (fClefKind) {
    case msrClef::kPercussionClef:
      return true;
      break;
    default:
      return false;
  } // switch
}

void msrClef::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrClef::acceptIn()" << endl;
      
  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrClef::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrClef::acceptOut()" << endl;

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrClef::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrClef& elt)
{
  elt->print (os);
  return os;
}

string msrClef::clefAsString () const
{
  stringstream s;

  s <<
    "Clef" " \"";
   
  switch (fClefKind) {
    case msrClef::k_NoClef:
      s << "none";
      break;
    case msrClef::kTrebleClef:
      s << "treble";
      break;
    case msrClef::kAltoClef:
      s << "alto";
      break;
    case msrClef::kTenorClef:
      s << "tenor";
      break;
    case msrClef::kBassClef:
      s << "bass";
      break;
    case msrClef::kTrebleLine1Clef:
      s << "treble line 1";
      break;
    case msrClef::kTrebleMinus15Clef:
      s << "treble -15";
      break;
    case msrClef::kTrebleMinus8Clef:
      s << "treble -8";
      break;
    case msrClef::kTreblePlus8Clef:
      s << "treble +8";
      break;
    case msrClef::kTreblePlus15Clef:
      s << "treble +15";
      break;
    case msrClef::kBassMinus15Clef:
      s << "bass -15";
      break;
    case msrClef::kBassMinus8Clef:
      s << "bass -8";
      break;
    case msrClef::kBassPlus8Clef:
      s << "bass +8";
      break;
    case msrClef::kBassPlus15Clef:
      s << "bass +15";
      break;
    case msrClef::kTablature4Clef:
      s << "tablature 4 lines";
      break;
    case msrClef::kTablature5Clef:
      s << "tablature 5 lines";
      break;
    case msrClef::kTablature6Clef:
      s << "tablature 6 lines";
      break;
    case msrClef::kTablature7Clef:
      s << "tablature 7 lines";
      break;
    case msrClef::kPercussionClef:
      s << "percussion";
      break;
  } // switch

  s <<
    "\", line " << fInputLineNumber;

  return s.str();
}

void msrClef::print (ostream& os)
{
  os << clefAsString () << endl;
}

//______________________________________________________________________________
S_msrKey msrKey::create (
  int           inputLineNumber,
  int           fifths,
  string        mode,
  int           cancel)
{
  msrKey* o =
    new msrKey (
      inputLineNumber, fifths, mode, cancel);
  assert (o!=0);
  return o;
}

msrKey::msrKey (
  int           inputLineNumber,
  int           fifths,
  string        mode,
  int           cancel)
    : msrElement (inputLineNumber)
{
  fKeyFifths = fifths;
  fKeyMode   = mode;
  fKeyCancel = cancel;

  string                 tonicNote;
  msrKey::msrKeyModeKind keyModeKind;
  
  switch (fKeyFifths) {
    case 0:
      tonicNote = "c";
      keyModeKind = msrKey::kMajor;
      break;
    case 1:
      tonicNote = "g";
      keyModeKind = msrKey::kMajor;
      break;
    case 2:
      tonicNote = "d";
      keyModeKind = msrKey::kMajor;
      break;
    case 3:
      tonicNote = "a";
      keyModeKind = msrKey::kMajor;
      break;
    case 4:
      tonicNote = "e";
      keyModeKind = msrKey::kMajor;
      break;
    case 5:
      tonicNote = "b";
      keyModeKind = msrKey::kMajor;
      break;
    case 6:
       tonicNote = "fis";
      keyModeKind = msrKey::kMajor;
      break;
    case 7:
      tonicNote = "cis";
      keyModeKind = msrKey::kMajor;
      break;
    case -1:
      tonicNote = "f";
      keyModeKind = msrKey::kMajor;
      break;
    case -2:
      tonicNote = "bes";
      keyModeKind = msrKey::kMajor;
      break;
    case -3:
      tonicNote = "ees";
      keyModeKind = msrKey::kMajor;
      break;
    case -4:
      tonicNote = "aes";
      keyModeKind = msrKey::kMajor;
      break;
    case -5:
      tonicNote = "des";
      keyModeKind = msrKey::kMajor;
      break;
    case -6:
      tonicNote = "ges";
      keyModeKind = msrKey::kMajor;
      break;
    case -7:
      tonicNote = "ces";
      keyModeKind = msrKey::kMajor;
      break;
    default: // unknown key sign !!
      {
      stringstream s;
      
      s << 
        "ERROR: unknown key sign \"" << fKeyFifths << "\"";
        
      msrMusicXMLError (
        fInputLineNumber,
        s.str());
      }
  } // switch
  
  fKeyTonic    = tonicNote;
  fKeyModeKind = keyModeKind; 
}

msrKey::~msrKey() {}

void msrKey::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrKey::acceptIn()" << endl;
      
  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrKey::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrKey::acceptOut()" << endl;

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrKey::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrKey::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrKey& elt)
{
  elt->print (os);
  return os;
}

string msrKey::keyAsString () const
{
  stringstream s;

  s << "Key \"" << fKeyTonic << " ";
  if (fKeyModeKind == kMajor)
    s << "major";
  else
    s << "minor";

  s <<
    "\", line " << fInputLineNumber;

  return s.str();
}

void msrKey::print (ostream& os)
{
  os << keyAsString () << endl;
}

//______________________________________________________________________________
S_msrTime msrTime::create (
  int           inputLineNumber,
  int           beatsNumber,
  int           beatsValue)
{
  msrTime* o =
    new msrTime (
      inputLineNumber,
      beatsNumber, beatsValue);
  assert (o!=0);
  return o;
}

msrTime::msrTime (
  int           inputLineNumber,
  int           beatsNumber,
  int           beatsValue)
    : msrElement (inputLineNumber)
{
  fBeatsNumber = beatsNumber;
  fBeatsValue  = beatsValue;
}

msrTime::~msrTime() {}

void msrTime::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTime::acceptIn()" << endl;
      
  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTime::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTime::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTime::acceptOut()" << endl;

  if (visitor<S_msrTime>*
    p =
      dynamic_cast<visitor<S_msrTime>*> (v)) {
        S_msrTime elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTime::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrTime::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTime& elt)
{
  elt->print (os);
  return os;
}

string msrTime::timeAsString () const
{
  stringstream s;

  s <<
    "Time \"" << 
    fBeatsNumber << "/" << fBeatsValue <<
    "\", line " << fInputLineNumber;

  return s.str();
}

void msrTime::print (ostream& os)
{
  os << timeAsString () << endl;
}

//______________________________________________________________________________
S_msrTranspose msrTranspose::create (
  int           inputLineNumber,
  int           transposeDiatonic,
  int           transposeChromatic)
{
  msrTranspose* o =
    new msrTranspose (
      inputLineNumber,
      transposeDiatonic, transposeChromatic);
  assert (o!=0);
  return o;
}

msrTranspose::msrTranspose (
  int           inputLineNumber,
  int           transposeDiatonic,
  int           transposeChromatic)
    : msrElement (inputLineNumber)
{
  fTransposeDiatonic  = transposeDiatonic;
  fTransposeChromatic = transposeChromatic;
}

msrTranspose::~msrTranspose() {}

void msrTranspose::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTranspose::acceptIn()" << endl;
      
  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTranspose::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTranspose::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTranspose::acceptOut()" << endl;

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTranspose::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrTranspose::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTranspose& elt)
{
  elt->print (os);
  return os;
}

string msrTranspose::transposeAsString () const
{
  stringstream s;

  s <<
    "Transpose " << 
    "diatonic = " << fTransposeDiatonic <<
    ", chromatic= " << fTransposeChromatic;

  return s.str();
}

void msrTranspose::print (ostream& os)
{
  os << transposeAsString () << endl;
}

//______________________________________________________________________________
S_msrWords msrWords::create (
  int                   inputLineNumber,
  msrWordsPlacementKind wordsPlacementKind,
  string                wordsContents,
  string                wordsFontStyle,
  string                wordsFontSize,
  string                wordsFontWeight,
  string                wordsFontXMLLang)
{
  msrWords* o =
    new msrWords (
      inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsFontStyle,
      wordsFontSize,
      wordsFontWeight,
      wordsFontXMLLang);
  assert(o!=0);
  return o;
}

msrWords::msrWords (
  int                   inputLineNumber,
  msrWordsPlacementKind wordsPlacementKind,
  string                wordsContents,
  string                wordsFontStyle,
  string                wordsFontSize,
  string                wordsFontWeight,
  string                wordsFontXMLLang)
    : msrElement (inputLineNumber)
{
  fWordsPlacementKind = wordsPlacementKind;
  
  fWordsContents      = wordsContents;

  fWordsFontStyle     = wordsFontStyle;
  fWordsFontSize      = wordsFontSize;
  fWordsFontWeight    = wordsFontWeight;
  fWordsFontXMLLang   = wordsFontXMLLang;
}
msrWords::~msrWords() {}

void msrWords::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWords::acceptIn()" << endl;
      
  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWords::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrWords::acceptOut()" << endl;

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrWords::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrWords::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWords& elt)
{
  elt->print (os);
  return os;
}

string msrWords::wordsAsString () const
{
  stringstream s;

  s <<
    "Words" << " " <<
    fWordsContents << ", placement = ";

  switch (fWordsPlacementKind) {
    case kAbove:
      s << "above";
      break;
    case kBelow:
      s << "below";
      break;
  } // switch

  return s.str();
}

void msrWords::print (ostream& os)
{
// JMI  os << wordsAsString () << endl;

  os <<
    "Words" " \"" << fWordsContents << "\"" <<
    endl;

  idtr++;

  os <<
    idtr <<
      setw(16) << "Placement" << " = ";

  switch (fWordsPlacementKind) {
    case kAbove:
      os << "above";
      break;
    case kBelow:
      os << "below";
      break;
  } // switch
  os << endl;

  os << left <<
    idtr <<
      setw(16) << "WordsFontStyle" << " = " << fWordsFontStyle <<
      endl <<
    idtr <<
      setw(16) << "WordsFontSize" << " = " << fWordsFontSize <<
      endl <<
    idtr <<
      setw(16) << "WordsFontWeight" << " = " << fWordsFontWeight <<
      endl <<
    idtr <<
      setw(16) << "WordsFontXMLLang" << " = " << fWordsFontXMLLang <<
      endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrTempo msrTempo::create (
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber, tempoUnit, perMinute);
  assert(o!=0);
  return o;
}

msrTempo::msrTempo (
  int           inputLineNumber,
  int           tempoUnit,
  int           perMinute)
    : msrElement (inputLineNumber)
{
  fTempoIndication = "";
  
  fTempoUnit = tempoUnit;
  fPerMinute = perMinute;
}
msrTempo::~msrTempo() {}

void msrTempo::setTempoIndication (string indication)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "Setting indication of tempo " <<
      fTempoUnit << " = " << fPerMinute <<
      " to \"" << indication << "\"" <<
      endl;
      
  fTempoIndication = indication;
}

void msrTempo::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTempo::acceptIn()" << endl;
      
  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTempo::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrTempo::acceptOut()" << endl;

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrTempo::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrTempo::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTempo& elt)
{
  elt->print (os);
  return os;
}

string msrTempo::tempoAsString () const
{
  stringstream s;

  s <<
    "Tempo" << " " <<
    ", indication = \"" << fTempoIndication << "\"" <<
    ", " << fTempoUnit << " = " << fPerMinute;

  return s.str();
}

void msrTempo::print (ostream& os)
{
  os << tempoAsString () << endl;
}

//______________________________________________________________________________
S_msrLyricschunk msrLyricschunk::create (
  int                inputLineNumber,
  msrLyricschunkKind lyricschunkKind,
  string             chunkText,
  int                divisions,
  S_msrLyrics        lyricschunkLyricsUplink)
{
  msrLyricschunk* o =
    new msrLyricschunk (
      inputLineNumber,
      lyricschunkKind, chunkText, divisions,
      lyricschunkLyricsUplink);
  assert(o!=0);
  return o;
}

msrLyricschunk::msrLyricschunk (
  int                inputLineNumber,
  msrLyricschunkKind lyricschunkKind,
  string             chunkText,
  int                divisions,
  S_msrLyrics        lyricschunkLyricsUplink)
    : msrElement (inputLineNumber)
{
  fLyricschunkKind = lyricschunkKind;
  fChunkText       = chunkText;
  fChunkDivisions  = divisions;

  // fLyricschunkNote will be set by setLyricschunkNote later
  
  fLyricschunkLyricsUplink = lyricschunkLyricsUplink;
}

msrLyricschunk::~msrLyricschunk()
{}

S_msrLyricschunk msrLyricschunk::createLyricschunkBareClone ()
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a lyrics chunk" <<
      endl;

  S_msrLyricschunk
    clone =
      msrLyricschunk::create (
        fInputLineNumber,
        fLyricschunkKind,
        fChunkText,
        fChunkDivisions,
        fLyricschunkLyricsUplink);
  
  return clone;
}

/*
S_msrElement msrLyricschunk::removeLastElementFromVoicechunk (
  int inputLineNumber)
{
  if (fVoicechunkMeasuresList.size ()) {
    return
      fVoicechunkMeasuresList.back ()->
        removeLastElementFromMeasure (inputLineNumber);
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromVoicechunk () since it is empty");
  }
}
*/

void msrLyricschunk::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyricschunk::acceptIn()" << endl;
      
  if (visitor<S_msrLyricschunk>*
    p =
      dynamic_cast<visitor<S_msrLyricschunk>*> (v)) {
        S_msrLyricschunk elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyricschunk::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLyricschunk::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyricschunk::acceptOut()" << endl;

  if (visitor<S_msrLyricschunk>*
    p =
      dynamic_cast<visitor<S_msrLyricschunk>*> (v)) {
        S_msrLyricschunk elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyricschunk::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrLyricschunk::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrLyricschunk& elt)
{
  elt->print (os);
  return os;
}

string msrLyricschunk::lyricschunkKindAsString ()
{
  string result;
    
  switch (fLyricschunkKind) {
    case msrLyricschunk::kSingleChunk:
      result = "single";
      break;
    case msrLyricschunk::kBeginChunk:
      result = "begin";
      break;
    case msrLyricschunk::kMiddleChunk:
      result = "middle";
      break;
    case msrLyricschunk::kEndChunk:
      result = "end";
      break;
      
    case msrLyricschunk::kSkipChunk:
      result = "skip";
      break;
      
    case msrLyricschunk::kSlurChunk:
      result = "slur";
      break;
    case msrLyricschunk::kSlurBeyondEndChunk:
      result = "slur beyond end";
      break;
      
    case msrLyricschunk::kTiedChunk:
      result = "tied";
      break;
      
    case msrLyricschunk::kBarcheckChunk:
      result = "barcheck";
      break;
      
    case msrLyricschunk::kBreakChunk:
      result = "break";
      break;
      
    case msrLyricschunk::k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

  return result;
}

string msrLyricschunk::lyricschunkAsString ()
{
  stringstream s;
  
  switch (fLyricschunkKind) {
    case kSingleChunk:
      s << left <<
        setw(15) << "single" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        ", " << "\"" << fChunkText << "\"" <<
        endl;
      break;
      
    case kBeginChunk:
      s << left <<
        setw(15) << "begin" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        ", " << "\"" << fChunkText << "\"" <<
        endl;
      break;
      
    case kMiddleChunk:
      s << left <<
        setw(15) << "middle" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        ", " << "\"" << fChunkText << "\"" <<
        endl;
      break;
      
    case kEndChunk:
      s << left <<
        setw(15) << "end" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        ", " << "\"" << fChunkText << "\"" <<
        endl;
      break;
      
    case kSkipChunk:
      s << left <<
        setw(15) << "skip" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " <<
  // JMI      fLyricschunkNote->notePitchAsString () <<
        ":" <<
  // JMI      fLyricschunkNote->noteDivisionsAsMSRString () <<
        endl;
      break;
      
    case kSlurChunk:
      s << left <<
        setw(15) << "slur" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
 // JMI       ", " << "\"" << fChunkText << "\"" <<
        endl;
      break;
      
    case kSlurBeyondEndChunk:
      s << left <<
        setw(15) << "slur beyond end" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
 // JMI       ", " << "\"" << fChunkText << "\"" <<
        endl;
      break;
      
    case kTiedChunk:
      s << left <<
        setw(15) << "tied" << ":" << fChunkDivisions <<
        ", line " << right << setw(5) << fInputLineNumber <<
        ", " << fLyricschunkNote->notePitchAsString () <<
        ":" << fLyricschunkNote->noteDivisionsAsMSRString () <<
        " " << "\"" << fChunkText << "\"" <<
        endl;
      break;
      
    case kBarcheckChunk:
      // fChunkText contains the measure number
      s << left <<
        setw(15) << "barCheck" <<
        " measure " << fChunkText <<
        endl;
      break;
      
    case kBreakChunk:
      // fChunkText contains the measure number
      s << left <<
        setw(15) << "break" <<
        " measure " << fChunkText <<
        endl << endl;
      break;
      
    case k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

  return s.str();
}


void msrLyricschunk::print (ostream& os)
{  
  os <<
    "Lyricschunk" << " " << setw(15) << lyricschunkAsString ();
}

//______________________________________________________________________________
S_msrLyrics msrLyrics::create (
  int                   inputLineNumber,
  int                   lyricsNumber,
  msrLyricsMasterStatus lyricsMasterStatus,
  S_msrVoice            lyricsVoiceUplink)
{
  msrLyrics* o =
    new msrLyrics (
      inputLineNumber,
      lyricsNumber,
      lyricsMasterStatus,
      lyricsVoiceUplink);
  assert(o!=0);
  return o;
}

msrLyrics::msrLyrics (
  int                   inputLineNumber,
  int                   lyricsNumber,
  msrLyricsMasterStatus lyricsMasterStatus,
  S_msrVoice            lyricsVoiceUplink)
    : msrElement (inputLineNumber)
{
  fLyricsNumber       = lyricsNumber;
  fLyricsMasterStatus = lyricsMasterStatus;
 
  fLyricsVoiceUplink  = lyricsVoiceUplink;
  
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating lyrics " << getLyricsName () << endl;

  fLyricsTextPresent = false;
}

string msrLyrics::getLyricsName () const
{
  // not stored in a field,
  // because the lyrics voice and staff may change name
  // when the part they belong to is re-used

  string
    lyricsNameSuffix =
 //     fLyricsMasterStatus == kMasterLyrics
      fLyricsNumber == 0
        ? "MASTER"
        : int2EnglishWord (fLyricsNumber);
        
  return
   fLyricsVoiceUplink->getVoiceName() +
    "_L_" +
    lyricsNameSuffix;
}

msrLyrics::~msrLyrics() {}

S_msrLyrics msrLyrics::createLyricsBareClone (S_msrVoice clonedVoice)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a lyrics" <<
      endl;

  S_msrLyrics
    clone =
      msrLyrics::create (
        fInputLineNumber,
        fLyricsNumber,
        fLyricsMasterStatus,
        clonedVoice);
  
  return clone;
}

/* JMI

void msrLyrics::addTextChunkToLyrics (
  int       inputLineNumber,
  string    syllabic,
  msrLyricschunk::msrLyricschunkKind
            lyricschunkKind,
  string    text,
  bool      elision,
  int       divisions,
  S_msrNote note)
{
  // create a lyrics text chunk
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding text lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions << 
      ", syllabic = \"" << syllabic << "\"" <<
      ", text = \"" << text << "\"";

/ *
    string lyricschunkKindAsString =
      lyricschunkAsString ();
  * /
    cerr <<
 //     ", type = \"" << lyricschunkKindAsString << "\"" <<
      ", elision: " << elision << 
      " to lyrics " << getLyricsName () << endl;
  }

  // create text lyrics chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        inputLineNumber,
        lyricschunkKind, text, divisions,
        note,
        this);

  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);

  fLyricsTextPresent = true;
}
*/

void msrLyrics::addSkipChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Skip' lyrics chunk:" << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics skip chunk
  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kSkipChunk, "", divisions,
        this);
  
  // add chunk to this lyrics
  fLyricschunks.push_back (lyricschunk);

/*
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff staff = fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart  part  = staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Skip' lyrics chunk:" << divisions <<
      " to " << getLyricsName () << endl;

    string result;
    int    computedNumberOfDots; // value not used
    string errorMessage;
  
    int divisionsPerWholeNote =
      fLyricsVoiceUplink->getDivisionsPerWholeNote ();
  
    string
      divisionsAsString =
        divisionsAsMSRDuration (
          divisions,
          divisionsPerWholeNote,
          computedNumberOfDots,
          errorMessage,
          false); // 'true' to debug it;
    
    if (errorMessage.size ())
      msrMusicXMLError (
        fInputLineNumber,
        errorMessage);

    cerr << idtr <<
      "--> Adding 'Skip' lyrics chunk:" << divisions <<
      "/" << divisionsPerWholeNote << "" <<
      "(" << divisionsAsString << ")" <<
      " to lyrics " << getLyricsName () << endl;
  }
  */
}

/*
void msrLyrics::addTiedChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Tied' lyrics chunk: " << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics tied chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kTiedChunk, "", divisions,
        note,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addSlurChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding 'Slur' lyrics chunk:" << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics slur chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kSlurChunk, "", divisions,
        note,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addSlurBeyondEndChunkToLyrics (
  int       inputLineNumber,
  int       divisions,
  S_msrNote note)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding a 'SlurBeyondEnd' lyrics chunk: " << divisions <<
      " to lyrics " << getLyricsName () << endl;
  }
  
  // create lyrics slur chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        note,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addBarcheckChunkToLyrics (
  int inputLineNumber,
  int nextMeasureNumber)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding a 'barcheck' lyrics chunk" <<
      " to lyrics " << getLyricsName () << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  s << nextMeasureNumber;
  
  // create lyrics break chunk
  / *
  S_msrDuration
    nullMsrDuration =
      msrDuration::create (
        inputLineNumber,
        0, 1, 0, "");
    * /    
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kBarcheckChunk,
        s.str(),
        0,
        0,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}
*/

void msrLyrics::addBreakChunkToLyrics (
  int inputLineNumber,
  int nextMeasureNumber)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    S_msrStaff
      staff =
        fLyricsVoiceUplink->getVoiceStaffUplink ();
    S_msrPart
      part =
        staff-> getStaffPartUplink ();
    
    cerr << idtr <<
      "--> Adding a 'Break' lyrics chunk" <<
      " to lyrics " << getLyricsName () << endl;
  }

  // convert nextMeasureNumber to string
  stringstream s;
  s << nextMeasureNumber;
  
  // create lyrics break chunk
  S_msrLyricschunk
    chunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kBreakChunk,
        s.str(),
        0,
        this);
        
  // add chunk to this lyrics
  fLyricschunks.push_back (chunk);
}

void msrLyrics::addChunkToLyrics (S_msrLyricschunk chunk)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "--> Adding chunk to lyrics" << getLyricsName () << endl;
      
  fLyricschunks.push_back (chunk);

  // does this lyrics contain text?
  switch (chunk->getLyricschunkKind ()) {
    case msrLyricschunk::kSingleChunk:
    case msrLyricschunk::kBeginChunk:
    case msrLyricschunk::kMiddleChunk:
    case msrLyricschunk::kEndChunk:
      // only now, in case addChunkToLyrics() is called
      // from LPSR for example
      this->setLyricsTextPresent ();
      break;
      
    case msrLyricschunk::kSkipChunk:
    case msrLyricschunk::kSlurChunk:
    case msrLyricschunk::kSlurBeyondEndChunk:
    case msrLyricschunk::kTiedChunk:
    case msrLyricschunk::kBarcheckChunk:
    case msrLyricschunk::kBreakChunk:
      break;
      
    case msrLyricschunk::k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

}

void msrLyrics::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyrics::acceptIn()" << endl;
      
  if (visitor<S_msrLyrics>*
    p =
      dynamic_cast<visitor<S_msrLyrics>*> (v)) {
        S_msrLyrics elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyrics::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrLyrics::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyrics::acceptOut()" << endl;

  if (visitor<S_msrLyrics>*
    p =
      dynamic_cast<visitor<S_msrLyrics>*> (v)) {
        S_msrLyrics elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrLyrics::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrLyrics::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrLyrics::browseData()" << endl;

  idtr++;
  
  // browse the lyrics chunks
  int n = fLyricschunks.size ();
  for (int i = 0; i < n; i++) {
    // browse the lyrics
    msrBrowser<msrLyricschunk> browser (v);
    browser.browse (*fLyricschunks [i]);
  } // for
  cerr << endl;

  idtr--;

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "<== msrLyrics::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrLyrics& elt)
{
  elt->print (os);
  return os;
}

void msrLyrics::print (ostream& os)
{  
  os <<
    "Lyrics" << " " << getLyricsName () <<
    " (" << fLyricschunks.size () << " lyrics chunks)";
    
  if (! fLyricsTextPresent)
    os << " (No actual text)";

  os << endl;

//  if (fLyricsTextPresent) {  JMI
    idtr++;

    int n = fLyricschunks.size();
    for (int i = 0; i < n; i++) {
      os << idtr << fLyricschunks [i];
    } // for
    os << endl;

    idtr--;
 // }
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  int                   inputLineNumber,
  char                  harmonyRootStep,
  float                 harmonyRootAlter,
  msrHarmonyKind        harmonyKind,
  string                harmonyKindText,
  char                  harmonyBassStep,
  float                 harmonyBassAlter,
  S_msrPart             harmonyPartUplink)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      harmonyRootStep, harmonyRootAlter,
      harmonyKind, harmonyKindText,
      harmonyBassStep, harmonyBassAlter,
      harmonyPartUplink);
  assert(o!=0);
  return o;
}

msrHarmony::msrHarmony (
  int                   inputLineNumber,
  char                  harmonyRootStep,
  float                 harmonyRootAlter,
  msrHarmonyKind        harmonyKind,
  string                harmonyKindText,
  char                  harmonyBassStep,
  float                 harmonyBassAlter,
  S_msrPart             harmonyPartUplink)
    : msrElement (inputLineNumber)
{
  fHarmonyRootStep   = harmonyRootStep;
  fHarmonyRootAlter  = harmonyRootAlter;
 
  fHarmonyKind       = harmonyKind;
  fHarmonyKindText   = harmonyKindText;
 
  fHarmonyBassStep   = harmonyBassStep;
  fHarmonyBassAlter  = harmonyBassAlter;
 
  fHarmonyPartUplink = harmonyPartUplink;
}

msrHarmony::~msrHarmony() {}

S_msrHarmony msrHarmony::createHarmonyBareClone (S_msrPart clonedPart)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone or a harmony" <<
      endl;

  S_msrHarmony
    clone =
      msrHarmony::create (
        fInputLineNumber,
        fHarmonyRootStep, fHarmonyRootAlter,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyBassStep, fHarmonyBassAlter,
        clonedPart);
  
  return clone;
}

string msrHarmony::harmonyKindAsString () const
{
  string result;
  
  switch (fHarmonyKind) {
    case msrHarmony::kMajor:
      result = "Major";
      break;
    case msrHarmony::kMinor:
      result = "Minor";
      break;
    case msrHarmony::kDominant:
      result = "Dominant";
      break;
    case msrHarmony::kSuspendedFourth:
      result = "SuspendedFourth";
      break;
    case msrHarmony::kMajorSeventh:
      result = "MajorSeventh";
      break;
    case msrHarmony::kMinorSeventh:
      result = "MinorSeventh";
      break;
    case msrHarmony::kMajorNinth:
      result = "MajorNinth";
      break;
    case msrHarmony::kMinorNinth:
      result = "MinorNinth";
      break;
  } // switch

  return result;
}

string msrHarmony::harmonyAsString () const
{
  stringstream s;

  s <<
    fHarmonyRootStep << fHarmonyRootAlter <<
    harmonyKindAsString () << fHarmonyKindText <<
    fHarmonyBassStep << fHarmonyBassAlter;

  return s.str();
}

void msrHarmony::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrHarmony::acceptIn()" << endl;
      
  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrHarmony::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrHarmony::acceptOut()" << endl;

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrHarmony::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrHarmony& elt)
{
  elt->print (os);
  return os;
}

void msrHarmony::print (ostream& os)
{  
  os <<
    "Harmony" <<
    endl;
    
  idtr++;

  os << left <<
    idtr <<
      setw(15) << "HarmonyRoot" << " = " << fHarmonyRootStep << fHarmonyRootAlter <<
      endl <<
    idtr <<
      setw(15) << "HarmonyKind" << " = " << harmonyKindAsString () <<
      endl <<
    idtr <<
      setw(15) << "HarmonyKindText" << " = " << fHarmonyKindText <<
      endl <<
    idtr <<
      setw(15) << "HarmonyBass" << " = " << fHarmonyBassStep << fHarmonyBassAlter <<
      endl;

  idtr--;
}

//______________________________________________________________________________
S_msrSegno msrSegno::create (
  int                       inputLineNumber)
{
  msrSegno* o =
    new msrSegno (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrSegno::msrSegno (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrSegno::~msrSegno() {}

void msrSegno::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrSegno::acceptIn()" << endl;
      
  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrSegno::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrSegno::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrSegno::acceptOut()" << endl;

  if (visitor<S_msrSegno>*
    p =
      dynamic_cast<visitor<S_msrSegno>*> (v)) {
        S_msrSegno elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrSegno::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrSegno::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrSegno& elt)
{
  elt->print (os);
  return os;
}

void msrSegno::print (ostream& os)
{
  os <<
    "Segno" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrCoda msrCoda::create (
  int                       inputLineNumber)
{
  msrCoda* o =
    new msrCoda (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrCoda::msrCoda (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrCoda::~msrCoda() {}

void msrCoda::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrCoda::acceptIn()" << endl;
      
  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrCoda::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrCoda::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrCoda::acceptOut()" << endl;

  if (visitor<S_msrCoda>*
    p =
      dynamic_cast<visitor<S_msrCoda>*> (v)) {
        S_msrCoda elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrCoda::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrCoda::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrCoda& elt)
{
  elt->print (os);
  return os;
}

void msrCoda::print (ostream& os)
{
  os <<
    "Coda" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrEyeglasses msrEyeglasses::create (
  int                       inputLineNumber)
{
  msrEyeglasses* o =
    new msrEyeglasses (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrEyeglasses::msrEyeglasses (
  int                       inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrEyeglasses::~msrEyeglasses() {}

void msrEyeglasses::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrEyeglasses::acceptIn()" << endl;
      
  if (visitor<S_msrEyeglasses>*
    p =
      dynamic_cast<visitor<S_msrEyeglasses>*> (v)) {
        S_msrEyeglasses elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrEyeglasses::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrEyeglasses::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrEyeglasses::acceptOut()" << endl;

  if (visitor<S_msrEyeglasses>*
    p =
      dynamic_cast<visitor<S_msrEyeglasses>*> (v)) {
        S_msrEyeglasses elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrEyeglasses::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrEyeglasses::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrEyeglasses& elt)
{
  elt->print (os);
  return os;
}

void msrEyeglasses::print (ostream& os)
{
  os <<
    "Eyeglasses" <<
    ", line " << fInputLineNumber <<
    endl;
}

//______________________________________________________________________________
S_msrPedal msrPedal::create (
  int              inputLineNumber,
  msrPedalType     pedalType,
  msrPedalLine     pedalLine)
{
  msrPedal* o =
    new msrPedal (
      inputLineNumber,
      pedalType, pedalLine);
  assert(o!=0);
  return o;
}

msrPedal::msrPedal (
  int              inputLineNumber,
  msrPedalType     pedalType,
  msrPedalLine     pedalLine)
    : msrElement (inputLineNumber)
{
  fPedalType = pedalType;
  fPedalLine = pedalLine;
}

msrPedal::~msrPedal() {}

string msrPedal::pedalTypeAsString ()
{
  string result;
  
  switch (fPedalType) {
    case kPedalStart:
      result = "start pedal";
      break;
    case kPedalContinue:
      result = "continue pedal";
      break;
    case kPedalChange:
      result = "change pedal";
      break;
    case kPedalStop:
      result = "stop pedal";
      break;
  } // switch

  return result;
}
      
string msrPedal::pedalLineAsString ()
{
  string result;
  
  switch (fPedalLine) {
    case kPedalLineYes:
      result = "pedal line: yes";
      break;
    case kPedalLineNo:
      result = "pedal line: no";
      break;
  } // switch

  return result;
}

void msrPedal::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPedal::acceptIn()" << endl;
      
  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPedal::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPedal::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPedal::acceptOut()" << endl;

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPedal::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPedal::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrPedal& elt)
{
  elt->print (os);
  return os;
}

void msrPedal::print (ostream& os)
{
  os <<
    "Pedal" <<
    ", line " << fInputLineNumber <<
    pedalTypeAsString () << ", " <<
    pedalLineAsString () <<
    endl;
}

//______________________________________________________________________________
S_msrBarline msrBarline::create (
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
{
  msrBarline* o =
    new msrBarline (
      inputLineNumber,
      barlineHasSegno, barlineHasCoda,
      location, style,
      endingType, endingNumber,
      repeatDirection, repeatWinged);
  assert(o!=0);
  return o;
}

msrBarline::msrBarline (
  int                       inputLineNumber,
  bool                      barlineHasSegno,
  bool                      barlineHasCoda,
  msrBarlineLocation        location,
  msrBarlineStyle           style,
  msrBarlineEndingType      endingType,
  string                    endingNumber,
  msrBarlineRepeatDirection repeatDirection,
  msrBarlineRepeatWinged    repeatWinged)
    : msrElement (inputLineNumber)
{
  fBarlineHasSegno = barlineHasSegno;
  fBarlineHasCoda  = barlineHasCoda;
  
  fLocation        = location;
  fStyle           = style;
  fEndingType      = endingType;
  fEndingNumber    = endingNumber;
  fRepeatDirection = repeatDirection;
  fRepeatWinged    = repeatWinged;
  
  // JMI cerr << "fEndingNumber = " << fEndingNumber << endl;
  
  // extract individual numbers from fEndingNumber
  // that may contain "1, 2"
  fEndingNumbersList =
    extractNumbersFromString (
      fEndingNumber, gGeneralOptions->fDebugDebug);
}

msrBarline::~msrBarline() {}

void msrBarline::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarline::acceptIn()" << endl;
      
  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarline::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrBarline::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrBarline::acceptOut()" << endl;

  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrBarline::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrBarline::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBarline& elt)
{
  elt->print (os);
  return os;
}

void msrBarline::print (ostream& os)
{
  os <<
    "Barline" <<
    ", line " << fInputLineNumber << ", ";

  if (fBarlineHasSegno)
    os << "has segno, ";
  if (fBarlineHasCoda)
      os << "has coda, ";

  switch (fBarlineCategory) {
    case kStandaloneBar:
      os << "standalone";
      break;
    case kRepeatStart:
      os << "repeat start";
      break;
    case kRepeatEnd:
      os << "repeat end";
      break;
    case kEndingStart:
      os << "ending start";
      break;
    case kHookedEndingEnd:
      os << "hooked ending end";
      break;
    case kHooklessEndingEnd:
      os << "hookless ending end";
      break;
  } // switch
  os << endl;

  idtr++;

  os <<
    idtr << left << setw(15) << "Location" << " : ";
  switch (fLocation) {
    case k_NoLocation:
      os << "none";
      break;
    case kLeft:
      os << "Left";
      break;
    case kMiddle:
      os << "Middle";
      break;
    case kRight:
      os << "Right";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "Style" << " : ";
  switch (fStyle) {
    case k_NoStyle:
      os << "none";
      break;
    case kRegular:
      os << "Regular";
      break;
    case kDotted:
      os << "Dotted";
      break;
    case kDashed:
      os << "Dashed";
      break;
    case kHeavy:
      os << "Heavy";
      break;
    case kLightLight:
      os << "LightLight";
      break;
    case kLightHeavy:
      os << "LightHeavy";
      break;
    case kHeavyLight:
      os << "HeavyLight";
      break;
    case kHeavyHeavy:
      os << "HeavyHeavy";
      break;
    case kTick:
      os << "Tick";
      break;
    case kShort:
      os << "Short";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "EndingType" << " : ";
  switch (fEndingType) {
    case k_NoEndingType:
      os << "none";
      break;
    case kStart:
      os << "Start";
      break;
    case kStop:
      os << "Stop";
      break;
    case kDiscontinue:
      os << "Discontinue";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "Ending number" << " : ";
  list<int>::const_iterator i;
  for (i=fEndingNumbersList.begin(); i!=fEndingNumbersList.end(); i++) {
    os << (*i) << " ";
  } // for
  os << endl;
 
  os <<
    idtr << setw(15) << "RepeatDirection" << " : ";
  switch (fRepeatDirection) {
    case k_NoRepeatDirection:
      os << "none";
      break;
    case kForward:
      os << "Forward";
      break;
    case kBackward:
      os << "Backward";
      break;
  } // switch
  os << endl;
  
  os <<
    idtr << setw(15) << "RepeatWinged" << " : ";
  switch (fRepeatWinged) {
    case k_NoRepeatWinged:
      os << "none"; // ??? JMI
      break;
    case kNone:
      os << "none";
      break;
    case kStraight:
      os << "Straight";
      break;
    case kCurved:
      os << "Curved";
      break;
    case kDoubleStraight:
      os << "DoubleStraight";
      break;
    case kDoubleCurved:
      os << "DoubleCurved";
      break;
  } // switch
  os << endl;
     
  idtr--;
}

//______________________________________________________________________________
S_msrMeasure msrMeasure::create (
  int             inputLineNumber,
  int             measureNumber,
  int             divisionsPerWholeNote,
  S_msrVoicechunk voicechunkUplink)
{
  msrMeasure* o =
    new msrMeasure (
      inputLineNumber,
      measureNumber, divisionsPerWholeNote,
      voicechunkUplink);
  assert(o!=0);
  return o;
}

msrMeasure::msrMeasure (
  int             inputLineNumber,
  int             measureNumber,
  int             divisionsPerWholeNote,
  S_msrVoicechunk voicechunkUplink)
    : msrElement (inputLineNumber)
{
  fMeasureNumber = measureNumber;
  
  fMeasureDivisionsPerWholeNote =
    divisionsPerWholeNote;

  fMeasureVoicechunkUplink = voicechunkUplink;

  // fetch measures's voice
  S_msrVoice
    voice =
      fMeasureVoicechunkUplink->
        getVoicechunkVoiceUplink ();

//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating measure " << fMeasureNumber <<
      " in voice \"" << voice->getExternalVoiceNumber () << "\"" <<
      endl;
  
  // set measure part direct link
  fMeasurePartDirectUplink =
    voice->
      getVoiceStaffUplink ()->
        getStaffPartUplink ();

  setMeasureTime (
    fMeasureVoicechunkUplink->
      getVoicechunkTime ());
      
  fMeasureKind = kRegularMeasure; // may be changed afterwards

  fMeasurePosition = 1; // ready to receive the first note

  // initialize measure position high tide
  fMeasurePartDirectUplink->
    setPartMeasurePositionHighTide (1);
}

msrMeasure::~msrMeasure()
{}

S_msrMeasure msrMeasure::createMeasureBareClone (
  S_msrVoicechunk clonedVoicechunk)
{
//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of measure " << fMeasureNumber <<
      endl;
  
  S_msrMeasure
    clone =
      msrMeasure::create (
        fInputLineNumber,
        fMeasureNumber,
        fMeasureDivisionsPerWholeNote,
        clonedVoicechunk);

  clone->
    setMeasureTime (fMeasureTime);
    
  clone->fMeasureKind =
    fMeasureKind;
    
  return clone;
}

void msrMeasure::setMeasureTime (S_msrTime time)
{ 
/*
  The divisions element indicates how many divisions per quarter note are used to indicate a note's duration. For example, if duration = 1 and divisions = 2, this is an eighth note duration.
*/

  fMeasureTime = time;
  
  fMeasureDivisionsPerWholeMeasure =
    fMeasureDivisionsPerWholeNote
      *
    time->getBeatsNumber ()
      /
    time->getBeatsValue ();
}

void msrMeasure::appendNoteToMeasure (S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
  if (note->getNoteKind () == msrNote::kChordMemberNote) {
    stringstream s;

    s <<
      "appendNoteToMeasure(): chord member note " <<
      note->noteAsString () <<
      " appears outside of a chord";

    msrInternalError (
      inputLineNumber,
      s.str());
  }

  // first check whether there is a measure change
// JMI  if (false && fMeasurePosition > fMeasureDivisionsPerWholeMeasure) {
  if (fMeasurePosition > fMeasureDivisionsPerWholeMeasure) { // XXL
    /*
      measure overflows, we must synchonize all voices in this part
    */
    
    // finalize this measure
    this->
      finalizeMeasure (inputLineNumber);
      
    // create a new measure
    S_msrMeasure
      newMeasure =
        msrMeasure::create (
          inputLineNumber,
          fMeasureNumber + 1,
          fMeasureDivisionsPerWholeNote,
          fMeasureVoicechunkUplink);

    // append it to the voice chunk
    fMeasureVoicechunkUplink->
      appendMeasureToVoicechunk (
        newMeasure);

    // append note to it thru the voice chunk
    fMeasureVoicechunkUplink->
      appendNoteToVoicechunk (note);
  }

  else {
    /*
      regular insertion in current measure
    */
    
    // populate measure uplink
    note->setNoteMeasureUplink (this);
  
    // register note measure position
    note->setNotePositionInMeasure (fMeasurePosition);
    
    // fetch note divisions
    int noteDivisions =
      note->getNoteDivisions ();
      
    // account for note duration in measure position
    fMeasurePosition += noteDivisions;
  
    // update part measure position high tide if need be
    fMeasurePartDirectUplink->
      updatePartMeasurePositionHighTide (
        inputLineNumber, fMeasurePosition);
  
    // determine if the note occupies a full measure
    if (noteDivisions == fMeasureDivisionsPerWholeMeasure)
      note->setNoteOccupiesAFullMeasure ();
  
    // append the note to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    fMeasureElementsList.push_back (note);
  }
}

void msrMeasure::appendChordToMeasure (S_msrChord chord) // XXL
{
  int inputLineNumber =
    chord->getInputLineNumber ();
    
  // first check whether there is a measure change
// JMI  if (false && fMeasurePosition > fMeasureDivisionsPerWholeMeasure) {
  if (fMeasurePosition > fMeasureDivisionsPerWholeMeasure) { // XXL
    /*
      measure overflows, we must synchonize all voices in this part
    */
    
    // finalize this measure
    this->
      finalizeMeasure (inputLineNumber);
      
    // create a new measure
    S_msrMeasure
      newMeasure =
        msrMeasure::create (
          inputLineNumber,
          fMeasureNumber + 1,
          fMeasureDivisionsPerWholeNote,
          fMeasureVoicechunkUplink);

    // append it to the voice chunk
    fMeasureVoicechunkUplink->
      appendMeasureToVoicechunk (
        newMeasure);

    // append chord to it thru the voice chunk
    fMeasureVoicechunkUplink->
      appendChordToVoicechunk (chord);
  }

  else {
    /*
      regular insertion in current measure
    */
    
    // populate measure uplink
    chord->setChordMeasureUplink (this);
  
    // register chord measure position
    chord->setChordPositionInMeasure (fMeasurePosition);
    
    // fetch chord divisions
    int chordDivisions =
      chord->getChordDivisions ();
      
    // account for chord duration in measure position
    fMeasurePosition += chordDivisions;
  
    // update part measure position high tide if need be
    fMeasurePartDirectUplink->
      updatePartMeasurePositionHighTide (
        inputLineNumber, fMeasurePosition);
  
    // determine if the chord occupies a full measure
// XXL    if (chordDivisions == fMeasureDivisionsPerWholeMeasure)
      // chord->setChordOccupiesAFullMeasure ();
  
    // append the chord to the measure elements list
    fMeasureElementsList.push_back (chord);
  }
}

S_msrElement msrMeasure::removeLastElementFromMeasure (
  int inputLineNumber)
{
  S_msrElement result;
  
  if (fMeasureElementsList.size ()) {
    result = fMeasureElementsList.back ();
    fMeasureElementsList.pop_back ();
  }
  
  else {
    msrInternalError (
      inputLineNumber,
      "cannot removeLastElementFromMeasure () since it is empty");
  }

  return result;
}

void msrMeasure::finalizeMeasure (int inputLineNumber)
{
//  if (gGeneralOptions->fDebug)
    cerr <<
      endl <<
      idtr <<
      "### --> finalizing measure " << fMeasureNumber <<
      ", line " << inputLineNumber <<
      endl << endl;

  int partMeasurePositionHighTide =
    fMeasurePartDirectUplink->
      getPartMeasurePositionHighTide ();
    
  if (fMeasurePosition < partMeasurePositionHighTide) {
    // appending a skip to this measure to reach measurePosition
    int skipDuration =
      partMeasurePositionHighTide - fMeasurePosition;

    // fetch the voice
    S_msrVoice
      voice =
        fMeasureVoicechunkUplink->
          getVoicechunkVoiceUplink ();
    
    // create the skip
    S_msrNote
      skip =
        msrNote::createSkipNote (
          inputLineNumber,
          skipDuration,
          fMeasureVoicechunkUplink->
            getVoicechunkDivisionsPerWholeNote (),
          voice->
            getVoiceStaffUplink ()->getStaffNumber (),
          voice->
            getExternalVoiceNumber ());

    // does the skip occupy a full measure?
    if (skipDuration == fMeasureDivisionsPerWholeMeasure)
      skip->setNoteOccupiesAFullMeasure ();
  
    // register skip's measure position
    skip->setNotePositionInMeasure (fMeasurePosition);
    
    // apppend the skip to the measure
 //   if (gGeneralOptions->fDebug)
      cerr << idtr <<
       "--> appending " << skip->noteAsString () <<
       " (" << skipDuration <<
       " divs.) to finalize \"" << voice->getVoiceName () <<
       "\" measure: @" << fMeasureNumber << ":" << fMeasurePosition <<
       " --> @" << fMeasureNumber <<
       ":" << partMeasurePositionHighTide <<
       endl;
       
    // append the skip to the measure elements list
    // only now to make it possible to remove it afterwards
    // if it happens to be the first note of a chord
    appendNoteToMeasure (skip);

    // account for skip duration in measure position
    fMeasurePosition += skipDuration;
  }
}

void msrMeasure::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrMeasure::acceptIn()" << endl;
      
  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrMeasure::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrMeasure::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrMeasure::acceptOut()" << endl;

  if (visitor<S_msrMeasure>*
    p =
      dynamic_cast<visitor<S_msrMeasure>*> (v)) {
        S_msrMeasure elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrMeasure::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrMeasure::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrMeasure::browseData()" << endl;

  for (
    list<S_msrElement>::iterator i = fMeasureElementsList.begin();
    i != fMeasureElementsList.end();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "<== msrMeasure::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasure& elt)
{
  elt->print (os);
  return os;
}

string msrMeasure::getMeasureLengthAsString () const
{
  string result;
  string errorMessage;

  int
    measureLength =
      getMeasureLength (); 
  
  if (gGeneralOptions->fDebug)
    cerr <<
      endl <<
      idtr <<
        "% --> measureLength = " << measureLength <<
        ", measureDivisionsPerWholeNote = " <<
        fMeasureDivisionsPerWholeNote <<
        ", measureDivisionsPerWholeMeasure = " <<
        fMeasureDivisionsPerWholeMeasure <<
      endl;

  if (measureLength > 0) {
    result =
      divisionsAsMSRDuration (
        measureLength,
        fMeasureDivisionsPerWholeMeasure,
        errorMessage,
        false); // 'true' to debug it;
  
    if (errorMessage.size ())
      msrMusicXMLError (
        fInputLineNumber,
        errorMessage);
  }
  else
    result = "0";

  return result;
}

string msrMeasure::getMeasureKindAsString () const
{
  string result;

  switch (fMeasureKind) {
    case kRegularMeasure:
      result = "regular";
      break;
    case kIncompleteLeftMeasure:
      result = "incomplete left";
      break;
    case kIncompleteRightMeasure:
      result = "incomplete right";
      break;
  } // switch

  return result;
}

void msrMeasure::print (ostream& os)
{
  os <<
    endl <<
    idtr << "Measure " << fMeasureNumber <<
      ", line " << fInputLineNumber <<
/*
      ", voice " <<
      fMeasureVoicechunkUplink->getVoiceUplink ()->getVoiceName () <<
*/
      ", length: " << getMeasureLength () << " divisions" <<
      " (" << getMeasureLengthAsString () << ")" <<
      ", pos: " << fMeasurePosition << 
      ", " << fMeasureElementsList.size () << " elements" <<
      ", " << getMeasureKindAsString () <<
    endl;

  idtr++;
    
  os <<
    idtr << "Elements:";
  
  if (! fMeasureElementsList.size ())
    os << " none";
    
  else {
    os << endl;
    idtr++;
    
    list<S_msrElement>::const_iterator
      iBegin = fMeasureElementsList.begin(),
      iEnd   = fMeasureElementsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
    
  idtr--;
  }
//  os << endl;
    
  idtr--;
}

//______________________________________________________________________________
S_msrVoicechunk msrVoicechunk::create (
  int           inputLineNumber,
  S_msrVoice    voicechunVoicekUplink)
{
  msrVoicechunk* o =
    new msrVoicechunk (
      inputLineNumber,
      voicechunVoicekUplink);
  assert(o!=0);
  return o;
}

msrVoicechunk::msrVoicechunk (
  int           inputLineNumber,
  S_msrVoice    voicechunVoicekUplink)
    : msrElement (inputLineNumber)
{
  fVoicechunVoicekUplink = voicechunVoicekUplink;

  fVoicechunkDivisionsPerWholeNote =
    fVoicechunVoicekUplink->
      getVoiceDivisionsPerWholeNote ();

  fVoicechunkTime =
    fVoicechunVoicekUplink->
      getVoiceTime ();

  if (! fVoicechunkTime) {
    // use the implicit initial 4/4 time signature
    fVoicechunkTime =
      msrTime::create (
        fInputLineNumber,
        4, 4);
  }

  // has measure number 0 been met?
  bool measureZeroHasBeenMet = // JMI
    fVoicechunVoicekUplink->
      getVoiceStaffUplink ()->
        getStaffPartUplink ()->
          getMeasureZeroHasBeenMetInPart ();

  // first measure number
  int firstMeasureNumber =
    measureZeroHasBeenMet
      ? 0
      : 1;

//  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr <<
      endl <<
      idtr <<
        "### --> Creating voice chunk first measure with number " <<
        firstMeasureNumber <<
        " in voice \"" << fVoicechunVoicekUplink->getVoiceName () << "\"" <<
        endl << endl;

  // create a first measure
  S_msrMeasure
    measure =
      msrMeasure::create (
        inputLineNumber,
        firstMeasureNumber,
        fVoicechunkDivisionsPerWholeNote,
        this);

  // append it to the voice chunk
  fVoicechunkMeasuresList.push_back (measure);

  fMeasureNumberHasBeenSetInVoiceChunk = false;
}

msrVoicechunk::~msrVoicechunk() {}

S_msrVoicechunk msrVoicechunk::createVoicechunkBareClone (
  S_msrVoice clonedVoice)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a voice chunk" <<
      endl;

  S_msrVoicechunk
    clone =
      msrVoicechunk::create (
        fInputLineNumber,
        clonedVoice);

  clone->fVoicechunkTime =
    fVoicechunkTime;
    
  clone->fVoicechunkDivisionsPerWholeNote =
    fVoicechunkDivisionsPerWholeNote;

  // remove the initial measure created implicitly
  fVoicechunkMeasuresList.clear ();
  
  return clone;
}

void msrVoicechunk::setVoicechunkMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{  
  // fetch voice chunk current measure
  S_msrMeasure
    currentMeasure =
      fVoicechunkMeasuresList.back ();

  // fetch its current measure position and length
  int
    currentMeasureNumber =
      currentMeasure->
        getMeasureNumber (),
        
    currentMeasurePosition =
      currentMeasure->
        getMeasurePosition (),
      
    currentMeasureLength =
      currentMeasure->
        getMeasureLength (),
      
    currentMeasureDivisionsPerWholeMeasure =
      currentMeasure->
        getMeasureDivisionsPerWholeMeasure ();

/* JMI
  msrMeasure::msrMeasureKind
    currentMeasureKind =
      currentMeasure->
        getMeasureKind ();
*/
        
 // if (gGeneralOptions->fDebug)
    cerr <<
      idtr <<
        setw(31) << "--> setVoicechunkMeasureNumber (" <<
        measureNumber <<
        "): " <<
        endl <<
      idtr <<
        setw(31) << "currentMeasureNumber" << " = " <<
        currentMeasureNumber <<
        endl <<
      idtr <<
        setw(31) << "currentMeasureDivisionsPerWholeMeasure" << " = " <<
        currentMeasureDivisionsPerWholeMeasure <<
        endl <<
      idtr <<
        setw(31) << "currentMeasurePosition" << " = " <<
        currentMeasurePosition <<
        endl <<
      idtr <<
        setw(31) << "currentMeasureLength" << " = " <<
        currentMeasureLength <<
        endl;
      
  // is the current measure full? (positions start at 1)
  if (currentMeasurePosition <= currentMeasureDivisionsPerWholeMeasure) {
    // no, register current measure as incomplete
    
     if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
        cerr <<
          idtr <<
            "==> measure " << measureNumber <<
            " is incomplete" <<
          endl <<
          idtr;
    
    if (fVoicechunkMeasuresList.size () == 1) {
      // this is the first measure in the voice chunk
      currentMeasure->
        setMeasureKind (
          msrMeasure::kIncompleteLeftMeasure);
    }
    
    else {
      // this is the last measure in the voice chunk
      currentMeasure->
        setMeasureKind (
          msrMeasure::kIncompleteRightMeasure);
    }
  }
  
  else {
    // yes, current measure is full JMI
  }
  
  fVoicechunkMeasureNumber = measureNumber; // JMI

  bool doCreateAMeasure = false;
              
  switch (measureNumber) {
    case 0:
      // measure 1 has already been created by default, re-number it a 0
    // JMI  if (gGeneralOptions->fDebug)
        cerr <<
          idtr <<
            "### there are currently " <<
            fVoicechunkMeasuresList.size () <<
            " measures in voice chunk" <<
            endl <<
          idtr <<
            setw(31) << "--> renumbering measure 1 as 0" <<
            endl;
  
      currentMeasure->
        setMeasureNumber (0);
        
      doCreateAMeasure = false;    
      break;

    case 1:
      if (currentMeasure->getMeasureNumber () == 0) {
        // this is the second measure, that should be created
      // JMI  if (gGeneralOptions->fDebug)
          cerr <<
            idtr <<
              "### there are currently " <<
              fVoicechunkMeasuresList.size () <<
            " measures in voice chunk" <<
              endl <<
            idtr <<
              setw(31) <<
                "--> measure 1 found after measure 0, "
                "a new measure is being created" <<
              endl;
              
        doCreateAMeasure = true;
      }
      // else
        // measure 1 has already been created by default
      break;
      
    default:
      // JMI  if (gGeneralOptions->fDebug)
        cerr <<
          idtr <<
            "### there are currently " <<
            fVoicechunkMeasuresList.size () <<
            " measures in voice chunk" <<
            endl <<
          idtr <<
            setw(31) <<
              "--> measure " << measureNumber <<
              " found, a new measure is being created" <<
            endl;
              
      doCreateAMeasure = true;    
  } // switch

  if (doCreateAMeasure) {
    // finalize current measure
    currentMeasure->
      finalizeMeasure (inputLineNumber);
      
    // create a new measure
    S_msrMeasure
      newMeasure =
        msrMeasure::create (
          inputLineNumber,
          measureNumber,
          fVoicechunkDivisionsPerWholeNote,
          this);
  
    // append it to the voice chunk's measures list
    fVoicechunkMeasuresList.push_back (
      newMeasure);
  }
  
  fMeasureNumberHasBeenSetInVoiceChunk = true;
}

void msrVoicechunk::forceVoicechunkMeasureNumberTo (int measureNumber)
{
  fVoicechunkMeasureNumber = measureNumber;

  if (fVoicechunkMeasuresList.size ()) {
    fVoicechunkMeasuresList.back ()->
      setMeasureNumber (measureNumber);
  }
};

void msrVoicechunk::finalizeLastMeasureOfVoicechunk (int inputLineNumber)
{
//  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> finalizing last measure in voice chunk" <<
      ", line " << inputLineNumber <<
      endl;

  fVoicechunkMeasuresList.back ()->
    finalizeMeasure (inputLineNumber);
}

void msrVoicechunk::appendTimeToVoicechunk (S_msrTime time)
{
  if (gGeneralOptions->fDebug)
    cerr <<
      idtr <<
        "--> appending time " << time->timeAsString () <<
        " to voice chunk" <<
      endl;
      
  // retister time in voice chunk
  fVoicechunkTime = time;

  // append it to this voice chunk
  S_msrElement t = time;
  appendElementToVoicechunk (t);
}

void msrVoicechunk::appendMeasureToVoicechunk (S_msrMeasure measure)
{
  if (fVoicechunkMeasuresList.size ()) {
    // don't append a measure if one with the same
    // measure number is already present
    
    if (fVoicechunkMeasuresList.back ()->getMeasureNumber ()
      ==
    measure->getMeasureNumber ()) {
      // remove previous measure with same number
  // JMI    fVoicechunkMeasuresList.pop_back ();
    }
  }

  // append measure to the voice chunk
  fVoicechunkMeasuresList.push_back (measure);
}

void msrVoicechunk::appendNoteToVoicechunk (S_msrNote note)
{
  fVoicechunkMeasuresList.back ()->
    appendNoteToMeasure (note);
}

void msrVoicechunk::appendChordToVoicechunk (S_msrChord chord) // XXL
{
  fVoicechunkMeasuresList.back ()->
    appendChordToMeasure (chord);
}

/* JMI
void msrVoicechunk::removeElementFromVoicechunk (
  S_msrElement elem)
{
  for (
    list<S_msrElement>::iterator i = fVoicechunkMeasuresList.begin();
    i != fVoicechunkMeasuresList.end();
    i++) {
    if ((*i) == elem) {
      fVoicechunkMeasuresList.erase (i);
      break;
    }
  } // for
}
*/
void msrVoicechunk::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoicechunk::acceptIn()" << endl;
      
  if (visitor<S_msrVoicechunk>*
    p =
      dynamic_cast<visitor<S_msrVoicechunk>*> (v)) {
        S_msrVoicechunk elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoicechunk::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVoicechunk::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoicechunk::acceptOut()" << endl;

  if (visitor<S_msrVoicechunk>*
    p =
      dynamic_cast<visitor<S_msrVoicechunk>*> (v)) {
        S_msrVoicechunk elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoicechunk::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrVoicechunk::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoicechunk::browseData()" << endl;

  for (
    list<S_msrMeasure>::iterator i = fVoicechunkMeasuresList.begin();
    i != fVoicechunkMeasuresList.end();
    i++) {
    // browse the element
    msrBrowser<msrMeasure> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "<== msrVoicechunk::browseData()" << endl;
}

string msrVoicechunk::voicechunkAsString ()
{
  stringstream s;

  s << "Voicechunk" ;
  if (! fVoicechunkMeasuresList.size ())
    s << "(No actual measures)";
  else
    s << "(" << fVoicechunkMeasuresList.size () << " measures)";

  return s.str();
}

ostream& operator<< (ostream& os, const S_msrVoicechunk& elt)
{
  elt->print (os);
  return os;
}

void msrVoicechunk::print (ostream& os)
{  
  os << idtr <<
    "Voicechunk" <<
    " (" << fVoicechunkMeasuresList.size() << " measures)" <<
    endl;

  idtr++;

  if (! fVoicechunkTime) {
    // use the implicit initial 4/4 time signature
    fVoicechunkTime =
      msrTime::create (
        fInputLineNumber,
        4, 4);
  }

  os <<
    idtr <<
      setw(32) << "(VoicechunkDivisionsPerWholeNote" << " = " <<
      fVoicechunkDivisionsPerWholeNote << ")" <<
      endl <<
    idtr <<
      setw(32) << "(fVoicechunkTime" << " = " <<
      fVoicechunkTime->timeAsString () << ")" <<
      endl;

  os <<
    idtr << "Measures:";
  
  if (! fVoicechunkMeasuresList.size ())
    os << " none";
    
  else {    
 //   os << endl;
    idtr++;
    
    list<S_msrMeasure>::const_iterator
      iBegin = fVoicechunkMeasuresList.begin(),
      iEnd   = fVoicechunkMeasuresList.end(),
      i      = iBegin;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for
    
  idtr--;
  }

  os << endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrRepeatending msrRepeatending::create (
  int                 inputLineNumber,
  string              repeatendingNumber, // may be "1, 2"
  msrRepeatendingKind repeatendingKind,
  S_msrVoicechunk     voicechunk,
  S_msrRepeat         repeatUplink)
{
  msrRepeatending* o =
    new msrRepeatending (
      inputLineNumber,
      repeatendingNumber,
      repeatendingKind,
      voicechunk,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatending::msrRepeatending (
  int                 inputLineNumber,
  string              repeatendingNumber, // may be "1, 2"
  msrRepeatendingKind repeatendingKind,
  S_msrVoicechunk     voicechunk,
  S_msrRepeat         repeatUplink)
    : msrElement (inputLineNumber)
{
  fRepeatendingNumber = repeatendingNumber;
  
  fRepeatendingInternalNumber = 0;
    // will be set by msrRepeat::addRepeatending ()
  
  fRepeatendingKind = repeatendingKind;
  
  fRepeatendingVoicechunk   = voicechunk;
  fRepeatendingRepeatUplink = repeatUplink;
}

msrRepeatending::~msrRepeatending() {}

S_msrRepeatending msrRepeatending::createRepeatendingBareClone (
  S_msrRepeat clonedRepeat)
{
  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a repeat ending" << endl;
  
  S_msrRepeatending
    clone =
      msrRepeatending::create (
        fInputLineNumber,
        fRepeatendingNumber,
        fRepeatendingKind,
        clonedRepeat->getRepeatCommonPart (),
        clonedRepeat);
  
  return clone;
}

void msrRepeatending::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeatending::acceptIn()" << endl;
      
  if (visitor<S_msrRepeatending>*
    p =
      dynamic_cast<visitor<S_msrRepeatending>*> (v)) {
        S_msrRepeatending elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeatending::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrRepeatending::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeatending::acceptOut()" << endl;

  if (visitor<S_msrRepeatending>*
    p =
      dynamic_cast<visitor<S_msrRepeatending>*> (v)) {
        S_msrRepeatending elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeatending::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrRepeatending::browseData (basevisitor* v)
{
  // browse the voice chunk
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fRepeatendingVoicechunk);
}

ostream& operator<< (ostream& os, const S_msrRepeatending& elt)
{
  elt->print (os);
  return os;
}

void msrRepeatending::print (ostream& os)
{
  os <<
    "Repeatending" <<
    ", line " << fInputLineNumber <<
    ", number " << fRepeatendingNumber << ", ";

  switch (fRepeatendingKind) {
    case kHookedEnding:
      os << "hooked";
      break;
    case kHooklessEnding:
      os << "hookless";
      break;
  } // switch
  os << endl;
  
  idtr++;

  os << fRepeatendingVoicechunk;

  idtr--;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  int             inputLineNumber,
  S_msrVoicechunk commonPart,
  S_msrVoice      voiceUplink)
{
  msrRepeat* o =
    new msrRepeat (
      inputLineNumber, commonPart, voiceUplink);
  assert(o!=0);
  return o;
}

msrRepeat::msrRepeat (
  int             inputLineNumber,
  S_msrVoicechunk commonPart,
  S_msrVoice      voiceUplink)
    : msrElement (inputLineNumber)
{
  fRepeatCommonPart     = commonPart;
  fRepeatEndingsCounter = 0;
  fRepeatVoiceUplink    = voiceUplink;
}

msrRepeat::~msrRepeat() {}

S_msrRepeat msrRepeat::createRepeatBareClone (S_msrVoice clonedVoice)
{
  S_msrVoicechunk
    voicechunk =
      msrVoicechunk::create (
        fInputLineNumber,
        clonedVoice);

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a repeat" << endl;
  
  S_msrRepeat
    clone =
      msrRepeat::create (
        fInputLineNumber,
        voicechunk,
        clonedVoice);
  
  return clone;
}

void msrRepeat::setRepeatCommonPart (
  S_msrVoicechunk repeatCommonPart)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting repeat common part containing " <<
      repeatCommonPart->getVoicechunkMeasuresList ().size () <<
      " measures" <<
      endl;
      
  fRepeatCommonPart = repeatCommonPart;
}

void msrRepeat::addRepeatending (S_msrRepeatending repeatending)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding an ending to repeat" << endl;
      
  fRepeatEndings.push_back (repeatending);

  repeatending->setRepeatendingNumber (
    ++ fRepeatEndingsCounter);
}

void msrRepeat::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeat::acceptIn()" << endl;
      
  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeat::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrRepeat::acceptOut()" << endl;

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrRepeat::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrRepeat::browseData (basevisitor* v)
{
  // browse the common part
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fRepeatCommonPart);
  
  // browse the alternatives
  for (
    vector<S_msrRepeatending>::iterator i = fRepeatEndings.begin();
    i != fRepeatEndings.end();
    i++) {
    // browse the alternative
    msrBrowser<msrRepeatending> browser (v);
    browser.browse (*(*i));
  } // for
}

ostream& operator<< (ostream& os, const S_msrRepeat& elt)
{
  elt->print (os);
  return os;
}

void msrRepeat::print (ostream& os)
{
  os <<
    endl <<
    idtr << "Repeat" <<
    ", line " << fInputLineNumber <<
    " (" << fRepeatEndings.size() << " repeat endings)" <<
    endl;
  
  idtr++;
  
  os << fRepeatCommonPart;
  
  vector<S_msrRepeatending>::const_iterator i;
  for (i=fRepeatEndings.begin(); i!=fRepeatEndings.end(); i++) {
    os << idtr << (*i);
  } // for
    
  idtr--;
}

//______________________________________________________________________________ 
S_msrVoice msrVoice::create (
  int           inputLineNumber,
  int           externalVoiceNumber,
  S_msrStaff    voiceStaffUplink)
{
  msrVoice* o =
    new msrVoice (
      inputLineNumber,
      externalVoiceNumber,
      voiceStaffUplink);
  assert(o!=0);
  return o;
}

S_msrVoice msrVoice::createVoiceBareClone (S_msrStaff clonedStaff)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of voice \"" <<
      getVoiceName () <<
      "\"" <<
      endl;

  S_msrVoice
    clone =
      msrVoice::create (
        fInputLineNumber,
        fExternalVoiceNumber,
        clonedStaff);

  // populate the voice clone
  clone->fVoiceDivisionsPerWholeNote =
    fVoiceDivisionsPerWholeNote;

  clone->fExternalVoiceNumber =
    fExternalVoiceNumber;

  clone->fMeasureZeroHasBeenMetInVoice =
    fMeasureZeroHasBeenMetInVoice;
  clone->fMeasureNumberHasBeenSetInVoice =
    fMeasureNumberHasBeenSetInVoice;
  clone->fMusicHasBeenInsertedInVoice =
    fMusicHasBeenInsertedInVoice;

/* JMI
  // create the voice chunk
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating the initial voice chunk clone for voice \"" <<
      clone->getVoiceName () << "\"" <<
      endl;
      
  clone->
    fVoiceVoicechunk =
      msrVoicechunk::create (
        clone->fInputLineNumber,
        clone);
  */
  
  return clone;
}

msrVoice::msrVoice (
  int           inputLineNumber,
  int           externalVoiceNumber,
  S_msrStaff    voiceStaffUplink)
    : msrElement (inputLineNumber)
{
  fExternalVoiceNumber = externalVoiceNumber;
  
  fStaffRelativeVoiceNumber = externalVoiceNumber;
    // may be changed afterwards
  
  fVoiceStaffUplink = voiceStaffUplink;

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating voice \"" << getVoiceName () <<
      "\" in staff \"" << fVoiceStaffUplink->getStaffName () << "\"" <<
      endl;

  // the external voice number should not be negative
  // (0 is used for the part voice master)
  if (externalVoiceNumber < 0) {
    stringstream s;

    s <<
      "voice number " << externalVoiceNumber <<
      " is not in the 0..4 range";
      
    msrAssert (false, s.str());
  }
  
  fVoiceDivisionsPerWholeNote =
    fVoiceStaffUplink->
      getStaffDivisionsPerWholeNote ();
    
  // there may be an anacrusis, but let's start with 1 anyway
  fVoiceMeasureNumber = 1;

  fMeasureZeroHasBeenMetInVoice   = false;
  fMeasureNumberHasBeenSetInVoice = false;
  fMusicHasBeenInsertedInVoice    = false;
  
  fVoiceActualNotesCounter = 0;

  // add the master lyrics to this voice, to
  // collect skips along the way that are used as a 'prelude'
  // by actual lyrics that start at later points
  fVoiceLyricsmaster =
    msrLyrics::create (
      inputLineNumber,
      0,    // this lyrics number is unused anyway
      msrLyrics::kMasterLyrics,
      this);

  // create the initial voice chunk for this voice
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating the initial voice chunk for voice \"" <<
      getVoiceName () << "\"" <<
      endl;
      
  fVoiceVoicechunk =
    msrVoicechunk::create (
      inputLineNumber,
      this);

  // get the initial clef from the staff if any
  {
  S_msrClef
    clef =
      fVoiceStaffUplink->getStaffClef ();
      /*
  if (! clef)
    // it doesn't exist yet, create default G clef
    clef =
      msrClef::create (
        inputLineNumber,
        "G", 2, 0);
        */
  if (clef) {
    // append it to the voice chunk
    S_msrElement c = clef;
    fVoiceVoicechunk->
      appendElementToVoicechunk (c);
    }
  }
    
  // get the initial key from the staff if any
  {
  S_msrKey
    key =
      fVoiceStaffUplink->getStaffKey ();
  /*    JMI
  if (! key)
    // it doesn't exist yet, create default C major key
    key =
      msrKey::create (
        inputLineNumber,
        0, "major", 0);
        */
    if (key) {
      // append it to the voice chunk
      S_msrElement k = key;
      fVoiceVoicechunk->
        appendElementToVoicechunk (k);
    }
  }
  
  // get the initial time from the staff if any
  {
    S_msrTime
      time =
        fVoiceStaffUplink->getStaffTime ();
        /*
    if (! time)
      // it doesn't exist yet, create default 4/4 time
      time =
        msrTime::create (
          inputLineNumber,
          4, 4);
  */
    if (time) {
      // append it to the voice chunk
      S_msrElement t = time;
      fVoiceVoicechunk->
        appendElementToVoicechunk (t);
    }
  }
  
  // get the initial transpose from the staff if any
  {
    S_msrTranspose
      transpose =
        fVoiceStaffUplink->getStaffTranspose ();
        
    if (transpose) {
      // append it to the voice chunk
      S_msrElement t = transpose;
      fVoiceVoicechunk->
        appendElementToVoicechunk (t);
    }
  }
}

msrVoice::~msrVoice() {}

string msrVoice::getVoiceName () const
{
  int voiceNumber =
    gMsrOptions-> fCreateStaffRelativeVoiceNumbers // JMI use
      ? fStaffRelativeVoiceNumber
      : fExternalVoiceNumber;

  string suffix =
    fStaffRelativeVoiceNumber == 0
      ? "MASTER"
      : int2EnglishWord (voiceNumber);
      
  return
    fVoiceStaffUplink->getStaffName() +
    "_V_" +
    suffix;
}

void msrVoice::setVoiceDivisionsPerWholeNote (
  int divisionsPerWholeNote)
{
  fVoiceDivisionsPerWholeNote =
    divisionsPerWholeNote;

  fVoiceVoicechunk->
    setVoicechunkDivisionsPerWholeNote (
      divisionsPerWholeNote);
}

void msrVoice::setVoiceMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
  fVoiceMeasureNumber = measureNumber;

  fVoiceVoicechunk->
    setVoicechunkMeasureNumber (
      inputLineNumber,
      fVoiceMeasureNumber);

  fMeasureNumberHasBeenSetInVoice = true;

  if (measureNumber == 0) {  
    fMeasureZeroHasBeenMetInVoice = true;
  }
}

void msrVoice::forceVoiceMeasureNumberTo (int measureNumber) // JMI
{
  fVoiceMeasureNumber = measureNumber;

  fVoiceVoicechunk->
    forceVoicechunkMeasureNumberTo (measureNumber);
};

void msrVoice::setNewVoicechunkForVoice (
  int inputLineNumber)
{
  // create the voice chunk
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating a new voice chunk for voice " <<
      getVoiceName () << endl;
      
  fVoiceVoicechunk =
    msrVoicechunk::create (
      inputLineNumber,
      this);
}

S_msrLyrics msrVoice::addLyricsToVoiceByItsNumber (
  int inputLineNumber,
  int lyricsNumber)
{
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    stringstream s;
    
    s <<
      "lyrics " << lyricsNumber <<
      " already exists in this voice";

    msrInternalError (
      inputLineNumber,
      s.str());

// JMI    return fVoiceLyricsMap [lyricsNumber];
  }

  // create the lyrics
  S_msrLyrics
    lyrics =
      msrLyrics::create (
        inputLineNumber,
        lyricsNumber,
        msrLyrics::kRegularLyrics,
        this);

  // add it to this voice
  addLyricsToVoice (lyrics);

  // return it
  return lyrics;
}

void msrVoice::addLyricsToVoice (S_msrLyrics lyrics)
{
  // get lyrics number
  int lyricsNumber =
    lyrics->getLyricsNumber ();
    
  // register lyrics in this voice
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding lyrics " << lyrics->getLyricsName () <<
      " (" << lyricsNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceLyricsMap [lyricsNumber] = lyrics;

  // catch up with fVoiceLyricsmaster
  // in case the lyrics does not start upon the first voice note
  vector<S_msrLyricschunk>
    masterChunks =
      fVoiceLyricsmaster->getLyricschunks ();

  if (masterChunks.size()) {
// JMI    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Copying current contents of voice master lyrics to new lyrics" << endl;
    for (
      vector<S_msrLyricschunk>::const_iterator i =
        masterChunks.begin();
      i != masterChunks.end();
      i++) {
      // add chunk to lyrics
      lyrics->addChunkToLyrics ((*i));
    } // for
  }
}

S_msrLyrics msrVoice::createLyricsInVoiceIfNeeded (
  int inputLineNumber,
  int lyricsNumber)
{
  if (false && gGeneralOptions->fTrace) // JMI
    cerr << idtr <<
      "### --> createLyricsInVoiceIfNeeded (" << inputLineNumber <<
      ", " << lyricsNumber << ")" <<
      ", fVoiceLyricsMap.size() = " << fVoiceLyricsMap.size () <<
      endl;

  S_msrLyrics lyrics;

  // is lyricsNumber already known in voice?
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    // yes, use it
    lyrics = fVoiceLyricsMap [lyricsNumber];
  }
  
  else {
    // no, add it to the voice
    lyrics =
      addLyricsToVoiceByItsNumber (
        inputLineNumber, lyricsNumber);
  }

  return lyrics;
}

void msrVoice::appendClefToVoice (S_msrClef clef)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending clef '" << clef->clefAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement c = clef;
  fVoiceVoicechunk->
    appendElementToVoicechunk (c);
}

void msrVoice::appendKeyToVoice (S_msrKey key)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending key '" << key->keyAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement k = key;
  fVoiceVoicechunk->
    appendElementToVoicechunk (k);
}

void msrVoice::appendTimeToVoice (S_msrTime time)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending time '" << time->timeAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  // register time in voice
  fVoiceTime = time;

  // append it to the voice chunk
  fVoiceVoicechunk->
    appendTimeToVoicechunk (time);
}

void msrVoice::appendTransposeToVoice (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending transpose '" << transpose->transposeAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement t = transpose;
  fVoiceVoicechunk->
    appendElementToVoicechunk (t);
}

/* JMI
void msrVoice::appendWordsToVoice (S_msrWords words)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending words '" << words->wordsAsString () <<
      "' to voice " << getVoiceName () << endl;

  S_msrElement w = words;
  fVoiceVoicechunk->
    appendElementToVoicechunk (w);
}
*/

void msrVoice::appendTempoToVoice (S_msrTempo tempo)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending tempo '" << tempo->tempoAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement t = tempo;
  fVoiceVoicechunk->
    appendElementToVoicechunk (t);
}

void msrVoice::appendOctaveShiftToVoice (S_msrOctaveShift octaveShift)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending octave shift '" <<
      octaveShift->octaveShiftKindAsString () <<
      "', size: " << octaveShift->getOctaveShiftSize () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement o = octaveShift;
  fVoiceVoicechunk->
    appendElementToVoicechunk (o);
}

void msrVoice::appendRehearsalToVoice (S_msrRehearsal rehearsal)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending rehearsal '" << rehearsal->getRehearsalText () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement r = rehearsal;
  fVoiceVoicechunk->
    appendElementToVoicechunk (r);
}

void msrVoice::appendNoteToVoice (S_msrNote note) {
  if (gGeneralOptions->fDebugDebug) {
    cerr << idtr <<
      "Appending note:" <<
      endl;

    idtr++;
    
    cerr <<
      idtr <<
        note <<
      idtr <<
        "to voice \"" << getVoiceName () << "\"" <<
        endl;

    idtr--;
  }

  // register actual note
  fVoiceActualNotesCounter++;
  fMusicHasBeenInsertedInVoice = true;

  // append the note to the voice chunk
  fVoiceVoicechunk->
    appendNoteToVoicechunk (note);
  
  // add a skip chunk of the same duration to the master lyrics
  int
    lyricsDivisions =
      note->getNoteDivisions ();

  fVoiceLyricsmaster->
    addSkipChunkToLyrics (
      note->getInputLineNumber (),
      lyricsDivisions,
      note); // JMI

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendChordToVoice (S_msrChord chord)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "Appending chord '" << chord <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceVoicechunk->
    appendChordToVoicechunk (chord);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendTupletToVoice (S_msrTuplet tuplet) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "Appending tuplet '" << tuplet <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement t = tuplet;
  fVoiceVoicechunk->
    appendElementToVoicechunk (t);

  fMusicHasBeenInsertedInVoice = true;
}

void msrVoice::appendGracenotesToVoice (
  S_msrGracenotes gracenotes)
{
  if (true || gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug)
//  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "Appending grace notes " << // JMI gracenotes <<
      " to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement g = gracenotes;
  fVoiceVoicechunk->
    appendElementToVoicechunk (g);

  fMusicHasBeenInsertedInVoice = true;
}

S_msrLyricschunk msrVoice::addTextLyricschunkToVoice (
  int       inputLineNumber,
  int       lyricsNumber,
  string    syllabic,
  msrLyricschunk::msrLyricschunkKind
            lyricschunkKind,
  string    text,
  bool      elision,
  int       divisions)
{
  // create a 'Text' lyrics chunk
 if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
  cerr << idtr <<
    "--> Adding 'Text' lyrics chunk"
    ", line " << inputLineNumber <<
    ", divisions = " << divisions << 
    ", lyricsNumber = \"" << lyricsNumber << "\"" <<
    ", syllabic = \"" << syllabic << "\"" <<
    ", text = \"" << text << "\"" <<
    ", elision: " << elision <<
    " in voice \"" << getVoiceName () << "\"" <<
    endl;
  }

  // fetch lyricsNumber in this voice
  S_msrLyrics
    lyrics =
      createLyricsInVoiceIfNeeded (
        inputLineNumber, lyricsNumber);

  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        inputLineNumber,
        lyricschunkKind,
        text,
        divisions,
        lyrics);

  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricschunk);

  // and return it
  return lyricschunk;
}

S_msrLyricschunk msrVoice::addSkipLyricschunkToVoice (
  int       inputLineNumber,
  int       lyricsNumber,
  int       divisions)
{
  // create a 'Skip' lyrics text chunk
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    cerr << idtr <<
      "--> Adding 'Skip' lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
      " in voice " << getVoiceName () << endl;
  }

  // fetch lyricsNumber in this voice
  S_msrLyrics
    lyrics =
      createLyricsInVoiceIfNeeded (
        inputLineNumber, lyricsNumber);
  
  // create 'Skip' lyrics chunk
  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kSkipChunk, "", divisions,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricschunk);

  // and return it
  return lyricschunk;
}

S_msrLyricschunk msrVoice::addTiedLyricschunkToVoice (
  int       inputLineNumber,
  int       lyricsNumber,
  int       divisions)
{
  // create a 'Tied' lyrics chunk
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    cerr << idtr <<
      "--> Adding 'Tied' lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
      " in voice " << getVoiceName () <<
      endl;
  }

  // fetch lyricsNumber in this voice
  S_msrLyrics
    lyrics =
      createLyricsInVoiceIfNeeded (
        inputLineNumber, lyricsNumber);

  // create lyrics slur chunk
  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kTiedChunk, "", divisions,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricschunk);

  // and return it
  return lyricschunk;
}

S_msrLyricschunk msrVoice::addSlurLyricschunkToVoice (
  int       inputLineNumber,
  int       lyricsNumber,
  int       divisions)
{
  // create a 'Slur' lyrics text chunk
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    cerr << idtr <<
      "--> Adding 'Slur' lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
      " in lyrics " << lyricsNumber <<
      " in voice " << getVoiceName () <<
      endl;
  }

  // fetch lyricsNumber in this voice
  S_msrLyrics
    lyrics =
      createLyricsInVoiceIfNeeded (
        inputLineNumber, lyricsNumber);
  
  // create lyrics slur chunk
  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kSlurChunk, "", divisions,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricschunk);

  // and return it
  return lyricschunk;
}

S_msrLyricschunk msrVoice::addSlurBeyondEndLyricschunkToVoice (
  int       inputLineNumber,
  int       lyricsNumber,
  int       divisions)
{
  // create a 'SlurBeyondEnd' lyrics chunk
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    cerr << idtr <<
      "--> Adding 'SlurBeyondEnd' lyrics chunk"
      ", line " << inputLineNumber <<
      ", divisions = " << divisions <<
      " in voice \"" << getVoiceName () << "\"" <<
      endl;
  }

  // fetch lyricsNumber in this voice
  S_msrLyrics
    lyrics =
      createLyricsInVoiceIfNeeded (
        inputLineNumber, lyricsNumber);
  
  // create lyrics 'SlurBeyondEnd' chunk
  S_msrLyricschunk
    lyricschunk =
      msrLyricschunk::create (
        inputLineNumber,
        msrLyricschunk::kSlurBeyondEndChunk, "", divisions,
        lyrics);
        
  // add it to the lyrics
  lyrics->
    addChunkToLyrics (lyricschunk);

  // and return it
  return lyricschunk;
}

void msrVoice::appendBarCheckToVoice (S_msrBarCheck barCheck)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending bar check '" << barCheck->barCheckAsString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;

  fVoiceVoicechunk->
    appendElementToVoicechunk (barCheck);

  // add bar check chunk to the voice master lyrics
  /* JMI
  fVoiceLyricsmaster->
    addBarcheckChunkToLyrics (
      barCheck->getInputLineNumber (),
      fVoiceMeasureLocation.fMeasureNumber);
      */
}

void msrVoice::appendBarnumberCheckToVoice (S_msrBarnumberCheck bnc)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending barnumber check '" << bnc->barnumberCheckAsString () <<
      "' to voice \"" << getVoiceName () <<  "\"" <<
      endl;

  fVoiceVoicechunk->
    appendElementToVoicechunk (bnc);

/*
  // add barnumber check chunk to the voice master lyrics
  fVoiceLyricsmaster->
    addBarnumberCheckChunkToLyrics (
      bnc->getInputLineNumber (),
      fVoiceMeasureLocation.fMeasureNumber);
*/
}

void msrVoice::appendBreakToVoice (S_msrBreak break_)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending break '" << break_->breakAsString () <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceVoicechunk->
    appendElementToVoicechunk (break_);

  // add break chunk to the voice master lyrics
  fVoiceLyricsmaster->
    addBreakChunkToLyrics (
      break_->getInputLineNumber (),
      break_->getNextBarNumber ());
}

void msrVoice::appendRepeatToVoice (S_msrRepeat repeat) {
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending repeat to voice \"" << getVoiceName () <<  "\"" <<
      endl;

  S_msrElement r = repeat;
  fVoiceVoicechunk->
    appendElementToVoicechunk (r);
}

void msrVoice::prependBarlineToVoice (S_msrBarline barline) {
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Prepending a barline to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;
      
  idtr++;
  cerr << idtr <<
    barline;
  idtr--;

  S_msrElement b = barline;
  fVoiceVoicechunk->
    prependElementToVoicechunk (b);
}

void msrVoice::appendBarlineToVoice (S_msrBarline barline) {
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending a barline to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;
      
  idtr++;
  cerr << idtr <<
    barline;
  idtr--;

  S_msrElement b = barline;
  fVoiceVoicechunk->
    appendElementToVoicechunk (b);
}

void msrVoice::appendSegnoToVoice (S_msrSegno segno) {
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending a segno to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement s = segno;
  fVoiceVoicechunk->
    appendElementToVoicechunk (s);
}

void msrVoice::appendCodaToVoice (S_msrCoda coda) {
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending a coda to voice \"" << getVoiceName () << "\"" <<
      ":" << endl;

  S_msrElement c = coda;
  fVoiceVoicechunk->
    appendElementToVoicechunk (c);
}

void msrVoice::appendEyeglassesToVoice (S_msrEyeglasses eyeglasses) {
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending a eyeglasses to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement e = eyeglasses;
  fVoiceVoicechunk->
    appendElementToVoicechunk (e);
}

void msrVoice::appendPedalToVoice (S_msrPedal pedal) {
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Appending a pedal to voice \"" << getVoiceName () << "\"" <<
      endl;

  S_msrElement e = pedal;
  fVoiceVoicechunk->
    appendElementToVoicechunk (e);
}

/* JMI
void msrVoice::appendElementToVoice (S_msrElement elem)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "Appending element '" << elem <<
      "' to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceVoicechunk->
    appendElementToVoicechunk (elem);
}
*/

S_msrElement msrVoice::removeLastElementFromVoice (
  int inputLineNumber)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "Removing last element " <<
      " from voice " << getVoiceName () << endl;

  return
    fVoiceVoicechunk->
      removeLastElementFromVoicechunk (inputLineNumber);
}

void msrVoice::finalizeLastMeasureOfVoice (int inputLineNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> finalizing last measure in voice " <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;

  fVoiceVoicechunk->
    finalizeLastMeasureOfVoicechunk (inputLineNumber);
}

void msrVoice::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoice::acceptIn()" << endl;
      
  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoice::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrVoice::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoice::acceptOut()" << endl;

  if (visitor<S_msrVoice>*
    p =
      dynamic_cast<visitor<S_msrVoice>*> (v)) {
        S_msrVoice elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrVoice::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrVoice::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrVoice::browseData()" << endl;

  // browse the voice chunk
  msrBrowser<msrVoicechunk> browser (v);
  browser.browse (*fVoiceVoicechunk);

  // browse the voice lyrics
  if (fVoiceLyricsMap.size ()) {
    for (
      map<int, S_msrLyrics>::iterator i = fVoiceLyricsMap.begin();
      i != fVoiceLyricsMap.end();
      i++) {
      // browse the lyrics
      msrBrowser<msrLyrics> browser (v);
      browser.browse (*((*i).second));
    } // for
  }

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "<== msrVoice::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}

void msrVoice::print (ostream& os)
{
  os <<
    "Voice" " \"" << getVoiceName () <<
    "\" (" <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceLyricsMap.size(), "lyric", "lyrics") <<
    ")" <<
    endl;

  idtr++;

  os <<
    idtr <<
      setw(32) << "(fMeasureZeroHasBeenMetInVoice" << " = " <<
      string (
        fMeasureZeroHasBeenMetInVoice
          ? "true"
          : "false") <<
      ")" <<
      endl <<
    idtr <<
      setw(32) << "(fMeasureNumberHasBeenSetInVoice" << " = " <<
      string (
        fMeasureNumberHasBeenSetInVoice
          ? "true"
          : "false") <<
      ")" <<
      endl <<
    idtr <<
      setw(32) << "(fMusicHasBeenInsertedInVoice" << " = " <<
      string (
        fMusicHasBeenInsertedInVoice
          ? "true"
          : "false") <<
      ")" <<
      endl;

  // print the voice chunk
  os << fVoiceVoicechunk;
  
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    // print the master lyrics
    os << idtr <<
      fVoiceLyricsmaster <<
      endl;    
  }
  
  if (! gMsrOptions->fDontDisplayMSRLyrics) {
    // print the lyrics
    if (fVoiceLyricsMap.size()) {
      map<int, S_msrLyrics>::const_iterator
        iBegin = fVoiceLyricsMap.begin(),
        iEnd   = fVoiceLyricsMap.end(),
        i      = iBegin;
        
      for ( ; ; ) {
        os << idtr << (*i).second;
        if (++i == iEnd) break;
        os << endl;
      } // for
    }
  }
  
  idtr--;
}

//______________________________________________________________________________
S_msrStafftuning msrStafftuning::create (
  int           inputLineNumber,
  int           stafftuningLineNumber,
  char          stafftuningStep,
  int           stafftuningOctave,
  float         staffTuningAlter)
{
  msrStafftuning* o =
    new msrStafftuning (
      inputLineNumber,
      stafftuningLineNumber,
      stafftuningStep,
      stafftuningOctave,
      staffTuningAlter);
  assert(o!=0);
  return o;
}

msrStafftuning::msrStafftuning (
  int           inputLineNumber,
  int           stafftuningLineNumber,
  char          stafftuningStep,
  int           stafftuningOctave,
  float         staffTuningAlter)
    : msrElement (inputLineNumber)
{
  fStafftuningLineNumber = stafftuningLineNumber;
  fStafftuningStep       = stafftuningStep;
  fStafftuningOctave     = stafftuningOctave;
  fStaffTuningAlter      = staffTuningAlter;
}

msrStafftuning::~ msrStafftuning ()
{}

S_msrStafftuning msrStafftuning::createStafftuningBareClone ()
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a staff tuning" <<
      endl;

 S_msrStafftuning
    clone =
      msrStafftuning::create (
        fInputLineNumber,
        fStafftuningLineNumber,
        fStafftuningStep,
        fStafftuningOctave,
        fStaffTuningAlter);
  
  return clone;
}

void msrStafftuning::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStafftuning::acceptIn()" << endl;
      
  if (visitor<S_msrStafftuning>*
    p =
      dynamic_cast<visitor<S_msrStafftuning>*> (v)) {
        S_msrStafftuning elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStafftuning::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrStafftuning::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStafftuning::acceptOut()" << endl;

  if (visitor<S_msrStafftuning>*
    p =
      dynamic_cast<visitor<S_msrStafftuning>*> (v)) {
        S_msrStafftuning elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStafftuning::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrStafftuning::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStafftuning& elt)
{
  elt->print (os);
  return os;
}

string msrStafftuning::stafftuningAsString () const
{
  stringstream s;

  s <<
    "line " << fStafftuningLineNumber <<
    ", " << fStafftuningStep <<
    ", octave " << fStafftuningOctave <<
    ", alter " << fStaffTuningAlter;
    
  return s.str();
}

void msrStafftuning::print (ostream& os)
{
  os <<
    "Stafftuning" <<
    endl;

  idtr++;

  os << left <<  
    idtr <<
      setw(21) << "StafftuningLineNumber" << " = " <<
      fStafftuningLineNumber <<
      endl <<
    idtr <<
      setw(21) << "StafftuningStep" << " = " <<
      fStafftuningStep <<
      endl <<
    idtr <<
      setw(21) << "StafftuningOctave" << " = " <<
      fStafftuningOctave <<
      endl <<
    idtr <<
      setw(21) << "StaffTuningAlter" << " = " <<
      fStaffTuningAlter <<
      endl;

  idtr--;
}

//______________________________________________________________________________
int msrStaff::gMaxStaffVoices = 4;

S_msrStaff msrStaff::create (
  int           inputLineNumber,
  int           staffNumber,
  S_msrPart     staffPartUplink)
{
  msrStaff* o =
    new msrStaff (
      inputLineNumber, staffNumber, staffPartUplink);
  assert(o!=0);
  return o;
}

msrStaff::msrStaff (
  int           inputLineNumber,
  int           staffNumber,
  S_msrPart     staffPartUplink)
    : msrElement (inputLineNumber)
{
  fStaffKind = kRegularStaff;
  
  fStaffNumber = staffNumber;
  fStaffPartUplink   = staffPartUplink;

  fRegisteredVoicesCounter = 0;

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating staff \"" << getStaffName () <<
      "\" in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // the staff number should not be negative
  // (0 is used for hidden staff containing the part voice master) JMI
  if (staffNumber < 0) {
    stringstream s;

    s <<
      "staff number " << staffNumber <<
      " is not positive";
      
    msrAssert (false, s.str());
  }

  // populate the staff
  fStaffDivisionsPerWholeNote =
    fStaffPartUplink->
      getPartDivisionsPerWholeNote ();

  // create the staff voice master with relative number 0
  // and all 'gMaxStaffVoices' voices for this staff
  // those that remain without music will be removed later
  for (int i = 0; i <= gMaxStaffVoices; i++) {
    addVoiceToStaffByItsRelativeNumber (
      fInputLineNumber, i);
  } // for

  // set staff voice master
  fStaffVoiceMaster =
    fStaffVoicesMap [0];

  // mark it as containing music, to prevent it being removed
  fStaffVoiceMaster->
    setMusicHasBeenInsertedInVoice ();

  // get the initial clef from the staff if any
  {
    S_msrClef
      clef =
        fStaffPartUplink->getPartClef ();
  
    if (clef) {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting staff clef '" << clef->clefAsString () <<
          "' in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      fStaffClef = clef;

      // is this a tablature or percussion staff?
      if (clef->clefIsATablatureClef ())
        fStaffKind = kTablatureStaff;
      else if (clef->clefIsAPercussionClef ())
        fStaffKind = kPercussionStaff;

      appendClefToAllStaffVoices (clef);
    }
    
    else {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting default treble clef " <<
          " in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      // create the implicit initial G line 2 clef
      fStaffClef =
        msrClef::create (
          inputLineNumber,
          msrClef::kTrebleClef);
    }
  }
    
  // get the initial key from the staff if any
  {
    S_msrKey
      key =
        fStaffPartUplink->getPartKey ();
  
    if (key) {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting staff key '" << key->keyAsString () <<
          "' in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      fStaffKey = key;
      
      appendKeyToAllStaffVoices (key);
    }
    else {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting default C major key " <<
          " in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;
          
      // create the implicit initial C major key
      fStaffKey =
        msrKey::create (
          inputLineNumber,
          0, "major", 0);
    }
  }
  
  // get the initial time from the staff if any
  {
    S_msrTime
      time =
        fStaffPartUplink->getPartTime ();

    if (time) {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting staff time '" << time->timeAsString () <<
          "' in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      fStaffTime = time;

      appendTimeToAllStaffVoices (time);
    }
    else {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting default 4/4 time " <<
          " in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;
          
      // create the implicit initial 4/4 time signature
      fStaffTime =
        msrTime::create (
          inputLineNumber,
          4, 4);
    }
  }
  
  // get the initial transpose from the staff if any
  {
    S_msrTranspose
      transpose =
        fStaffPartUplink->getPartTranspose ();
        
    if (transpose) {
      fStaffTranspose = transpose;
      appendTransposeToAllStaffVoices (transpose);
    }
  }
}

msrStaff::~msrStaff()
{}

S_msrStaff msrStaff::createStaffBareClone (S_msrPart clonedPart)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of staff \"" <<
      getStaffName () <<
      "\"" <<
      endl;

  S_msrStaff
    clone =
      msrStaff::create (
        fInputLineNumber,
        fStaffNumber,
        clonedPart);

  clone->fStaffKind = fStaffKind;
  
  return clone;
}

string msrStaff::getStaffName () const
  {
  return
    fStaffNumber == 0
      ?
        fStaffPartUplink->getPartMSRName () +
        "_S_" +
        "(MASTER)"
      :
        fStaffPartUplink->getPartMSRName () +
        "_S_" +
        int2EnglishWord (fStaffNumber);
  }

void msrStaff::setStaffDivisionsPerWholeNote (
  int divisionsPerWholeNote)
{
  fStaffDivisionsPerWholeNote =
    divisionsPerWholeNote;

  setAllStaffVoicesDivisionsPerWholeNote (
    divisionsPerWholeNote);
}

void msrStaff::setAllStaffVoicesDivisionsPerWholeNote (int divisions)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->setVoiceDivisionsPerWholeNote (divisions);
  } // for
}

void msrStaff::setStaffMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> setStaffMeasureNumber(), " <<
      ", line " << inputLineNumber <<
      ", measureNumber = " << measureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      endl;

  // set part measure location
  fStaffMeasureNumber = measureNumber;

  // propagate it to all staves
  setAllStaffVoicesMeasureNumber (
    inputLineNumber, measureNumber);  
}

S_msrVoice msrStaff::addVoiceToStaffByItsRelativeNumber (
  int inputLineNumber,
  int voiceRelativeNumber)
{
  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        inputLineNumber,
        voiceRelativeNumber,
        this);

  // add it to this staff
  fStaffVoicesMap [voiceRelativeNumber] = voice;
  
  // return the voice
  return voice;
}

S_msrVoice msrStaff::registerVoiceInStaffByItsExternalNumber (
  int inputLineNumber,
  int externalVoiceNumber)
{
  // take this new voice into account
  fRegisteredVoicesCounter++;

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Registering voice " << externalVoiceNumber <<
       " as relative voice " << fRegisteredVoicesCounter <<
     " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
 // JMI     " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // are there too many voices in this staff? 
  if (fRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " << msrStaff::gMaxStaffVoices <<
      " voices, voice " << externalVoiceNumber << " overflows it" <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str());
  }

  // fetch the voice
  S_msrVoice
    voice =
      fStaffVoicesMap [fRegisteredVoicesCounter];
      
  // update it's voice number
  voice->
    setExternalVoiceNumber (externalVoiceNumber);

  // register it by its number
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Voice " << externalVoiceNumber <<
      " in staff " << getStaffName () <<
      " gets staff relative number " << fRegisteredVoicesCounter <<
      endl;
      
  fStaffVoicesCorrespondanceMap [externalVoiceNumber] =
    voice;

  return voice;
}

S_msrVoice msrStaff::fetchVoiceFromStaff (
  int inputLineNumber, int externalVoiceNumber)
{
  S_msrVoice result;

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> fetching external voice number " << externalVoiceNumber <<
     " in staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesCorrespondanceMap.begin();
    i != fStaffVoicesCorrespondanceMap.end();
    i++) {
    if (
      (*i).second->getExternalVoiceNumber ()
        ==
      externalVoiceNumber  ) {
      if (gGeneralOptions->fDebug) {
        cerr << idtr <<
          "--> voice " << externalVoiceNumber <<
          " in staff " << getStaffName () <<
          " has staff relative number " << (*i).first <<
          endl;
      }
        
      result = (*i).second;
      break;
    }
  } // for

  return result;
}

void msrStaff::registerVoiceInStaff (
  int inputLineNumber, S_msrVoice voice)
{
  // take this new voice into account
  fRegisteredVoicesCounter++;

  // are there too many voices in this staff? 
  if (fRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " << msrStaff::gMaxStaffVoices <<
      " voices, voice " << voice->getVoiceName () << " overflows it" <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str());
  }

  // register voice in this staff
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " << fRegisteredVoicesCounter <<
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // register is by its relative number
  fStaffVoicesMap [fRegisteredVoicesCounter] = voice;

  // register it by its number
  fStaffVoicesCorrespondanceMap [voice->getExternalVoiceNumber ()] =
    voice;
}

void msrStaff::setStaffClef (S_msrClef clef)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting staff clef '" << clef->clefAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff clef
  fStaffClef = clef;

  // is this a tablature or percussion staff?
  if (clef->clefIsATablatureClef ())
    fStaffKind = kTablatureStaff;
  else if (clef->clefIsAPercussionClef ())
    fStaffKind = kPercussionStaff;
  
  // propagate clef to all voices
  appendClefToAllStaffVoices (clef);
}

void msrStaff::setStaffKey  (S_msrKey  key)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting key '" << key->keyAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff key
  fStaffKey = key;

  // propagate it to all voices
  appendKeyToAllStaffVoices (key);
}

void msrStaff::setStaffTime (S_msrTime time)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting time '" << time->timeAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff time
  fStaffTime = time;

  // propagate it to all voices
  appendTimeToAllStaffVoices (time);
}

void msrStaff::setStaffTranspose (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting transpose '" << transpose->transposeAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff transpose
  fStaffTranspose = transpose;

  // propagate it to all voices
  appendTransposeToAllStaffVoices (transpose);
}

void msrStaff::appendClefToAllStaffVoices (S_msrClef clef)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendClefToVoice (clef);
  } // for
}

void msrStaff::appendKeyToAllStaffVoices (S_msrKey  key)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendKeyToVoice (key);
  } // for
}

void msrStaff::appendTimeToAllStaffVoices (S_msrTime time)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendTimeToVoice (time);
  } // for
}

void msrStaff::appendTransposeToAllStaffVoices (S_msrTranspose transpose)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendTransposeToVoice (transpose);
  } // for
}

void msrStaff::setAllStaffVoicesMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> setAllStaffVoicesMeasureNumber(), " <<
      ", line " << inputLineNumber <<
      ", measureNumber = " << measureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      endl;

  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->
      setVoiceMeasureNumber (
        inputLineNumber, measureNumber);
  } // for
}

void msrStaff::removeStaffEmptyVoices ()
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
 // JMI   if (! (*i).second->getVoiceActualNotesCounter ()) {
    if (! (*i).second->getMusicHasBeenInsertedInVoice ()) {
      fStaffVoicesMap.erase (i);
    }
  } // for
}

void msrStaff::finalizeLastMeasureOfStaff (int inputLineNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> finalizing last measure in staff " <<
      getStaffName () <<
      ", line " << inputLineNumber <<
      endl;

  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->finalizeLastMeasureOfVoice (inputLineNumber);
  } // for
}

void msrStaff::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStaff::acceptIn()" << endl;
      
  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStaff::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrStaff::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStaff::acceptOut()" << endl;

  if (visitor<S_msrStaff>*
    p =
      dynamic_cast<visitor<S_msrStaff>*> (v)) {
        S_msrStaff elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrStaff::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrStaff::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrStaff::browseData()" << endl;

  /*
    fStaffClef, fStaffKey and fStaffTime are used to populate
    newly created voices, not to create music proper:
    they're thus not browsed
  */

  if (fStafftuningsList.size ()) {
    for (
      list<S_msrStafftuning>::iterator i = fStafftuningsList.begin();
      i != fStafftuningsList.end();
      i++) {
      // browse the voice
      msrBrowser<msrStafftuning> browser (v);
      browser.browse (*(*i));
    } // for
    idtr--;
  }

  if (fStaffVoicesMap.size ()) {
    for (
      map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
      i != fStaffVoicesMap.end();
      i++) {
      if ((*i).second->getStaffRelativeVoiceNumber () != 0) {
        // browse the voice, but not if it is the master voice JMI
        msrBrowser<msrVoice> browser (v);
        browser.browse (*((*i).second));
      }
    } // for
  }

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "<== msrStaff::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print (os);
  return os;
}

string msrStaff::staffKindAsString () const
{
  string result;
  
  switch (fStaffKind) {
    case msrStaff::kRegularStaff:
      result = "regular";
      break;
    case msrStaff::kTablatureStaff:
      result = "tablature";
      break;
    case msrStaff::kPercussionStaff:
      result = "percussion";
      break;
  } // switch

  return result;
}

void msrStaff::print (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    " (" <<
    singularOrPlural (
      fStaffVoicesMap.size(), "voice", "voices") <<
    ")" <<
    endl;

  idtr++;

  if (fStaffClef)
    os << idtr << fStaffClef;
  else
    os << idtr << "NO_CLEF" << endl;

  if (fStaffKey)
    os << idtr << fStaffKey;
  else
    os << idtr << "NO_KEY" << endl;

  if (fStaffTime)
    os << idtr << fStaffTime;
  else
    os << idtr << "NO_TIME" << endl;

  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;

  if (fStafftuningsList.size ()) {
    os <<
      idtr << "Staff tunings:" <<
      endl;
      
    list<S_msrStafftuning>::const_iterator
      iBegin = fStafftuningsList.begin(),
      iEnd   = fStafftuningsList.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr <<
        (*i)->stafftuningAsString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    idtr--;
  }

  os << endl;

  // print the registered voices
  if (fStaffVoicesMap.size ()) {
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoicesMap.begin(),
      iEnd   = fStaffVoicesMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      os << idtr <<
        (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  idtr--;
}

void msrStaff::printStructure (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    " (" <<
    singularOrPlural (
      fStaffVoicesMap.size(), "voice", "voices") <<
    ")" <<
    endl;

  idtr++;

  if (fStaffClef)
    os << idtr << fStaffClef;
  else
    os << idtr << "NO_CLEF" << endl;

  if (fStaffKey)
    os << idtr << fStaffKey;
  else
    os << idtr << "NO_KEY" << endl;

  if (fStaffTime)
    os << idtr << fStaffTime;
  else
    os << idtr << "NO_TIME" << endl;

  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;


  if (fStafftuningsList.size ()) {
    os <<
      idtr << "Staff tunings:" <<
      endl;
      
    list<S_msrStafftuning>::const_iterator
      iBegin = fStafftuningsList.begin(),
      iEnd   = fStafftuningsList.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr << (*i)->stafftuningAsString ();
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
    idtr--;
  }

  // print the registered voices names
  if (fStaffVoicesMap.size ()) {
    os << idtr <<
      "Voices:" <<
      endl;
  
    idtr++;
    
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoicesMap.begin(),
      iEnd   = fStaffVoicesMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrVoice
        voice =
          (*i).second;
          
      os << idtr << left <<
        setw(30) << voice->getVoiceName () <<
        " (" <<
        singularOrPlural (
          voice->getVoiceActualNotesCounter (),
          "actual note",
          "actual notes") <<
        ", " <<
        singularOrPlural (
          voice->getVoiceLyricsMap ().size(),
          "lyric",
          "lyrics") <<
        ")";
      if (++i == iEnd) break;
      os << endl;
    } // for

    idtr --;
  }

  idtr--;
}

//______________________________________________________________________________ 
S_msrPart msrPart::create (
  int            inputLineNumber,
  string         partID,
  S_msrPartgroup partPartgroupUplink)
{
  msrPart* o =
    new msrPart (
      inputLineNumber, partID, partPartgroupUplink);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  int            inputLineNumber,
  string         partID,
  S_msrPartgroup partPartgroupUplink)
    : msrElement (inputLineNumber)
{
  // replace spaces in part ID
  for_each (
    partID.begin(),
    partID.end(),
    stringSpaceReplacer (fPartID, '_'));
 
  fPartPartgroupUplink = partPartgroupUplink;

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      gMsrOptions->fPartsRenaming.find (fPartID);
        
  if (it != gMsrOptions->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMSRName = (*it).second;
  }
  else {
    // coin the name from the argument
    fPartMSRName =
      "P_"+stringNumbersToEnglishWords (fPartID);
  }
    
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating part " << getPartCombinedName () << endl;

  fPartDivisionsPerWholeNote = 0;

  fMeasureZeroHasBeenMetInPart = false;
  
  fPartMeasurePositionHighTide = 1;

/* JMI
  // create a first staff for the part
  this->
    addStaffToPart (
      inputLineNumber, 1);

  // create the part voice master
  S_msrStaff
    hiddenMasterStaff =
      msrStaff::create (
        0,            // inputLineNumber
        0,            // staffNumber
        this);        // fStaffPartUplink

  fPartVoicemaster =
    msrVoice::create (
      0,            // inputLineNumber
      0,            // staffRelativeVoiceNumber
      hiddenMasterStaff); // voiceStaffUplink
      *
*/
}

msrPart::~msrPart() {}

S_msrPart msrPart::createPartBareClone (S_msrPartgroup clonedPartgroup)
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of part " <<
      getPartCombinedName () <<
      endl;

  S_msrPart
    clone =
      msrPart::create (
        fInputLineNumber,
        fPartID,
        clonedPartgroup);

  clone->fPartName           = fPartName;
  clone->fPartAbbreviation   = fPartAbbreviation;
  clone->fPartInstrumentName = fPartInstrumentName;
  
  return clone;
}

void msrPart::updatePartMeasurePositionHighTide (
  int inputLineNumber,
  int measurePosition)
{
  if (measurePosition > fPartMeasurePositionHighTide)
    fPartMeasurePositionHighTide = measurePosition;
}

void msrPart::setPartMSRName (string partMSRName)
{
  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      gMsrOptions->fPartsRenaming.find (fPartMSRName);
        
  if (it != gMsrOptions->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMSRName = (*it).second;

    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMSRName << "\"" <<
         endl;
  }
  else {
    // use the argument
    fPartMSRName = partMSRName;

    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Keeping partID \"" << partMSRName <<
        "\" as part name  for " << getPartCombinedName () <<
      endl;
  }
}

string msrPart::getPartCombinedName () const
{
  return
    "\"" + fPartMSRName + "\"" + " (" + fPartID + ")";
}

void msrPart::setPartDivisionsPerWholeNote (
  int divisionsPerWholeNote)
{
  fPartDivisionsPerWholeNote =
    divisionsPerWholeNote;

  setAllPartStavesDivisionsPerWholeNote (
    divisionsPerWholeNote);
}

void msrPart::setPartMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr <<
      endl <<
      idtr <<
        "### --> setPartMeasureNumber()" <<
        ", line " << inputLineNumber <<
        ", measureNumber = " << measureNumber <<
        ", in part " << getPartCombinedName () <<
        endl << endl;

  // set part measure location
  fPartMeasureNumber = measureNumber;

  if (measureNumber == 0) {  
    fMeasureZeroHasBeenMetInPart = true;
  }

  // propagate it to all staves
  setAllPartStavesMeasureNumber (
    inputLineNumber, measureNumber);  
}

void msrPart::setPartClef (S_msrClef clef)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part clef \"" << clef->clefAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part clef
  fPartClef = clef;

  // propagate it to all staves
  setAllPartStavesClef (clef);
}

void msrPart::setPartKey  (S_msrKey  key)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part key \"" << key->keyAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part key
  fPartKey = key;

  // propagate it to all staves
  setAllPartStavesKey (key);
}

void msrPart::setPartTime (S_msrTime time)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part time \"" << time->timeAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part time
  fPartTime = time;

  // propagate it to all staves
  setAllPartStavesTime (time);
}

void msrPart::setPartTranspose (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part transpose \"" << transpose->transposeAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part transpose
  fPartTranspose = transpose;

  // propagate it to all staves
  setAllPartStavesTranspose (transpose);
}

void msrPart::setAllPartStavesDivisionsPerWholeNote (int divisions)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffDivisionsPerWholeNote (divisions);
  } // for
}

void msrPart::setAllPartStavesMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> setAllPartStavesMeasureNumber()" <<
      ", line " << inputLineNumber <<
      ", measureNumber = " << measureNumber <<
      ", in part " << getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      setStaffMeasureNumber (
        inputLineNumber, measureNumber);
  } // for
}

void msrPart::setAllPartStavesClef (S_msrClef clef)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffClef (clef);
  } // for
}

void msrPart::setAllPartStavesKey (S_msrKey  key)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffKey (key);
  } // for
}
          
void msrPart::setAllPartStavesTime  (S_msrTime time)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffTime (time);
  } // for
}
          
void msrPart::setAllPartStavesTranspose (S_msrTranspose transpose)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffTranspose (transpose);
  } // for
}

S_msrStaff msrPart::addStaffToPartByItsNumber (
  int inputLineNumber,
  int staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    cerr << idtr <<
      "### Internal error: staffNumber " << staffNumber <<
      " already exists in part " << " " << getPartCombinedName () <<
      endl;

    return fPartStavesMap [staffNumber];
  }

// JMI  if (gGeneralOptions->fForceDebug || gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      endl;
  
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        staffNumber,
        this);

  // register it in this part
  fPartStavesMap [staffNumber] = staff;

  // return it
  return staff;
}

void msrPart::addStaffToPart (S_msrStaff staff)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding staff \"" << staff->getStaffName () <<
      "\" to part " << getPartCombinedName () <<
      endl;

  // register staff in this part
  fPartStavesMap [staff->getStaffNumber ()] = staff;
}

S_msrStaff msrPart::fetchStaffFromPart (
  int staffNumber)
{
  S_msrStaff result;
  
  if (fPartStavesMap.count (staffNumber)) {
    result = fPartStavesMap [staffNumber];
  }

  return result;
}

void msrPart::appendHarmonyToPart (S_msrHarmony harmony)
{
  // JMI
}

void msrPart:: handleBackup (int divisions)
{
 // JMI 
}

void msrPart::removePartEmptyVoices ()
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->removeStaffEmptyVoices ();
  } // for
}

void msrPart::finalizeLastMeasureOfPart (int inputLineNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> finalizing last measure in part " <<
      getPartName () <<
      ", line " << inputLineNumber <<
      endl;

  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->finalizeLastMeasureOfStaff (inputLineNumber);
  } // for
}

void msrPart::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPart::acceptIn()" << endl;
      
  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPart::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPart::acceptOut()" << endl;

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPart::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPart::browseData()" << endl;
  
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    // browse the staff
    msrBrowser<msrStaff> browser (v);
    browser.browse (*((*i).second));
  } // for

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "<== msrPart::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print (os);
  return os;
}

void msrPart::print (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size(), "staff", "staves") <<
    ", position high tide " << fPartMeasurePositionHighTide <<
    ")" <<
    endl;
    
  idtr++;
  
  os << left <<
    idtr <<
      setw(25) << "PartDivisionsPerWholeNote" << ": " <<
      fPartDivisionsPerWholeNote << endl <<
    idtr <<
      setw(25) << "PartMSRName" << ": \"" <<
      fPartMSRName << "\"" << endl <<
    idtr <<
      setw(25) << "PartName" << ": \"" <<
      fPartName << "\"" << endl <<
    idtr <<
      setw(25) << "PartAbbrevation" << ": \"" <<
      fPartAbbreviation << "\"" << endl <<
    idtr <<
      setw(25) << "PartInstrumentName" << ": \"" <<
      fPartInstrumentName << "\"" << endl;

  if (fPartStavesMap.size()) {
    os << endl;
    for (
      map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
      i != fPartStavesMap.end();
      i++) {
      os <<
        idtr << (*i).second <<
        endl;
    } // for
  }

  idtr--;
}

void msrPart::printStructure (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" <<
    singularOrPlural (
      fPartStavesMap.size(), "staff", "staves") <<
    ", position high tide " << fPartMeasurePositionHighTide <<
    ")" <<
    endl;
    
  idtr++;
  
  os << left <<
    idtr <<
      setw(25) << "PartDivisionsPerWholeNote" << ": " <<
      fPartDivisionsPerWholeNote << endl <<
    idtr <<
      setw(25) << "PartMSRName" << ": \"" <<
      fPartMSRName << "\"" << endl <<
    idtr <<
      setw(25) << "PartName" << ": \"" <<
      fPartName << "\"" << endl <<
    idtr <<
      setw(25) << "PartAbbrevation" << ": \"" <<
      fPartAbbreviation << "\"" << endl <<
    idtr <<
      setw(25) << "PartInstrumentName" << ": \"" <<
      fPartInstrumentName << "\"" << endl;

  if (fPartStavesMap.size()) {
    os << endl;
    for (
      map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
      i != fPartStavesMap.end();
      i++) {
      os <<
        idtr;
        (*i).second->printStructure (os);
        os <<
          endl;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
int msrPartgroup::gPartgroupsCounter = 0;

S_msrPartgroup msrPartgroup::create (
  int                    inputLineNumber,
  int                    partgroupNumber,
  string                 partgroupName,
  string                 partgroupAbbreviation,
  msrPartgroupSymbolKind partgroupSymbolKind,
  int                    partgroupSymbolDefaultX,
  bool                   partgroupBarline,
  S_msrPartgroup         partgroupPartgroupUplink)
{
  msrPartgroup* o =
    new msrPartgroup (
      inputLineNumber,
      partgroupNumber,
      partgroupName,
      partgroupAbbreviation,
      partgroupSymbolKind,
      partgroupSymbolDefaultX,
      partgroupBarline,
      partgroupPartgroupUplink);
  assert(o!=0);
  return o;
}

msrPartgroup::msrPartgroup (
  int                    inputLineNumber,
  int                    partgroupNumber,
  string                 partgroupName,
  string                 partgroupAbbreviation,
  msrPartgroupSymbolKind partgroupSymbolKind,
  int                    partgroupSymbolDefaultX,
  bool                   partgroupBarline,
  S_msrPartgroup         partgroupPartgroupUplink)
    : msrElement (inputLineNumber)
{
  fPartgroupAbsoluteNumber = ++gPartgroupsCounter;
  
  fPartgroupNumber = partgroupNumber;
        
  fPartgroupName = partgroupName;
  fPartgroupAbbreviation = partgroupAbbreviation;

  fPartgroupSymbolKind = partgroupSymbolKind;
  fPartgroupSymbolDefaultX = partgroupSymbolDefaultX;

  fPartgroupBarline = partgroupBarline;

  fPartgroupPartgroupUplink = partgroupPartgroupUplink;
  
  if (gGeneralOptions->fTrace)
    cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Creating part group " << fPartgroupNumber <<
      endl;
}

msrPartgroup::~msrPartgroup()
{}

S_msrPartgroup msrPartgroup::createPartgroupBareClone (
  S_msrPartgroup clonedPartgroup)
{
  if (gGeneralOptions->fTrace)
    cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Creating a bare clone part group " <<
      getPartgroupCombinedName () <<
      endl;

  S_msrPartgroup
    clone =
      msrPartgroup::create (
        fInputLineNumber,
        fPartgroupNumber,
        fPartgroupName,
        fPartgroupAbbreviation,
        fPartgroupSymbolKind,
        fPartgroupSymbolDefaultX,
        fPartgroupBarline,
        clonedPartgroup);

  // avoid part group clone to keep its (new) absolute number
  clone->fPartgroupAbsoluteNumber =
    fPartgroupAbsoluteNumber;
  
  return clone;
}

string msrPartgroup::getPartgroupCombinedName () const
{
  stringstream s;

  s <<
    "PG_" << fPartgroupAbsoluteNumber <<
    " (" << fPartgroupNumber << ")";

  return s.str();
}

S_msrPart msrPartgroup::addPartToPartgroupByItsID (
  int    inputLineNumber,
  string partID)
{
  if (fPartgroupPartsMap.count (partID)) {
    stringstream s;

    s <<
      "partID \"" << partID <<
      "\" already exists in part group " <<
      getPartgroupCombinedName ();

    msrMusicXMLWarning ( // JMI
      inputLineNumber,
      s.str ());

    return fPartgroupPartsMap [partID];
  }

  // create the part
  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        partID,
        this);

  // register it in this part group
  if (gGeneralOptions->fTrace) {
    cerr << idtr <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartgroupNumber <<
      endl;
  }
  
  fPartgroupPartsMap [partID] = part;
  fPartgroupElements.push_back (part);

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
    cerr <<
    endl <<
    idtr <<
      "==> After addPartToPartgroupByItsID, fPartgroupPartsMap contains:" << endl;
    idtr++;
    for (
        map<string, S_msrPart>::const_iterator i = fPartgroupPartsMap.begin();
        i != fPartgroupPartsMap.end();
        i++) {
      cerr << idtr <<
        "\"" << (*i).first << "\" ----> " <<
        (*i).second->getPartCombinedName() <<
        endl;
    } // for
    idtr--;
    cerr << idtr <<
      "<== addPartToPartgroup" <<
      endl << endl <<

    idtr <<
      "==> After addPartToPartgroupByItsID, fPartgroupPartsList contains:" <<
      endl;
    if (fPartgroupElements.size()) {
      list<S_msrElement>::const_iterator
        iBegin = fPartgroupElements.begin(),
        iEnd   = fPartgroupElements.end(),
        i      = iBegin;
        
      idtr++;
      for ( ; ; ) {
        cerr << idtr << (*i);
        if (++i == iEnd) break;
        cerr << endl;
      } // for
      idtr--;
    }
    cerr <<
      idtr << "<== addPartToPartgroupByItsID" <<
      endl << endl;
  }
  
  // return the part
  return part;
}

void msrPartgroup::addPartToPartgroup (S_msrPart part)
{
  // register part in this part group
  if (gGeneralOptions->fTrace) {
    cerr << idtr <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartgroupNumber <<
      endl;
  }
  
  fPartgroupPartsMap [part->getPartID ()] = part;
  fPartgroupElements.push_back (part);
}

void msrPartgroup::prependSubPartgroupToPartgroup (
  S_msrPartgroup partgroup)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding (sub-)part group " << partgroup->getPartgroupNumber () <<
      " to part group " << getPartgroupNumber ()  << endl;

  // register it in this part group
  fPartgroupElements.push_front (partgroup);
}

S_msrPart msrPartgroup::fetchPartFromPartgroup (
  string partID)
{
  if (gGeneralOptions->fDebugDebug) {
    cerr << idtr <<
      "==> fetchPartFromPartgroup, fPartgroupPartsMap contains:" << endl;
    for (
        map<string, S_msrPart>::const_iterator i = fPartgroupPartsMap.begin();
        i != fPartgroupPartsMap.end();
        i++) {
      cerr << idtr <<
        (*i).first << " ----> " <<
        (*i).second->getPartCombinedName() <<
        endl;
    } // for
    cerr << idtr <<
      "<== fetchPartFromPartgroup" <<
      endl;
  }
  
  S_msrPart result;
  
  if (fPartgroupPartsMap.count (partID)) {
    result = fPartgroupPartsMap [partID];
  }

  return result;
}

void msrPartgroup::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPartgroup::acceptIn()" << endl;
      
  if (visitor<S_msrPartgroup>*
    p =
      dynamic_cast<visitor<S_msrPartgroup>*> (v)) {
        S_msrPartgroup elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPartgroup::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrPartgroup::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPartgroup::acceptOut()" << endl;

  if (visitor<S_msrPartgroup>*
    p =
      dynamic_cast<visitor<S_msrPartgroup>*> (v)) {
        S_msrPartgroup elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrPartgroup::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrPartgroup::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrPartgroup::browseData()" << endl;
  
  for (
    list<S_msrElement>::iterator i = fPartgroupElements.begin();
    i != fPartgroupElements.end();
    i++) {
    // browse the part element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "<== msrPartgroup::browseData()" << endl;
}

ostream& operator<< (ostream& os, const S_msrPartgroup& elt)
{
  elt->print (os);
  return os;
}

string msrPartgroup::pargroupSymbolKindAsString (
  msrPartgroupSymbolKind partgroupSymbolKind)
{
  string result;
  
  switch (partgroupSymbolKind) {
    case k_NoPartgroupSymbol:
      result = "none";
      break;
    case kBracePartgroupSymbol:
      result = "brace";
      break;
    case kBracketPartgroupSymbol:
      result = "bracket";
      break;
    case kLinePartgroupSymbol:
      result = "line";
      break;
    case kSquarePartgroupSymbol:
      result = "square";
      break;
  } // switch

  return result;
}

void msrPartgroup::print (ostream& os)
{
  os <<
    "Partgroup" " \"" << getPartgroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartgroupPartsMap.size(), "part", "parts") <<
    ")" <<
    endl;
    
  idtr++;

  os << left <<
    idtr <<
      setw(24) << "PartgroupName" << " : \"" <<
      fPartgroupName <<
      "\"" <<
      endl <<
    idtr <<
      setw(24) << "PartgroupAbbrevation" << " : \"" <<
      fPartgroupAbbreviation <<
      "\"" <<
      endl <<
    idtr <<
      setw(24) << "fPartgroupSymbolDefaultX" << " : " <<
      fPartgroupSymbolDefaultX <<
        endl <<
    idtr <<
      setw(24) << "fPartgroupSymbolKind" << " : \"" <<
      pargroupSymbolKindAsString (fPartgroupSymbolKind) <<
      "\"" <<
      endl;
    
  os <<
    idtr << "PartgroupBarline         : ";
  if (fPartgroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  if (fPartgroupElements.size()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartgroupElements.begin(),
      iEnd   = fPartgroupElements.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    idtr--;
  }
  
  idtr--;
}

void msrPartgroup::printStructure (ostream& os)
{
  os <<
    "Partgroup" " \"" << getPartgroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartgroupPartsMap.size(), "part", "parts") <<
    ")" <<
    endl;
    
  idtr++;

  os << left <<
    idtr <<
      setw(24) << "PartgroupName" << " : \"" <<
      fPartgroupName <<
      "\"" <<
      endl <<
    idtr <<
      setw(24) << "PartgroupAbbrevation" << " : \"" <<
      fPartgroupAbbreviation <<
      "\"" <<
      endl <<
    idtr <<
      setw(24) << "fPartgroupSymbolDefaultX" << " : " <<
      fPartgroupSymbolDefaultX <<
        endl <<
    idtr <<
      setw(24) << "fPartgroupSymbolKind" << " : \"" <<
      pargroupSymbolKindAsString (fPartgroupSymbolKind) <<
      "\"" <<
      endl;
    
  os <<
    idtr << "PartgroupBarline         : ";
  if (fPartgroupBarline)
    os << "true";
  else
    os << "false";
  os << endl;

  if (fPartgroupElements.size()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartgroupElements.begin(),
      iEnd   = fPartgroupElements.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      os << idtr;
      (*i)->printStructure (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    idtr--;
  }
  
  idtr--;
}

//______________________________________________________________________________
S_msrIdentification msrIdentification::create (
  int           inputLineNumber)
{
  msrIdentification* o =
    new msrIdentification (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrIdentification::msrIdentification (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrIdentification::~msrIdentification() {}

void msrIdentification::setWorkNumber (
  int    inputLineNumber,
  string val)
  {
  fWorkNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      "work-number", val);
  }

void msrIdentification::setWorkTitle (
  int    inputLineNumber,
  string val)
  {
  fWorkTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      "work-title", val);
  }

void msrIdentification::setMovementNumber (
  int    inputLineNumber,
  string val)
  {
  fMovementNumber =
    msrVarValAssoc::create (
      inputLineNumber,
      "movement-number", val);
  }

void msrIdentification::setMovementTitle (
  int    inputLineNumber,
  string val)
{
  fMovementTitle =
    msrVarValAssoc::create (
      inputLineNumber,
      "movement-title", val);
}

void msrIdentification::addCreator (
  int    inputLineNumber,
  string type,
  string val)
{
  fCreators.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      type, val)
  );
}

void msrIdentification::setRights (
  int    inputLineNumber,
  string val)
  {
  fRights =
    msrVarValAssoc::create (
      inputLineNumber,
      "rights", val);
  }

void msrIdentification::addSoftware (
  int    inputLineNumber,
  string val)
{
  fSoftwares.push_back(
    msrVarValAssoc::create (
      inputLineNumber,
      "software", val)
  );
}

void msrIdentification::setEncodingDate (
  int    inputLineNumber,
  string val)
{
  fEncodingDate =
    msrVarValAssoc::create (
      inputLineNumber,
      "encoding-date", val);
}

void msrIdentification::setScoreInstrumentAssoc (
  int    inputLineNumber,
  string val)
{
  fScoreInstrumentAssoc =
    msrVarValAssoc::create (
      inputLineNumber,
      "score-instrument", val);
}

void msrIdentification::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrIdentification::acceptIn()" << endl;
      
  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrIdentification::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrIdentification::acceptOut()" << endl;

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrIdentification::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrIdentification::browseData (basevisitor* v)
{
  if (fWorkNumber) {
    // browse fWorkNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkNumber);
  }

  if (fWorkTitle) {
    // browse fWorkTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fWorkTitle);
  }

  if (fMovementNumber) {
    // browse fMovementNumber
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementNumber);
  }

  if (fMovementTitle) {
    // fMovementTitle fMovementTitle
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fCreators.begin(); i!=fCreators.end(); i++) {
      // browse creator
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }
    
  if (fRights) {
    // browse rights
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i;
    for (i=fSoftwares.begin(); i!=fSoftwares.end(); i++) {
      // browse software
      msrBrowser<msrVarValAssoc> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fEncodingDate) {
    // browse encoding date
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fScoreInstrumentAssoc) {
    // browse score instrument
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fScoreInstrumentAssoc);
  }
}

ostream& operator<< (ostream& os, const S_msrIdentification& elt)
{
  elt->print (os);
  return os;
}

void msrIdentification::print (ostream& os)
{
  os << "Identification" << endl;

  bool emptyIdentification = true;

  idtr++;
  
  if (fWorkNumber) {
    os << idtr << fWorkNumber;
    emptyIdentification = false;
  }
  
  if (fWorkTitle) {
    os << idtr << fWorkTitle;
    emptyIdentification = false;
  }
    
  if (fMovementNumber) {
    os << idtr << fMovementNumber;
    emptyIdentification = false;
  }
    
  if (fMovementTitle) {
    os << idtr << fMovementTitle;
    emptyIdentification = false;
  }
    // JMI for   ; ; 
  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      os << idtr << (*i1);
    } // for
    emptyIdentification = false;
  }
    
  if (fRights) {
    os << idtr << fRights;
    emptyIdentification = false;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      os << idtr << (*i2);
    } // for
    emptyIdentification = false;
  }
    
  if (fEncodingDate) {
    os << idtr << fEncodingDate;
    emptyIdentification = false;
  }

  if (emptyIdentification)
    os << idtr <<
      " " << "nothing specified" << endl;

  os << endl;
  
  idtr--;
}

//______________________________________________________________________________
S_msrScore msrScore::create (
  int           inputLineNumber)
{
  msrScore* o =
    new msrScore (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  int           inputLineNumber)
    : msrElement (inputLineNumber)
{
  fIdentification =
    msrIdentification::create (
      inputLineNumber);
      
  fPageGeometry =
    msrPageGeometry::create (
      inputLineNumber);
}

msrScore::~msrScore() {}

S_msrScore msrScore::createScoreBareClone ()
{
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> Creating a bare clone of a score" <<
      endl;

  S_msrScore
    clone =
      msrScore::create (
        fInputLineNumber);

  return clone;
}

void msrScore::addPartgroupToScore (S_msrPartgroup partgroup)
{
  /* JMI
  if (fScorePartgroupsMap.count (partgroupNumber)) {
    cerr << idtr <<
      "### Internal error: part group " << partgroupNumber <<
      " already exists in this score" << endl;

    return fScorePartgroupsMap [partgroupNumber];
  }
*/

  // register it in this score
  fPartgroupsList.push_back (partgroup);
}

/*
S_msrPartgroup msrScore::fetchScorePartgroup (
  int partgroupNumber)
{
  S_msrPartgroup result;
  
  if (fScorePartgroupsMap.count (partgroupNumber)) {
    result = fScorePartgroupsMap [partgroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrScore::acceptIn()" << endl;
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrScore::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrScore::acceptOut()" << endl;

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrScore::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrScore::browseData()" << endl;
    
  if (fIdentification) {
    // browse identification
    msrBrowser<msrIdentification> browser (v);
    browser.browse (*fIdentification);
  }

  if (fPageGeometry) {
    // browse page geometry
    msrBrowser<msrPageGeometry> browser (v);
    browser.browse (*fPageGeometry);
  }

  for (
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    // browse the part group
    msrBrowser<msrPartgroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "<== msrScore::browseData()" << endl;
  }

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  elt->print (os);
  return os;
}

void msrScore::print (ostream& os)
{
  os <<
    "MSR Score" <<
    " (" << fPartgroupsList.size() << " part groups)" <<
    endl << endl;

  idtr++;
  
  if (fIdentification) {
    os << idtr << fIdentification;
  }
  
  if (fPageGeometry) {
    os << idtr << fPageGeometry;
  }
  
  for (
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    os << idtr << (*i);
  } // for
  
  idtr--;
}

void msrScore::printStructure (ostream& os)
{
  os <<
    "MSR structure" <<
    " (" << fPartgroupsList.size() << " part groups)" <<
    endl << endl;

  idtr++;
  
  if (fIdentification) {
    os << idtr << fIdentification;
  }
  
  if (fPageGeometry) {
    os << idtr << fPageGeometry;
  }
  
  for (
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    os << idtr;
    (*i)->printStructure (os);
  } // for
  
  idtr--;
}

//______________________________________________________________________________
S_msrMidi msrMidi::create (
  int                    inputLineNumber)
{
  msrMidi* o =
    new msrMidi (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrMidi::msrMidi (
  int                    inputLineNumber)
    : msrElement (inputLineNumber)
{
}
msrMidi::~msrMidi() {}

void msrMidi::acceptIn (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrMidi::acceptIn()" << endl;
      
  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
        
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrMidi::visitStart()" << endl;
        p->visitStart (elem);
  }
}

void msrMidi::acceptOut (basevisitor* v) {
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "==> msrMidi::acceptOut()" << endl;

  if (visitor<S_msrMidi>*
    p =
      dynamic_cast<visitor<S_msrMidi>*> (v)) {
        S_msrMidi elem = this;
      
        if (gGeneralOptions->fDebugDebug)
          cerr << idtr <<
            "==> Launching msrMidi::visitEnd()" << endl;
        p->visitEnd (elem);
  }
}


void msrMidi::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrMidi& elt)
{
  elt->print (os);
  return os;
}

void msrMidi::print (ostream& os)
{
  os << "Midi" << endl;

  idtr++;
  
  os << idtr << "% to be completed" << endl;
  
  idtr--;
}


}

/*
  switch (lyricschunkKind) {
    case msrLyricschunk::kSingleChunk:
    case msrLyricschunk::kBeginChunk:
      {  
      // add lyrics chunk to this lyrics
      fLyricschunks.push_back (chunk);
      }
      break;

    case msrLyricschunk::kMiddleChunk:
    case msrLyricschunk::kEndChunk:
      // add chunk to this lyrics
      fLyricschunks.push_back (chunk);
      break;
      
    case msrLyricschunk::kSkipChunk:
    case msrLyricschunk::kSlurChunk:
    case msrLyricschunk::kSlurBeyondEndChunk:
    case msrLyricschunk::kTiedChunk:
    case msrLyricschunk::kBarcheckChunk:
    case msrLyricschunk::kBreakChunk:
      {
        msrInternalError (
          fInputLineNumber,
          "a text chunk type can only be "
          "'single', 'begin', 'middle' or 'end'");
      }
      break;
      
    case msrLyricschunk::k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch


*/


/*
  switch (lyricschunkKind) {
    case msrLyricschunk::kSingleChunk:
    case msrLyricschunk::kBeginChunk:
      {  
      // add lyrics chunk to this lyrics
      fLyricschunks.push_back (lyricschunk);
      }
      break;

    case msrLyricschunk::kMiddleChunk:
    case msrLyricschunk::kEndChunk:
      // add chunk to this lyrics
      fLyricschunks.push_back (lyricschunk);
      break;
      
    case msrLyricschunk::kSkipChunk:
    case msrLyricschunk::kSlurChunk:
    case msrLyricschunk::kSlurBeyondEndChunk:
    case msrLyricschunk::kTiedChunk:
    case msrLyricschunk::kBarcheckChunk:
    case msrLyricschunk::kBreakChunk:
      {
        msrInternalError (
          fInputLineNumber,
          "a text chunk type can only be "
          "'single', 'begin', 'middle' or 'end'");
      }
      break;
      
    case msrLyricschunk::k_NoChunk:
      msrInternalError (
        fInputLineNumber,
        "lyrics chunk type has not been set");
      break;
  } // switch

  fLyricsTextPresent = true;
  */

  /*
    switch (fNoteMsrPitch) {
      
      case k_aeseh: s << "aeseh"; break;
      case k_aes:   s << "aes"; break;
      case k_aeh:   s << "aeh"; break;
      case k_a:     s << "a"; break;
      case k_aih:   s << "aih"; break;
      case k_ais:   s << "ais"; break;
      case k_aisih: s << "aisih"; break;
        
      case k_beseh: s << "beseh"; break;
      case k_bes:   s << "bes"; break;
      case k_beh:   s << "beh"; break;
      case k_b:     s << "b"; break;
      case k_bih:   s << "bih"; break;
      case k_bis:   s << "bis"; break;
      case k_bisih: s << "bisih"; break;
        
      case k_ceseh: s << "ceseh"; break;
      case k_ces:   s << "ces"; break;
      case k_ceh:   s << "ceh"; break;
      case k_c:     s << "c"; break;
      case k_cih:   s << "cih"; break;
      case k_cis:   s << "cis"; break;
      case k_cisih: s << "cisih"; break;
        
      case k_deseh: s << "deseh"; break;
      case k_des:   s << "des"; break;
      case k_deh:   s << "deh"; break;
      case k_d:     s << "d"; break;
      case k_dih:   s << "dih"; break;
      case k_dis:   s << "dis"; break;
      case k_disih: s << "disih"; break;
  
      case k_eeseh: s << "eeseh"; break;
      case k_ees:   s << "ees"; break;
      case k_eeh:   s << "eeh"; break;
      case k_e:     s << "e"; break;
      case k_eih:   s << "eih"; break;
      case k_eis:   s << "eis"; break;
      case k_eisih: s << "eisih"; break;
        
      case k_feseh: s << "feseh"; break;
      case k_fes:   s << "fes"; break;
      case k_feh:   s << "feh"; break;
      case k_f:     s << "f"; break;
      case k_fih:   s << "fih"; break;
      case k_fis:   s << "fis"; break;
      case k_fisih: s << "fisih"; break;
        
      case k_geseh: s << "geseh"; break;
      case k_ges:   s << "ges"; break;
      case k_geh:   s << "geh"; break;
      case k_g:     s << "g"; break;
      case k_gih:   s << "gih"; break;
      case k_gis:   s << "gis"; break;
      case k_gisih: s << "gisih"; break;
      
      default: s << "Note" << fNoteMsrPitch << "???";
    } // switch
    */
