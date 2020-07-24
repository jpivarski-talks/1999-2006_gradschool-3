#if !defined(ADRGEOM_ADRSENSEPARAMS_H)
#define ADRGEOM_ADRSENSEPARAMS_H
// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRSenseParams
// 
// Description: definitions of constants to be used in ADRSenseWireStore
//
// Usage:
//    <usage>
//
// Author:      Inga Karliner
// Created:     December 20, 1999
//
// Revision history
//
// 9/19/00 I.Karliner
//         Add diameters of the sense wire and field wire
//
// system include files
#include <assert.h>

// user include files

// forward declarations

class ADRSenseParams
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      
  enum
  { 
    kFirstLayer           =    1,
    kNumberOfLayers       =   47
  };
  
      static const int  kADRSenseWiresTotal;
      static const int  kADR_NLayers;
      static const int  kADR_NWiresInLayer[kNumberOfLayers]; 
      static const int  kADR_NWiresLyr1;
      static const int  kADR_NWiresLyr2;
      static const int  kADR_NWiresLyr3;
      static const int  kADR_NWiresLyr4;
      static const int  kADR_NWiresLyr5;
      static const int  kADR_NWiresLyr6;
      static const int  kADR_NWiresLyr7;
      static const int  kADR_NWiresLyr8;
      static const int  kADR_NWiresLyr9;
      static const int  kADR_NWiresLyr10;
      static const int  kADR_NWiresLyr11;
      static const int  kADR_NWiresLyr12;
      static const int  kADR_NWiresLyr13;
      static const int  kADR_NWiresLyr14;
      static const int  kADR_NWiresLyr15;
      static const int  kADR_NWiresLyr16;
      static const int  kADR_NWiresLyr17;
      static const int  kADR_NWiresLyr18;
      static const int  kADR_NWiresLyr19;
      static const int  kADR_NWiresLyr20;
      static const int  kADR_NWiresLyr21;
      static const int  kADR_NWiresLyr22;
      static const int  kADR_NWiresLyr23;
      static const int  kADR_NWiresLyr24;
      static const int  kADR_NWiresLyr25;
      static const int  kADR_NWiresLyr26;
      static const int  kADR_NWiresLyr27;
      static const int  kADR_NWiresLyr28;
      static const int  kADR_NWiresLyr29;
      static const int  kADR_NWiresLyr30;
      static const int  kADR_NWiresLyr31;
      static const int  kADR_NWiresLyr32;
      static const int  kADR_NWiresLyr33;
      static const int  kADR_NWiresLyr34;
      static const int  kADR_NWiresLyr35;
      static const int  kADR_NWiresLyr36;
      static const int  kADR_NWiresLyr37;
      static const int  kADR_NWiresLyr38;
      static const int  kADR_NWiresLyr39;
      static const int  kADR_NWiresLyr40;
      static const int  kADR_NWiresLyr41;
      static const int  kADR_NWiresLyr42;
      static const int  kADR_NWiresLyr43;
      static const int  kADR_NWiresLyr44;
      static const int  kADR_NWiresLyr45;
      static const int  kADR_NWiresLyr46;
      static const int  kADR_NWiresLyr47;
      static const double kADR_SenseWireDiameter;
      static const double kADR_FieldWireDiameter;
      
      // ---------- Constructors and destructor ----------------
      ADRSenseParams();
      virtual ~ADRSenseParams();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------
  // Number of wires in layer iLayer = 1:47
      static int nWiresInLayer( unsigned int iLayer);

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ADRSenseParams( const ADRSenseParams& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ADRSenseParams& operator=( const ADRSenseParams& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions
inline
int 
ADRSenseParams::nWiresInLayer(unsigned int iLayer) 
{
  assert( ( kFirstLayer <= iLayer ) && (kNumberOfLayers >= iLayer ) );
  return kADR_NWiresInLayer[iLayer -kFirstLayer]; 
}
// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/ADRSenseParams.cc"
//#endif

#endif /* PACKAGE_ADRSENSEPARAMS_H */
