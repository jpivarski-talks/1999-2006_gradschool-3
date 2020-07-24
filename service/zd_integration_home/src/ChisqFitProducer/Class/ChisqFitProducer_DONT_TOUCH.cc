// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitProducer_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor ChisqFitProducer:
//              creates a new ChisqFitProducer instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Werner Sun
// Created:     Sat Jun 12 00:06:11 EDT 1999
// $Id: ChisqFitProducer_DONT_TOUCH.cc,v 1.1.1.1 1999/06/16 02:48:27 wsun Exp $
//
// Revision history
//
// $Log: ChisqFitProducer_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/06/16 02:48:27  wsun
// First submission.
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "ChisqFitProducer/ChisqFitProducer.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqFitProducer_DONT_TOUCH.cc,v 1.1.1.1 1999/06/16 02:48:27 wsun Exp $";
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
   return new ChisqFitProducer;
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
