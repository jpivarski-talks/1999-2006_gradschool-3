// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RunStatisticsSourceModule_DONT_TOUCH.cc
// 
// Description: factory function to create RunStatisticsSourceModule
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Jan 25 09:04:46 EST 2000
// $Id: RunStatisticsSourceModule_DONT_TOUCH.cc,v 1.1.1.1 2000/01/29 20:47:37 cdj Exp $
//
// Revision history
//
// $Log: RunStatisticsSourceModule_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2000/01/29 20:47:37  cdj
// imported RunStatisticsSourceModule source
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "RunStatisticsSourceModule/RunStatisticsSourceModule.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: RunStatisticsSourceModule_DONT_TOUCH.cc,v 1.1.1.1 2000/01/29 20:47:37 cdj Exp $";
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
   return new RunStatisticsSourceModule;
}

const char*
versionString( void )
{
   return kTagString;
}
