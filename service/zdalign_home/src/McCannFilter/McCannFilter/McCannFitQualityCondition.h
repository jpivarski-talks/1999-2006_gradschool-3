#if !defined(KALMANFILTER_KALMANFITQUALITYCONDITION_H)
#define KALMANFILTER_KALMANFITQUALITYCONDITION_H
// -*- C++ -*-
//
// Package:     <McCannFilter>
// Module:      McCannFitQualityCondition
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 02:42:59 EDT 1999
// $Id: McCannFitQualityCondition.h,v 1.1 1999/04/27 22:19:50 wsun Exp $
//
// Revision history
//
// $Log: McCannFitQualityCondition.h,v $
// Revision 1.1  1999/04/27 22:19:50  wsun
// Major reorganization and structural changes.
//

// system include files

// user include files
#include "HelixIntersection/HICondition.h"
#include "HelixIntersection/HIHelix.h"

// forward declarations

class McCannFitQualityCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      McCannFitQualityCondition( double aChisqCut = 1e30 ) ;

      virtual ~McCannFitQualityCondition();

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
      McCannFitQualityCondition( const McCannFitQualityCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const McCannFitQualityCondition& operator=( const McCannFitQualityCondition& ); // stop default

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
//# include "McCannFilter/Template/McCannFitQualityCondition.cc"
//#endif

#endif /* KALMANFILTER_KALMANFITQUALITYCONDITION_H */
