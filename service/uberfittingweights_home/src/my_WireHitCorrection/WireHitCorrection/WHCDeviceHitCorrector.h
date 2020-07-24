#if !defined(WIREHITCORRECTION_WHCDEVICEHITCORRECTOR_H)
#define WIREHITCORRECTION_WHCDEVICEHITCORRECTOR_H
// -*- C++ -*-
//
// Package:     WireHitCorrection
// Module:      WHCDeviceHitCorrector
// 
/**\class WHCDeviceHitCorrector WHCDeviceHitCorrector.h WireHitCorrection/WHCDeviceHitCorrector.h

 Description: Splitted and abstracted functionality of HitCorrector.  See
 DRHitCorrectorProd/HitCorrector.h for more information.  This class defines
 the device-specific wire hit correctors.

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Wed Aug 27 14:01:26 EDT 2003
// $Id: WHCDeviceHitCorrector.h,v 1.2 2003/08/30 00:00:24 wsun Exp $
//
// Revision history
//
// $Log: WHCDeviceHitCorrector.h,v $
// Revision 1.2  2003/08/30 00:00:24  wsun
// Added documentation.
//
// Revision 1.1.1.1  2003/08/29 23:43:27  wsun
// Imported WireHitCorrection sources.
//

// system include files

// user include files
#include "WireHitCorrection/WHCBaseHitCorrector.h"

// forward declarations

// DF = drift function, WS = wire store
template< class DF, class WS >
class WHCDeviceHitCorrector : public WHCBaseHitCorrector
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      WHCDeviceHitCorrector( const DF* calibration,
			     const WS* geometry,
			     const MagneticField* field );
      // virtual ~WHCDeviceHitCorrector();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual void correctDriftTimeAndDist(
         const KTHelix&               xtrack,                    // input
         const CalibratedDRHit&       hit,                       // input
         GeV                          mass,                      // input
         Meters                       signeddrift,               // input
         double                       averagedCharge,            // input
         CalibratedTimeHit::PicoSec&  correctedTime,             // OUTPUT
         Meters&                      correctedDriftDist,        // OUTPUT
         Meters&                      resolutionValue,           // OUTPUT
	 DABoolean                    reverseCorrection = false  // input
         ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void correctDriftTimeAndDist(
         const KTHelix&               xtrack,                   // input
         const CalibratedDRHit&       hit,                      // input
         GeV                          mass,                     // input
         Meters                       signeddrift,              // input
         CalibratedTimeHit::PicoSec&  correctedTime,            // OUTPUT
         Meters&                      correctedDriftDist,       // OUTPUT
	 DABoolean                    reverseCorrection = false // input
         ) const ;

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      // WHCDeviceHitCorrector( const WHCDeviceHitCorrector& ); // stop default

      // ---------- assignment operator(s) ---------------------
      // const WHCDeviceHitCorrector& operator=( const WHCDeviceHitCorrector& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const DF* m_calibration;
      const WS* m_geometry;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "WireHitCorrection/Template/WHCDeviceHitCorrector.cc"
//#endif

#endif /* WIREHITCORRECTION_WHCDEVICEHITCORRECTOR_H */
