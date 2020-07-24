#if !defined(DUALTRACKPROD_DUALTRACKCONSTRAINT_H)
#define DUALTRACKPROD_DUALTRACKCONSTRAINT_H
// -*- C++ -*-
//
// DualTrackProd:     <DualTrackProd>
// Module:      DualTrackConstraint
// 
/**\class DualTrackConstraint DualTrackConstraint.h DualTrackProd/DualTrackConstraint.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:50:23 EST 2003
// $Id: DualTrackConstraint.h,v 1.2 2003/12/15 17:51:37 mccann Exp $
//
// Revision history
//
// $Log: DualTrackConstraint.h,v $
// Revision 1.2  2003/12/15 17:51:37  mccann
// adding hit lattices for dual-constrained residuals
//
// Revision 1.1.1.1  2003/12/11 17:14:28  mccann
// imported DualTrackProd sources
//

// system include files

// user include files
#include "HelixIntersection/HIFitConstraint.h"
#include "DualTrackProd/DualTrackFitHelix.h"
#include "CLHEP/Geometry/Vector3D.h"

// forward declarations

class DualTrackConstraint : public HIFitConstraint
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum Constraints { kD0 = 0,
			 kZ0 = 1,

			 kPx = 0,
			 kPy = 1,

			 kPz = 0 };

      // ---------- Constructors and destructor ----------------
      DualTrackConstraint();
      DualTrackConstraint(double magneticField,
			  HepVector3D pVirtualPhoton,
			  DABoolean pointConstraint,
			  DABoolean ptConstraint,
			  DABoolean pzConstraint);
      virtual ~DualTrackConstraint();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual int numberOfConstraints() const
      { return (m_pointConstraint ? 2 : 0) + (m_ptConstraint ? 2 : 0) + (m_pzConstraint ? 1 : 0); }
      virtual HepVector constraint(const vector<HIFitHelix*>& vect) const;
      virtual HepMatrix constraintDerivatives(const vector<HIFitHelix*>& vect) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DualTrackConstraint( const DualTrackConstraint& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DualTrackConstraint& operator=( const DualTrackConstraint& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_magneticField;
      HepVector3D m_pVirtualPhoton;
      DABoolean m_pointConstraint;
      DABoolean m_ptConstraint;
      DABoolean m_pzConstraint;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DualTrackProd/Template/DualTrackConstraint.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACKCONSTRAINT_H */
