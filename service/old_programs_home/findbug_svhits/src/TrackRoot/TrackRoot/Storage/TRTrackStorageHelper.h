#if !defined(TRACKROOT_TRTRACKSTORAGEHELPER_H)
#define TRACKROOT_TRTRACKSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackStorageHelper
// 
// Description: StorageHelper to store a TRTrack
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Wed Nov 3 16:55:38 EDT 1999
// $Id: TRTrackStorageHelper.h,v 1.1 2000/01/05 18:51:06 cdj Exp $
//
// Revision history
//
// $Log: TRTrackStorageHelper.h,v $
// Revision 1.1  2000/01/05 18:51:06  cdj
// moved to new directory
//
// Revision 1.1  1999/11/04 14:45:00  mkl
// added storage helper for TRTrack
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "TrackRoot/TRTrack.h"

// forward declarations

class TRTrackStorageHelper : public SMStorageHelper<TRTrack>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      TRTrackStorageHelper() ;
      //virtual ~TRTrackStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const TRTrack& iData );

      //functions to deliver specific storage versions of TRTrack
      TRTrack* deliverV1( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const TRTrack& getDefault() const;
      virtual DABoolean compare( const TRTrack* iNewData, 
				 const TRTrack& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      TRTrackStorageHelper( const TRTrackStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRTrackStorageHelper& operator=( const TRTrackStorageHelper& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( TRTrack* (TRTrackStorageHelper::*iMethod)( SMSourceStream&) );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* TRACKROOT_TRTRACKSTORAGEHELPER_H */
