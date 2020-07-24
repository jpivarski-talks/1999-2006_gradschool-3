#if !defined(KALMANFILTER_KALMANUPDATEPARAMSOPERATION_H)
#define KALMANFILTER_KALMANUPDATEPARAMSOPERATION_H
// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanUpdateParamsOperation
// 
// Description: 
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Aug 17 13:57:41 EDT 1999
// $Id: KalmanUpdateParamsOperation.h,v 1.2 2001/03/27 06:39:22 wsun Exp $
//
// Revision history
//
// $Log: KalmanUpdateParamsOperation.h,v $
// Revision 1.2  2001/03/27 06:39:22  wsun
// Added flag to always update.
//
// Revision 1.1  1999/08/27 20:32:20  wsun
// First submission.
//
//
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations
class HICondition ;

class KalmanUpdateParamsOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      KalmanUpdateParamsOperation(
	 HICondition* aUpdateRphiParamsCondition = 0,
	 HICondition* aUpdateZParamsCondition = 0 ) ;

      virtual ~KalmanUpdateParamsOperation();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      void setAlwaysUpdate( DABoolean flag )
      { m_alwaysUpdate = flag ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      KalmanUpdateParamsOperation( const KalmanUpdateParamsOperation& );
      // stop default

      // ---------- assignment operator(s) ---------------------
      const KalmanUpdateParamsOperation& operator=(
	 const KalmanUpdateParamsOperation& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HICondition* m_updateRphiParamsCondition ;
      HICondition* m_updateZParamsCondition ;
      DABoolean m_alwaysUpdate ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "KalmanFilter/Template/KalmanUpdateParamsOperation.cc"
//#endif

#endif /* KALMANFILTER_KALMANUPDATEPARAMSOPERATION_H */
