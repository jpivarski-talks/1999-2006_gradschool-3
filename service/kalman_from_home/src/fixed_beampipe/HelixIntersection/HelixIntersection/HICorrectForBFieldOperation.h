#if !defined(HELIXINTERSECTION_HICORRECTFORBFIELDOPERATION_H)
#define HELIXINTERSECTION_HICORRECTFORBFIELDOPERATION_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICorrectForBFieldOperation
// 
/**\class HICorrectForBFieldOperation HICorrectForBFieldOperation.h HelixIntersection/HICorrectForBFieldOperation.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Thu Dec 13 16:58:38 EST 2001
// $Id: HICorrectForBFieldOperation.h,v 1.1 2002/01/23 22:17:45 llh14 Exp $
//
// Revision history
//
// $Log: HICorrectForBFieldOperation.h,v $
// Revision 1.1  2002/01/23 22:17:45  llh14
// Added HICorrectForBFieldOperation, and small modifications to HISingleWireCylinder to incorporate the quad fields
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations

class HICorrectForBFieldOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICorrectForBFieldOperation(DABoolean aApplyBFieldCorrection);
      virtual ~HICorrectForBFieldOperation();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      virtual DABoolean perform( HIHelix& aHelixAfterOperation,
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
      HICorrectForBFieldOperation( const HICorrectForBFieldOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HICorrectForBFieldOperation& operator=( const HICorrectForBFieldOperation& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      DABoolean m_applyBFieldCorrection;      

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICorrectForBFieldOperation.cc"
//#endif

#endif /* HELIXINTERSECTION_HICORRECTFORBFIELDOPERATION_H */
