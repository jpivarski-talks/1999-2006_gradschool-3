#if !defined(CALIBRATEDDATA_CALIBRATEDZDHITPROXY_H)
#define CALIBRATEDDATA_CALIBRATEDZDHITPROXY_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CalibratedZDHitProxy
// 
// Description: Proxy to "handle" CalibratedZDHit 
//
// Usage:
//    The user triggers this proxy by asking for the CalibratedZDHit list.
//
// Author:      Brian Heltsley
// Created:     Fri Aug 21 15:13:51 EDT 1998
// $Id: CalibratedZDHitProxy.h,v 1.1.1.1 2002/09/27 20:34:03 bkh Exp $
//
// Revision history
//
// $Log: CalibratedZDHitProxy.h,v $
// Revision 1.1.1.1  2002/09/27 20:34:03  bkh
// imported CalibratedZDHitProd sources
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "CalibratedData/CalibratedZDHit.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class CalibratedZDHitProxy : 
   public ProxyTemplate< FAPtrTable< CalibratedZDHit > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< CalibratedZDHit > value_type ;

      // Constructors and destructor
      CalibratedZDHitProxy();
      virtual ~CalibratedZDHitProxy();

      // member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // const member functions

      // static member functions

   protected:
      // protected member functions
      
      //////////////////////////////////////////////////////////////////////
      // CalibrateZDHits - Turn raw ZD hits into Calibrated ZD Hits.
      void CalibrateZDHits( const Record& iRecord, double tOffset = 0.0 );

      // protected const member functions

      // Data members (must be in protected area so derived classes can
      // get to it)
      value_type* m_ptrTable;


   private:
      // Constructors and destructor
      CalibratedZDHitProxy( const CalibratedZDHitProxy& );

      // assignment operator(s)
      const CalibratedZDHitProxy& operator=( const CalibratedZDHitProxy& );

      // private member functions
            

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

#endif /* CALIBRATEDDATA_CALIBRATEDZDHITPROXY_H */
