// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiSensorAddress
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alexander Undrus
// Created:     Sun Aug 16 15:44:25 EDT 1998
// $Id: ASiSensorAddress.cc,v 1.5 2000/01/26 22:24:33 undrus Exp $
//
// Revision history
//
#include "Experiment/Experiment.h"
// $Log: ASiSensorAddress.cc,v $
// Revision 1.5  2000/01/26 22:24:33  undrus
//  numbering of sensors get direction from West to East
//
// Revision 1.4  2000/01/25 21:58:23  undrus
//  ASiStore : fixed bug in function inHybridForSensor; ASiSensorAddress : fixed bug which was responsible for wrong LA for sensors in east hybrids
//
// Revision 1.3  2000/01/20 00:20:14  undrus
// sensors are enumerated sequentially in a ladder, the number of sensors in a hybrid is stored for each hybrid individually, new functions isEast() and isWest() for sensor object
//
// Revision 1.2  1999/12/20 01:19:54  undrus
//  changed number of sensors in hybrids for layer 3 (outer radius ladders - 4 to the West, 3 to the East) and for layer 1 (ladder 1-3 - 2 sensors to the West)
//
// Revision 1.1  1999/07/03 22:27:26  cleo3
//  ASiSensorAddress is transferred from ASiSensor package, ASiStorePro becomes producer, all references to ASiSensor are eliminated
//
// Revision 1.11  1998/11/11 22:31:06  cleo3
// added scope definition 'ASiSensorAddress::kAdNLayers' to avoid errors at SunOS
//
// Revision 1.10  1998/10/21 02:54:10  cleo3
//  changed function names: getLadderForSensor -> getLadderInLayerForSensor, getLadderForHybrid -> getLadderInLayerForHybird, getCoorInLadder -> getCoorSensorInLadder, getCoorInSi -> getCoorSensorInSi; new functions returning the ladder number (1-61): getLadderForSensor, getLadderForHybrid
//
#include <stdlib.h>
#include <assert.h>
//#include <iostream.h>
// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ASiStorePro/ASiSensorAddress.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ASiStorePro.ASiSensorAddress" ;

const int ASiSensorAddress::kAdNSensorsInLadderWest[ASiSensorAddress::kAdNLadders] = { 2,2,2,1,1,1,1,
        2,2,2,2,2,2,2,2,2,2,
        3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,
        5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5 };


const int ASiSensorAddress::kAdNSensorsInLadder[ASiSensorAddress::kAdNLayers] = { 3,4,7,10 };
const int ASiSensorAddress::kAdNLaddersInLayer[ASiSensorAddress::kAdNLayers] = { 7,10,18,26 };

//
// static data member definitions
//

//
// constructors and destructor
//
ASiSensorAddress::ASiSensorAddress()
{
}

// ASiSensorAddress::ASiSensorAddress( const ASiSensorAddress& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ASiSensorAddress::~ASiSensorAddress()
{
}

//
// assignment operators
//
// const ASiSensorAddress& ASiSensorAddress::operator=( const ASiSensorAddress& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//

//const ASiSensorAddress::AddressArrays&
//ASiSensorAddress::getAddress()
//{  
//   static AddressArrays arrays = calculateAddresses();
//   m_arrays=arrays;
//   return arrays;
//}

SmallCount ASiSensorAddress::ggetLayerForSensor(SmallCount NSensor)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.layerForSensor[NSensor];
//  cout << "calling ggetLayerForSensor("<<NSensor<<"), returning " << aaa << endl;
  return aaa;
}

SmallCount ASiSensorAddress::ggetLadderInLayerForSensor(SmallCount NSensor)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.ladderForSensor[NSensor];
  return aaa;
}

SmallCount ASiSensorAddress::ggetInLadderForSensor(SmallCount NSensor)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.inLadderForSensor[NSensor];
//  cout << " OOOOO " << NSensor << " OO " << aaa << endl;
  return aaa;
}

SmallCount ASiSensorAddress::ggetHybridForSensor(SmallCount NSensor)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.hybridForSensor[NSensor];
  return aaa;
}

SmallCount ASiSensorAddress::ggetSensorForHybrid(SmallCount NSensor)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.sensorForHybrid[NSensor];
  return aaa;
}

SmallCount ASiSensorAddress::ggetGangForSensor(SmallCount NSensor)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.gangForSensor[NSensor];
  return aaa;
}

SmallCount ASiSensorAddress::ggetGangForHybrid(SmallCount NHybrid)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.gangForHybrid[NHybrid];
  return aaa;
}

SmallCount ASiSensorAddress::ggetLadderInLayerForHybrid(SmallCount NHybrid)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.ladderForHybrid[NHybrid];
  return aaa;
}

SmallCount ASiSensorAddress::ggetLayerForHybrid(SmallCount NHybrid)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.layerForHybrid[NHybrid];
  return aaa;
}

SmallCount ASiSensorAddress::ggetLayerForLadder(SmallCount NLadder)
{
  static AddressArrays abc=calculateAddresses();
  SmallCount aaa = abc.layerForLadder[NLadder];
  return aaa;
}

Count ASiSensorAddress::ggetLogicalAddressForSensor(SmallCount NSide, SmallCount NSensor)
{
  static AddressArrays abc=calculateAddresses();
  if ( NSensor >= kAdNSensors + 1 )
  {
    report( EMERGENCY, kFacilityString )
       << " input NSensor for ggetLogicalAddressForSensor is " <<  NSensor
       << " must be 1 - (kAdNSensors+1)" << endl;
    assert( false );
    exit(1);
  }    
  Count aaa = 0; 
  if (NSide == 0) { aaa = abc.logicalRAddressForSensor[NSensor]; }
  if (NSide == 1) { aaa = abc.logicalZAddressForSensor[NSensor]; }
  if (NSide > 1)
  {
    report( EMERGENCY, kFacilityString )
       << " first arg of ggetLogicalAddressForSensor is " << NSide 
       << " must be 0 or 1 " << endl;
    assert( false );
    exit(1);
  }    
  return aaa;
}

ASiSensorAddress::AddressArrays
ASiSensorAddress::calculateAddresses()
{  
   AddressArrays returnValue;

   int  icn = 1;
   int  ich = 0;
   
//////// HANDLING WEST HYBRIDS
   for(int ly = 0; ly <  kAdNLayers; ly++)
   {  
//     cout << "  kAdNLayers " << kAdNLayers << " - " << ly << endl;
      for(int ld = 0; ld < kAdNLaddersInLayer[ly] ; ld++)
      {
//       cout << "  kAdNLaddersInLayer " << kAdNLaddersInLayer[ly] << " - " << ld << endl;
        
         returnValue.layerForLadder[ich] = ly; 
         returnValue.layerForHybrid[ich] = ly; 
         returnValue.ladderForHybrid[ich] = ld; 
         int  kAdNSensorsInLadderEast = 
              kAdNSensorsInLadder[ly] - kAdNSensorsInLadderWest[ich];
	 returnValue.sensorForHybrid[ich] = icn ;
         returnValue.gangForHybrid[ich] = kAdNSensorsInLadderWest[ich];
         ich++;

	 for(int sn = 0; sn < kAdNSensorsInLadder[ly] ; sn++)
         {
//	  cout << " kAdNSensorsInLadderWest  " << kAdNSensorsInLadderWest[ly] << " - " << sn << endl;
	    returnValue.layerForSensor[icn] = ly;
	    returnValue.ladderForSensor[icn] = ld;
	    returnValue.inLadderForSensor[icn] = sn;

            if ( sn < kAdNSensorsInLadderWest[ich-1] ) 
            {
	    returnValue.hybridForSensor[icn] = ich - 1;
	    returnValue.gangForSensor[icn] = kAdNSensorsInLadderWest[ich-1];
	    returnValue.logicalRAddressForSensor[icn] =  
                              kAdNchannelSensor * (ich - 1) + 1;

	    returnValue.logicalZAddressForSensor[icn] =  
                  kAdNchannelSensor*(ich - 1) +  kAdZoffset + 1;
            }

//            cout << "++" << icn << "++" << ly << "++" << ld << "++" <<  sn  << endl;  
	    icn++;
	 }	  
      }     
   }   
   
//////// HANDLING EAST HYBRIDS
   icn = 1;
   int ildr = 0;

   for(int lyy = 0; lyy <  kAdNLayers; lyy++)
   {
      for(int ldd = 0; ldd < kAdNLaddersInLayer[lyy] ; ldd++)
      {
         returnValue.layerForHybrid[ich] = lyy; 
         returnValue.ladderForHybrid[ich] = ldd; 
         int  kAdNSensorsInLadderEast = 
              kAdNSensorsInLadder[lyy] - kAdNSensorsInLadderWest[ildr];
	 returnValue.sensorForHybrid[ich] = icn + kAdNSensorsInLadderWest[ildr];
         returnValue.gangForHybrid[ich] = 
                              kAdNSensorsInLadderEast;
         ich++; ildr++;
	 for(int snn = 0; snn < kAdNSensorsInLadder[lyy] ; snn++)
	 {
	    returnValue.layerForSensor[icn] = lyy;
	    returnValue.ladderForSensor[icn] = ldd;

            if ( snn > kAdNSensorsInLadderWest[ildr-1] - 1 ) 
            {
	    returnValue.hybridForSensor[icn] = ich - 1;
	    returnValue.gangForSensor[icn] = kAdNSensorsInLadderEast;
	    returnValue.logicalRAddressForSensor[icn] =  
                              kAdNchannelSensor * (ich - 1) + 1;

	    returnValue.logicalZAddressForSensor[icn] =  
                  kAdNchannelSensor*(ich - 1) +  kAdZoffset + 1;
            }
 
//            cout << "+-" << icn << "++" << lyy << "++" << ldd << "++" <<  snn << endl;  
	    icn++;
	 }	  
      }   
   }   
     
   return returnValue;
}









