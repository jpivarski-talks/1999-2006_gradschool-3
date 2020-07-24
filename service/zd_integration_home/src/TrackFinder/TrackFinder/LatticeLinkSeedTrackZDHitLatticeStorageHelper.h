#if !defined(TRACKFINDERLATTICELINKSeedTrackZDHitLatticeSTORAGEHELPER_H)
#define TRACKFINDERLATTICELINKSeedTrackZDHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      LatticeLinkSeedTrackZDHitLatticeStorageHelper
// 
// Description: Storage helper to store a SeedZDHitLink which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkSeedTrackZDHitLatticeStorageHelper.h,v 1.1 2002/09/27 21:03:06 bkh Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackZDHitLatticeStorageHelper.h,v $
// Revision 1.1  2002/09/27 21:03:06  bkh
// Add ZD capability
//
// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "TrackFinder/SeedZDHitLink.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkSeedTrackZDHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< SeedTrackZDHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< SeedTrackZDHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkSeedTrackZDHitLatticeStorageHelper();
      virtual ~LatticeLinkSeedTrackZDHitLatticeStorageHelper();

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
      LatticeLinkSeedTrackZDHitLatticeStorageHelper( const LatticeLinkSeedTrackZDHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkSeedTrackZDHitLatticeStorageHelper& operator=( const LatticeLinkSeedTrackZDHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkSeedTrackZDHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkSeedTrackZDHitLatticeStorageHelper.cc"
//#endif

#endif /* TRACKFINDERLATTICELINKSeedTrackZDHitLatticeSTORAGEHELPER_H */
