#if !defined(TRACKFITTERLATTICELINKTrackFitSVZHitLatticeSTORAGEHELPER_H)
#define TRACKFITTERLATTICELINKTrackFitSVZHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      LatticeLinkTrackFitSVZHitLatticeStorageHelper
// 
// Description: Storage helper to store a TrackFitSVZHitLink which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkTrackFitSVZHitLatticeStorageHelper.h,v 1.3 2000/08/16 19:34:08 wsun Exp $
//
// Revision history
//
// $Log: LatticeLinkTrackFitSVZHitLatticeStorageHelper.h,v $
// Revision 1.3  2000/08/16 19:34:08  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.2  2000/08/16 19:33:58  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.1  2000/02/24 17:26:19  wsun
// First submission.
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "TrackFitter/TrackFitSVZHitLink.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class LatticeLinkTrackFitSVZHitLatticeStorageHelper :
   public SMStorageHelper< 
      SMLatticeLinkInfoBase< Lattice< TRHelixCandidateFit< H > ,
      CalibratedSVZHit ,
      TrackFitSVZHitLink > > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef Lattice< TRHelixCandidateFit< H > ,
	 CalibratedSVZHit,
	 TrackFitSVZHitLink > TrackFitSVZHitLattice ;
      typedef SMLatticeLinkInfoBase< TrackFitSVZHitLattice > StoreType ;

      // ---------- Constructors and destructor ----------------
      LatticeLinkTrackFitSVZHitLatticeStorageHelper();
      virtual ~LatticeLinkTrackFitSVZHitLatticeStorageHelper();

      // ---------- member functions ---------------------------
      virtual StoreType* deliverV1( SMSourceStream& iSource );
      virtual StoreType* deliverV2( SMSourceStream& iSource );

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
      LatticeLinkTrackFitSVZHitLatticeStorageHelper( const LatticeLinkTrackFitSVZHitLatticeStorageHelper<H>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkTrackFitSVZHitLatticeStorageHelper& operator=( const LatticeLinkTrackFitSVZHitLatticeStorageHelper<H>& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( StoreType* (LatticeLinkTrackFitSVZHitLatticeStorageHelper<H>::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "TrackFitter/Storage/Template/LatticeLinkTrackFitSVZHitLatticeStorageHelper.cc"
#endif

#endif /* TRACKFITTERLATTICELINKTrackFitSVZHitLatticeSTORAGEHELPER_H */
