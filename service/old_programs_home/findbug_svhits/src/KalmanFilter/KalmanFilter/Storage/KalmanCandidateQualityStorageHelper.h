#if !defined(KALMANFILTER_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H)
#define KALMANFILTER_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanCandidateQualityStorageHelper
// 
// Description: StorageHelper to store a KalmanCandidateQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 00:32:04 EDT 2000
// $Id: KalmanCandidateQualityStorageHelper.h,v 1.4 2001/02/17 22:11:39 cleo3 Exp $
//
// Revision history
//
// $Log: KalmanCandidateQualityStorageHelper.h,v $
// Revision 1.4  2001/02/17 22:11:39  cleo3
// added include of SMDeliverer in StorageHelper (needed by linux)
//
// Revision 1.3  2000/12/14 16:19:13  wsun
// Added #hits, avg residuals, and rms residuals by subdetector to fit quality.
//
// Revision 1.2  2000/12/02 01:33:09  wsun
// Added moreBadHits and pullOfNextWorstHit to KalmanCandidateQuality.
//
// Revision 1.1  2000/08/16 20:07:28  wsun
// First submission.
//
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "KalmanFilter/KalmanCandidateQuality.h"
#include "StorageManagement/SMDeliverer.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class KalmanCandidateQualityStorageHelper :
   public SMStorageHelper<KalmanCandidateQuality<H> >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      KalmanCandidateQualityStorageHelper() ;
      //virtual ~KalmanCandidateQualityStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const KalmanCandidateQuality<H>& iData);

      //functions to deliver specific storage versions of KalmanCandidateQuality
      KalmanCandidateQuality<H>* deliverV1( SMSourceStream& iSource );
      KalmanCandidateQuality<H>* deliverV2( SMSourceStream& iSource );
      KalmanCandidateQuality<H>* deliverV3( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const KalmanCandidateQuality<H>& getDefault() const;
      virtual DABoolean compare(
	 const KalmanCandidateQuality<H>* iNewData, 
	 const KalmanCandidateQuality<H>& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      KalmanCandidateQualityStorageHelper( const KalmanCandidateQualityStorageHelper<H>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const KalmanCandidateQualityStorageHelper& operator=( const KalmanCandidateQualityStorageHelper<H>& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( KalmanCandidateQuality<H>* (KalmanCandidateQualityStorageHelper<H>::*iMethod)( SMSourceStream&) )
      {
	 registerDeliverVersion(
	    new SMDeliverer< KalmanCandidateQuality<H> ,  
	    KalmanCandidateQualityStorageHelper<H> >( iMethod, this ) ); 
      }

      //NOTE: registerVersion is defined here rather than using the
      //  SM_REGISTER_VERSION_DEF in the .cc file  to avoid a problem
      //  with solaris

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* KALMANFILTER_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H */
