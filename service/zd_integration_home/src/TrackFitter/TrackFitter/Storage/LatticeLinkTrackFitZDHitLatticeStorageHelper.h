#if !defined(TRACKFITTERLATTICELINKTrackFitZDHitLatticeSTORAGEHELPER_H)
#define TRACKFITTERLATTICELINKTrackFitZDHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      LatticeLinkTrackFitZDHitLatticeStorageHelper
// 
// Description: Storage helper to store a TrackFitZDHitLink which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkTrackFitZDHitLatticeStorageHelper.h,v 1.1 2003/04/17 17:03:35 bkh Exp $
//
// Revision history
//
// $Log: LatticeLinkTrackFitZDHitLatticeStorageHelper.h,v $
// Revision 1.1  2003/04/17 17:03:35  bkh
// Add ZD Lattice Object & Instantiations
//
// Revision 1.6  2001/09/26 08:01:21  wsun
// Added signed drift distance as data member.
//
// Revision 1.5  2000/10/11 06:31:22  wsun
// Added fittingWeight() to TrackFitZDHitLink.
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
#include "TrackFitter/TrackFitZDHitLink.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class LatticeLinkTrackFitZDHitLatticeStorageHelper :
   public SMStorageHelper< 
      SMLatticeLinkInfoBase< Lattice< TRHelixCandidateFit< H > ,
      CalibratedZDHit ,
      TrackFitZDHitLink > > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef Lattice< TRHelixCandidateFit< H > ,
	 CalibratedZDHit,
	 TrackFitZDHitLink > TrackFitZDHitLattice ;
      typedef SMLatticeLinkInfoBase< TrackFitZDHitLattice > StoreType ;

      // ---------- Constructors and destructor ----------------
      LatticeLinkTrackFitZDHitLatticeStorageHelper();
      virtual ~LatticeLinkTrackFitZDHitLatticeStorageHelper();

      // ---------- member functions ---------------------------
      virtual StoreType* deliverV1( SMSourceStream& iSource );

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
      LatticeLinkTrackFitZDHitLatticeStorageHelper( const LatticeLinkTrackFitZDHitLatticeStorageHelper<H>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkTrackFitZDHitLatticeStorageHelper& operator=( const LatticeLinkTrackFitZDHitLatticeStorageHelper<H>& ); // stop default

      // ---------- private member functions -------------------
      void registerVersion( StoreType* (LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "TrackFitter/Storage/Template/LatticeLinkTrackFitZDHitLatticeStorageHelper.cc"
#endif

#endif /* TRACKFITTERLATTICELINKTrackFitZDHitLatticeSTORAGEHELPER_H */
