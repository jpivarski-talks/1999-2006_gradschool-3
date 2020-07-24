#if !defined(DUALTRACKPROD_DUALTRACK_H)
#define DUALTRACKPROD_DUALTRACK_H
// -*- C++ -*-
//
// DualTrackProd:     <DualTrackProd>
// Module:      DualTrackProd
// 
/**\class DualTrack DualTrack.h DualTrackProd/DualTrack.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:06:26 EST 2003
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files
#include "KinematicTrajectory/KTHelix.h"
#include "DualTrackProd/DualTrackConstraint.h"
#include "HelixIntersection/HIFitHelix.h"

// forward declarations

class DualTrack
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DualTrack();
      DualTrack(unsigned int posid,
		const KTHelix* positive,
		unsigned int negid,
		const KTHelix* negative,
		double magneticField,
		HepVector3D& pVirtualPhoton,
		DABoolean pointConstraint = true,
		DABoolean ptConstraint = true,
		DABoolean pzConstraint = true);
      virtual ~DualTrack();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      const HIFitHelix& positive() const { return m_positive; }
      const HIFitHelix& negative() const { return m_negative; }
      const DualTrackConstraint& constraint() const { return m_constraint; }
      const HIFitConstraint::Results& results() const { return m_results; }
      const HepVector3D& pVirtualPhoton() const { return m_pVirtualPhoton; }
      DABoolean pointConstraint() const { return m_pointConstraint; }
      DABoolean ptConstraint() const { return m_ptConstraint; }
      DABoolean pzConstraint() const { return m_pzConstraint; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DualTrack( const DualTrack& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DualTrack& operator=( const DualTrack& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      HIFitHelix m_positive;
      HIFitHelix m_negative;
      DualTrackConstraint m_constraint;
      HIFitConstraint::Results m_results;
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
//# include "DualTrackProd/Template/DualTrack.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACK_H */
