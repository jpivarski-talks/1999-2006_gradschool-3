#if !defined(HELIXINTERSECTION_HIMSDEDXOPERATION_H)
#define HELIXINTERSECTION_HIMSDEDXOPERATION_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMSDedxOperation
// 
/**\class HIMSDedxOperation HIMSDedxOperation.h HelixIntersection/HIMSDedxOperation.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Thu Dec  2 12:59:55 EST 1999
// $Id: HIMSDedxOperation.h,v 1.5 2002/02/27 00:07:18 wsun Exp $
//
// Revision history
//
// $Log: HIMSDedxOperation.h,v $
// Revision 1.5  2002/02/27 00:07:18  wsun
// Fixed Runge-Kutta integration for mixtures.
//
// Revision 1.4  2002/01/23 22:17:46  llh14
// Added HICorrectForBFieldOperation, and small modifications to HISingleWireCylinder to incorporate the quad fields
//
// Revision 1.3  2001/08/23 03:13:00  wsun
// Fixed dE/dx calculation.
//
// Revision 1.2  2001/03/27 05:29:50  wsun
// Added reverse transport.
//
// Revision 1.1  1999/12/15 21:06:23  wsun
// Merged functionality of HIDedxOperation and HIMultScatOperation.
//

// system include files

// user include files
#include "HelixIntersection/HIHelixOperation.h"

// forward declarations
class DGConstMaterialPtr ;

class HIMSDedxOperation : public HIHelixOperation
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIMSDedxOperation( DABoolean aModifyHelixParametersForMS = false,
			 DABoolean aApplyStragglingToTrack = false,
			 DABoolean aApplyStragglingToError = true,
			 double aStragglingCoeff = 0.001,
			 DABoolean aApplyMultScat = true,
			 DABoolean aApplyDedx = true,
			 DABoolean aReverseTransport = false ) ;
      virtual ~HIMSDedxOperation();

      // ---------- member functions ---------------------------
      void setApplyMultScat( DABoolean aFlag )
      { m_applyMS = aFlag ; }

      void setApplyDedx( DABoolean aFlag )
      { m_applyDedx = aFlag ; }

      void setStragglingCoeff( double aCoeff )
      { m_stragglingCoeff = aCoeff ; }

      void setApplyStragglingToTrack( DABoolean aFlag )
      { m_applyStragglingToTrack = aFlag ; }

      void setApplyStragglingToError( DABoolean aFlag )
      { m_applyStragglingToError = aFlag ; }

      void setModifyHelixParametersForMS( DABoolean flag )
      { m_modifyHelixParametersForMS = flag ; }

      void setReverseTransport( DABoolean flag )
      { m_reverseTransport = flag ; }

      // ---------- const member functions ---------------------
      virtual DABoolean perform( 
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface ) const ;

      virtual DABoolean perform(
	 HIHelix& aHelixAfterOperation,
	 const HIHelix& aHelixBeforeOperation,
	 const HIHelix& aHelixAtLastIntersection,
	 HIIntersectionSurface& aCurrentSurface,
	 const HIIntersectionSurface& aLastSurface,
	 const DGConstMaterialPtr& aMaterial ) const ;

      DABoolean modifyHelixParametersForMS() const
      { return m_modifyHelixParametersForMS ; }

      GeV energyLossPerPathLength(
	 const DGConstMaterialPtr& aMaterial,
	 double sumOfWeights,
	 GeV kinE,
	 double gamma,
	 double massFrac ) const ;

      // ---------- static member functions --------------------
      static GeV energyLossPerPathLength( const double& atomicNumber,
					  const double& rhoZOverA,
					  const double& ionizationPotential,
					  const double& logI,
					  const double& C,
					  const double& x0,
					  const double& x1,
					  const double& a,
					  const double& gamma,
					  const double& kineticEnergy,
					  const GeV& particleMassFrac ) ;
   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIMSDedxOperation( const HIMSDedxOperation& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIMSDedxOperation& operator=( const HIMSDedxOperation& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_applyMS ;
      DABoolean m_applyDedx ;

      DABoolean m_modifyHelixParametersForMS ;

      double m_stragglingCoeff ;
      DABoolean m_applyStragglingToTrack ;
      DABoolean m_applyStragglingToError ;

      DABoolean m_reverseTransport ;

      static const double m_ionizationPotentials[ 92 ] ;
      static const double m_logIonizationPotentials[ 92 ] ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIMSDedxOperation.cc"
//#endif

#endif /* HELIXINTERSECTION_HIMSDEDXOPERATION_H */
