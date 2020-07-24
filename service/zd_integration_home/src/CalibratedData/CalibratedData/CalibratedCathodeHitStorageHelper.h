#if !defined(CALIBRATEDDATA_CALIBRATEDCATHODEHITSTORAGEHELPER_H)
#define CALIBRATEDDATA_CALIBRATEDCATHODEHITSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      CalibratedCathodeHitStorageHelper
// 
// Description: StorageHelper to store a CalibratedCathodeHit
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: CalibratedCathodeHitStorageHelper.h,v 1.1 2000/03/03 15:14:03 lyon Exp $
//
// Revision history
//
// $Log: CalibratedCathodeHitStorageHelper.h,v $
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
#include "CalibratedData/CalibratedCathodeHit.h"

// forward declarations

class CalibratedCathodeHitStorageHelper : public SMStorageHelper<CalibratedCathodeHit>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      CalibratedCathodeHitStorageHelper() ;
      //virtual ~CalibratedCathodeHitStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const CalibratedCathodeHit& iData );

      //functions to deliver specific storage versions of CalibratedCathodeHit
      CalibratedCathodeHit* deliverV1( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const CalibratedCathodeHit& getDefault() const;
      virtual DABoolean compare( const CalibratedCathodeHit* iNewData, 
				 const CalibratedCathodeHit& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      CalibratedCathodeHitStorageHelper( const CalibratedCathodeHitStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const CalibratedCathodeHitStorageHelper& operator=( const CalibratedCathodeHitStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( CalibratedCathodeHit* (CalibratedCathodeHitStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* CALIBRATEDDATA_CALIBRATEDCATHODEHITSTORAGEHELPER_H */
