// -*- C++ -*-
//
// Package:     DRAlignmentBhabhaFilterProc
// Module:      DRAlignmentBhabhaFilterProc_DONT_TOUCH2
// 
// Description: DONT TOUCH THIS FILE!
//
//              Factory method to create processor DRAlignmentBhabhaFilterProc:
//              creates a new DRAlignmentBhabhaFilterProc instance each time 
//              it is called; it is used by Suez 
//              to create a Processor after loading in 
//              the code from a shared library.
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Fri Dec  5 12:08:39 EST 2003
// $Id: DRAlignmentBhabhaFilterProc_DONT_TOUCH2.cc,v 1.1.1.1 2003/12/05 17:54:58 mccann Exp $
//
// Revision history
//
// $Log: DRAlignmentBhabhaFilterProc_DONT_TOUCH2.cc,v $
// Revision 1.1.1.1  2003/12/05 17:54:58  mccann
// imported DRAlignmentBhabhaFilterProc sources
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             

// user include files
#include "DRAlignmentBhabhaFilterProc/DRAlignmentBhabhaFilterProc.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRAlignmentBhabhaFilterProc_DONT_TOUCH2.cc,v 1.1.1.1 2003/12/05 17:54:58 mccann Exp $";
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
   return new DRAlignmentBhabhaFilterProc;
}

const char*
versionString( void )
{
   return kTagString;
}

