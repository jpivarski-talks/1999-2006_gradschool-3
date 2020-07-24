// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DualTrackToUsageTagProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor DualTrackToUsageTagProd:
//              creates a new DualTrackToUsageTagProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Mon Dec 15 14:41:36 EST 2003
// $Id: DualTrackToUsageTagProd_DONT_TOUCH.cc,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DualTrackToUsageTagProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "DualTrackToUsageTagProd/DualTrackToUsageTagProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackToUsageTagProd_DONT_TOUCH.cc,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $";
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
   return new DualTrackToUsageTagProd;
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
