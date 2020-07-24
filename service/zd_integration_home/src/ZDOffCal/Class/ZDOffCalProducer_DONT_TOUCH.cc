// -*- C++ -*-
//
// Package:     <package>
// Module:      ZDOffCalProducer_DONT_TOUCH
// 
// Description: Miscellaneous methods which the user 
//              is not supposed to touch, 
//              but which need to be present 
//              in the shared processor library.
//
//              1.) Factory method to create processor ZDOffCalProducer:
//                  creates a new ZDOffCalProducer instance each time 
//                  it is called; it is used by Suez 
//                  to create a Processor after loading in 
//                  the code from a shared library.
//
// Implementation:
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDOffCalProducer_DONT_TOUCH.cc,v 1.1.1.1 2003/02/03 20:11:56 huanggs Exp $
//
// Revision history
//
// $Log: ZDOffCalProducer_DONT_TOUCH.cc,v $
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
#include "ZDOffCal/ZDOffCalProducer.h"

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
   return new ZDOffCalProducer;
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
