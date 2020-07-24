// -*- C++ -*-
//
// Package:     <FakeATrackProd>
// Module:      FakeATrackProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor FakeATrackProd:
//              creates a new FakeATrackProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 13:31:29 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "FakeATrackProd/FakeATrackProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: producer_DONT_TOUCH.cc,v 1.6 1998/12/01 21:03:28 mkl Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

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
   return new FakeATrackProd;
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
