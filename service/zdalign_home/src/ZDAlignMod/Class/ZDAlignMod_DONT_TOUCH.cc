// -*- C++ -*-
//
// Package:     <ZDAlignMod>
// Module:      ZDAlignMod_DONT_TOUCH.cc
// 
// Description: factory function to create ZDAlignMod
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 09:37:30 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "ZDAlignMod/ZDAlignMod.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule_DONT_TOUCH.cc,v 1.1 2000/03/17 18:52:07 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

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
   return new ZDAlignMod;
}

const char*
versionString( void )
{
   return kTagString;
}
