// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStoreProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alexander Undrus
// Created:     Sat Jan  9 13:40:27 EST 1999
// $Id: ASiStoreProxy.cc,v 1.11 2000/05/16 16:34:47 ajm36 Exp $
//
// Revision history
//
// $Log: ASiStoreProxy.cc,v $
// Revision 1.11  2000/05/16 16:34:47  ajm36
// made several ASiStore functions faster by caching data
//
// Revision 1.10  2000/04/10 15:43:05  pg
// Chips numbered from 1 to 976. Active element can now be extracted from both BaseGeometry and SVAlignment streams
//
// Revision 1.9  2000/03/28 15:47:09  pg
// Changed all coordinate transformations to use aligned geometry.
//
// Revision 1.8  1999/07/12 18:44:09  cleo3
//  function ASiStore::strip is restored, copy constructor of ASiSensorInfo becomes private again, in ASiSensorInfo::strip a const_cast construction is implemented
//
// Revision 1.7  1999/07/11 03:05:53  cleo3
//  removed function ASiStore::strip ; in the method ASiSensorInfo::strip return nonconstant ASiStrip object ; added explicit public copy constructors to ASiSensorInfo and ASiStrip
//
// Revision 1.6  1999/07/09 17:34:45  cleo3
//  arguments returned by reference are declared const
//
// Revision 1.5  1999/07/03 22:27:31  cleo3
//  ASiSensorAddress is transferred from ASiSensor package, ASiStorePro becomes producer, all references to ASiSensor are eliminated
//
// Revision 1.4  1999/06/28 22:37:47  cleo3
//  print diagnostics if param printkey = 1
//
// Revision 1.3  1999/03/24 02:51:32  cleo3
// added new ASiStoreConstants.h, added functions with STL vector of subelements, added functions returning various objects
//
// Revision 1.2  1999/03/13 03:17:16  undrus
//  introduced new ASiStrip object, simplified name of functions, oveloaded assignment operators, simplified access to the detector elements
//
// Revision 1.1.1.1  1999/01/23 02:28:02  undrus
// imported c3tr sources
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>
#include <iostream.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <set>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "ASiStorePro/ASiStoreProxy.h"
#include "ASiStorePro/ASiStorePro.h"
#include "DetectorGeometry/DGDetectorStore.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ASiStorePro.ASiStoreProxy" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ASiStoreProxy::ASiStoreProxy(const ASiStorePro& iProducer)
   : m_SiStore( 0 ), m_producer(iProducer)
{
}

// ASiStoreProxy::ASiStoreProxy( const ASiStoreProxy& iproxy )
// {
//   *this = iproxy;
// }

ASiStoreProxy::~ASiStoreProxy()
{
   delete m_SiStore ;
}

//
// assignment operators
//
// const ASiStoreProxy& ASiStoreProxy::operator=( const ASiStoreProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
ASiStoreProxy::invalidateCache()
{
   delete m_SiStore ;
   m_SiStore = 0 ;
}

const ASiStoreProxy::value_type*
ASiStoreProxy::faultHandler( const Record& iRecord,
			     const DataKey& iKey )
{

   report( NOTICE, kFacilityString )
      << "beginning of faultHandler of ASiStoreProxy "
      << endl;
   //Extract the Si geometry

   FAItem< DGDetectorStore > spSiStore;

   extract( iRecord, spSiStore,
	    "Si" );

   if( ! spSiStore.valid() ){
      report( WARNING, kFacilityString )
	 << "Could not find DGDetectorStore Si in record "
	 << iRecord.stream().value() 
	 << endl;
      return 0;
   }


   if( spSiStore.valid() ){
      report( NOTICE, kFacilityString )
	 << " ASiStoreProxy: found DGDetectorStore in record "
	 << iRecord.stream().value()
	 << endl;
   }

//Now get SiStore
   m_SiStore = new ASiStore( spSiStore );
   
   if( 0 == m_SiStore ){
      report( EMERGENCY, kFacilityString )
	 << "out of memory"
	 << endl; 
      assert( false );
      exit(1);
   }
   else {
   }
   
////////////////
   
   if( m_producer.printkey() == 1 )
   {
      
      cout << " Some results for sensor 318 " << endl;
      
      SmallCount l318 = (*m_SiStore).layerForSensor(318);
      cout << " layer: " << l318 << endl;
      SmallCount ldd318 = (*m_SiStore).ladderForSensor(318);
      cout << " ladder in that layer: " << ldd318 << endl;
      SmallCount ld318 = (*m_SiStore).ladderInLayerForSensor(318);
      cout << " ladder in that layer: " << ld318 << endl;;
      SmallCount hb318 = (*m_SiStore).hybridInLadderForSensor(318);
      cout << " hybrid in thar ladder: " << hb318 << endl;
      SmallCount sn318 = (*m_SiStore).inHybridForSensor(318);
      cout << " sensor in thar hybrid: " << sn318 << endl;
      HepPoint3D csn318 = (*m_SiStore).coorSensorInSi(318);
      cout << " coor sensor in Si   : " << csn318 << endl;
      
      const ASiLayer& layer1 = (*m_SiStore).layer(l318);
      const ASiLadder& ladder1 = layer1.ladder(ld318);
      const ASiHybrid& hybrid1 = ladder1.hybrid(hb318);
      const ASiSensorInfo& sensor19 = hybrid1.sensor(sn318);
      const ASiSensorInfo& sensor1 = (*m_SiStore).sensor(318);
      
      cout << " ------------ " << endl;
      SmallCount l_3189 = sensor19.layer();
      SmallCount l_318 = sensor1.layer();
      cout << " layer from sensor object: " << l_318 << " ** " << l_3189 << endl;
      SmallCount l_in_l_318 = sensor1.ladderInLayer();
      cout << " ladder in that layer: " << l_in_l_318 << endl;
      SmallCount ld_318 = sensor1.ladder();
      cout << " ladder: " << ld_318 << endl;
      SmallCount ns_318 = sensor1.sensorInLadder();
      cout << " N Sensor In Ladder: " << ns_318 << endl;
      SmallCount hb_318 = sensor1.hybrid();
      cout << " hybrid: " << hb_318 << endl;
      
      SmallCount gl318 = sensor1.gang();
      cout << " gang: " << gl318 << endl;
      Count aa318 = sensor1.lAddress(0);
      cout << " LAddress (R-Phi side): " << aa318 << endl;
      Count bb318 = sensor1.lAddress(1);
      cout << " LAddress (Z     side): " << bb318 << endl;
      Count cc318 = sensor1.lAddress( 1, 111 ); 
      cout << " StripAddress (#111, Z side): " << cc318 << endl;
      
      HepPoint3D coorLad318 = sensor1.coorInLadder();
      cout << " coordinates of that sensor in local ladder system: " << coorLad318 << endl;
      
      HepPoint3D coorSi318 = sensor1.coorInSi();
      cout << " coordinates of that sensor in Si system " << coorSi318 << endl;
      
      HepTransform3D transSi318 = sensor1.transToSi();
      HepPoint3D coorSi3189(0.,0.,0.);
      cout << " coordinates of that sensor in Si system using trans object" << transSi318*coorSi3189 << endl;
      
      HepPoint3D APPPP(0.01,0.01,0.005);
      HepPoint3D coorSi3181 = sensor1.coorInSi(APPPP);
      cout << " coordinates of point (0.01,0.01,0.005) in that sensor transformed to Si system: " << coorSi3181 << endl;
      
//-----------------
      cout << "Some results for hybrid 28" << endl;
      
      const  SmallCount  ss28 = (*m_SiStore).sensorForHybrid(28);
      const  SmallCount  l28 = (*m_SiStore).layerForSensor(ss28);
      const  SmallCount ld28 = (*m_SiStore).ladderInLayerForSensor(ss28);
      const  SmallCount hb28 = (*m_SiStore).hybridInLadderForSensor(ss28);
      
      const  ASiLayer& layer11 = (*m_SiStore).layer(l28);
      const  ASiLadder& ladder11 = layer11.ladder(ld28);
      const  ASiHybrid& hybrid119 = ladder11.hybrid(hb28);
      const  ASiHybrid& hybrid11 = (*m_SiStore).hybrid(28);
      
      SmallCount  n28 = hybrid11.number();
      cout << " number: " << n28 << endl;
      SmallCount  sss28 = hybrid11.sensor();
      cout << " sensor: " << sss28 << endl;
      SmallCount  gg28 = hybrid11.gang();
      cout << " gang  : " << gg28 << endl;
      SmallCount  ll28 = hybrid11.ladderInLayer();
      cout << " ladder in layer: " << ll28 << endl;
      SmallCount  lld28 = hybrid11.ladder();
      cout << " ladder: " << lld28 << endl;
      SmallCount  lr28 = hybrid11.layer();
      cout << " layer: " << lr28 << endl;
//-----------------
      cout << "Result for STL vector of sensors " << endl;
      
      const STL_VECTOR( const ASiSensorInfo* ) tlll=hybrid11.v_sensors();
      cout << " size: " << tlll.size() << endl;
      vector <const ASiSensorInfo*>::const_iterator pp;
      for( pp=tlll.begin(); pp != tlll.end(); ++pp)
	 cout << " numbers " << (**pp).number() << '\t';
      cout << endl;
      
//-----------------
      cout << "Result for ladder 8" << endl;
      
      const     SmallCount  ss8 = (*m_SiStore).sensorForHybrid(8);
      const     SmallCount  l8 = (*m_SiStore).layerForSensor(ss8);
      const     SmallCount ld8 = (*m_SiStore).ladderInLayerForSensor(ss8);
      
      const     ASiLayer& layer111 = (*m_SiStore).layer(l8);
      const     ASiLadder& ladder1119 = layer111.ladder(ld8);
      const     ASiLadder& ladder111 = (*m_SiStore).ladder(8);
      const     ASiLayer& layer1119 = ladder111.layerOwner();
      
      SmallCount  llr8 = ladder111.layer();
      SmallCount  llr88 = layer1119.number();
      float vx8 = layer111.halfLengthXOfLadder(); 
      float vy8 = layer111.halfLengthYOfLadder(); 
      float vz8 = layer111.halfLengthZOfLadder(); 
      SmallCount nabc8 = layer111.numberOfLadders();
      SmallCount ndef8 = layer111.numberOfSensorsInLadder();
      cout << " layer: " << llr8 << " (via layer function): " << llr88 << endl;
      cout << " numberOfLaddersForLayer: " << nabc8 << endl;
      cout << " numberOfSensorsInLadderForLayer: " << ndef8 << endl;
      cout << " HalfLengthX: " << vx8 << endl;
      cout << " HalfLengthX: " << vy8 << endl;
      cout << " HalfLengthZ: " << vz8 << endl;
      
      float phrot8 = ladder111.phiRotationInSi();
      float phph8 = ladder111.phiInSi();
      float rrrr8 = ladder111.radiusInSi();
      cout << " phirot: " <<  phrot8 << " phi: " <<  phph8 << " rad: " << rrrr8 << endl;
      
      HepPoint3D coorl8 = ladder111.coorInSi();
      cout << " coordinates of that ladder in Si system " << coorl8 << endl;
      
   HepPoint3D DPPPP(0.01,0.01,0.005);
   HepPoint3D coorpl8 = ladder111.coorInSi(DPPPP);
   cout << " coordinates of point (0.01,0.01,0.005) in that ladder transformed to Si system: " << coorpl8 << endl;

//-----------------
   cout << "Result for layer 2" << endl;

const     ASiLayer& layer1111 = (*m_SiStore).layer(2);

    SmallCount  lrrr2 = layer1111.ladder();
    SmallCount  sea2 = layer1111.sensorEast();
    SmallCount  swe2 = layer1111.sensorWest();

    cout << " ladder: " << lrrr2 << endl;
    cout << " SensorEast: " << sea2 << endl;
    cout << " SensorWest: " << swe2 << endl;

//-----------------
    cout << "Result for STL vector of layers " << endl;

const      STL_VECTOR( const ASiLayer* ) tltl=(*m_SiStore).v_layers();
    cout << " size: " << tltl.size() << endl;
vector <const ASiLayer*>::const_iterator p;
    for( p=tltl.begin(); p != tltl.end(); ++p)
    cout << " numbers " << (**p).number() << '\t';
    cout << endl;

//-----------------
   cout << "UnitVectors for ladder 8" << endl;

   HepVector3D crr0 = ladder111.unitVectorZInSi();
   HepVector3D crr1 = ladder111.unitVectorYInSi();
   HepVector3D crr2 = ladder111.unitVectorXInSi();
   HepVector3D crrR = ladder111.rPhiPointer();
   HepVector3D crrZ = ladder111.zPointer();
   cout << " X : " << crr2 << endl; 
   cout << " Y : " << crr1 << " Rpointer: " << crrR << "Zpointer: " << crrZ << endl;  
   cout << " Z : " << crr0 << endl; 
   cout << " halfLenghX: " <<  ladder111.halfLengthX() << " halfLenghY: " <<  ladder111.halfLengthY() << " halfLenghZ: " << ladder111.halfLengthZ() << endl; 

//-----------------
   cout << "UnitVectors for ladder 7" << endl;
const     ASiLadder& ladder117 = (*m_SiStore).ladder(7);

   HepVector3D crr07 = ladder117.unitVectorZInSi();
   HepVector3D crr17 = ladder117.unitVectorYInSi();
   HepVector3D crr27 = ladder117.unitVectorXInSi();

   cout << " X : " << crr27 << endl; 
   cout << " Y : " << crr17 << endl;  
   cout << " Z : " << crr07 << endl; 

//-----------------
   cout << "UnitVectors for ladder 6" << endl;
const     ASiLadder& ladder116 = (*m_SiStore).ladder(6);

   HepVector3D crr06 = ladder116.unitVectorZInSi();
   HepVector3D crr16 = ladder116.unitVectorYInSi();
   HepVector3D crr26 = ladder116.unitVectorXInSi();

   cout << " X : " << crr26 << endl; 
   cout << " Y : " << crr16 << endl;  
   cout << " Z : " << crr06 << endl; 

//-----------------
   cout << "UnitVectorsInSi for sensor 207" << endl;

    SmallCount  wl443 = (*m_SiStore).layerForSensor(443);
    SmallCount wld443 = (*m_SiStore).ladderInLayerForSensor(443);
    SmallCount whb443 = (*m_SiStore).hybridInLadderForSensor(443);
    SmallCount wsn443 = (*m_SiStore).inHybridForSensor(443);

    cout << " sensor 443 " << wl443 << " - " << wld443 << " - " << whb443 << " - " << wsn443 << endl;

const     ASiSensorInfo& sensor443 = (*m_SiStore).sensor(443);

    SmallCount  wl207 = (*m_SiStore).layerForSensor(207);
    SmallCount wld207 = (*m_SiStore).ladderInLayerForSensor(207);
    SmallCount whb207 = (*m_SiStore).hybridInLadderForSensor(207);
    SmallCount wsn207 = (*m_SiStore).inHybridForSensor(207);

    cout << " sensor 207 " << wl207 << " - " << wld207 << " - " << whb207 << " - " << wsn207 << endl;
const     ASiSensorInfo& sensor222 = (*m_SiStore).sensor(207);


   HepVector3D srr0 = sensor222.unitVectorZInSi();
   HepVector3D srr1 = sensor222.unitVectorYInSi();
   HepVector3D srr2 = sensor222.unitVectorXInSi();

   cout << " X : " << srr2 << endl; 
   cout << " Y : " << srr1 << endl; 
   cout << " Z : " << srr0 << endl; 

   cout << " halfLenghX: " <<  sensor222.halfLengthX() << " halfLenghY: " <<  sensor222.halfLengthY() << " halfLenghZ: " <<  sensor222.halfLengthZ() << endl; 

//   ASiSensorInfo sensor222a = sensor222;
   ASiStrip abb = sensor222.strip(0,100);
   cout << "Some results for Strip 100 (Side 0) of Sensor 207" << endl;
      cout << " layer " << abb.layer() << endl; 
      cout << " ladder " << abb.ladder() << endl; 
      cout << " sensor in ladder " <<  abb.sensorInLadder() << endl;
      cout << " hybrid " <<  abb.hybrid() << endl;
      cout << " gang " <<  abb.gang() << endl;
      cout << " side " <<  abb.side() << endl;
      if (abb.isRphi()) {cout << " isRphi "  << endl;}
      if (abb.isZ()) {cout << " isZ "  << endl;}
      cout << " lAddress " <<  abb.lAddress() << endl;
      cout << " localStrip " <<  abb.localStrip() << endl;
      cout << " chip " <<  abb.chip() << endl;
      cout << " pitch " <<  abb.pitch() << endl;
      cout << " localCoor " << abb.localCoor() << endl;

//-----------------
   cout << "Some results for L Address 94521 (functions of ASiStore)" << endl;

      SmallCount jjj1 = (*m_SiStore).chipForLAddress( 94521 );  
      cout << " chip  : " << jjj1 << endl;
      SmallCount jjj2 = (*m_SiStore).sensorForLAddress( 94521 ); 
      cout << " sensor: " << jjj2 << endl;
      SmallCount jjj3 = (*m_SiStore).localStripForLAddress( 94521 ); 
      cout << " localStrip: " << jjj3 << endl;
      SmallCount jjj4 = (*m_SiStore).layerForLAddress( 94521 ); 
      cout << " layer : " << jjj4 << endl;
      SmallCount jjjside = (*m_SiStore).sideForLAddress( 94521 ); 
      cout << " side : " << jjjside << endl;

   cout << "Some results for L Address 94521 (functions of ASiStrip)" << endl;
//      ASiStrip aaa = (*m_SiStore).strip(94521);
const ASiSensorInfo& sensor722 = (*m_SiStore).sensor(jjj2);
//      ASiSensorInfo sensor722a = sensor722;
      ASiStrip aaa = sensor722.strip(jjjside,jjj3);
      cout << " layer " << aaa.layer() << endl; 
      cout << " ladder " << aaa.ladder() << endl; 
      cout << " sensor in ladder " <<  aaa.sensorInLadder() << endl;
      cout << " hybrid " <<  aaa.hybrid() << endl;
      cout << " gang " <<  aaa.gang() << endl;
      cout << " side " <<  aaa.side() << endl;
      if (aaa.isRphi()) {cout << " isRphi "  << endl;}
      if (aaa.isZ()) {cout << " isZ "  << endl;}
      cout << " lAddress " <<  aaa.lAddress() << endl;
      cout << " localStrip " <<  aaa.localStrip() << endl;
      cout << " chip " <<  aaa.chip() << endl;
      cout << " pitch " <<  aaa.pitch() << endl;
      cout << " localCoor " << aaa.localCoor() << endl;
      cout << " sensor in ladder " <<  aaa.sensorInLadder() << endl;

//-----------------
   cout << "Some results for L Address 94421 (functions of ASiStore)" << endl;

      SmallCount iii1 = (*m_SiStore).chipForLAddress( 94421 );  
      cout << " chip  : " << iii1 << endl;
      SmallCount iii2 = (*m_SiStore).sensorForLAddress( 94421 ); 
      cout << " sensor: " << iii2 << endl;
      SmallCount iii3 = (*m_SiStore).localStripForLAddress( 94421 ); 
      cout << " localStrip: " << iii3 << endl;
      SmallCount iii4 = (*m_SiStore).layerForLAddress( 94421 ); 
      cout << " layer : " << iii4 << endl;
      SmallCount iiiside = (*m_SiStore).sideForLAddress( 94421 ); 
      cout << " side : " << iiiside << endl;
   cout << "Some results for L Address 94421 (functions of ASiStrip)" << endl;
      ASiStrip aab = (*m_SiStore).strip(94421);
//const ASiSensorInfo& sensor822 = (*m_SiStore).sensor(iii2);
//      ASiSensorInfo sensor822a = sensor822;
//      ASiStrip aab = sensor822a.strip(iiiside,iii3);

      cout << " layer " << aab.layer() << endl; 
      cout << " ladder " << aab.ladder() << endl; 
      cout << " sensor in ladder " <<  aab.sensorInLadder() << endl;
      cout << " hybrid " <<  aab.hybrid() << endl;
      cout << " gang " <<  aab.gang() << endl;
      cout << " side " <<  aab.side() << endl;
      if (aab.isRphi()) {cout << " isRphi "  << endl;}
      if (aab.isZ()) {cout << " isZ "  << endl;}
      cout << " lAddress " <<  aab.lAddress() << endl;
      cout << " localStrip " <<  aab.localStrip() << endl;
      cout << " chip " <<  aab.chip() << endl;
      cout << " pitch " <<  aab.pitch() << endl;
      cout << " localCoor " << aab.localCoor() << endl;

   cout << "Some results for L Address 256 (functions of ASiStrip)" << endl;
      ASiStrip aac = (*m_SiStore).strip(256);
      cout << " layer " << aac.layer() << endl; 
      cout << " ladder " << aac.ladder() << endl; 
      cout << " sensor in ladder " <<  aac.sensorInLadder() << endl;
      cout << " hybrid " <<  aac.hybrid() << endl;
      cout << " gang " <<  aac.gang() << endl;
      cout << " side " <<  aac.side() << endl;
      if (aac.isRphi()) {cout << " isRphi "  << endl;}
      if (aac.isZ()) {cout << " isZ "  << endl;}
      cout << " lAddress " <<  aac.lAddress() << endl;
      cout << " localStrip " <<  aac.localStrip() << endl;
      cout << " chip " <<  aac.chip() << endl;
      cout << " pitch " <<  aac.pitch() << endl;
      cout << " localCoor " << aac.localCoor() << endl;
   }
   
   return ( m_SiStore ) ;
}

//
// const member functions
//

//
// static member functions
//





