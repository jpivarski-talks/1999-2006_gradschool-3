#if !defined(ASISTOREPRO_ASISENSORADDRESS_H)
#define ASISTOREPRO_ASISENSORADDRESS_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiSensorAddress
// 
// Description: Class that holds the elements of Si detector
//

// system include files
#include "Experiment/Experiment.h"

// user include files


// forward declarations
class DummyFriend;

class ASiSensorAddress
{
      // friend classes and functions
      friend class DummyFriend;

   public:
      // constants, enums and typedefs


   enum { kAdNSensors = 447, kAdNLadders = 61, kAdNLayers = 4 };
   enum { kAdNHybrids = kAdNLadders*2 };

   enum { kAdNchannelSensor = 512 };
   enum { kAdNchannelChip = 128 };
   enum { kAdNChip = kAdNHybrids*8 }; // =976
   enum { kAdNchannelTotal = kAdNChip*kAdNchannelChip }; // =124928
   enum { kAdZoffset = kAdNchannelTotal/2 }; // 
   enum { kAdChargePerM = 73333333 }; // e-h pairs/meter generated in Si
//                                    // 22k for 300 microns
   static const int kAdNSensorsInLadderWest[kAdNLadders];
   static const int kAdNSensorsInLadder[kAdNLayers];
   static const int kAdNLaddersInLayer[kAdNLayers];

   struct AddressArrays {
	    SmallCount layerForSensor[kAdNSensors+1];  
	    SmallCount ladderForSensor[kAdNSensors+1];  
	    SmallCount inLadderForSensor[kAdNSensors+1];  
	    SmallCount hybridForSensor[kAdNSensors+1];  
	    SmallCount gangForSensor[kAdNSensors+1];  
	    SmallCount sensorForHybrid[kAdNSensors+1];  
	    SmallCount gangForHybrid[kAdNSensors+1];  
	         Count logicalRAddressForSensor[kAdNSensors+1];  
	         Count logicalZAddressForSensor[kAdNSensors+1];  
	    SmallCount ladderForHybrid[kAdNSensors+1];  
	    SmallCount layerForHybrid[kAdNSensors+1];  
	    SmallCount layerForLadder[kAdNSensors+1];  
   };

      // Constructors and destructor

      // member functions
        static SmallCount ggetLayerForSensor(SmallCount NSensor);
        static SmallCount ggetLadderInLayerForSensor(SmallCount NSensor);
        static SmallCount ggetInLadderForSensor(SmallCount NSensor);
        static SmallCount ggetHybridForSensor(SmallCount NSensor);
        static SmallCount ggetSensorForHybrid(SmallCount NHybrid);
        static SmallCount ggetGangForSensor(SmallCount NSensor);
        static SmallCount ggetGangForHybrid(SmallCount NHybrid);
        static SmallCount ggetLadderInLayerForHybrid(SmallCount NHybrid);
        static SmallCount ggetLayerForHybrid(SmallCount NHybrid);
        static SmallCount ggetLayerForLadder(SmallCount NLadder);
        static Count ggetLogicalAddressForSensor(SmallCount NSide, SmallCount NSensor);

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ASiSensorAddress( const ASiSensorAddress& );
      ASiSensorAddress();
 
      virtual ~ASiSensorAddress();


      // assignment operator(s)
      const ASiSensorAddress& operator=( const ASiSensorAddress& );

      // private member functions
      static AddressArrays calculateAddresses();

      // private const member functions

      // data members

      // static data members      
};

// inline function definitions


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "SiGeom/Template/ASiSensorAddress.cc"
//#endif

#endif /* ASISTOREPRO_ASISENSORADDRESS_H */


 






