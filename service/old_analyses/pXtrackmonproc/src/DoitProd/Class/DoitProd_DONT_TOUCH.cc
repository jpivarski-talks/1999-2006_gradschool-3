// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor DoitProd:
//              creates a new DoitProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Adam Lyon
// Created:     Fri Jul 23 17:06:16 EDT 1999
// $Id: DoitProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/08 18:53:33 lyon Exp $
//
// Revision history
//
// $Log: DoitProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/09/08 18:53:33  lyon
// imported DoitProd sources
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "DoitProd/DoitProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/08 18:53:33 lyon Exp $";
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
   return new DoitProd;
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
