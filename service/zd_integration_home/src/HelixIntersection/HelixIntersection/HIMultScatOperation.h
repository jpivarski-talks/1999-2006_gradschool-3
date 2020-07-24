#if !defined(HELIXINTERSECTION_HIMULTSCATOPERATION_H)
#define HELIXINTERSECTION_HIMULTSCATOPERATION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIMultScatOperation
// 
// Description: HIHelixOperation subclass for modifying the error matrix
//              of a helix to account for multiple scattering based on the
//              last arc length and materials of the associated
//              HIIntersectionSurface.  In addition, the two track angles
//              can be smeared if specified by the user.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Sep 28 11:25:48 EDT 1998
// $Id: HIMultScatOperation.h,v 1.5 1999/08/30 21:37:22 wsun Exp $
//
// Revision history
//
// $Log: HIMultScatOperation.h,v $
// Revision 1.5  1999/08/30 21:37:22  wsun
// Added energy loss straggling.
//
// Revision 1.4  1999/08/27 20:12:00  wsun
// New signature for HIHelixOperation::perform().
//
// Revision 1.3  1999/06/02 23:04:24  wsun
// Added features for low-momentum fitting.
//
// Revision 1.2  1999/04/27 22:04:05  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:20  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations

class HIMultScatOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIMultScatOperation( DABoolean aModifyHelixParameters = false ) ;
      virtual ~HIMultScatOperation();

      // ---------- member functions ---------------------------
      void setModifyHelixParameters( DABoolean flag )
      { m_modifyHelixParameters = flag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      DABoolean modifyHelixParameters() const
      { return m_modifyHelixParameters ; }

      // ---------- static member functions --------------------
      static double scatteringAngle2( const DGConstMaterialPtr& aMaterial,
				      const HIHelix& aHelix ) ;

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIMultScatOperation( const HIMultScatOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIMultScatOperation& operator=( const HIMultScatOperation& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_modifyHelixParameters ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIMultScatOperation.cc"
//#endif

#endif /* HELIXINTERSECTION_HIMULTSCATOPERATION_H */
