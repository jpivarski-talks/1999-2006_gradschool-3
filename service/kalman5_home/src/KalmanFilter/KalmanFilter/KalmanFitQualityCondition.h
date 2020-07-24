#if !defined(KALMANFILTER_KALMANFITQUALITYCONDITION_H)
#define KALMANFILTER_KALMANFITQUALITYCONDITION_H
// -*- C++ -*-
//
// Package:     <KalmanFilter>
// Module:      KalmanFitQualityCondition
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 02:42:59 EDT 1999
// $Id: KalmanFitQualityCondition.h,v 1.1 1999/04/27 22:19:50 wsun Exp $
//
// Revision history
//
// $Log: KalmanFitQualityCondition.h,v $
// Revision 1.1  1999/04/27 22:19:50  wsun
// Major reorganization and structural changes.
//

// system include files

// user include files
#include "HelixIntersection/HICondition.h"
#include "HelixIntersection/HIHelix.h"

// forward declarations

class KalmanFitQualityCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      KalmanFitQualityCondition( double aChisqCut = 1e30 ) ;

      virtual ~KalmanFitQualityCondition();

      // ---------- member functions ---------------------------
      void setChisqCut( double aChisqCut )
      { m_chisqCut = aChisqCut ; }

      // ---------- const member functions ---------------------
      virtual DABoolean satisfied( const HIHelix& aHelix,
				   HIIntersectionSurface& aSurface ) const ;

      double chisqCut() const
      { return m_chisqCut ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      KalmanFitQualityCondition( const KalmanFitQualityCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const KalmanFitQualityCondition& operator=( const KalmanFitQualityCondition& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_chisqCut ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "KalmanFilter/Template/KalmanFitQualityCondition.cc"
//#endif

#endif /* KALMANFILTER_KALMANFITQUALITYCONDITION_H */
