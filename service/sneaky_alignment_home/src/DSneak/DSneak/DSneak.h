// -*- C++ -*-
#if !defined(DSNEAK_DSNEAK_H)
#define DSNEAK_DSNEAK_H
//
// Package:     <DSneak>
// Module:      DSneak
//
/**\class DSneak DSneak.h DSneak/DSneak.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Mar 17 15:12:47 EST 2003
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

class DSneak : public Processor
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
	 kProPosPx,
	 kProPosPy,
	 kProPosPz,
	 kProNegPx,
	 kProNegPy,
	 kProNegPz,
	 kSecPosPx,
	 kSecPosPy,
	 kSecPosPz,
	 kSecNegPx,
	 kSecNegPy,
	 kSecNegPz,
	 kKaonMass,
	 kKaonBirthX,
	 kKaonBirthY,
	 kKaonBirthZ,
	 kKaonBirthZSep,
	 kKaonDeathX,
	 kKaonDeathY,
	 kKaonDeathZ,
	 kKaonDeathZSep,
	 kDirectionCosine,
	 kDeeMassFromBC,
	 kDeeMass,
	 kDeePx,
	 kDeePy,
	 kDeePz,
	 kDeeE,
					  
	 kEBeam,
	 kMissingPx,
	 kMissingPy,
	 kMissingPz,
	 kMissingE,

	 kNumEntries
      };

      // ------------ Constructors and destructor ----------------
      DSneak( void );                      // anal1 
      virtual ~DSneak();                   // anal5 

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
      DSneak( const DSneak& );

      // ------------ assignment operator(s) ---------------------
      const DSneak& operator=( const DSneak& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (DSneak::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      HepPoint3D event_vertex( vector<KTHelix>& good_tracks, HepPoint3D& beamSpot );
      DABoolean calc_intersection( HepPoint3D& intersection,
				   double& zsep, double& sigz,
				   double& aphi, double& bphi,
				   KTHelix& a, KTHelix& b );
      DABoolean arclength_test( double x, double y, KTHelix& a, KTHelix& b,
				double& aarclength, double& barclength );
      Hep3Vector rotate_momentum(Hep3Vector momentum, double angle);

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter<DABoolean> m_zeroBeamSpot;

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* DSNEAK_DSNEAK_H */
