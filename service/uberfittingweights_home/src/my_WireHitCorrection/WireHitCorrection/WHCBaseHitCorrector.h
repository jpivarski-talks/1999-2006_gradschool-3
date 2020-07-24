#if !defined(WIREHITCORRECTION_WHCBASEHITCORRECTOR_H)
#define WIREHITCORRECTION_WHCBASEHITCORRECTOR_H
// -*- C++ -*-
//
// Package:     WireHitCorrection
// Module:      WHCBaseHitCorrector
// 
/**\class WHCBaseHitCorrector WHCBaseHitCorrector.h WireHitCorrection/WHCBaseHitCorrector.h

 Description: Splitted and abstracted functionality of HitCorrector.  See
 DRHitCorrectorProd/HitCorrector.h for more information.  The (templated) hit
 correctors for the DR and ZD inherit from this class.

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Wed Aug 27 14:01:17 EDT 2003
// $Id: WHCBaseHitCorrector.h,v 1.2 2003/08/30 00:00:23 wsun Exp $
//
// Revision history
//
// $Log: WHCBaseHitCorrector.h,v $
// Revision 1.2  2003/08/30 00:00:23  wsun
// Added documentation.
//
// Revision 1.1.1.1  2003/08/29 23:43:27  wsun
// Imported WireHitCorrection sources.
//

// system include files

// user include files
#include "CalibratedData/CalibratedDRHit.h"

// forward declarations
class MagneticField ;
class KTHelix ;

class WHCBaseHitCorrector
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      WHCBaseHitCorrector( const MagneticField* field )
	 : m_field( field ),
	   m_disableAllCorrections( false ),
	   m_applySignalPropCorr( true ),
	   m_applyTimeOfFlightCorr( true ),
	   m_applyPulseHeightCorr( true ),
	   m_applyEntranceAngleCorr( true ) {} ;

      // virtual ~WHCBaseHitCorrector() ;

      // ---------- member functions ---------------------------
      // This overrides all other parameters except m_disableAllCorrections.
      void setApplyStandardCorrections()
      {
         m_applySignalPropCorr    = true ;
         m_applyTimeOfFlightCorr  = true ;
         m_applyPulseHeightCorr   = true ;
         m_applyEntranceAngleCorr = true ;
      }

      // This overrides ALL other parameters.
      void setDisableAllCorrections( DABoolean aFlag )
      { m_disableAllCorrections = aFlag ; }

      void setApplySignalPropCorr( DABoolean aFlag )
      { m_applySignalPropCorr = aFlag ; }

      void setApplyTimeOfFlightCorr( DABoolean aFlag )
      { m_applyTimeOfFlightCorr = aFlag ; }

      void setApplyPulseHeightCorr( DABoolean aFlag )
      { m_applyPulseHeightCorr = aFlag ; }

      void setApplyEntranceAngleCorr( DABoolean aFlag )
      { m_applyEntranceAngleCorr = aFlag ; }


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
         ) const = 0 ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

      const MagneticField* m_field;

      DABoolean m_disableAllCorrections ;
      DABoolean m_applySignalPropCorr ;
      DABoolean m_applyTimeOfFlightCorr ;
      DABoolean m_applyPulseHeightCorr ;
      DABoolean m_applyEntranceAngleCorr ;

   private:
      // ---------- Constructors and destructor ----------------
      // WHCBaseHitCorrector( const WHCBaseHitCorrector& ); // stop default

      // ---------- assignment operator(s) ---------------------
      // const WHCBaseHitCorrector& operator=( const WHCBaseHitCorrector& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "WireHitCorrection/Template/WHCBaseHitCorrector.cc"
//#endif

#endif /* WIREHITCORRECTION_WHCBASEHITCORRECTOR_H */
