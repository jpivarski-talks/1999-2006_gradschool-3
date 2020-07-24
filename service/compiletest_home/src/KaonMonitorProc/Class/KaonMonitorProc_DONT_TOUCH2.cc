// -*- C++ -*-
//
// Package:     KaonMonitorProc
// Module:      KaonMonitorProc_DONT_TOUCH2
// 
// Description: DONT TOUCH THIS FILE!
//
//              Factory method to create processor KaonMonitorProc:
//              creates a new KaonMonitorProc instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Thu Oct 30 17:46:23 EST 2003
// $Id: KaonMonitorProc_DONT_TOUCH2.cc,v 1.1.1.1 2003/11/18 22:04:18 mccann Exp $
//
// Revision history
//
// $Log: KaonMonitorProc_DONT_TOUCH2.cc,v $
// Revision 1.1.1.1  2003/11/18 22:04:18  mccann
// imported KaonMonitorProc sources
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             

// user include files
#include "KaonMonitorProc/KaonMonitorProc.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KaonMonitorProc_DONT_TOUCH2.cc,v 1.1.1.1 2003/11/18 22:04:18 mccann Exp $";
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
   return new KaonMonitorProc;
}

const char*
versionString( void )
{
   return kTagString;
}

