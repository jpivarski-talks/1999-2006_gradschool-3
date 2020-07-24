#if !defined(CALIBRATEDDATA_CDPRELIMINARYID_H)
#define CALIBRATEDDATA_CDPRELIMINARYID_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CDPreliminaryID
// 
/**\class CDPreliminaryID CDPreliminaryID.h CalibratedData/CDPreliminaryID.h

 Description: A holder for the CDRCalibratedDataPreliminaryProxy 2ndary ID 
              string.

	      You probably will never want to make one of these. Just use:
	      CDPreliminaryID::kSecondaryLabel

 Usage:
    <usage>

*/
//
// Author:      Adam Lyon
// Created:     Tue Feb  8 11:25:44 EST 2000
// $Id: CDPreliminaryID.h,v 1.1 2000/02/08 18:25:56 lyon Exp $
//
// Revision history
//
// $Log: CDPreliminaryID.h,v $
// Revision 1.1  2000/02/08 18:25:56  lyon
// Moved from CalibratedDRHitProd
//

// system include files

// user include files

// forward declarations

class CDPreliminaryID
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const char* const kSecondaryLabel;

      // ---------- Constructors and destructor ----------------
      CDPreliminaryID();
      virtual ~CDPreliminaryID();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      CDPreliminaryID( const CDPreliminaryID& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const CDPreliminaryID& operator=( const CDPreliminaryID& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "CalibratedData/Template/CDPreliminaryID.cc"
//#endif

#endif /* CALIBRATEDDATA_CDPRELIMINARYID_H */
