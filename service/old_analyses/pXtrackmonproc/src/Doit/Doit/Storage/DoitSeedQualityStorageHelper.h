#if !defined(DOIT_DOITSEEDQUALITYSTORAGEHELPER_H)
#define DOIT_DOITSEEDQUALITYSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     Doit
// Module:      DoitSeedQualityStorageHelper
// 
// Description: StorageHelper to store a DoitSeedQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Aug 15 23:47:22 EDT 2000
// $Id: DoitSeedQualityStorageHelper.h,v 1.4 2001/12/05 16:47:39 lyon Exp $
//
// Revision history
//
// $Log: DoitSeedQualityStorageHelper.h,v $
// Revision 1.4  2001/12/05 16:47:39  lyon
// Add more bits to the DoitStatusInfo object
//
// Revision 1.3  2001/01/05 05:37:39  lyon
// Added Doit status words
//
// Revision 1.2  2000/11/19 03:24:31  cdj
// added missing version for DoitSeedQuality storage helper
//
// Revision 1.1  2000/08/16 19:42:15  wsun
// First submission.
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "Doit/DoitSeedQuality.h"

// forward declarations

class DoitSeedQualityStorageHelper : public SMStorageHelper<DoitSeedQuality>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DoitSeedQualityStorageHelper() ;
      //virtual ~DoitSeedQualityStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const DoitSeedQuality& iData );

      //functions to deliver specific storage versions of DoitSeedQuality
      DoitSeedQuality* deliverV1( SMSourceStream& iSource );
      DoitSeedQuality* deliverV2( SMSourceStream& iSource );
      DoitSeedQuality* deliverV3( SMSourceStream& iSource );
      DoitSeedQuality* deliverV4( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const DoitSeedQuality& getDefault() const;
      virtual DABoolean compare( const DoitSeedQuality* iNewData, 
				 const DoitSeedQuality& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      DoitSeedQualityStorageHelper( const DoitSeedQualityStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DoitSeedQualityStorageHelper& operator=( const DoitSeedQualityStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( DoitSeedQuality* (DoitSeedQualityStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* DOIT_DOITSEEDQUALITYSTORAGEHELPER_H */
