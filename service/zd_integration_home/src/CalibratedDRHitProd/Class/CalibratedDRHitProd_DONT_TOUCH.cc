// -*- C++ -*-
//
// Package:     <CalibratedDRHitProd>
// Module:      CalibratedDRHitProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor CalibratedDRHitProd:
//              creates a new CalibratedDRHitProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Chris D Jones
// Created:     Wed Sep 22 14:17:43 EDT 1999
// $Id: CalibratedDRHitProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/23 02:44:15 cdj Exp $
//
// Revision history
//
// $Log: CalibratedDRHitProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/09/23 02:44:15  cdj
// imported CalibratedDRHitProd sources
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "CalibratedDRHitProd/CalibratedDRHitProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CalibratedDRHitProd_DONT_TOUCH.cc,v 1.1.1.1 1999/09/23 02:44:15 cdj Exp $";
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
   return new CalibratedDRHitProd;
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
