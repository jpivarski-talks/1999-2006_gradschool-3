// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistoManager
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Mon Feb  8 15:38:12 EST 1999
// $Id: HIHistoManager.cc,v 1.3 1999/09/07 18:53:22 mkl Exp $
//
// Revision history
//
// $Log: HIHistoManager.cc,v $
// Revision 1.3  1999/09/07 18:53:22  mkl
// new histogram interface
//
// Revision 1.2  1999/02/23 19:51:03  cdj
// do not need string include
//
// Revision 1.1.1.1  1999/02/16 22:07:05  cdj
// first submission
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "HistogramInterface/HIHistoManager.h"

// STL includes
#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HIHistoManager" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIHistoManager.cc,v 1.3 1999/09/07 18:53:22 mkl Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIHistoManager::HIHistoManager()
{
}

// HIHistoManager::HIHistoManager( const HIHistoManager& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIHistoManager::~HIHistoManager()
{
}

//
// assignment operators
//
// const HIHistoManager& HIHistoManager::operator=( const HIHistoManager& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//
