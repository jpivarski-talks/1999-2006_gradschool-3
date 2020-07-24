// -*- C++ -*-
//
// Package:     <package>
// Module:      ZDDriftFunctionProd_DONT_TOUCH
// 
// Description: Miscellaneous methods which the user 
//              is not supposed to touch, 
//              but which need to be present 
//              in the shared processor library.
//
//              1.) Factory method to create processor ZDDriftFunctionProd:
//                  creates a new ZDDriftFunctionProd instance each time 
//                  it is called; it is used by Suez 
//                  to create a Processor after loading in 
//                  the code from a shared library.
//
// Implementation:
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDriftFunctionProd_DONT_TOUCH.cc,v 1.1.1.1 2003/06/26 19:02:50 huanggs Exp $
//
// Revision history
//
// $Log: ZDDriftFunctionProd_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2003/06/26 19:02:50  huanggs
// imported ZDDriftFunctionProd source
//
// Revision 1.1.1.1  2003/02/03 20:11:56  huanggs
// imported source
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "ZDDriftFunctionProd/ZDDriftFunctionProd.h"

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
   return new ZDDriftFunctionProd;
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
