// -*- C++ -*-
//
// Package:     HistogramViewerProc
// Module:      HistogramViewerProc_DONT_TOUCH2
// 
// Description: DONT TOUCH THIS FILE!
//
//              Factory method to create processor HistogramViewerProc:
//              creates a new HistogramViewerProc instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Chris D Jones
// Created:     Fri Jul 20 09:24:37 EDT 2001
// $Id: HistogramViewerProc_DONT_TOUCH2.cc,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $
//
// Revision history
//
// $Log: HistogramViewerProc_DONT_TOUCH2.cc,v $
// Revision 1.1.1.1  2001/07/30 14:11:45  cdj
// importing
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             

// user include files
#include "HistogramViewerProc/HistogramViewerProc.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HistogramViewerProc_DONT_TOUCH2.cc,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $";
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
   return new HistogramViewerProc;
}

const char*
versionString( void )
{
   return kTagString;
}

