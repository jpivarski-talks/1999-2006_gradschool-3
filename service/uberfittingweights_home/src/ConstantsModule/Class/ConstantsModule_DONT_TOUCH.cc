// -*- C++ -*-
//
// Package:     <ConstantsModule>
// Module:      ConstantsModule_DONT_TOUCH.cc
// 
// Description: factory function to create ConstantsModule
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Apr 21 10:20:18 EDT 1999
// $Id: ConstantsModule_DONT_TOUCH.cc,v 1.1 1999/04/29 19:20:49 cdj Exp $
//
// Revision history
//
// $Log: ConstantsModule_DONT_TOUCH.cc,v $
// Revision 1.1  1999/04/29 19:20:49  cdj
// rewritten to allow multiple databases at the same time
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "ConstantsModule/ConstantsModule.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ConstantsModule_DONT_TOUCH.cc,v 1.1 1999/04/29 19:20:49 cdj Exp $";
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
   return new ConstantsModule;
}

const char*
versionString( void )
{
   return kTagString;
}
