#if !defined(DUALTRACKHELICES_DUALTRACKHELICESPROXY_H)
#define DUALTRACKHELICES_DUALTRACKHELICESPROXY_H
// -*- C++ -*-
//
// Package:     <DualTrackHelicesProd>
// Module:      DualTrackHelicesProxy
//
/**\class DualTrackHelicesProxy DualTrackHelicesProxy.h DualTrackHelices/DualTrackHelicesProxy.h
 
 Description: Proxy to "handle" DualTrackHelices

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Fri Jul  7 17:21:22 EDT 2000
// $Id: DualTrackHelicesProxy.h,v 1.1.1.1 2001/02/01 17:40:13 llh14 Exp $
//
// Revision history
//
// $Log: DualTrackHelicesProxy.h,v $
// Revision 1.1.1.1  2001/02/01 17:40:13  llh14
// imported DualTrackHelicesProd
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "DualTrackHelices/DualTrackHelices.h"
#include "DualTrackHelicesProd/DualTrackHelicesProd.h"

#include "DataHandler/Frame.h"

#include "HelixIntersection/HIFitHelix.h"

#include "HelixIntersection/HIDualTrackConstraint.h"

// forward declarations
#if !( defined(LOOSES_NESTED_CLASS_DECLARATION_BUG) && defined(FRAMEACCESS_FAPTRTABLE_H) )
template < class T > class FAPtrTable ;
#endif /* LOOSES_NESTED_CLASS_DECLARATION_BUG && FRAMEACCESS_FAPTRTABLE_H */


class DualTrackHelicesProxy : public ProxyBindableTemplate< FAPtrTable< DualTrackHelices > >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef FAPtrTable< DualTrackHelices > value_type ;
      enum layers {kL1, kL2, kL3, kL4, kNumLayers};

      // Constructors and destructor
      DualTrackHelicesProxy(DualTrackHelicesProd* aDualTrackHelicesProd = 0);
      virtual ~DualTrackHelicesProxy();

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
      DualTrackHelicesProxy( const DualTrackHelicesProxy& );

      // assignment operator(s)
      const DualTrackHelicesProxy& operator=( const DualTrackHelicesProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (DualTrackHelicesProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      void addSiliconHits(Frame& iFrame, STL_VECTOR(HIFitHelix*)&
			  helixVector, const FATable<NavTrack>& 
			  eventTrackTable);

      // private const member functions

      // data members
      value_type* m_ptrTable ;

      DualTrackHelicesProd* m_DualTrackHelicesProd; 

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DualTrackHelices/Template/DualTrackHelicesProxy.cc"
//#endif

#endif /* DUALTRACKHELICES_DUALTRACKHELICESPROXY_H */
