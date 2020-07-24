// -*- C++ -*-
//
// Package:     <CalibratedZDHitProd>
// Module:      CalibratedZDHitProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor CalibratedZDHitProd:
//              creates a new CalibratedZDHitProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Chris D Jones
// Created:     Wed Sep 22 14:17:43 EDT 1999
// $Id: CalibratedZDHitProd_DONT_TOUCH.cc,v 1.1.1.1 2002/09/27 20:34:03 bkh Exp $
//
// Revision history
//
// $Log: CalibratedZDHitProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2002/09/27 20:34:03  bkh
// imported CalibratedZDHitProd sources
//
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "CalibratedZDHitProd/CalibratedZDHitProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CalibratedZDHitProd_DONT_TOUCH.cc,v 1.1.1.1 2002/09/27 20:34:03 bkh Exp $";
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
   return new CalibratedZDHitProd;
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
