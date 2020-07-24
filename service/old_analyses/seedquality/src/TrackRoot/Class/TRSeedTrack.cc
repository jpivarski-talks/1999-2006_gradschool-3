// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRSeedTrack
// 
// Description: Seed tracks from pattern recognition
//
// Implementation:
//     TRSeedTrack is derived from KTHelix!!
//
// Author:      Adam Lyon
// Created:     Thu Jun 25 18:08:02 EDT 1998
// $Id: TRSeedTrack.cc,v 1.4 2000/06/16 02:48:57 marsh Exp $
//
// Revision history
//
// $Id: TRSeedTrack.cc,v 1.4 2000/06/16 02:48:57 marsh Exp $
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRSeedTrack.h"
#include "Experiment/report.h"

// STL classes

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "TrackRoot.TRSeedTrack" ;

//
// static data member definitions
//

//
// constructors and destructor
//
TRSeedTrack::TRSeedTrack(Identifier identifier, double curv, 
			 double phi0, double d0, 
			 double tandip, double z0 ) :
   KTHelix( curv, phi0, d0, tandip, z0 ),
   m_id( identifier )
{}

TRSeedTrack::TRSeedTrack() : KTHelix(),m_id(0)  
{}

TRSeedTrack::TRSeedTrack( Identifier identifier, const KTHelix& aHelix ) :
   KTHelix( aHelix ),
   m_id( identifier )
{}

TRSeedTrack::TRSeedTrack( Identifier identifier,
                          const KTKinematicData&  aKinematicData,
                          const HepVector3D&      aMagneticField,
                          const HepPoint3D&       aReferencePoint ) :
   KTHelix( aKinematicData, aMagneticField, aReferencePoint ),
   m_id( identifier )
{}

TRSeedTrack::TRSeedTrack( Identifier identifier,
                          const KTKinematicData&    aKinematicData,
                          const KTHelix::ValueType  aBFieldMagnitude,
                          const HepPoint3D&         aReferencePoint ) :
   KTHelix( aKinematicData, aBFieldMagnitude, aReferencePoint ),
   m_id( identifier )
{}

DABoolean TRSeedTrack::operator == (const TRSeedTrack& rhs) const
{
  
  return ( (m_id==rhs.m_id) && (KTHelix::operator==(rhs)) );
}
