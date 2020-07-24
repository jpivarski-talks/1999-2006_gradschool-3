#if !defined(HELIXINTERSECTION_HIMOVEITERATEOPERATION_H)
#define HELIXINTERSECTION_HIMOVEITERATEOPERATION_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMoveIterateOperation
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Wed May 19 23:47:24 EDT 1999
// $Id: HIMoveIterateOperation.h,v 1.3 1999/08/27 20:12:00 wsun Exp $
//
// Revision history
//
// $Log: HIMoveIterateOperation.h,v $
// Revision 1.3  1999/08/27 20:12:00  wsun
// New signature for HIHelixOperation::perform().
//
// Revision 1.2  1999/06/18 19:25:36  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/06/02 23:04:14  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations

class HIMoveIterateOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIMoveIterateOperation( HIHelixOperation* aOperation = 0 );
      virtual ~HIMoveIterateOperation();

      // ---------- member functions ---------------------------
      void setStepArcLength( Meters aStepArcLength )
      { m_stepArcLength = aStepArcLength ; }

      void setNumberIterations( int aNumber )
      { m_numberIterations = aNumber ; }

      // ---------- const member functions ---------------------
      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIMoveIterateOperation( const HIMoveIterateOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIMoveIterateOperation& operator=( const HIMoveIterateOperation& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HIHelixOperation* m_operation ;

      Meters m_stepArcLength ;
      int m_numberIterations ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIMoveIterateOperation.cc"
//#endif

#endif /* HELIXINTERSECTION_HIMOVEITERATEOPERATION_H */
