// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor DualTrackProd:
//              creates a new DualTrackProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:04:40 EST 2003
// $Id: DualTrackProd_DONT_TOUCH.cc,v 1.1.1.1 2003/12/11 17:14:28 mccann Exp $
//
// Revision history
//
// $Log: DualTrackProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2003/12/11 17:14:28  mccann
// imported DualTrackProd sources
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "DualTrackProd/DualTrackProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackProd_DONT_TOUCH.cc,v 1.1.1.1 2003/12/11 17:14:28 mccann Exp $";
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
   return new DualTrackProd;
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
