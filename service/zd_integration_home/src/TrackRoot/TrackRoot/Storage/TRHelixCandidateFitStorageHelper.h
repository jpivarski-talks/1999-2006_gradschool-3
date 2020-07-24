#if !defined(TRACKROOT_TRHELIXCANDIDATEFITSTORAGEHELPER_H)
#define TRACKROOT_TRHELIXCANDIDATEFITSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFitStorageHelper
// 
// Description: StorageHelper to store a TRHelixCandidateFit
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: TRHelixCandidateFitStorageHelper.h,v 1.2 2003/01/30 15:57:23 cleo3 Exp $
//
// Revision history
//
// $Log: TRHelixCandidateFitStorageHelper.h,v $
// Revision 1.2  2003/01/30 15:57:23  cleo3
// added included needed for g++ template instantiation
//
// Revision 1.1  2000/01/05 18:51:24  cdj
// first submission
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "StorageManagement/SMDeliverer.h"
#include "TrackRoot/TRHelixCandidateFit.h"

// forward declarations
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class TRHelixCandidateFitStorageHelper : public SMStorageHelper<TRHelixCandidateFit<H> >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      TRHelixCandidateFitStorageHelper() ;
      //virtual ~TRHelixCandidateFitStorageHelper();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const TRHelixCandidateFit<H>& iData );

      //functions to deliver specific storage versions of TRHelixCandidateFit
      TRHelixCandidateFit<H>* deliverV1( SMSourceStream& iSource );

      // ---------- protected const member functions -----------
      virtual const TRHelixCandidateFit<H>& getDefault() const;
      virtual DABoolean compare( const TRHelixCandidateFit<H>* iNewData, 
				 const TRHelixCandidateFit<H>& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      TRHelixCandidateFitStorageHelper( const TRHelixCandidateFitStorageHelper<H>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRHelixCandidateFitStorageHelper& operator=( const TRHelixCandidateFitStorageHelper<H>& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( TRHelixCandidateFit<H>* (TRHelixCandidateFitStorageHelper<H>::*iMethod)( SMSourceStream&) ) {
registerDeliverVersion( new SMDeliverer< TRHelixCandidateFit<H> ,  
			                 TRHelixCandidateFitStorageHelper<H> >( iMethod, this ) ); 
      }        
      //NOTE: registerVersion is defined here rather than using the
      //  SM_REGISTER_VERSION_DEF in the .cc file  to avoid a problem
      //  with solaris

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* TRACKROOT_TRHELIXCANDIDATEFITSTORAGEHELPER_H */
