#if !defined(CDOFFCAL_DFDATAHOLDER_H)
#define CDOFFCAL_DFDATAHOLDER_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DFDataHolder
// 
/**\class DFDataHolder DFDataHolder.h CDOffCal/DFDataHolder.h

 Description: Hold all the data from the Frame used by the drift function

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Feb  2 08:43:01 EST 2000
// $Id: DFDataHolder.h,v 1.4 2000/10/31 23:09:19 mclean Exp $
//
// Revision history
//
// $Log: DFDataHolder.h,v $
// Revision 1.4  2000/10/31 23:09:19  mclean
// *** empty log message ***
//
// Revision 1.3  2000/05/12 16:33:51  mclean
// Added t0global constant.
//
// Revision 1.2  2000/03/29 22:33:02  mclean
// Added hooks for drt0channelv2 and drt0hvcard
//
// Revision 1.1  2000/02/02 15:29:33  cdj
// first submission
//

// system include files

// user include files
#include "ADRGeom/ADRSenseWireStore.h"

#include "DBDRvarious.hh"
#include "DBDRadc2charge.hh"
#include "DBDRtdc2time.hh"
#include "DBDRDaqCellIDMap.hh"
#include "DBDRGarfield_DistanceCorrection.hh"
#include "DBDRGarfield_TimeCorrection.hh"
#include "DBDRWeightDrift.hh"
#include "DBDRWeightLayer.hh"
#include "DBDRWeightPhi.hh"
#include "DBDRWeightTheta.hh"
#include "DBDRWireQuality.hh"
#include "DBDRt0global.hh"
#include "DBDRt0crate.hh"
#include "DBDRt0card.hh"
#include "DBDRt0hvcard.hh"
#include "DBDRadcpedestal.hh"
#include "DBDRDriftData.hh"
#include "DBDRDriftDatav2.hh"
#include "DBDRt0channel.hh"
#include "DBDRt0channelv2.hh"

#include "FrameAccess/FAConstants.h"

// forward declarations

class DFDataHolder
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DFDataHolder() {}
      //virtual ~DFDataHolder();

      // ---------- member functions ---------------------------
      FAItem     < ADRSenseWireStore >& Geometry()     {
	 return m_Geometry; }

      FAConstants< DBDRvarious       >& various()       {
	 return m_various; }

      FAConstants< DBDRadc2charge    >& ADCpar()       {
	 return m_ADCparameters; }
      FAConstants< DBDRtdc2time      >& TDCpar()      {
	 return m_TDCparameters; }
      FAConstants< DBDRDaqCellIDMap  >& WireMap()     {
	 return m_WireMap;
      }
      FAConstants< DBDRGarfield_DistanceCorrection >& DriftCorr() {
	 return m_DriftCorrections;
      }
      FAConstants< DBDRGarfield_TimeCorrection     >& TimeCorr()  {
	 return m_TimeCorrections; }
      FAConstants< DBDRWeightDrift   >& DriftWeight() {
	 return m_DriftWeights;
      }
      FAConstants< DBDRWeightLayer   >& LayerWeight() {
	 return m_LayerWeights;
      }
      FAConstants< DBDRWeightPhi     >& PhiWeight()   {
	 return m_PhiWeights;
      }
      FAConstants< DBDRWeightTheta   >& ThetaWeight() {
	 return m_ThetaWeights;
      }
      FAConstants< DBDRWireQuality   >& WireQuality() {
	 return m_BadWires;
      }
      FAConstants< DBDRt0card        >& T0Card()      {
	 return m_CardT0;
      }
      FAConstants< DBDRt0hvcard      >& T0HVCard()    {
	 return m_HVCardT0;
      }
      FAConstants< DBDRt0global      >& T0Global()    {
	 return m_GlobalT0_nottheparameter;
      }
      FAConstants< DBDRt0crate       >& T0Crate()     {
	 return m_CrateT0;
      }
      FAConstants< DBDRadcpedestal   >& ADCped()      {
	 return m_ADCpedestals;
      }
      FAConstants< DBDRDriftData     >& DriftData()   {
	 return m_DriftData;
      }
      FAConstants< DBDRDriftDatav2   >& DriftDatav2()   {
	 return m_DriftDatav2;
      }
      FAConstants< DBDRt0channelv2   >& T0Channel()   {
	 return m_ChannelT0;
      }

      // ---------- const member functions ---------------------
      FAItem     < ADRSenseWireStore > Geometry()    const {
	 return m_Geometry; }

      FAConstants< DBDRvarious       > various()      const {
	 return m_various; }

      FAConstants< DBDRadc2charge    > ADCpar()      const {
	 return m_ADCparameters; }
      FAConstants< DBDRtdc2time      > TDCpar()      const {
	 return m_TDCparameters; }
      FAConstants< DBDRDaqCellIDMap  > WireMap()     const {
	 return m_WireMap;
      }
      FAConstants< DBDRGarfield_DistanceCorrection > DriftCorr() const {
	 return m_DriftCorrections;
      }
      FAConstants< DBDRGarfield_TimeCorrection     > TimeCorr()  const {
	 return m_TimeCorrections; }
      FAConstants< DBDRWeightDrift   > DriftWeight() const {
	 return m_DriftWeights;
      }
      FAConstants< DBDRWeightLayer   > LayerWeight() const {
	 return m_LayerWeights;
      }
      FAConstants< DBDRWeightPhi     > PhiWeight()   const {
	 return m_PhiWeights;
      }
      FAConstants< DBDRWeightTheta   > ThetaWeight() const {
	 return m_ThetaWeights;
      }
      FAConstants< DBDRWireQuality   > WireQuality() const {
	 return m_BadWires;
      }
      FAConstants< DBDRt0card        > T0Card()      const {
	 return m_CardT0;
      }
      FAConstants< DBDRt0hvcard      > T0HVCard()    const {
	 return m_HVCardT0;
      }
      FAConstants< DBDRt0global      > T0Global()    const {
	 return m_GlobalT0_nottheparameter;
      }
      FAConstants< DBDRt0crate       > T0Crate()     const {
	 return m_CrateT0;
      }
      FAConstants< DBDRadcpedestal   > ADCped()      const {
	 return m_ADCpedestals;
      }
      FAConstants< DBDRDriftData     > DriftData()   const {
	 return m_DriftData;
      }
      FAConstants< DBDRDriftDatav2   > DriftDatav2() const {
	 return m_DriftDatav2;
      }
      FAConstants< DBDRt0channelv2   > T0Channel()   const {
	 return m_ChannelT0;
      }
      
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DFDataHolder( const DFDataHolder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DFDataHolder& operator=( const DFDataHolder& ); // stop default

      // ---------- private member functions -------------------
      FAItem     < ADRSenseWireStore > m_Geometry;
      FAConstants< DBDRvarious       > m_various;
      FAConstants< DBDRadc2charge    > m_ADCparameters;
      FAConstants< DBDRtdc2time      > m_TDCparameters;
      FAConstants< DBDRDaqCellIDMap  > m_WireMap;
      FAConstants< DBDRGarfield_DistanceCorrection > m_DriftCorrections;
      FAConstants< DBDRGarfield_TimeCorrection     > m_TimeCorrections;
      FAConstants< DBDRWeightDrift   > m_DriftWeights;
      FAConstants< DBDRWeightLayer   > m_LayerWeights;
      FAConstants< DBDRWeightPhi     > m_PhiWeights;
      FAConstants< DBDRWeightTheta   > m_ThetaWeights;
      FAConstants< DBDRWireQuality   > m_BadWires;
      FAConstants< DBDRt0card        > m_CardT0;
      FAConstants< DBDRt0hvcard      > m_HVCardT0;
      FAConstants< DBDRt0global      > m_GlobalT0_nottheparameter;
      FAConstants< DBDRt0crate       > m_CrateT0;
      FAConstants< DBDRDriftData     > m_DriftData;
      FAConstants< DBDRDriftDatav2   > m_DriftDatav2;
      FAConstants< DBDRadcpedestal   > m_ADCpedestals;
      FAConstants< DBDRt0channelv2   > m_ChannelT0;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "CDOffCal/Template/DFDataHolder.cc"
//#endif

#endif /* CDOFFCAL_DFDATAHOLDER_H */
