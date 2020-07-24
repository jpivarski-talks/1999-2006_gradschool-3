// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRGeom_DONT_TOUCH
// 
// Description: Miscellaneous methods which the user 
//              is not supposed to touch, 
//              but which need to be present 
//              in the shared processor library.
//
//              1.) Factory method to create processor ADRGeom:
//                  creates a new ADRGeom instance each time 
//                  it is called; it is used by Suez 
//                  to create a Processor after loading in 
//                  the code from a shared library.
//
// Implementation:
//
// Author:       Inga Karliner
// Created:      Sep 23, 1998
//
// Revision history
//
// New ADRGeom library
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom)
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "ADRGeomProd/ADRGeomProd.h"

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
   return new ADRGeomProd;
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







