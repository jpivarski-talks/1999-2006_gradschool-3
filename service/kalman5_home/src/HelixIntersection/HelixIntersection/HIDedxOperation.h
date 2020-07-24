#if !defined(HELIXINTERSECTION_HIDEDXOPERATION_H)
#define HELIXINTERSECTION_HIDEDXOPERATION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIDedxOperation
// 
// Description: HIHelixOperation subclass for applying energy loss to a
//              track based on its last arc length and the materials of
//              the associated HIIntersectionSurface.  Both the track
//              parameters and the error matrix are modified.
//
// Usage:
//    The width of the Gaussian straggling is given by:
//
//        d( deltaE ) = coeff^2 * deltaE
//
//    where the coefficient is set by the user in the constructor.
//
// Author:      Werner Sun
// Created:     Mon Sep 28 11:25:18 EDT 1998
// $Id: HIDedxOperation.h,v 1.6 1999/09/16 19:37:57 wsun Exp $
//
// Revision history
//
// $Log: HIDedxOperation.h,v $
// Revision 1.6  1999/09/16 19:37:57  wsun
// Added error flag as argument to deltaEnergy().
//
// Revision 1.5  1999/08/30 21:37:23  wsun
// Added energy loss straggling.
//
// Revision 1.4  1999/08/27 20:11:58  wsun
// New signature for HIHelixOperation::perform().
//
// Revision 1.3  1999/06/02 23:04:21  wsun
// Added features for low-momentum fitting.
//
// Revision 1.2  1999/04/27 22:03:58  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:19  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations
class DGConstMaterialPtr ;

class HIDedxOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIDedxOperation( DABoolean aApplyStragglingToTrack = false,
		       DABoolean aApplyStragglingToError = true,
		       double aStragglingCoeff = 0.001 ) ;
      virtual ~HIDedxOperation();

      // ---------- member functions ---------------------------
      void setStragglingCoeff( double aCoeff )
      { m_stragglingCoeff = aCoeff ; }

      void setApplyStragglingToTrack( DABoolean aFlag )
      { m_applyStragglingToTrack = aFlag ; }

      void setApplyStragglingToError( DABoolean aFlag )
      { m_applyStragglingToError = aFlag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      // ---------- static member functions --------------------
      static GeV deltaEnergy( const DGConstMaterialPtr& aMaterial,
			      const HIHelix& aHelix,
			      DABoolean& aOK ) ;  // output

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIDedxOperation( const HIDedxOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIDedxOperation& operator=( const HIDedxOperation& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_stragglingCoeff ;
      DABoolean m_applyStragglingToTrack ;
      DABoolean m_applyStragglingToError ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIDedxOperation.cc"
//#endif

#endif /* HELIXINTERSECTION_HIDEDXOPERATION_H */
