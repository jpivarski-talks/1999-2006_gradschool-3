#if !defined(HELIXINTERSECTION_HISILADDERPLANE_H)
#define HELIXINTERSECTION_HISILADDERPLANE_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HISiLadderPlane
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Apr 19 18:21:48 EDT 1999
// $Id: HISiLadderPlane.h,v 1.4 2001/03/29 01:40:46 llh14 Exp $
//
// Revision history
//
// $Log: HISiLadderPlane.h,v $
// Revision 1.4  2001/03/29 01:40:46  llh14
// Separated Si Rphi and Z Hit Surfaces
//
// Revision 1.3  1999/11/21 22:26:51  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.2  1999/04/28 22:57:02  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:04:07  wsun
// Major reorganization and structural changes.
//

// system include files

// user include files
#include "HelixIntersection/HIIntersectionPlane.h"

#include "CLHEP/Vector/ThreeVector.h"

// forward declarations
class ASiLadder ;

class HISiLadderPlane : public HIIntersectionPlane
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HISiLadderPlane() ;
      HISiLadderPlane(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 const ASiLadder& aLadder,
	 HICondition* = 0,
	 HIHelixOperation* = 0,
	 const HepPoint3D& aCorrectionInSi = (0., 0., 0.) ) ;

      virtual ~HISiLadderPlane();

      // ---------- member functions ---------------------------
      void setLadder( const ASiLadder& aLadder )
      { m_ladder = &aLadder ; }

      // ---------- const member functions ---------------------
      const ASiLadder& ladder() const
      { return *m_ladder ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HISiLadderPlane::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HISiLadderPlane( const HISiLadderPlane& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HISiLadderPlane& operator=( const HISiLadderPlane& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const ASiLadder* m_ladder ;
      HepPoint3D m_CorrectionInSi ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HISiLadderPlane.cc"
//#endif

#endif /* HELIXINTERSECTION_HISILADDERPLANE_H */
