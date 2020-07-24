// -*- C++ -*-
#if !defined(SNEAKY_SNEAKY_H)
#define SNEAKY_SNEAKY_H
//
// Package:     <Sneaky>
// Module:      Sneaky
//
/**\class Sneaky Sneaky.h Sneaky/Sneaky.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Mar 18 15:25:19 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"
#include "KinematicTrajectory/KTHelix.h"
#include "CLHEP/Geometry/Point3D.h"

// forward declarations

class Sneaky : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kRa, kXa, kYa,
	 kRb, kXb, kYb,
	 kNumCirc
      };
      enum {
	 kCurva, kD0a, kPhi0a,
	 kCurvb, kD0b, kPhi0b,
	 kNumTrack
      };

      // ------------ Constructors and destructor ----------------
      Sneaky( void );                      // anal1 
      virtual ~Sneaky();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      //virtual ActionBase::ActionResult beginRun( Frame& iFrame);
      //virtual ActionBase::ActionResult endRun( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      Sneaky( const Sneaky& );

      // ------------ assignment operator(s) ---------------------
      const Sneaky& operator=( const Sneaky& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (Sneaky::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      HepPoint3D event_vertex( vector<KTHelix>& good_tracks, HepPoint3D& beamSpot );
      DABoolean calc_intersection( HepPoint3D& intersection,
				   double& zsep, double& aphi, double& bphi,
				   const KTHelix* a, const KTHelix* b );
      DABoolean arclength_test( double x, double y, const KTHelix* a, const KTHelix* b,
				double& aarclength, double& barclength );
      Hep3Vector rotate_momentum(Hep3Vector momentum, double angle);

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter<DABoolean> m_zeroBeamSpot;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* SNEAKY_SNEAKY_H */
