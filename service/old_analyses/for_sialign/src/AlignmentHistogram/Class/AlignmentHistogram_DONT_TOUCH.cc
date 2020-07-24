// -*- C++ -*-
//
// Package:     <AlignmentHistogram>
// Module:      AlignmentHistogram_DONT_TOUCH.cc
// 
// Description: factory function to create AlignmentHistogram
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Mon Jun  5 11:46:38 EDT 2000
// $Id: AlignmentHistogram_DONT_TOUCH.cc,v 1.1.1.1 2001/03/07 18:03:00 llh14 Exp $
//
// Revision history
//
// $Log: AlignmentHistogram_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2001/03/07 18:03:00  llh14
// imported AlignmentHistogram
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "AlignmentHistogram/AlignmentHistogram.h"

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: AlignmentHistogram_DONT_TOUCH.cc,v 1.1.1.1 2001/03/07 18:03:00 llh14 Exp $";
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
   return new AlignmentHistogram;
}

const char*
versionString( void )
{
   return kTagString;
}
