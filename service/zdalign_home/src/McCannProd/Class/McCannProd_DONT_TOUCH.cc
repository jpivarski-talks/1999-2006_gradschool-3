// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor McCannProd:
//              creates a new McCannProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Werner Sun
// Created:     Tue May 11 02:22:32 EDT 1999
// $Id: McCannProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/10 21:58:58 wsun Exp $
//
// Revision history
//
// $Log: McCannProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from McCannFilter
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
#include "McCannProd/McCannProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/10 21:58:58 wsun Exp $";
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
   return new McCannProd;
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
