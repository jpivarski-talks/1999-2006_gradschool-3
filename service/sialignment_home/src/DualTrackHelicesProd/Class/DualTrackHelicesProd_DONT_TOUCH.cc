// -*- C++ -*-
//
// Package:     <DualTrackHelicesProd>
// Module:      DualTrackHelicesProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor DualTrackHelicesProd:
//              creates a new DualTrackHelicesProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Lauren Hsu
// Created:     Fri Jul  7 14:15:27 EDT 2000
// $Id: DualTrackHelicesProd_DONT_TOUCH.cc,v 1.1.1.1 2001/02/01 17:40:13 llh14 Exp $
//
// Revision history
//
// $Log: DualTrackHelicesProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2001/02/01 17:40:13  llh14
// imported DualTrackHelicesProd
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "DualTrackHelicesProd/DualTrackHelicesProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackHelicesProd_DONT_TOUCH.cc,v 1.1.1.1 2001/02/01 17:40:13 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// function definitions
//

extern "C" {
   Producer* makeProducer( void );
   const char* versionString( void );
}

Producer*
makeProducer( void )
{
   return new DualTrackHelicesProd;
}

const char*
versionString( void )
{
   return kTagString;
}

//
// static data member definitions
//

//
// member functions
//

//
// const member functions
//

//
// static member functions
//
