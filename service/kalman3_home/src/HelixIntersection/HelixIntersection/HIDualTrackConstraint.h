#if !defined(HELIXINTERSECTION_HIDUALTRACKCONSTRAINT_H)
#define HELIXINTERSECTION_HIDUALTRACKCONSTRAINT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIDualTrackConstraint
// 
/**\class HIDualTrackConstraint HIDualTrackConstraint.h HelixIntersection/HIDualTrackConstraint.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Sat Oct  9 12:54:07 EDT 1999
// $Id: HIDualTrackConstraint.h,v 1.4 2001/08/24 21:40:24 wsun Exp $
//
// Revision history
//
// $Log: HIDualTrackConstraint.h,v $
// Revision 1.4  2001/08/24 21:40:24  wsun
// Fixed factor of two error in virtual photon beta.
//
// Revision 1.3  2001/08/23 03:15:56  wsun
// Forgot to boost Pparallel into virtual photon rest frame.
//
// Revision 1.2  2000/10/11 05:39:04  wsun
// Added option for releasing constraint on two p_t components.
//
// Revision 1.1  1999/10/14 18:50:36  wsun
// First submission.
//
//

// system include files

// user include files
#include "HelixIntersection/HIFitConstraint.h"
#include "CLHEP/Geometry/Vector3D.h"

// forward declarations

class HIDualTrackConstraint : public HIFitConstraint
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum Constraints { kD0,
			 kZ0,
			 kPPrll,  // momenta parallel to boost direction
			 kPPerp,  // momenta perpendicular to boost direction
                         kNConstraints } ;

      // ---------- Constructors and destructor ----------------

      // Any z-component of aVirtualPhotonDir will be ignored -- the
      // boost direction is assumed to be purely radial.
      //
      // If the crossing angle is zero, it doesn't matter what the beam
      // energy is.
      // 
      // The half-crossing-angle is the angle between the beam line and
      // one of the incident particles.

      HIDualTrackConstraint(
	 double aHalfCrossingAngleInRadians = 0.,
	 GeV aBeamEnergy = 5.29,
	 const HepVector3D& aVirtualPhotonDir = HepVector3D( -1., 0., 0. ),
	 DABoolean aConstrainPT = true,
	 GeV aParticleMass = 0.105658 ) ;  // muon is default
      virtual ~HIDualTrackConstraint();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual int numberOfConstraints() const
      { return kNConstraints  - ( m_constrainPT ? 0 : 2 ) ; }

      // The constraints evaluated at the given helix parameters.  Can also
      // be functions of initialUnknowns().  Should have length r.
      virtual HepVector constraint(
         const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      // The matrix dH/dEta (see above) -- should be N x r.
      virtual HepMatrix constraintDerivatives( 
         const STL_VECTOR( HIFitHelix* )& aHelices ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIDualTrackConstraint( const HIDualTrackConstraint& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIDualTrackConstraint& operator=( const HIDualTrackConstraint& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_halfAngle ;
      HepVector3D m_vPhotonDir ;
      DABoolean m_constrainPT ;
      GeV m_ebm ;
      GeV m_mass ;
      GeV m_beta ;
      GeV m_gamma ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIDualTrackConstraint.cc"
//#endif

#endif /* HELIXINTERSECTION_HIDUALTRACKCONSTRAINT_H */
