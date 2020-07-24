#if !defined(CHISQFITPRODUCER_KALMANDBTRACKERVALUESPROXY_H)
#define CHISQFITPRODUCER_KALMANDBTRACKERVALUESPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      ChisqDBTrackerValuesProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue May 11 23:19:12 EDT 1999
// $Id: ChisqDBTrackerValuesProxy.h,v 1.1.1.1 1999/06/16 02:48:26 wsun Exp $
//
// Revision history
//
// $Log: ChisqDBTrackerValuesProxy.h,v $
// Revision 1.1.1.1  1999/06/16 02:48:26  wsun
// First submission.
//
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "CleoDB/DBTrackerValues.h"

// forward declarations

class ChisqDBTrackerValuesProxy :
   public ProxyBindableTemplate< DBTrackerValues >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DBTrackerValues value_type;

      // Constructors and destructor
      ChisqDBTrackerValuesProxy();
      virtual ~ChisqDBTrackerValuesProxy();

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
      ChisqDBTrackerValuesProxy( const ChisqDBTrackerValuesProxy& );

      // assignment operator(s)
      const ChisqDBTrackerValuesProxy& operator=( const ChisqDBTrackerValuesProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (ChisqDBTrackerValuesProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ChisqDBTrackerValues ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFilter/Template/ChisqDBTrackerValuesProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_KALMANDBTRACKERVALUESPROXY_H */
