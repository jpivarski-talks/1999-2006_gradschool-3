// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavigationProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor NavigationProd:
//              creates a new NavigationProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Mon Jul 19 12:25:53 EDT 1999
// $Id: NavigationProd_DONT_TOUCH.cc,v 1.1.1.1 1999/07/19 16:47:46 bkh Exp $
//
// Revision history
//
// $Log: NavigationProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/07/19 16:47:46  bkh
// imported NavigationProd sources
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "NavigationProd/NavigationProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavigationProd_DONT_TOUCH.cc,v 1.1.1.1 1999/07/19 16:47:46 bkh Exp $";
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
   return new NavigationProd;
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
