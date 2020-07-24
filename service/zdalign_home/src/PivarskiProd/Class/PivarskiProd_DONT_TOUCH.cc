// -*- C++ -*-
//
// Package:     <PivarskiProd>
// Module:      PivarskiProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor PivarskiProd:
//              creates a new PivarskiProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Werner Sun
// Created:     Tue May 11 02:22:32 EDT 1999
// $Id: PivarskiProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/10 21:58:58 wsun Exp $
//
// Revision history
//
// $Log: PivarskiProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from PivarskiFilter
//
// Revision 1.1  1999/05/14 01:12:59  wsun
// First submission.
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "PivarskiProd/PivarskiProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: PivarskiProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/10 21:58:58 wsun Exp $";
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
   return new PivarskiProd;
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
