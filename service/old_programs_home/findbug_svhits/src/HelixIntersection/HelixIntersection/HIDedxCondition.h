#if !defined(HELIXINTERSECTION_HIDEDXCONDITION_H)
#define HELIXINTERSECTION_HIDEDXCONDITION_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIDedxCondition
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Wed May 19 18:26:45 EDT 1999
// $Id: HIDedxCondition.h,v 1.2 1999/06/18 19:25:34 wsun Exp $
//
// Revision history
//
// $Log: HIDedxCondition.h,v $
// Revision 1.2  1999/06/18 19:25:34  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/06/02 23:02:14  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HICondition.h"

// forward declarations
class HIMoveIterateOperation ;

class HIDedxCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIDedxCondition( double aMaxFractionalMomentumChange = 0.0008,
		       HIMoveIterateOperation* = 0 ) ;
      virtual ~HIDedxCondition();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // Returns true if change in momentum less than 0.1%
      virtual DABoolean satisfied( const HIHelix& aHelix,
                                   HIIntersectionSurface& ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIDedxCondition( const HIDedxCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIDedxCondition& operator=( const HIDedxCondition& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_maxFracDP ;
      HIMoveIterateOperation* m_moveIterateOperation ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIDedxCondition.cc"
//#endif

#endif /* HELIXINTERSECTION_HIDEDXCONDITION_H */
