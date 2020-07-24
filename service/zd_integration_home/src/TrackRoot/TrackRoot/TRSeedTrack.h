#if !defined(TRACKROOT_TRSEEDTRACK_H)
#define TRACKROOT_TRSEEDTRACK_H
// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRSeedTrack
//
/**\class TRSeedTrack TRSeedTrack.h TrackRoot/TRSeedTrack.h

 Description: Seed track - the results of pattern recognition. 

 Usage:
    TRSeedTracks are made by track finders.

    This is essentially a KTHelix with an identifier.

*/
//
// Author:      Adam Lyon
// Created:     Thu Jun 25 18:08:01 EDT 1998
// $Id: TRSeedTrack.h,v 1.6 2000/06/16 02:49:01 marsh Exp $
//
// Revision history
//
// $Log: TRSeedTrack.h,v $
// Revision 1.6  2000/06/16 02:49:01  marsh
// Purged tool box typedefs.
//
// Revision 1.5  2000/01/05 18:56:15  cdj
// changed variable names in constructor, added default constructor and operator==
//
// Revision 1.4  1999/10/07 16:33:50  marsh
// Added doxygen-style comments.
//
// Revision 1.3  1999/06/23 20:59:01  marsh
// Added more KTHelix constructors
//
// Revision 1.2  1998/11/06 19:25:03  marsh
// Changed identifier() to return a const reference.
//
// Revision 1.1  1998/08/10 18:53:10  lyon
// SeedTrack now in TrackRoot as TRSeedTrack
//
// Revision 1.1.1.1  1998/07/26 19:33:06  lyon
// TRSeedTrack
//
//

// system include files
#include "Experiment/types.h"
#include "Experiment/Experiment.h"

// user include files

#include "KinematicTrajectory/KTHelix.h"

// forward declarations

class TRSeedTrack : public KTHelix
{
      // ------------ friend classses and functions --------------

   public:

      typedef Count Identifier;

      TRSeedTrack(Identifier identifier, double curvature, double phi0, double d0, 
		  double cotTheta, double z0 );
      TRSeedTrack();
      TRSeedTrack( Identifier identifier, const KTHelix& aHelix );
      TRSeedTrack( Identifier identifier,
                   const KTKinematicData&  aKinematicData,
                   const HepVector3D&      aMagneticField,
                   const HepPoint3D&       aReferencePoint =
                      HepPoint3D(0,0,0) );
      TRSeedTrack( Identifier identifier,
                   const KTKinematicData&    aKinematicData,
                   const KTHelix::ValueType  aBFieldMagnitude,
                   const HepPoint3D&         aReferencePoint =
                      HepPoint3D(0,0,0) );


      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      virtual ~TRSeedTrack() {};               

      // ------------ member functions ---------------------------
      /**
       * \return unique (distinct) track number.
       * This identifier is shared by all representations of a track.
       */
      const Identifier& identifier() const {return m_id;} ;
      DABoolean operator ==(const TRSeedTrack& rhs) const;
   private:
      
      const Count  m_id;
};
      

// inline function definitions

#endif /* TRACKROOT_TRSEEDTRACK_H */
