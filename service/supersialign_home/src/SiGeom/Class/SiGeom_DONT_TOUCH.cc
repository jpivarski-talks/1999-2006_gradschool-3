// -*- C++ -*-
//
// Package:     <package>
// Module:      SiGeom_DONT_TOUCH
// 
// Description: Miscellaneous methods which the user 
//              is not supposed to touch, 
//              but which need to be present 
//              in the shared processor library.
//
//              1.) Factory method to create processor SiGeom:
//                  creates a new SiGeom instance each time 
//                  it is called; it is used by Suez 
//                  to create a Processor after loading in 
//                  the code from a shared library.
//
// Implementation:
//
// Author:      Alexander Undrus
// Created:     Fri May 22 20:51:24 EDT 1998
// $Id: SiGeom_DONT_TOUCH.cc,v 1.1.1.1 1998/08/18 07:42:04 cleo3 Exp $
//
// Revision history
//
// $Log: SiGeom_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1998/08/18 07:42:04  cleo3
// imported SiGeom sources
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "SiGeom/SiGeom.h"

// STL classes

//
// constants, enums and typedefs
//

//
// function definitions
//

// ---------------------------factory method -------------------
extern "C" {
   Producer* makeProducer( void );
}

Producer*
makeProducer( void )
{
   return new SiGeom;
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
