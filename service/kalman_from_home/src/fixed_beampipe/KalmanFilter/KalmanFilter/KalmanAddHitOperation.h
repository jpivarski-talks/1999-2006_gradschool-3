#if !defined(KALMANFILTER_KALMANADDHITOPERATION_H)
#define KALMANFILTER_KALMANADDHITOPERATION_H
// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanAddHitOperation
// 
// Description: HIHelixOperation subclass for adding a hit in the Kalman
//              filter.  Information needed by the chi-square fit (e.g.
//              measured and predicted distance from the hit, fitting
//              weight) is obtained from the associated HIIntersectionSurface.
//              
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Oct 27 00:31:08 EST 1998
// $Id: KalmanAddHitOperation.h,v 1.6 2001/03/27 06:39:31 wsun Exp $
//
// Revision history
//
// $Log: KalmanAddHitOperation.h,v $
// Revision 1.6  2001/03/27 06:39:31  wsun
// Mods for hit removal.
//
// Revision 1.5  1999/12/15 21:11:53  wsun
// * More explicit matrix manipulation.
// * Removed HIFitQualityCondition.
// * Added static function invert5x5SymMatrix(), which is much faster than
//   HepSymMatrix::invert().
//
// Revision 1.4  1999/08/27 20:34:34  wsun
// * New HIHelixOperation::perform().
// * Moved track parameter update to KalmanUpdateParamsOperation.
//
// Revision 1.3  1999/06/02 23:07:28  wsun
// Improved low-momentum fitting.
//
// Revision 1.2  1999/04/29 20:16:32  wsun
// Additional fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:19:49  wsun
// Major reorganization and structural changes.
//
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations
class HICondition ;

class KalmanAddHitOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      KalmanAddHitOperation( double aMaxHitChisq = 15. ) ;

      virtual ~KalmanAddHitOperation();

      // ---------- member functions ---------------------------
      void setRemoveHitsFromFit( DABoolean aFlag )
      { m_removeHitsFromFit = aFlag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      // ---------- static member functions --------------------
      static void invert5x5SymMatrix( const HepSymMatrix& input,
				      HepSymMatrix& output ) ;

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      KalmanAddHitOperation( const KalmanAddHitOperation& );
      // stop default

      // ---------- assignment operator(s) ---------------------
      const KalmanAddHitOperation& operator=(
	 const KalmanAddHitOperation& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_maxHitChisq ;
      DABoolean m_removeHitsFromFit ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "KalmanFilter/Template/KalmanAddHitOperation.cc"
//#endif

#endif /* KALMANFILTER_KALMANADDHITOPERATION_H */
