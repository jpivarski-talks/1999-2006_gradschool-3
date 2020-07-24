// -*- C++ -*-
//
// Package:     KaonMonitorProc
// Module:      KaonMonitorProc_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Definition of bind action
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Thu Oct 30 17:46:23 EST 2003
// $Id: KaonMonitorProc_DONT_TOUCH.cc,v 1.1.1.1 2003/11/18 22:04:18 mccann Exp $
//
// Revision history
//
// $Log: KaonMonitorProc_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2003/11/18 22:04:18  mccann
// imported KaonMonitorProc sources
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "KaonMonitorProc/KaonMonitorProc.h"
#include "Processor/Action.h"                

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KaonMonitorProc_DONT_TOUCH.cc,v 1.1.1.1 2003/11/18 22:04:18 mccann Exp $";
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
KaonMonitorProc::bind(
   ActionBase::ActionResult (KaonMonitorProc::*iMethod)( Frame& ),
   const Stream::Type& iStream )
{
   bindAction( iStream, new Action<KaonMonitorProc>( iMethod, this ) );
}

//
// const member functions
//

//
// static member functions
//
