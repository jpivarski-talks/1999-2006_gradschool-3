#if !defined(KALMANFILTER_KALMANUPDATEPARAMSOPERATION_H)
#define KALMANFILTER_KALMANUPDATEPARAMSOPERATION_H
// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      McCannUpdateParamsOperation
// 
// Description: 
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Aug 17 13:57:41 EDT 1999
// $Id: McCannUpdateParamsOperation.h,v 1.2 2001/03/27 06:39:22 wsun Exp $
//
// Revision history
//
// $Log: McCannUpdateParamsOperation.h,v $
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

class McCannUpdateParamsOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      McCannUpdateParamsOperation(
	 HICondition* aUpdateRphiParamsCondition = 0,
	 HICondition* aUpdateZParamsCondition = 0 ) ;

      virtual ~McCannUpdateParamsOperation();

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
      McCannUpdateParamsOperation( const McCannUpdateParamsOperation& );
      // stop default

      // ---------- assignment operator(s) ---------------------
      const McCannUpdateParamsOperation& operator=(
	 const McCannUpdateParamsOperation& ); // stop default

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
//# include "McCannFilter/Template/McCannUpdateParamsOperation.cc"
//#endif

#endif /* KALMANFILTER_KALMANUPDATEPARAMSOPERATION_H */
