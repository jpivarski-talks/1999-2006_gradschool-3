#if !defined(CALIBRATEDDATA_CALIBRATEDDRHITSTORAGEHELPER_H)
#define CALIBRATEDDATA_CALIBRATEDDRHITSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      CalibratedDRHitStorageHelper
// 
// Description: StorageHelper to store a CalibratedDRHit
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: CalibratedDRHitStorageHelper.h,v 1.2 2001/11/29 22:11:54 bkh Exp $
//
// Revision history
//
// $Log: CalibratedDRHitStorageHelper.h,v $
// Revision 1.2  2001/11/29 22:11:54  bkh
// Add raw charge and time to object
// Add raw charge and time to storage helper
// Fix operator== object to not just use identifier
//
// Revision 1.1  2000/03/03 15:14:03  lyon
// Storage helpers
//
// Revision 1.1  1999/10/18 18:13:05  cdj
// first submission
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "CalibratedData/CalibratedDRHit.h"

// forward declarations

class CalibratedDRHitStorageHelper : public SMStorageHelper<CalibratedDRHit>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      CalibratedDRHitStorageHelper() ;
      //virtual ~CalibratedDRHitStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const CalibratedDRHit& iData );

      //functions to deliver specific storage versions of CalibratedDRHit
      CalibratedDRHit* deliverV1( SMSourceStream& iSource );
      CalibratedDRHit* deliverV2( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const CalibratedDRHit& getDefault() const;
      virtual DABoolean compare( const CalibratedDRHit* iNewData, 
				 const CalibratedDRHit& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      CalibratedDRHitStorageHelper( const CalibratedDRHitStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const CalibratedDRHitStorageHelper& operator=( const CalibratedDRHitStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( CalibratedDRHit* (CalibratedDRHitStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* CALIBRATEDDATA_CALIBRATEDDRHITSTORAGEHELPER_H */
