#if !defined(KALMANPROD_KALMANDBTRACKERVALUESPROXY_H)
#define KALMANPROD_KALMANDBTRACKERVALUESPROXY_H
// -*- C++ -*-
//
// Package:     <PivarskiProd>
// Module:      PivarskiDBTrackerValuesProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue May 11 23:19:12 EDT 1999
// $Id: PivarskiDBTrackerValuesProxy.h,v 1.1.1.1 1999/09/10 21:58:57 wsun Exp $
//
// Revision history
//
// $Log: PivarskiDBTrackerValuesProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from PivarskiFilter
//
// Revision 1.1  1999/05/14 01:11:02  wsun
// First submission.
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "CleoDB/DBTrackerValues.h"

// forward declarations

class PivarskiDBTrackerValuesProxy :
   public ProxyBindableTemplate< DBTrackerValues >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DBTrackerValues value_type;

      // Constructors and destructor
      PivarskiDBTrackerValuesProxy();
      virtual ~PivarskiDBTrackerValuesProxy();

      // member functions

      // use this form of a function when calling 'bind'
      // void boundMethod( const Record& iRecord );

      // const member functions

      // static member functions

   protected:
      // protected member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // protected const member functions

   private:
      // Constructors and destructor
      PivarskiDBTrackerValuesProxy( const PivarskiDBTrackerValuesProxy& );

      // assignment operator(s)
      const PivarskiDBTrackerValuesProxy& operator=( const PivarskiDBTrackerValuesProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (PivarskiDBTrackerValuesProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_PivarskiDBTrackerValues ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "PivarskiProd/Template/PivarskiDBTrackerValuesProxy.cc"
//#endif

#endif /* KALMANPROD_KALMANDBTRACKERVALUESPROXY_H */
