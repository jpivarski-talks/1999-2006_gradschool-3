#if !defined(DUALTRACKPROD_DUALTRACKDRHITLINK_H)
#define DUALTRACKPROD_DUALTRACKDRHITLINK_H
// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackDRHitLink
// 
/**\class DualTrackDRHitLink DualTrackDRHitLink.h DualTrackProd/DualTrackDRHitLink.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:06:16 EST 2003
// $Id: DualTrackDRHitLink.h,v 1.1 2003/12/15 17:51:40 mccann Exp $
//
// Revision history
//
// $Log: DualTrackDRHitLink.h,v $
// Revision 1.1  2003/12/15 17:51:40  mccann
// adding hit lattices for dual-constrained residuals
//

// system include files

// user include files
#include "TrackFitter/TrackFitDRHitLink.h"
#include "DualTrackProd/DualTrackFitHelix.h"
#include "CalibratedData/CalibratedDRHit.h"

// forward declarations

class DualTrackDRHitLink : public TrackFitDRHitLink
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DualTrackDRHitLink(DABoolean iUsed,
			 double iResidual,
			 double iResidualError,
			 double iMomentum,
			 const HepPoint3D& iTrackRefPt,
			 Meters iSignedDcaToWire,
			 Meters iSignedDcaError,
			 double iSinTrackToRadial,
			 CalibratedTimeHit::PicoSec iDriftTime,
			 double iFittingWeight,
			 Meters iSignedDriftDistance,

			 double iSingleTrackResidual,
			 double iSingleTrackResidualError,
			 double iSingleTrackMomentum,
			 const HepPoint3D& iSingleTrackTrackRefPt,
			 Meters iSingleTrackSignedDcaToWire,
			 Meters iSingleTrackSignedDcaError,
			 double iSingleTrackSinTrackToRadial)
	 : TrackFitDRHitLink(iUsed,
			     iResidual,
			     iResidualError,
			     iMomentum,
			     iTrackRefPt,
			     iSignedDcaToWire,
			     iSignedDcaError,
			     iSinTrackToRadial,
			     iDriftTime,
			     iFittingWeight,
			     iSignedDriftDistance)
	 , m_singleTrackResidual(iSingleTrackResidual)
	 , m_singleTrackResidualError(iSingleTrackResidualError)
	 , m_singleTrackMomentum(iSingleTrackMomentum)
	 , m_singleTrackTrackRefPt(iSingleTrackTrackRefPt)
	 , m_singleTrackSignedDcaToWire(iSingleTrackSignedDcaToWire)
	 , m_singleTrackSignedDcaError(iSingleTrackSignedDcaError)
	 , m_singleTrackSinTrackToRadial(iSingleTrackSinTrackToRadial)
      { };
      DualTrackDRHitLink() : TrackFitDRHitLink()
      { };
      DualTrackDRHitLink(const DualTrackDRHitLink& rhs)
	 : TrackFitDRHitLink(rhs.used(),
			     rhs.residual(),
			     rhs.residualError(),
			     rhs.momentum(),
			     rhs.trackRefPt(),
			     rhs.signedDcaToWire(),
			     rhs.signedDcaError(),
			     rhs.sinTrackToRadial(),
			     rhs.driftTime(),
			     rhs.fittingWeight(),
			     rhs.signedDriftDistance())
	 , m_singleTrackResidual(rhs.singleTrack_residual())
	 , m_singleTrackResidualError(rhs.singleTrack_residualError())
	 , m_singleTrackMomentum(rhs.singleTrack_momentum())
	 , m_singleTrackTrackRefPt(rhs.singleTrack_trackRefPt())
	 , m_singleTrackSignedDcaToWire(rhs.singleTrack_signedDcaToWire())
	 , m_singleTrackSignedDcaError(rhs.singleTrack_signedDcaError())
	 , m_singleTrackSinTrackToRadial(rhs.singleTrack_sinTrackToRadial())
      { };

      virtual ~DualTrackDRHitLink() { };

      DABoolean operator==(const DualTrackDRHitLink& rhs) const {
	 return TrackFitDRHitLink::operator==(rhs); };
      DABoolean operator!=(const DualTrackDRHitLink& rhs) const {
	 return TrackFitDRHitLink::operator!=(rhs); };
      const DualTrackDRHitLink& operator=(const DualTrackDRHitLink& rhs) {
	 setUsed(rhs.used());
	 setResidual(rhs.residual());
	 setResidualError(rhs.residualError());
	 setMomentum(rhs.momentum());
	 setTrackRefPt(rhs.trackRefPt());
	 setSignedDcaToWire(rhs.signedDcaToWire());
	 setSignedDcaError(rhs.signedDcaError());
	 setSinTrackToRadial(rhs.sinTrackToRadial());
	 setDriftTime(rhs.driftTime());
	 setFittingWeight(rhs.fittingWeight());
	 setSignedDriftDistance(rhs.signedDriftDistance());
	 
	 m_singleTrackResidual = rhs.singleTrack_residual();
	 m_singleTrackResidualError = rhs.singleTrack_residualError();
	 m_singleTrackMomentum = rhs.singleTrack_momentum();
	 m_singleTrackTrackRefPt = rhs.singleTrack_trackRefPt();
	 m_singleTrackSignedDcaToWire = rhs.singleTrack_signedDcaToWire();
	 m_singleTrackSignedDcaError = rhs.singleTrack_signedDcaError();
	 m_singleTrackSinTrackToRadial = rhs.singleTrack_sinTrackToRadial();
	 return *this;
      };

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      double singleTrack_residual() const { return m_singleTrackResidual; };
      double singleTrack_residualError() const { return m_singleTrackResidualError; };
      double singleTrack_momentum() const { return m_singleTrackMomentum; };
      const HepPoint3D& singleTrack_trackRefPt() const { return m_singleTrackTrackRefPt; };
      Meters singleTrack_signedDcaToWire() const { return m_singleTrackSignedDcaToWire; };
      Meters singleTrack_signedDcaError() const { return m_singleTrackSignedDcaError; };
      double singleTrack_sinTrackToRadial() const { return m_singleTrackSinTrackToRadial; };

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_singleTrackResidual;
      double m_singleTrackResidualError;
      double m_singleTrackMomentum;
      HepPoint3D m_singleTrackTrackRefPt;
      Meters m_singleTrackSignedDcaToWire;
      Meters m_singleTrackSignedDcaError;
      double m_singleTrackSinTrackToRadial;

      // ---------- static data members ------------------------

};

typedef Lattice<DualTrackFitHelix, CalibratedDRHit, DualTrackDRHitLink> DualTrackDRHitLattice;

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DualTrackProd/Template/DualTrackDRHitLink.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACKDRHITLINK_H */
