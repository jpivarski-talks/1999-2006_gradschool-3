// -*- C++ -*-
//
// Package:     HighCotThetaTrackSplitting
// Module:      HighCotThetaTrackSplitting_DONT_TOUCH2
// 
// Description: DONT TOUCH THIS FILE!
//
//              Factory method to create processor HighCotThetaTrackSplitting:
//              creates a new HighCotThetaTrackSplitting instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Jim McCann
// Created:     Mon Feb 26 10:28:14 EST 2001
// $Id$
//
// Revision history
//
// $Log$
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             

// user include files
#include "HighCotThetaTrackSplitting/HighCotThetaTrackSplitting.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor_DONT_TOUCH2.cc,v 1.4 1998/12/01 21:11:59 mkl Exp $";
static const char* const kTagString = "$Name: v03_02_01 $";

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
   return new HighCotThetaTrackSplitting;
}

const char*
versionString( void )
{
   return kTagString;
}

