// -*- C++ -*-
//
// Package:     <EnderSiAlignmentMod>
// Module:      EnderSiAlignmentMod_DONT_TOUCH.cc
// 
// Description: factory function to create EnderSiAlignmentMod
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri Sep 14 12:59:14 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "EnderSiAlignmentMod/EnderSiAlignmentMod.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule_DONT_TOUCH.cc,v 1.1 2000/03/17 18:52:07 cdj Exp $";
static const char* const kTagString = "$Name: v03_02_01 $";

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
   return new EnderSiAlignmentMod;
}

const char*
versionString( void )
{
   return kTagString;
}
