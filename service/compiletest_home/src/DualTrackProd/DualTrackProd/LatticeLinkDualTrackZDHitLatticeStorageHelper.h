#if !defined(DUALTRACKPRODLATTICELINKDualTrackZDHitLatticeSTORAGEHELPER_H)
#define DUALTRACKPRODLATTICELINKDualTrackZDHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      LatticeLinkDualTrackZDHitLatticeStorageHelper
// 
// Description: Storage helper to store a DualTrackZDHitLink which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkDualTrackZDHitLatticeStorageHelper.h,v 1.1 2003/12/15 17:51:49 mccann Exp $
//
// Revision history
//
// $Log: LatticeLinkDualTrackZDHitLatticeStorageHelper.h,v $
// Revision 1.1  2003/12/15 17:51:49  mccann
// adding hit lattices for dual-constrained residuals
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
#include "DualTrackProd/DualTrackZDHitLink.h"
#include "DualTrackProd/DualTrackZDHitLink.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkDualTrackZDHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< DualTrackZDHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< DualTrackZDHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkDualTrackZDHitLatticeStorageHelper();
      virtual ~LatticeLinkDualTrackZDHitLatticeStorageHelper();

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
      LatticeLinkDualTrackZDHitLatticeStorageHelper( const LatticeLinkDualTrackZDHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkDualTrackZDHitLatticeStorageHelper& operator=( const LatticeLinkDualTrackZDHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkDualTrackZDHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkDualTrackZDHitLatticeStorageHelper.cc"
//#endif

#endif /* DUALTRACKPRODLATTICELINKDualTrackZDHitLatticeSTORAGEHELPER_H */
