// -*- C++ -*-
//
// Package:     <DRFittingWeights>
// Module:      DRFittingWeights_DONT_TOUCH.cc
// 
// Description: factory function to create DRFittingWeights
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Mon Jul 30 11:00:23 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DRFittingWeights/DRFittingWeights.h"

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
   return new DRFittingWeights;
}

const char*
versionString( void )
{
   return kTagString;
}
