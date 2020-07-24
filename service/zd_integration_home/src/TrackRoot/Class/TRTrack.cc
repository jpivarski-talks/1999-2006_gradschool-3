// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrack
// 
// Description: Namespace for Identifier
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Simon Patton
// Created:     Tue Jun 24 13:59:27 EDT 1997
// $Id: TRTrack.cc,v 1.1 1999/06/24 12:43:50 marsh Exp $
//
// Revision history
//
// $Log: TRTrack.cc,v $
// Revision 1.1  1999/06/24 12:43:50  marsh
// Moved DBTrack into TrackRoot.
//
// Revision 1.5  1997/08/27 03:39:52  sjp
// Updated to new name for bug flags
//
// Revision 1.4  1997/08/06 18:08:16  sjp
// Removed dependence on DataField
//
// Revision 1.3  1997/07/28 17:59:53  sjp
// Fixed to work under cxx 5.6
//
// Revision 1.2  1997/07/09 18:01:28  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/06/27 18:54:20  sjp
// new file for composite track class
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRTrack.h"

// STL classes
#if defined(OLD_CXX_STRING_CLASS_BUG)
#include <String.h>
#else
#include <string>
#endif /* OLD_CXX_STRING_CLASS_BUG */

//
// constants, enums and typedefs
//

//
// static data member definitions
//

const string kFacilityString = "TrackRoot.TRTrack" ;

const Stream::Type& TRTrack::kStream = Stream::kEvent ;

//
// constructors and destructor
//
TRTrack::TRTrack( const Identifier& aId) :
   m_id( aId )
{
}

// TRTrack::TRTrack( const TRTrack& )
// {
// }

TRTrack::~TRTrack()
{
}

//
// assignment operators
//
// const TRTrack& TRTrack::operator=( const TRTrack& )
// {
// }

//
// member functions
//

//
// const member functions
//

const TRTrack::Identifier&
TRTrack::identifier() const
{
   return ( m_id ) ;
}

//
// static member functions
//
