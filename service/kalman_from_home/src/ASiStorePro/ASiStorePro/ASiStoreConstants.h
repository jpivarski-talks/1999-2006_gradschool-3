#if !defined(ASISTOREPRO_ASISTORECONSTANTS_H)
#define ASISTOREPRO_ASISTORECONSTANTS_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStoreConstants
// 
// Description: Class that holds useful constants related to ASiStorePro
//
// Author:      Alex Undrus
// 
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files

// forward declarations (replace these with Class declarations)

class ASiStoreConstants
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
   enum { kSensors = 447, kLadders = 61, kLayers = 4 };
   enum { kHybrids = kLadders*2 };

   enum { kChannelsSensor = 512 };
   enum { kChannelsChip = 128 };
   enum { kChip = kHybrids*8 }; // =976
   enum { kChannelsTotal = kChip*kChannelsChip }; // =124928
   enum { kZoffset = kChannelsTotal/2 }; //
   enum { kChargeMeter = 73333333 }; // e-h pairs/meter generated in Si
//                                    // 22k for 300 microns
   static const int kSensorsLadderWest[kLayers];
   static const int kSensorsLadderEast[kLayers];
   static const int kSensorsLadder[kLayers];
   static const int kLaddersLayer[kLayers];

      // Constructors and destructor
      ASiStoreConstants(){};
      virtual ~ASiStoreConstants(){};

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ASiStoreConstants( const ASiStoreConstants& );

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      // static data members

};

// inline  definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ASiStorePro/Template/ASiStoreConstants.cc"
//#endif

#endif /* ASISTOREPRO_ASISTORECONSTANTS_H */












