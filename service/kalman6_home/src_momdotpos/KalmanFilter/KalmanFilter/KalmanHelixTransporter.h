#if !defined(KALMANFILTER_KALMANHELIXTRANSPORTER_H)
#define KALMANFILTER_KALMANHELIXTRANSPORTER_H
// -*- C++ -*-
//
// Package:     <KalmanFilter>
// Module:      KalmanHelixTransporter
// 
/**\class KalmanHelixTransporter KalmanHelixTransporter.h KalmanFilter/KalmanHelixTransporter.h

 Description: Facility for transporting a helix through material.

 Usage: The member function transportThroughMaterial(...) propagates a helix
 through the detector's material volumes, applying energy loss and multiple
 scattering at each boundary.  Multiple scattering only affects the error
 matrix; energy loss affects both the track parameters and the error matrix.
 These two effects can be disabled independently.

*/
//
// Author:      Werner Sun
// Created:     Sat Feb 10 20:45:30 EST 2001
// $Id: KalmanHelixTransporter.h,v 1.1 2001/03/27 06:33:40 wsun Exp $
//
// Revision history
//
// $Log: KalmanHelixTransporter.h,v $
// Revision 1.1  2001/03/27 06:33:40  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIMSDedxOperation.h"

// forward declarations
class HIIntersectionSurface ;
class KTHelix ;
class HepPoint3D ;
class Frame ;

#include "STLUtility/fwd_vector.h"

class KalmanHelixTransporter
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      KalmanHelixTransporter( Frame& aFrame );
      virtual ~KalmanHelixTransporter();

      // ---------- member functions ---------------------------
      void generateSurfaces( Frame& theFrame ) ;

      void setPrintDiagnostics( DABoolean flag )
      {
	 m_printDiagnostics = flag ;
	 m_msdedxOperation.setPrintDiagnostics( flag ) ;
      }

      void setApplyDedx( DABoolean flag )
      { m_msdedxOperation.setApplyDedx( flag ) ; }

      void setApplyStraggling( DABoolean flag )
      { m_msdedxOperation.setApplyStragglingToError( flag ) ; }

      void setApplyMultScat( DABoolean flag )
      { m_msdedxOperation.setApplyMultScat( flag ) ; }

      // Set to true to REMOVE multiple scattering contribution to error
      // matrix.  I.e. errors are DEFLATED at each step.  Also at each step,
      // to get the error matrix right, the energy loss is applied at the
      // helix' previous position and then retransported to the current surface
      void setReverseTransport( DABoolean flag )
      { m_msdedxOperation.setReverseTransport( flag ) ; }

      // ---------- const member functions ---------------------

      // Returns false if transport fails or if error matrix becomes ill-
      // behaved (negative diagonal elements or correlation terms too large).
      DABoolean transportThroughMaterial(
	 KTHelix& aHelixToTransport,
	 GeV aMass,
	 HepPoint3D& aDestinationRefPt ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      KalmanHelixTransporter( const KalmanHelixTransporter& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const KalmanHelixTransporter& operator=( const KalmanHelixTransporter& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_surfacesOK ;
      DABoolean m_printDiagnostics ;
      HIMSDedxOperation m_msdedxOperation ;
      STL_VECTOR( HIIntersectionSurface* )* m_surfaces ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "KalmanFilter/Template/KalmanHelixTransporter.cc"
//#endif

#endif /* KALMANFILTER_KALMANHELIXTRANSPORTER_H */
