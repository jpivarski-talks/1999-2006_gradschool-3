#if !defined(CALIBRATEDDATA_CDRCALIBRATEDDRHITPROXY_H)
#define CALIBRATEDDATA_CDRCALIBRATEDDRHITPROXY_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CDRCalibratedDRHitProxy
// 
// Description: Proxy to "handle" CalibratedDRHit - these are the final
//               CalibratedDRHits (corrected according to the bunch finder)
//
// Usage:
//    The user triggers this proxy by asking for the CalibratedDRHit list.
//       This proxy in turn triggers CDRCalibratedDRHitPreliminaryProxy and
//       the BunchDecisionProxy.
//
// Author:      Adam Lyon
// Created:     Fri Aug 21 15:13:51 EDT 1998
// $Id: CDRCalibratedDRHitProxy.h,v 1.1.1.1 1999/09/23 02:44:15 cdj Exp $
//
// Revision history
//
// $Log: CDRCalibratedDRHitProxy.h,v $
// Revision 1.1.1.1  1999/09/23 02:44:15  cdj
// imported CalibratedDRHitProd sources
//
// Revision 1.1  1998/09/10 15:28:48  lyon
// New final DR hit proxy
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "CalibratedData/CalibratedDRHit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class CDRCalibratedDRHitProxy : 
   public ProxyTemplate< FAPtrTable< CalibratedDRHit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< CalibratedDRHit > value_type ;

      // Constructors and destructor
      CDRCalibratedDRHitProxy();
      virtual ~CDRCalibratedDRHitProxy();

      // member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // const member functions

      // static member functions

   protected:
      // protected member functions
      
      //////////////////////////////////////////////////////////////////////
      // CalibrateDRHits - Turn raw DR hits into Calibrated DR Hits.
      void CalibrateDRHits( const Record& iRecord, double tOffset = 0.0 );

      // protected const member functions

      // Data members (must be in protected area so derived classes can
      // get to it)
      value_type* m_ptrTable;


   private:
      // Constructors and destructor
      CDRCalibratedDRHitProxy( const CDRCalibratedDRHitProxy& );

      // assignment operator(s)
      const CDRCalibratedDRHitProxy& operator=( const CDRCalibratedDRHitProxy& );

      // private member functions
            

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "CalibratedData/Template/CDRCalibratedDRHitProxy.cc"
//#endif

#endif /* CALIBRATEDDATA_CDRCALIBRATEDDRHITPROXY_H */
