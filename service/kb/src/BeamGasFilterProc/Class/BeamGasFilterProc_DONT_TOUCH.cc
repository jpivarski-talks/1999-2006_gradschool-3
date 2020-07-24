// -*- C++ -*-
//
// Package:     BeamGasFilterProc
// Module:      BeamGasFilterProc_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Definition of bind action
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Thu Oct 31 15:55:25 EST 2002
// $Id: BeamGasFilterProc_DONT_TOUCH.cc,v 1.1 2002/10/31 21:44:32 mccann Exp $
//
// Revision history
//
// $Log: BeamGasFilterProc_DONT_TOUCH.cc,v $
// Revision 1.1  2002/10/31 21:44:32  mccann
// something went wrong: adding files one by one
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "BeamGasFilterProc/BeamGasFilterProc.h"
#include "Processor/Action.h"                

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: BeamGasFilterProc_DONT_TOUCH.cc,v 1.1 2002/10/31 21:44:32 mccann Exp $";
static const char* const kTagString = "$Name:  $";

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
BeamGasFilterProc::bind(
   ActionBase::ActionResult (BeamGasFilterProc::*iMethod)( Frame& ),
   const Stream::Type& iStream )
{
   bindAction( iStream, new Action<BeamGasFilterProc>( iMethod, this ) );
}

//
// const member functions
//

//
// static member functions
//
