#if !defined(KALMANFILTER_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H)
#define KALMANFILTER_TRTRACKCANDIDATEFITQUALITYSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     McCannFilter
// Module:      McCannCandidateQualityStorageHelper
// 
// Description: StorageHelper to store a McCannCandidateQuality
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 00:32:04 EDT 2000
// $Id: McCannCandidateQualityStorageHelper.h,v 1.4 2001/02/17 22:11:39 cleo3 Exp $
//
// Revision history
//
// $Log: McCannCandidateQualityStorageHelper.h,v $
// Revision 1.4  2001/02/17 22:11:39  cleo3
// added include of SMDeliverer in StorageHelper (needed by linux)
//
// Revision 1.3  2000/12/14 16:19:13  wsun
// Added #hits, avg residuals, and rms residuals by subdetector to fit quality.
//
// Revision 1.2  2000/12/02 01:33:09  wsun
// Added moreBadHits and pullOfNextWorstHit to McCannCandidateQuality.
//
// Revision 1.1  2000/08/16 20:07:28  wsun
// First submission.
//
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "McCannFilter/McCannCandidateQuality.h"
#include "StorageManagement/SMDeliverer.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class McCannCandidateQualityStorageHelper :
   public SMStorageHelper<McCannCandidateQuality<H> >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      McCannCandidateQualityStorageHelper() ;
      //virtual ~McCannCandidateQualityStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const McCannCandidateQuality<H>& iData);

      //functions to deliver specific storage versions of McCannCandidateQuality
      McCannCandidateQuality<H>* deliverV1( SMSourceStream& iSource );
      McCannCandidateQuality<H>* deliverV2( SMSourceStream& iSource );
      McCannCandidateQuality<H>* deliverV3( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const McCannCandidateQuality<H>& getDefault() const;
      virtual DABoolean compare(
	 const McCannCandidateQuality<H>* iNewData, 
	 const McCannCandidateQuality<H>& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      McCannCandidateQualityStorageHelper( const McCannCandidateQualityStorageHelper<H>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const McCannCandidateQualityStorageHelper& operator=( const McCannCandidateQualityStorageHelper<H>& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( McCannCandidateQuality<H>* (McCannCandidateQualityStorageHelper<H>::*iMethod)( SMSourceStream&) )
      {
	 registerDeliverVersion(
	    new SMDeliverer< McCannCandidateQuality<H> ,  
	    McCannCandidateQualityStorageHelper<H> >( iMethod, this ) ); 
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
