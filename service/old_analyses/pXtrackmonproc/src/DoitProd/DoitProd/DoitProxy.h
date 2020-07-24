#if !defined(DOITPROD_DOITPROXY_H)
#define DOITPROD_DOITPROXY_H
// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitProxy
// 
// Description: Runs the DoitTrackFinder
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 14:43:57 EDT 1999
// $Id: DoitProxy.h,v 1.5 2001/04/05 02:02:56 lyon Exp $
//
// Revision history
//
// $Log: DoitProxy.h,v $
// Revision 1.5  2001/04/05 02:02:56  lyon
// Add new stop on kSVEfficiency stream
//
// Revision 1.4  2000/02/08 18:37:38  lyon
// Added DoitModeNumber parameter
//
// Revision 1.3  2000/01/15 20:18:01  lyon
// Fix PACKAGE in beginning #ifdef
//
// Revision 1.2  2000/01/14 02:54:34  lyon
// Add parameters to continue if hits are missing
//
// Revision 1.1.1.1  1999/09/08 18:53:32  lyon
// imported DoitProd sources
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "Doit/DoitTrackFinder.h"

// forward declarations
class DoitProd;

class DoitProxy : public ProxyBindableTemplate< DoitTrackFinder >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DoitTrackFinder value_type;

      // Constructors and destructor
      DoitProxy(DoitProd* aDoitProd);
      virtual ~DoitProxy();

      // member functions

      // use this form of a function when calling 'bind'
      void beginRun( const Record& iRecord );

      // Handle new SV efficiency constants
      void svEfficiencyConstants(const Record& iRecord );

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
      DoitProxy( const DoitProxy& );

      // assignment operator(s)
      const DoitProxy& operator=( const DoitProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_doitTrackFinder ;
      DoitTrackFinder m_theDoitTrackFinder;  // This one does the work

      // A pointer to the producer to which the proxy is attached
      DoitProd* m_doitProd;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProd/Template/DoitProxy.cc"
//#endif

#endif /* DOITPROD_DOITPROXY_H */
