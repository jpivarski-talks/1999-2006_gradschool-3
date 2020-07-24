#if !defined(LATTICELINKSeedTrackCathodeHitLatticeSTORAGEHELPER_H)
#define LATTICELINKSeedTrackCathodeHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     
// Module:      LatticeLinkSeedTrackCathodeHitLatticeStorageHelper
// 
// Description: Storage helper to store a DefaultLinkData which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkSeedTrackCathodeHitLatticeStorageHelper.h,v 1.1 2000/02/08 18:28:23 lyon Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackCathodeHitLatticeStorageHelper.h,v $
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
#include "Lattice/LatticeLink.h"
#include "TrackFinder/SeedTrackCathodeHitLattice.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkSeedTrackCathodeHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkSeedTrackCathodeHitLatticeStorageHelper();
      virtual ~LatticeLinkSeedTrackCathodeHitLatticeStorageHelper();

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
      LatticeLinkSeedTrackCathodeHitLatticeStorageHelper( const LatticeLinkSeedTrackCathodeHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkSeedTrackCathodeHitLatticeStorageHelper& operator=( const LatticeLinkSeedTrackCathodeHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkSeedTrackCathodeHitLatticeStorageHelper.cc"
//#endif

#endif /* LATTICELINKSeedTrackCathodeHitLatticeSTORAGEHELPER_H */
