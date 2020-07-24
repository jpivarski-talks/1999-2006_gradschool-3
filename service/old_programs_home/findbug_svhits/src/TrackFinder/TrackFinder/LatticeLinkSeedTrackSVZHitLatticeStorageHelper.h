#if !defined(LATTICELINKSeedTrackSVZHitLatticeSTORAGEHELPER_H)
#define LATTICELINKSeedTrackSVZHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     
// Module:      LatticeLinkSeedTrackSVZHitLatticeStorageHelper
// 
// Description: Storage helper to store a DefaultLinkData which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkSeedTrackSVZHitLatticeStorageHelper.h,v 1.2 2001/10/23 19:12:58 cdj Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackSVZHitLatticeStorageHelper.h,v $
// Revision 1.2  2001/10/23 19:12:58  cdj
// modified StorageHelpers so they store individual identifiers rather than lists
//
// Revision 1.1  2000/02/23 16:11:04  lyon
// Added SVZ lattice storage helper
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
#include "Lattice/LatticeLink.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkSeedTrackSVZHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< SeedTrackSVZHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< SeedTrackSVZHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkSeedTrackSVZHitLatticeStorageHelper();
      virtual ~LatticeLinkSeedTrackSVZHitLatticeStorageHelper();

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
      LatticeLinkSeedTrackSVZHitLatticeStorageHelper( const LatticeLinkSeedTrackSVZHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkSeedTrackSVZHitLatticeStorageHelper& operator=( const LatticeLinkSeedTrackSVZHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkSeedTrackSVZHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkSeedTrackSVZHitLatticeStorageHelper.cc"
//#endif

#endif /* LATTICELINKSeedTrackSVZHitLatticeSTORAGEHELPER_H */
