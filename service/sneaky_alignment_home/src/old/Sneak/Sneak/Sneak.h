// -*- C++ -*-
#if !defined(SNEAK_SNEAK_H)
#define SNEAK_SNEAK_H
//
// Package:     <Sneak>
// Module:      Sneak
//
/**\class Sneak Sneak.h Sneak/Sneak.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Mar 12 11:25:36 EST 2003
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

class Sneak : public Processor
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

      enum {
	 kPlusEOverP, kMinusEOverP,
	 kPlusDedx, kMinusDedx,
	 kPlusRich, kMinusRich,
	 kZSep,
	 kPlusPx, kPlusPy, kPlusPz,
	 kMinusPx, kMinusPy, kMinusPz,
	 kKMass, 
	 kX0, kY0, kZ0,
	 kX, kY, kZ, kDecay,
	 kDirectionCosine,

	 kNumEntries
      };

      // ------------ Constructors and destructor ----------------
      Sneak( void );                      // anal1 
      virtual ~Sneak();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      virtual ActionBase::ActionResult beginRun( Frame& iFrame);
      //virtual ActionBase::ActionResult endRun( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      Sneak( const Sneak& );

      // ------------ assignment operator(s) ---------------------
      const Sneak& operator=( const Sneak& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (Sneak::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      HepPoint3D event_vertex(vector<KTHelix> good_tracks);
      DABoolean calc_intersection( double& x, double& y, double& sigx, double& sigy,
				   double& z, double& zsep, double& sigz, 
				   KTHelix& a, KTHelix& b );
      DABoolean arclength_test( double x, double y, KTHelix& a, KTHelix& b );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter<DABoolean> m_explicitBeamSpot;
      Parameter<double> m_beamX;
      Parameter<double> m_beamY;
      Parameter<double> m_beamZ;
      double m_beam_x;
      double m_beam_y;
      double m_beam_z;

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* SNEAK_SNEAK_H */
