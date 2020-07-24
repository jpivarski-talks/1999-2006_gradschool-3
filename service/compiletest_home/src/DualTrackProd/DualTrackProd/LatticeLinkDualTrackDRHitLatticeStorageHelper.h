#if !defined(DUALTRACKPRODLATTICELINKDualTrackDRHitLatticeSTORAGEHELPER_H)
#define DUALTRACKPRODLATTICELINKDualTrackDRHitLatticeSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     DualTrackProd
// Module:      LatticeLinkDualTrackDRHitLatticeStorageHelper
// 
// Description: Storage helper to store a DualTrackDRHitLink which is a Lattice link
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:46:26 EDT 1999
// $Id: LatticeLinkDualTrackDRHitLatticeStorageHelper.h,v 1.1 2003/12/15 17:51:48 mccann Exp $
//
// Revision history
//
// $Log: LatticeLinkDualTrackDRHitLatticeStorageHelper.h,v $
// Revision 1.1  2003/12/15 17:51:48  mccann
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
#include "DualTrackProd/DualTrackDRHitLink.h"
#include "DualTrackProd/DualTrackDRHitLink.h"
#include "StorageManagement/SMLatticeLinkInfoBase.h"

// forward declarations

class LatticeLinkDualTrackDRHitLatticeStorageHelper :
   public SMStorageHelper< 
           SMLatticeLinkInfoBase< DualTrackDRHitLattice > >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef SMLatticeLinkInfoBase< DualTrackDRHitLattice > StoreType;

      // ---------- Constructors and destructor ----------------
      LatticeLinkDualTrackDRHitLatticeStorageHelper();
      virtual ~LatticeLinkDualTrackDRHitLatticeStorageHelper();

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
      LatticeLinkDualTrackDRHitLatticeStorageHelper( const LatticeLinkDualTrackDRHitLatticeStorageHelper& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const LatticeLinkDualTrackDRHitLatticeStorageHelper& operator=( const LatticeLinkDualTrackDRHitLatticeStorageHelper& ); // stop default

      // ---------- private member functions -------------------
   void registerVersion( StoreType* (LatticeLinkDualTrackDRHitLatticeStorageHelper::*iMethod)( SMSourceStream&) );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/LatticeLinkDualTrackDRHitLatticeStorageHelper.cc"
//#endif

#endif /* DUALTRACKPRODLATTICELINKDualTrackDRHitLatticeSTORAGEHELPER_H */
