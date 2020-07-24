#if !defined(TRACKROOT_TRSEEDTRACKQUALITYSTORAGEHELPER_H)
#define TRACKROOT_TRSEEDTRACKQUALITYSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRSeedTrackQualityStorageHelper
// 
// Description: StorageHelper to store a TRSeedTrackQuality
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: TRSeedTrackQualityStorageHelper.h,v 1.3 2000/10/30 17:35:31 ajm36 Exp $
//
// Revision history
//
// $Log: TRSeedTrackQualityStorageHelper.h,v $
// Revision 1.3  2000/10/30 17:35:31  ajm36
// added version of delivery for old qualities
//
// Revision 1.2  2000/10/28 20:34:51  ajm36
// added numberLayers, numberMultipleHits, useForExtrapolation to seed qualities
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
#include "TrackRoot/TRSeedTrackQuality.h"

// forward declarations

class TRSeedTrackQualityStorageHelper : public SMStorageHelper<TRSeedTrackQuality>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      TRSeedTrackQualityStorageHelper() ;
      //virtual ~TRSeedTrackQualityStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const TRSeedTrackQuality& iData );

      //functions to deliver specific storage versions of TRSeedTrackQuality
      TRSeedTrackQuality* deliverV1( SMSourceStream& iSource );
      TRSeedTrackQuality* deliverV2( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const TRSeedTrackQuality& getDefault() const;
      virtual DABoolean compare( const TRSeedTrackQuality* iNewData, 
				 const TRSeedTrackQuality& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      TRSeedTrackQualityStorageHelper( const TRSeedTrackQualityStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRSeedTrackQualityStorageHelper& operator=( const TRSeedTrackQualityStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( TRSeedTrackQuality* (TRSeedTrackQualityStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* TRACKROOT_TRSEEDTRACKQUALITYSTORAGEHELPER_H */
