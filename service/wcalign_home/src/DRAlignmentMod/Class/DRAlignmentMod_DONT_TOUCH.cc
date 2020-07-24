// -*- C++ -*-
//
// Package:     <DRAlignmentMod>
// Module:      DRAlignmentMod_DONT_TOUCH.cc
// 
// Description: factory function to create DRAlignmentMod
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:45:56 EST 2003
// $Id: DRAlignmentMod_DONT_TOUCH.cc,v 1.1.1.1 2003/12/15 18:05:02 mccann Exp $
//
// Revision history
//
// $Log: DRAlignmentMod_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2003/12/15 18:05:02  mccann
// imported DRAlignmentMod sources
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DRAlignmentMod/DRAlignmentMod.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRAlignmentMod_DONT_TOUCH.cc,v 1.1.1.1 2003/12/15 18:05:02 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// function definitions
//

extern "C" { 
   Module* makeModule( void ); 
   const char* versionString( void );
}

Module*
makeModule( void )
{
   return new DRAlignmentMod;
}

const char*
versionString( void )
{
   return kTagString;
}
