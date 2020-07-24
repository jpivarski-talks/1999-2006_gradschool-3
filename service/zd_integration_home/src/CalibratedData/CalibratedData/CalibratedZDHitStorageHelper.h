#if !defined(CALIBRATEDDATA_CALIBRATEDZDHITSTORAGEHELPER_H)
#define CALIBRATEDDATA_CALIBRATEDZDHITSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      CalibratedZDHitStorageHelper
// 
// Description: StorageHelper to store a CalibratedZDHit
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: CalibratedZDHitStorageHelper.h,v 1.1 2002/09/27 20:30:06 bkh Exp $
//
// Revision history
//
// $Log: CalibratedZDHitStorageHelper.h,v $
// Revision 1.1  2002/09/27 20:30:06  bkh
// Add ZD
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "CalibratedData/CalibratedZDHit.h"

// forward declarations

class CalibratedZDHitStorageHelper : public SMStorageHelper<CalibratedZDHit>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      CalibratedZDHitStorageHelper() ;
      //virtual ~CalibratedZDHitStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const CalibratedZDHit& iData );

      //functions to deliver specific storage versions of CalibratedZDHit
      CalibratedZDHit* deliverV1( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const CalibratedZDHit& getDefault() const;
      virtual DABoolean compare( const CalibratedZDHit* iNewData, 
				 const CalibratedZDHit& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      CalibratedZDHitStorageHelper( const CalibratedZDHitStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const CalibratedZDHitStorageHelper& operator=( const CalibratedZDHitStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( CalibratedZDHit* (CalibratedZDHitStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* CALIBRATEDDATA_CALIBRATEDZDHITSTORAGEHELPER_H */
