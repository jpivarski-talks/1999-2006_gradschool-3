#if !defined(TRACKFINDERLATTICELINKSeedTrackDRHitLatticeSTORAGEHELPER_H)
#define TRACKFINDERLATTICELINKSeedTrackDRHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      LatticeLinkSeedTrackDRHitLatticeStorageHelper
// 
// Description: Storage helper to store a SeedDRHitLink which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkSeedTrackDRHitLatticeStorageHelper.h,v 1.3 2001/10/23 19:12:57 cdj Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackDRHitLatticeStorageHelper.h,v $
// Revision 1.3  2001/10/23 19:12:57  cdj
// modified StorageHelpers so they store individual identifiers rather than lists
//
// Revision 1.2  2000/10/25 20:08:26  lyon
// Add DeliverV2 and make DeliverV1 backwards compatable
//
// Revision 1.1  2000/02/08 18:28:23  lyon
// Storage helpers
//
// Revision 1.1  1999/10/18 21:30:19  cdj
// renamed files
//
// Revision 1.1  1999/10/11 22:00:37  cdj
// added Lattice storage code
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkSeedTrackDRHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< SeedTrackDRHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< SeedTrackDRHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkSeedTrackDRHitLatticeStorageHelper();
      virtual ~LatticeLinkSeedTrackDRHitLatticeStorageHelper();

      // ---------- member functions ---------------------------
      virtual StoreType* deliverV1( SMSourceStream& iSource );
      virtual StoreType* deliverV2( SMSourceStream& iSource );
      virtual StoreType* deliverV3( SMSourceStream& iSource );

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
      LatticeLinkSeedTrackDRHitLatticeStorageHelper( const LatticeLinkSeedTrackDRHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkSeedTrackDRHitLatticeStorageHelper& operator=( const LatticeLinkSeedTrackDRHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkSeedTrackDRHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkSeedTrackDRHitLatticeStorageHelper.cc"
//#endif

#endif /* TRACKFINDERLATTICELINKSeedTrackDRHitLatticeSTORAGEHELPER_H */
