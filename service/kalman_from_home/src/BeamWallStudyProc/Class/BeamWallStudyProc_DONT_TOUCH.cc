// -*- C++ -*-
//
// Package:     BeamWallStudyProc
// Module:      BeamWallStudyProc_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Definition of bind action
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Tue Oct 29 13:58:18 EST 2002
// $Id$
//
// Revision history
//
// $Log$
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "BeamWallStudyProc/BeamWallStudyProc.h"
#include "Processor/Action.h"                

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor_DONT_TOUCH.cc,v 1.5 1998/12/01 21:11:58 mkl Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// function definitions
//

//
// static data member definitions
//

//
// member functions
//
// ---------------- binding method to stream -------------------
void
BeamWallStudyProc::bind(
   ActionBase::ActionResult (BeamWallStudyProc::*iMethod)( Frame& ),
   const Stream::Type& iStream )
{
   bindAction( iStream, new Action<BeamWallStudyProc>( iMethod, this ) );
}

//
// const member functions
//

//
// static member functions
//
