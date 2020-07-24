// -*- C++ -*-
//
// Package:     <DRHitCorrectorProd>
// Module:      DRHitCorrectorProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor DRHitCorrectorProd:
//              creates a new DRHitCorrectorProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Ken McLean
// Created:     Wed Jul 28 16:40:55 EDT 1999
// $Id: DRHitCorrectorProd_DONT_TOUCH.cc,v 1.2 1999/08/21 14:37:04 mclean Exp $
//
// Revision history
//
// $Log: DRHitCorrectorProd_DONT_TOUCH.cc,v $
// Revision 1.2  1999/08/21 14:37:04  mclean
// Add corrections for flight time and signal propagation over sense wires.
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "DRHitCorrectorProd/DRHitCorrectorProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRHitCorrectorProd_DONT_TOUCH.cc,v 1.2 1999/08/21 14:37:04 mclean Exp $";
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
   return new DRHitCorrectorProd;
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
