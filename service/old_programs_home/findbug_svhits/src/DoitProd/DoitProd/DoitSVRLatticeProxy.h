#if !defined(DOITPROD_DOITSVRLATTICEPROXY_H)
#define DOITPROD_DOITSVRLATTICEPROXY_H
// -*- C++ -*-
//
// DoitProd:     <DoitProd>
// Module:      DoitSVRLatticeProxy
//
/**\class DoitSVRLatticeProxy DoitSVRLatticeProxy.h DoitProd/DoitSVRLatticeProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Peter Gaidarev
// Created:     Tue May 30 11:06:11 EDT 2000
// $Id: DoitSVRLatticeProxy.h,v 1.1 2000/05/30 20:45:27 pg Exp $
//
// Revision history
//
// $Log: DoitSVRLatticeProxy.h,v $
// Revision 1.1  2000/05/30 20:45:27  pg
// Added proxies for Si lattices
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"

// forward declarations

class DoitSVRLatticeProxy : public ProxyBindableTemplate< SeedTrackSVRHitLattice >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef SeedTrackSVRHitLattice value_type;

      // Constructors and destructor
      DoitSVRLatticeProxy();
      virtual ~DoitSVRLatticeProxy();

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
      DoitSVRLatticeProxy( const DoitSVRLatticeProxy& );

      // assignment operator(s)
      const DoitSVRLatticeProxy& operator=( const DoitSVRLatticeProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DoitSVRLatticeProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DoitSVRLattice ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DoitProd/Template/DoitSVRLatticeProxy.cc"
//#endif

#endif /* DOITPROD_DOITSVRLATTICEPROXY_H */
