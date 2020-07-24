#if !defined(KALMANPROD_KALMANTRACKFITTERPROXY_H)
#define KALMANPROD_KALMANTRACKFITTERPROXY_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanTrackFitterProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon May 10 17:59:11 EDT 1999
// $Id: KalmanTrackFitterProxy.h,v 1.3 2000/01/11 20:39:41 wsun Exp $
//
// Revision history
//
// $Log: KalmanTrackFitterProxy.h,v $
// Revision 1.3  2000/01/11 20:39:41  wsun
// Replaced control flags with a pointer to KalmanProd.
//
// Revision 1.2  1999/10/08 23:23:58  wsun
// * Added param to optimize inward+outward fitting for a single mass hypo.
// * Added param to optimize fitting of all tracks to all hypotheses.
//
// Revision 1.1.1.1  1999/09/10 21:58:56  wsun
// Transfered from KalmanFilter
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
#include "KalmanFilter/KalmanTrackFitter.h"

// forward declarations
class KalmanProd ;

class KalmanTrackFitterProxy : public ProxyBindableTemplate<KalmanTrackFitter>
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef KalmanTrackFitter value_type;

      // Constructors and destructor
      KalmanTrackFitterProxy( KalmanProd* aKalmanProd = 0 ) ;
      virtual ~KalmanTrackFitterProxy();

      // member functions

      // These functions get bound to the beginrun stream.
      void updateMagField( const Record& iRecord ) ;
      void updateSiGeom( const Record& iRecord ) ;
      void updateIRGeom( const Record& iRecord ) ;
      void updateDRGeom( const Record& iRecord ) ;

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
      KalmanTrackFitterProxy( const KalmanTrackFitterProxy& );

      // assignment operator(s)
      const KalmanTrackFitterProxy& operator=( const KalmanTrackFitterProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (KalmanTrackFitterProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_kalmanTrackFitter ;

      // a pointer to the Producer to which this Proxy is attached.
      KalmanProd* m_kalmanProd ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "KalmanProd/Template/KalmanTrackFitterProxy.cc"
//#endif

#endif /* KALMANPROD_KALMANTRACKFITTERPROXY_H */
