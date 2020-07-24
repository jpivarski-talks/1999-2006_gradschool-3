// -*- C++ -*-
//
// Package:     FitterDiagnosticsProc
// Module:      FitterDiagnosticsProc_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Definition of bind action
//
// Implementation:
//
// Author:      Werner Sun
// Created:     Wed Apr 21 16:57:08 EDT 1999
// $Id: FitterDiagnosticsProc_DONT_TOUCH.cc,v 1.1.1.1 1999/06/07 21:26:37 wsun Exp $
//
// Revision history
//
// $Log: FitterDiagnosticsProc_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/06/07 21:26:37  wsun
// First submission.
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "FitterDiagnosticsProc/FitterDiagnosticsProc.h"
#include "Processor/Action.h"                

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: FitterDiagnosticsProc_DONT_TOUCH.cc,v 1.1.1.1 1999/06/07 21:26:37 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// function definitions
//

//
// static data member definitions
//

//
// member functions
//
// ---------------- binding method to stream -------------------
void
FitterDiagnosticsProc::bind(
   ActionBase::ActionResult (FitterDiagnosticsProc::*iMethod)( Frame& ),
   const Stream::Type& iStream )
{
   bindAction( iStream, new Action<FitterDiagnosticsProc>( iMethod, this ) );
}

//
// const member functions
//

//
// static member functions
//
