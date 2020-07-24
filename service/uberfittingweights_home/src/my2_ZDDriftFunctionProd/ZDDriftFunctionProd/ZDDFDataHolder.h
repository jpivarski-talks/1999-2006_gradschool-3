#if !defined(ZDDRIFTFUNCTION_ZDDFDATAHOLDER_H)
#define ZDDRIFTFUNCTION_ZDDFDATAHOLDER_H
// -*- C++ -*-
//
// Package:     <ZDDriftFunctionProd>
// Module:      ZDDFDataHolder
// 
/**\class DFDataHolder DFDataHolder.h ZDDriftFunctionProd/DFDataHolder.h

 Description: Hold all the data from the Frame used by the drift function

 Usage:
    <usage>

*/
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDFDataHolder.h,v 1.2 2003/10/10 17:36:43 mccann Exp $
//
// Revision history
//
// $Log: ZDDFDataHolder.h,v $
// Revision 1.2  2003/10/10 17:36:43  mccann
// Totally vamped the fittingweights representation and delivery to use the new ZDWeight2* constants
//
// Revision 1.1.1.1  2003/06/26 19:02:49  huanggs
// imported ZDDriftFunctionProd source
//
// Revision 1.1  2003/04/24 18:08:16  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:57  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//
//

// system include files

// user include files
#include "AZDGeom/AZDSenseWireStore.h"

//waiting#include "DBZDWirePropagation.hh" //from "DBZDvarious.hh"
#include "DBZDadc2charge.hh"
#include "DBZDDaqCellIDMap.hh"
//waiting#include "DBZDGarfield_DistanceCorrection.hh"
//waiting#include "DBZDGarfield_TimeCorrection.hh"

// #include "DBZDWeightDrift.hh"
// #include "DBZDWeightLayer.hh"
// #include "DBZDWeightPhi.hh"
// #include "DBZDWeightTheta.hh"
#include "DBZDWeight2LayerDriftEntAng.hh"
#include "DBZDWeight2LayerTheta.hh"
#include "DBZDWeight2LayerPhi.hh"

#include "DBZDWireQuality.hh"
#include "DBZDt0global.hh"
#include "DBZDt0crate.hh"
#include "DBZDt0card.hh"
#include "DBZDt0hvcard.hh"
#include "DBZDadcpedestal.hh"
#include "DBZDDriftData.hh"
#include "DBZDt0channel.hh"

#include "FrameAccess/FAConstants.h"
#include "FrameAccess/FANonSeqConstants.h"

// forward declarations

class ZDDFDataHolder
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ZDDFDataHolder() {}
      //virtual ~ZDDFDataHolder();

      // ---------- member functions ---------------------------
      FAItem     < AZDSenseWireStore >& Geometry()     {
	 return m_Geometry; }

//      FAConstants< DBZDWirePropagation >& WirePropagation() {
//	 return m_WirePropagation; }

      FAConstants< DBZDadc2charge    >& ADCpar()       {
	 return m_ADCparameters; }
      FAConstants< DBZDDaqCellIDMap  >& WireMap()     {
	 return m_WireMap;
      }
//      FAConstants< DBZDGarfield_DistanceCorrection >& DriftCorr() {
//	 return m_DriftCorrections;
//      }
//      FAConstants< DBZDGarfield_TimeCorrection     >& TimeCorr()  {
//	 return m_TimeCorrections; }

//       FAConstants< DBZDWeightDrift   >& DriftWeight() {
// 	 return m_DriftWeights;
//       }
//       FAConstants< DBZDWeightLayer   >& LayerWeight() {
// 	 return m_LayerWeights;
//       }
//       FAConstants< DBZDWeightPhi     >& PhiWeight()   {
// 	 return m_PhiWeights;
//       }
//       FAConstants< DBZDWeightTheta   >& ThetaWeight() {
// 	 return m_ThetaWeights;
//       }

      FANonSeqConstants<DBZDWeight2LayerDriftEntAng>& LayerDriftEntAngWeight() {
	 return m_LayerDriftEntAngWeight;
      }
      FANonSeqConstants<DBZDWeight2LayerTheta>& LayerThetaWeight() {
	 return m_LayerThetaWeight;
      }
      FANonSeqConstants<DBZDWeight2LayerPhi>& LayerPhiWeight() {
	 return m_LayerPhiWeight;
      }

// done

      FAConstants< DBZDWireQuality   >& WireQuality() {
	 return m_BadWires;
      }
      FAConstants< DBZDt0card        >& T0Card()      {
	 return m_CardT0;
      }
      FAConstants< DBZDt0hvcard      >& T0HVCard()    {
	 return m_HVCardT0;
      }
      FAConstants< DBZDt0global      >& T0Global()    {
	 return m_GlobalT0_nottheparameter;
      }
      FAConstants< DBZDt0crate       >& T0Crate()     {
	 return m_CrateT0;
      }
      FAConstants< DBZDadcpedestal   >& ADCped()      {
	 return m_ADCpedestals;
      }
      FAConstants< DBZDDriftData   >& DriftData()   {
	 return m_DriftData;
      }
      FAConstants< DBZDt0channel   >& T0Channel()   {
	 return m_ChannelT0;
      }

      // ---------- const member functions ---------------------
      FAItem     < AZDSenseWireStore > Geometry()    const {
	 return m_Geometry; }

//      FAConstants< DBZDWirePropagation > WirePropagation() const {
//	 return m_WirePropagation; }

      FAConstants< DBZDadc2charge    > ADCpar()      const {
	 return m_ADCparameters; }
      FAConstants< DBZDDaqCellIDMap  > WireMap()     const {
	 return m_WireMap;
      }
//      FAConstants< DBZDGarfield_DistanceCorrection > DriftCorr() const {
//	 return m_DriftCorrections;
//      }
//      FAConstants< DBZDGarfield_TimeCorrection     > TimeCorr()  const {
//	 return m_TimeCorrections; }

//       FAConstants< DBZDWeightDrift   > DriftWeight() const {
// 	 return m_DriftWeights;
//       }
//       FAConstants< DBZDWeightLayer   > LayerWeight() const {
// 	 return m_LayerWeights;
//       }
//       FAConstants< DBZDWeightPhi     > PhiWeight()   const {
// 	 return m_PhiWeights;
//       }
//       FAConstants< DBZDWeightTheta   > ThetaWeight() const {
// 	 return m_ThetaWeights;
//       }

//       FANonSeqConstants<DBZDWeight2LayerDriftEntAng> LayerDriftEntAngWeight() const {
// 	 return m_LayerDriftEntAngWeight;
//       }
//       FANonSeqConstants<DBZDWeight2LayerTheta> LayerThetaWeight() const {
// 	 return m_LayerThetaWeight;
//       }
//       FANonSeqConstants<DBZDWeight2LayerPhi> LayerPhiWeight() const {
// 	 return m_LayerPhiWeight;
//       }

// done

      FAConstants< DBZDWireQuality   > WireQuality() const {
	 return m_BadWires;
      }
      FAConstants< DBZDt0card        > T0Card()      const {
	 return m_CardT0;
      }
      FAConstants< DBZDt0hvcard      > T0HVCard()    const {
	 return m_HVCardT0;
      }
      FAConstants< DBZDt0global      > T0Global()    const {
	 return m_GlobalT0_nottheparameter;
      }
      FAConstants< DBZDt0crate       > T0Crate()     const {
	 return m_CrateT0;
      }
      FAConstants< DBZDadcpedestal   > ADCped()      const {
	 return m_ADCpedestals;
      }
      FAConstants< DBZDDriftData   > DriftData() const {
	 return m_DriftData;
      }
      FAConstants< DBZDt0channel   > T0Channel()   const {
	 return m_ChannelT0;
      }
      
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ZDDFDataHolder( const ZDDFDataHolder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ZDDFDataHolder& operator=( const ZDDFDataHolder& ); // stop default

      // ---------- private member functions -------------------
      FAItem     < AZDSenseWireStore > m_Geometry;
//      FAConstants< DBZDWirePropagation > m_WirePropagation;
      FAConstants< DBZDadc2charge    > m_ADCparameters;
      FAConstants< DBZDDaqCellIDMap  > m_WireMap;
//      FAConstants< DBZDGarfield_DistanceCorrection > m_DriftCorrections;
//      FAConstants< DBZDGarfield_TimeCorrection     > m_TimeCorrections;

//       FAConstants< DBZDWeightDrift   > m_DriftWeights;
//       FAConstants< DBZDWeightLayer   > m_LayerWeights;
//       FAConstants< DBZDWeightPhi     > m_PhiWeights;
//       FAConstants< DBZDWeightTheta   > m_ThetaWeights;
      FANonSeqConstants<DBZDWeight2LayerDriftEntAng> m_LayerDriftEntAngWeight;
      FANonSeqConstants<DBZDWeight2LayerTheta> m_LayerThetaWeight;
      FANonSeqConstants<DBZDWeight2LayerPhi> m_LayerPhiWeight;

      FAConstants< DBZDWireQuality   > m_BadWires;
      FAConstants< DBZDt0card        > m_CardT0;
      FAConstants< DBZDt0hvcard      > m_HVCardT0;
      FAConstants< DBZDt0global      > m_GlobalT0_nottheparameter;
      FAConstants< DBZDt0crate       > m_CrateT0;
      FAConstants< DBZDDriftData     > m_DriftData;
      FAConstants< DBZDadcpedestal   > m_ADCpedestals;
      FAConstants< DBZDt0channel     > m_ChannelT0;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ZDDriftFunctionProd/Template/DFDataHolder.cc"
//#endif

#endif /* ZDDRIFTFUNCTION_ZDDFDATAHOLDER_H */
