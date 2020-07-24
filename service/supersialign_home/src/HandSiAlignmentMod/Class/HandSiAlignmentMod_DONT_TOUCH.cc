// -*- C++ -*-
//
// Package:     <HandSiAlignmentMod>
// Module:      HandSiAlignmentMod_DONT_TOUCH.cc
// 
// Description: factory function to create HandSiAlignmentMod
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue Apr  9 15:11:19 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "HandSiAlignmentMod/HandSiAlignmentMod.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule_DONT_TOUCH.cc,v 1.1 2000/03/17 18:52:07 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

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
   return new HandSiAlignmentMod;
}

const char*
versionString( void )
{
   return kTagString;
}
