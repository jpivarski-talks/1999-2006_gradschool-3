#if !defined(DOITPROD_DOITSVZLATTICEPROXY_H)
#define DOITPROD_DOITSVZLATTICEPROXY_H
// -*- C++ -*-
//
// DoitProd:     <DoitProd>
// Module:      DoitsvzLatticeProxy
//
/**\class DoitsvzLatticeProxy DoitsvzLatticeProxy.h DoitProd/DoitsvzLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Peter Gaidarev
// Created:     Tue May 30 11:06:11 EDT 2000
// $Id: DoitSVZLatticeProxy.h,v 1.1 2000/05/30 20:45:27 pg Exp $
//
// Revision history
//
// $Log: DoitSVZLatticeProxy.h,v $
// Revision 1.1  2000/05/30 20:45:27  pg
// Added proxies for Si lattices
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"

// forward declarations

class DoitSVZLatticeProxy : public ProxyBindableTemplate< SeedTrackSVZHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef SeedTrackSVZHitLattice value_type;

      // Constructors and destructor
      DoitSVZLatticeProxy();
      virtual ~DoitSVZLatticeProxy();

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
      DoitSVZLatticeProxy( const DoitSVZLatticeProxy& );

      // assignment operator(s)
      const DoitSVZLatticeProxy& operator=( const DoitSVZLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitSVZLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DoitSVZLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProd/Template/DoitsvzLatticeProxy.cc"
//#endif

#endif /* DOITPROD_DOITSVZLATTICEPROXY_H */
