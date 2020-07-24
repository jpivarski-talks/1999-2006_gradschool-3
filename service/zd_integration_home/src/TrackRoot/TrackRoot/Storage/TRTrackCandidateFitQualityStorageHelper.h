#if !defined(TRACKROOT_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H)
#define TRACKROOT_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackCandidateFitQualityStorageHelper
// 
// Description: StorageHelper to store a TRTrackCandidateFitQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Thu Feb 24 00:08:21 EST 2000
// $Id: TRTrackCandidateFitQualityStorageHelper.h,v 1.4 2003/01/30 15:57:24 cleo3 Exp $
//
// Revision history
//
// $Log: TRTrackCandidateFitQualityStorageHelper.h,v $
// Revision 1.4  2003/01/30 15:57:24  cleo3
// added included needed for g++ template instantiation
//
// Revision 1.3  2000/12/14 16:01:29  wsun
// Added #hits, average residuals, and rms residuals by subdetector.
//
// Revision 1.2  2000/08/01 23:29:37  wsun
// Updated to new TRTrackCandidateFitQuality.
//
// Revision 1.1  2000/02/25 01:08:23  wsun
// First submission.
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "StorageManagement/SMDeliverer.h"
#include "TrackRoot/TRTrackCandidateFitQuality.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class TRTrackCandidateFitQualityStorageHelper :
   public SMStorageHelper<TRTrackCandidateFitQuality<H> >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      TRTrackCandidateFitQualityStorageHelper() ;
      //virtual ~TRTrackCandidateFitQualityStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const TRTrackCandidateFitQuality<H>& iData);

      //functions to deliver specific storage versions of TRTrackCandidateFitQuality
      TRTrackCandidateFitQuality<H>* deliverV1( SMSourceStream& iSource );
      TRTrackCandidateFitQuality<H>* deliverV2( SMSourceStream& iSource );
      TRTrackCandidateFitQuality<H>* deliverV3( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const TRTrackCandidateFitQuality<H>& getDefault() const;
      virtual DABoolean compare(
	 const TRTrackCandidateFitQuality<H>* iNewData, 
	 const TRTrackCandidateFitQuality<H>& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      TRTrackCandidateFitQualityStorageHelper( const TRTrackCandidateFitQualityStorageHelper<H>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRTrackCandidateFitQualityStorageHelper& operator=( const TRTrackCandidateFitQualityStorageHelper<H>& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( TRTrackCandidateFitQuality<H>* (TRTrackCandidateFitQualityStorageHelper<H>::*iMethod)( SMSourceStream&) )
      {
	 registerDeliverVersion(
	    new SMDeliverer< TRTrackCandidateFitQuality<H> ,  
	    TRTrackCandidateFitQualityStorageHelper<H> >( iMethod, this ) ); 
      }

      //NOTE: registerVersion is defined here rather than using the
      //  SM_REGISTER_VERSION_DEF in the .cc file  to avoid a problem
      //  with solaris

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* TRACKROOT_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H */
