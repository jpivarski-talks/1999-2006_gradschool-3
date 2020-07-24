// -*- C++ -*-
//
// Package:     BeamGasFilterProc
// Module:      BeamGasFilterProc_DONT_TOUCH2
// 
// Description: DONT TOUCH THIS FILE!
//
//              Factory method to create processor BeamGasFilterProc:
//              creates a new BeamGasFilterProc instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Thu Oct 31 15:55:26 EST 2002
// $Id: BeamGasFilterProc_DONT_TOUCH2.cc,v 1.1 2002/10/31 21:44:44 mccann Exp $
//
// Revision history
//
// $Log: BeamGasFilterProc_DONT_TOUCH2.cc,v $
// Revision 1.1  2002/10/31 21:44:44  mccann
// something went wrong: adding files one by one
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             

// user include files
#include "BeamGasFilterProc/BeamGasFilterProc.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: BeamGasFilterProc_DONT_TOUCH2.cc,v 1.1 2002/10/31 21:44:44 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// function definitions
//

extern "C" {
   Processor* makeProcessor( void );
   const char* versionString( void );
}

Processor*
makeProcessor( void )
{
   return new BeamGasFilterProc;
}

const char*
versionString( void )
{
   return kTagString;
}

