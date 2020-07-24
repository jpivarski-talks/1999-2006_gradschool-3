// -*- C++ -*-
//
// Package:     <ZDHitCorrectorProd>
// Module:      ZDHitCorrectorProd_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor ZDHitCorrectorProd:
//              creates a new ZDHitCorrectorProd instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Guangshun Huang
// Created:     Thu Feb 27 2003
// $Id: ZDHitCorrectorProd_DONT_TOUCH.cc,v 1.1.1.1 2003/08/30 00:17:08 wsun Exp $
//
// Revision history
//
// $Log: ZDHitCorrectorProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2003/08/30 00:17:08  wsun
// Imported ZDHitCorrectorProd sources.
//
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "ZDHitCorrectorProd/ZDHitCorrectorProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ZDHitCorrectorProd_DONT_TOUCH.cc,v 1.1.1.1 2003/08/30 00:17:08 wsun Exp $";
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
   return new ZDHitCorrectorProd;
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
