// -*- C++ -*-
//
// Package:     <IRGeom>
// Module:      IRGeom_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Factory method to create processor IRGeom:
//              creates a new IRGeom instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Lawrence Gibbons
// Created:     Thu Jan  7 14:54:35 EST 1999
// $Id: IRGeom_DONT_TOUCH.cc,v 1.2 1999/07/22 21:57:10 lkg Exp $
//
// Revision history
//
// $Log: IRGeom_DONT_TOUCH.cc,v $
// Revision 1.2  1999/07/22 21:57:10  lkg
// sharemodule name change: IRGeom -> IRGeomProd
//
// Revision 1.1.1.1  1999/07/22 14:38:42  lkg
// First release
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "IRGeom/IRGeomProd.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: IRGeom_DONT_TOUCH.cc,v 1.2 1999/07/22 21:57:10 lkg Exp $";
static const char* const kTagString = "$Name: v01_01_02 $";

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
   return new IRGeomProd;
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
