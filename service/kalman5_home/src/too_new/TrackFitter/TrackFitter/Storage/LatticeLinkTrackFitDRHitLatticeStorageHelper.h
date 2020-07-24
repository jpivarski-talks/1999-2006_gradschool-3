#if !defined(TRACKFITTERLATTICELINKTrackFitDRHitLatticeSTORAGEHELPER_H)
#define TRACKFITTERLATTICELINKTrackFitDRHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      LatticeLinkTrackFitDRHitLatticeStorageHelper
// 
// Description: Storage helper to store a TrackFitDRHitLink which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkTrackFitDRHitLatticeStorageHelper.h,v 1.6 2001/09/26 08:01:21 wsun Exp $
//
// Revision history
//
// $Log: LatticeLinkTrackFitDRHitLatticeStorageHelper.h,v $
// Revision 1.6  2001/09/26 08:01:21  wsun
// Added signed drift distance as data member.
//
// Revision 1.5  2000/10/11 06:31:22  wsun
// Added fittingWeight() to TrackFitDRHitLink.
//
// Revision 1.4  2000/08/16 19:34:07  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.3  2000/08/16 19:33:57  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.2  2000/06/26 18:17:15  wsun
// Added deliverV2 and added compression hints.
//
// Revision 1.1  2000/02/24 17:26:18  wsun
// First submission.
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "TrackFitter/TrackFitDRHitLink.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class LatticeLinkTrackFitDRHitLatticeStorageHelper :
   public SMStorageHelper< 
      SMLatticeLinkInfoBase< Lattice< TRHelixCandidateFit< H > ,
      CalibratedDRHit ,
      TrackFitDRHitLink > > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef Lattice< TRHelixCandidateFit< H > ,
	 CalibratedDRHit,
	 TrackFitDRHitLink > TrackFitDRHitLattice ;
      typedef SMLatticeLinkInfoBase< TrackFitDRHitLattice > StoreType ;

      // ---------- Constructors and destructor ----------------
      LatticeLinkTrackFitDRHitLatticeStorageHelper();
      virtual ~LatticeLinkTrackFitDRHitLatticeStorageHelper();

      // ---------- member functions ---------------------------
      virtual StoreType* deliverV1( SMSourceStream& iSource );
      virtual StoreType* deliverV2( SMSourceStream& iSource );
      virtual StoreType* deliverV3( SMSourceStream& iSource );
      virtual StoreType* deliverV4( SMSourceStream& iSource );
      virtual StoreType* deliverV5( SMSourceStream& iSource );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void implementStore( SMSinkStream& iSink,
				   const StoreType& iData );

      // ---------- protected const member functions -----------
      virtual const StoreType& getDefault() const;
      virtual DABoolean compare( const StoreType* iNewData, 
				 const StoreType& iOldData ) const;

   private:
      // ---------- Constructors and destructor ----------------
      LatticeLinkTrackFitDRHitLatticeStorageHelper( const LatticeLinkTrackFitDRHitLatticeStorageHelper<H>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkTrackFitDRHitLatticeStorageHelper& operator=( const LatticeLinkTrackFitDRHitLatticeStorageHelper<H>& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( StoreType* (LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "TrackFitter/Storage/Template/LatticeLinkTrackFitDRHitLatticeStorageHelper.cc"
#endif

#endif /* TRACKFITTERLATTICELINKTrackFitDRHitLatticeSTORAGEHELPER_H */
