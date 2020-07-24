// -*- C++ -*-
//
// Package:     FitterDiagnosticsProc
// Module:      FitterDiagnosticsProc_DONT_TOUCH2
// 
// Description: DONT TOUCH THIS FILE!
//
//              Factory method to create processor FitterDiagnosticsProc:
//              creates a new FitterDiagnosticsProc instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Werner Sun
// Created:     Wed Apr 21 16:57:08 EDT 1999
// $Id: FitterDiagnosticsProc_DONT_TOUCH2.cc,v 1.1.1.1 1999/06/07 21:26:37 wsun Exp $
//
// Revision history
//
// $Log: FitterDiagnosticsProc_DONT_TOUCH2.cc,v $
// Revision 1.1.1.1  1999/06/07 21:26:37  wsun
// First submission.
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             

// user include files
#include "FitterDiagnosticsProc/FitterDiagnosticsProc.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: FitterDiagnosticsProc_DONT_TOUCH2.cc,v 1.1.1.1 1999/06/07 21:26:37 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// function definitions
//

extern "C" {
   Processor* makeProcessor( void );
   const char* versionString( void );
}

Processor*
makeProcessor( void )
{
   return new FitterDiagnosticsProc;
}

const char*
versionString( void )
{
   return kTagString;
}

