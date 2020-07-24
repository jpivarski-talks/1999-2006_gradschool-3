#if !defined(KALMANPROD_KALMANDBTRACKERVALUESPROXY_H)
#define KALMANPROD_KALMANDBTRACKERVALUESPROXY_H
// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannDBTrackerValuesProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue May 11 23:19:12 EDT 1999
// $Id: McCannDBTrackerValuesProxy.h,v 1.1.1.1 1999/09/10 21:58:57 wsun Exp $
//
// Revision history
//
// $Log: McCannDBTrackerValuesProxy.h,v $
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from McCannFilter
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

class McCannDBTrackerValuesProxy :
   public ProxyBindableTemplate< DBTrackerValues >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DBTrackerValues value_type;

      // Constructors and destructor
      McCannDBTrackerValuesProxy();
      virtual ~McCannDBTrackerValuesProxy();

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
      McCannDBTrackerValuesProxy( const McCannDBTrackerValuesProxy& );

      // assignment operator(s)
      const McCannDBTrackerValuesProxy& operator=( const McCannDBTrackerValuesProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (McCannDBTrackerValuesProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_McCannDBTrackerValues ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "McCannProd/Template/McCannDBTrackerValuesProxy.cc"
//#endif

#endif /* KALMANPROD_KALMANDBTRACKERVALUESPROXY_H */
