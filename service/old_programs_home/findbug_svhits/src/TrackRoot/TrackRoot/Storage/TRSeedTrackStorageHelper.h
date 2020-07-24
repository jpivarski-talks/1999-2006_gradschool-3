#if !defined(TRACKROOT_TRSEEDTRACKSTORAGEHELPER_H)
#define TRACKROOT_TRSEEDTRACKSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRSeedTrackStorageHelper
// 
// Description: StorageHelper to store a TRSeedTrack
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: TRSeedTrackStorageHelper.h,v 1.1 2000/01/05 18:51:24 cdj Exp $
//
// Revision history
//
// $Log: TRSeedTrackStorageHelper.h,v $
// Revision 1.1  2000/01/05 18:51:24  cdj
// first submission
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
#include "TrackRoot/TRSeedTrack.h"

// forward declarations

class TRSeedTrackStorageHelper : public SMStorageHelper<TRSeedTrack>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      TRSeedTrackStorageHelper() ;
      //virtual ~TRSeedTrackStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const TRSeedTrack& iData );

      //functions to deliver specific storage versions of TRSeedTrack
      TRSeedTrack* deliverV1( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const TRSeedTrack& getDefault() const;
      virtual DABoolean compare( const TRSeedTrack* iNewData, 
				 const TRSeedTrack& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      TRSeedTrackStorageHelper( const TRSeedTrackStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRSeedTrackStorageHelper& operator=( const TRSeedTrackStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( TRSeedTrack* (TRSeedTrackStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* TRACKROOT_TRSEEDTRACKSTORAGEHELPER_H */
