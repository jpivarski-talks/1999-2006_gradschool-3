#if !defined(CHISQFITTER_CHISQADDHITOPERATION_H)
#define CHISQFITTER_CHISQADDHITOPERATION_H
// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      ChisqAddHitOperation
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Wed Jun  9 03:13:43 EDT 1999
// $Id: ChisqAddHitOperation.h,v 1.3 2000/02/22 17:23:08 wsun Exp $
//
// Revision history
//
// $Log: ChisqAddHitOperation.h,v $
// Revision 1.3  2000/02/22 17:23:08  wsun
// Added flag for disabling hit rejection.
//
// Revision 1.2  1999/08/27 21:02:03  wsun
// Update to new HIHelixOperation::perform() and TrackFitDRHitLink.
//
// Revision 1.1.1.1  1999/06/16 02:46:47  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations

class ChisqAddHitOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ChisqAddHitOperation( double aMaxHitChisq = 1.e10,
			    DABoolean aRejectHits = true );
      virtual ~ChisqAddHitOperation();

      // ---------- member functions ---------------------------
      void setMaxHitChisq( double aChisq ) { m_maxHitChisq = aChisq ; }
      void setRejectHits( DABoolean aFlag ) { m_rejectHits = aFlag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      double maxHitChisq() const { return m_maxHitChisq ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ChisqAddHitOperation( const ChisqAddHitOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ChisqAddHitOperation& operator=( const ChisqAddHitOperation& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_maxHitChisq ;
      DABoolean m_rejectHits ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ChisqFitter/Template/ChisqAddHitOperation.cc"
//#endif

#endif /* CHISQFITTER_CHISQADDHITOPERATION_H */
