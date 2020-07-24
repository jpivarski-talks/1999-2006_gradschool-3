#if !defined(KALMANPROD_KALMANTRACKFITTERPROXY_H)
#define KALMANPROD_KALMANTRACKFITTERPROXY_H
// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannTrackFitterProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 17:59:11 EDT 1999
// $Id: McCannTrackFitterProxy.h,v 1.5 2003/09/10 20:59:52 wsun Exp $
//
// Revision history
//
// $Log: McCannTrackFitterProxy.h,v $
// Revision 1.5  2003/09/10 20:59:52  wsun
// Replaced updateXXGeom() functions bound to geometry streams with checks of
// Record::cacheIdentifier() in faultHandler."
//
// Revision 1.4  2003/09/09 23:51:19  wsun
// Added updateZDGeom().
//
// Revision 1.3  2000/01/11 20:39:41  wsun
// Replaced control flags with a pointer to McCannProd.
//
// Revision 1.2  1999/10/08 23:23:58  wsun
// * Added param to optimize inward+outward fitting for a single mass hypo.
// * Added param to optimize fitting of all tracks to all hypotheses.
//
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from McCannFilter
//
// Revision 1.6  1999/08/30 21:51:54  wsun
// New constructor with parameters for dE/dx straggling.
//
// Revision 1.5  1999/07/07 16:46:29  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.4  1999/06/30 23:52:32  wsun
// Added updateSiGeom() function bound to beginrun.
//
// Revision 1.3  1999/06/24 22:44:15  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.2  1999/06/02 23:07:32  wsun
// Improved low-momentum fitting.
//
// Revision 1.1  1999/05/14 01:11:06  wsun
// First submission.
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "McCannFilter/McCannTrackFitter.h"

// forward declarations
class McCannProd ;

class McCannTrackFitterProxy : public ProxyBindableTemplate<McCannTrackFitter>
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef McCannTrackFitter value_type;

      // Constructors and destructor
      McCannTrackFitterProxy( McCannProd* aMcCannProd = 0 ) ;
      virtual ~McCannTrackFitterProxy();

      // member functions

      // These functions get bound to the beginrun or the appropriate
      // aligned geometry stream.
      void updateMagField( const Record& iRecord ) ;
//       void updateSiGeom( const Record& iRecord ) ;
//       void updateIRGeom( const Record& iRecord ) ;
//       void updateDRGeom( const Record& iRecord ) ;
//       void updateZDGeom( const Record& iRecord ) ;

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
      McCannTrackFitterProxy( const McCannTrackFitterProxy& );

      // assignment operator(s)
      const McCannTrackFitterProxy& operator=( const McCannTrackFitterProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (McCannTrackFitterProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_kalmanTrackFitter ;

      // a pointer to the Producer to which this Proxy is attached.
      McCannProd* m_kalmanProd ;

      unsigned long m_siCacheIdentifier ;
      unsigned long m_irCacheIdentifier ;
      unsigned long m_drCacheIdentifier ;
      unsigned long m_zdCacheIdentifier ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "McCannProd/Template/McCannTrackFitterProxy.cc"
//#endif

#endif /* KALMANPROD_KALMANTRACKFITTERPROXY_H */
