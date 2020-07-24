// -*- C++ -*-
//
// Package:     <ASiStorePro>
// Module:      ASiStorePro_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor ASiStorePro:
//              creates a new ASiStorePro instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      CLEO DAQ/sun/mfr
// Created:     Fri Jul  2 14:03:22 EDT 1999
// $Id: ASiStorePro_DONT_TOUCH.cc,v 1.2 1999/07/03 22:27:30 cleo3 Exp $
//
// Revision history
//
// $Log: ASiStorePro_DONT_TOUCH.cc,v $
// Revision 1.2  1999/07/03 22:27:30  cleo3
//  ASiSensorAddress is transferred from ASiSensor package, ASiStorePro becomes producer, all references to ASiSensor are eliminated
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "ASiStorePro/ASiStorePro.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ASiStorePro_DONT_TOUCH.cc,v 1.2 1999/07/03 22:27:30 cleo3 Exp $";
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
   return new ASiStorePro;
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
