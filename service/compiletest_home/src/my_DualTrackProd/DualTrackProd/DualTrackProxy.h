// -*- C++ -*-
#if !defined(DUALTRACKPROD_DUALTRACKPROXY_H)
#define DUALTRACKPROD_DUALTRACKPROXY_H
//
// Package:     <DualTrackProd>
// Module:      DualTrackProxy
//
/**\class DualTrackProxy DualTrackProxy.h DualTrackProd/DualTrackProxy.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 09:29:10 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "DualTrackProd/DualTrack.h"
#include "CleoDB/DBCandidate.h"

// forward declarations

class DualTrackProxy : public ProxyBindableTemplate<DualTrack>
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DualTrack value_type;

      // Constructors and destructor
      DualTrackProxy(DBCandidate::Hypo massHypothesis = DBCandidate::kChargedPion,
                     HepVector3D& pVirtualPhoton = HepVector3D(0., 0., 0.),
                     DABoolean pointConstraint = false,
                     DABoolean ptConstraint = false,
                     DABoolean pzConstraint = false,
                     DABoolean printOutP = false);
      virtual ~DualTrackProxy();

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
      DualTrackProxy( const DualTrackProxy& );

      // assignment operator(s)
      const DualTrackProxy& operator=( const DualTrackProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DualTrackProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_DualTrack ;

      DBCandidate::Hypo m_massHypothesis;
      HepVector3D m_pVirtualPhoton;
      DABoolean m_pointConstraint;
      DABoolean m_ptConstraint;
      DABoolean m_pzConstraint;
      DABoolean m_printOutP;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackProd/Template/DualTrackProxy.cc"
//#endif

#endif /* DUALTRACKPROD_DUALTRACKPROXY_H */
