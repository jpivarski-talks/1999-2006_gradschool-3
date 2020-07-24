// -*- C++ -*-
//
// Package:     <package>
// Module:      CDOffCalProducer_DONT_TOUCH
// 
// Description: Miscellaneous methods which the user 
//              is not supposed to touch, 
//              but which need to be present 
//              in the shared processor library.
//
//              1.) Factory method to create processor CDOffCalProducer:
//                  creates a new CDOffCalProducer instance each time 
//                  it is called; it is used by Suez 
//                  to create a Processor after loading in 
//                  the code from a shared library.
//
// Implementation:
//
// Author:      Adam Lyon
// Created:     Thu Apr 16 13:04:56 EDT 1998
// $Id: CDOffCalProducer_DONT_TOUCH.cc,v 1.1 1998/04/24 20:32:02 mclean Exp $
//
// Revision history
//
// $Log: CDOffCalProducer_DONT_TOUCH.cc,v $
// Revision 1.1  1998/04/24 20:32:02  mclean
// First Real CDOffCal Code
//
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "CDOffCal/CDOffCalProducer.h"

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
   return new CDOffCalProducer;
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
