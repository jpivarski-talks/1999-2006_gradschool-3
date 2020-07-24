#if !defined(KALMANFILTER_KALMANUPDATEPARAMSCONDITION_H)
#define KALMANFILTER_KALMANUPDATEPARAMSCONDITION_H
// -*- C++ -*-
//
// Package:     <KalmanFilter>
// Module:      KalmanUpdateParamsCondition
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 02:47:11 EDT 1999
// $Id: KalmanUpdateParamsCondition.h,v 1.2 2001/03/27 06:39:10 wsun Exp $
//
// Revision history
//
// $Log: KalmanUpdateParamsCondition.h,v $
// Revision 1.2  2001/03/27 06:39:10  wsun
// Added flag to ignore number of hits -- used in removal of hits from fit.
//
// Revision 1.1  1999/04/27 22:19:54  wsun
// Major reorganization and structural changes.
//

// system include files

// user include files
#include "HelixIntersection/HICondition.h"

// forward declarations

class KalmanUpdateParamsCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      KalmanUpdateParamsCondition();
      virtual ~KalmanUpdateParamsCondition();

      // ---------- member functions ---------------------------
      void setIgnoreNumberOfHits( DABoolean flag )
      { m_ignoreNumberOfHits = flag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean satisfied( const HIHelix& aHelix,
				   HIIntersectionSurface& ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      KalmanUpdateParamsCondition( const KalmanUpdateParamsCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const KalmanUpdateParamsCondition& operator=( const KalmanUpdateParamsCondition& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_ignoreNumberOfHits ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "KalmanFilter/Template/KalmanUpdateParamsCondition.cc"
//#endif

#endif /* KALMANFILTER_KALMANUPDATEPARAMSCONDITION_H */
