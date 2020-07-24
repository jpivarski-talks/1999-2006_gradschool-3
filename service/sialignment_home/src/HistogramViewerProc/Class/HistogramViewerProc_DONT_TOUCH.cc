// -*- C++ -*-
//
// Package:     HistogramViewerProc
// Module:      HistogramViewerProc_DONT_TOUCH
// 
// Description: DONT TOUCH THIS FILE
//
//              Definition of bind action
//
// Implementation:
//
// Author:      Chris D Jones
// Created:     Fri Jul 20 09:24:39 EDT 2001
// $Id: HistogramViewerProc_DONT_TOUCH.cc,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $
//
// Revision history
//
// $Log: HistogramViewerProc_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  2001/07/30 14:11:45  cdj
// importing
//
 
#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */            

// user include files
#include "HistogramViewerProc/HistogramViewerProc.h"
#include "Processor/Action.h"                

// STL classes

//
// constants, enums and typedefs
//
// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HistogramViewerProc_DONT_TOUCH.cc,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $";
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
HistogramViewerProc::bind(
   ActionBase::ActionResult (HistogramViewerProc::*iMethod)( Frame& ),
   const Stream::Type& iStream )
{
   bindAction( iStream, new Action<HistogramViewerProc>( iMethod, this ) );
}

//
// const member functions
//

//
// static member functions
//
