// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixFit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Thu Jun 25 16:42:48 EDT 1998
// $Id: TRHelixFit.cc,v 1.2 1998/10/21 14:43:31 marsh Exp $
//
// Revision history
//
// $Log: TRHelixFit.cc,v $
// Revision 1.2  1998/10/21 14:43:31  marsh
// Re-ordered initializations in ctor to put base class first.
//
// Revision 1.1.1.1  1998/07/20 21:16:10  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:27:55  marsh
// New library for track objects
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackRoot/TRHelixFit.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackRoot.TRHelixFit" ;

//
// static data member definitions
//

//
// constructors and destructor
//
TRHelixFit::TRHelixFit( const Identifier& aId, const KTHelix& aHelix ) :
   KTHelix( aHelix ) ,
   m_id( aId )
{
}

TRHelixFit::~TRHelixFit()
{
}

//
// const member functions
//
const TRHelixFit::Identifier&
TRHelixFit::identifier() const
{
   return m_id;
}

//
// static member functions
//
