#if !defined(ADRGEOM_ADRSENSEWIRESTORE_H)
#define ADRGEOM_ADRSENSEWIRESTORE_H
// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRSenseWireStore
// 
// Description: Class that holds the ADR3 = active elements of DR3
//
// Usage:
//---------------------------------------------------------------------
// Access functions: 
//    numberOfLayers
//    numberOfAxialLayers
//    numberOfULayers
//    numberOfVLayers
//   =>To loop over A/U/V wires with the same algorithm user can use
//    aLayer [0:15] = DR layer # 1-16
//    uLayer [0:15] = DR layer # see the implementation file  
//    vLayer [0:14] = DR layer #        "
//   => get layer, wire in layer for CellID= RawAddress
//    layerWire(CellID=1:9796) = (Layer=1:47,  WireInLayer=1:Nmax)
//      10/07/98 --------------------------->  WireInLayer=0:Nmax-1
//      LayerWire layerWire(const UInt32 CellId) const;
//   => get CellID= RawAddress for layer, wire in layer:  
//      UInt32 cellID(const unsigned short Layer,
//	  	      const unsigned short Wire) const; 
// Access to layer/wire information is via ActiveElements :
//    AEWireLayerCylindrical& layer( unsigned int iLayerID )
//    AEWireInfo wire( const DRWireAddress& iWireAddress )
//    see ActiveElements/AEWireLayerCylindrical.h       
//---------------------------------------------------------------------
//
// Author:      Inga Karliner adapted Chris D. Jones's DRWireExample
// Revision history
// Revision 10/05/98 Inga Karliner (from Adam Lyon)
//     add cellPhiWidth = cell width per layer
// Revision 10/06/98 Inga Karliner
//     add layer.isAxial, .isStereo, .isStereoU, .isStereoV  
// Revision 10/28/98 Inga Karliner
//     wire maker  ADRSenseWireStore::wire() becomes wireMk() 
//     (disabled)  ADRSenseWireStore::wireInfo() will read wire from m_wires 
// Revision 11/11/98 Inga Karliner: add <utility> for pair
// Revision  3/10/99 Inga Karliner: 
//     unsigned int layerNumber(const AEWireLayerCylindrical&)
//          returns layer# (layer object)
//     unsigned int layerNumber(DABoolean&, const HepPoint3D&)
//          returns layer# (success, 3D point), 
//          returns success=false, layer=0 if fails
// Revision 3/11/99 I.Karliner 
//     const AEWireInfo& wireObjectClosest
//          ( DABoolean& success, HepPoint3D& iPoint, unsigned int ilayer)
//          returns closest wire object for 3D point within the layer 
//          defined by radial extent, zmin, zmax
//          returns success = false if point not in the layer, 
//                               or if distance algorithm fails 
//    Note: 
//    these layer qualities are also accessible DIRECTLY as layer.function()   
//    unsigned int numberOfWiresLyr(unsigned int layer) const; 
//    double radius(unsigned int layer) const;
//    double phi0(unsigned int layer) const;
//    double cellPhiWidth(unsigned int layer) const;
//    double tanStereoAngle(unsigned int layer) const;
//    double zMax(unsigned int layer) const;
//    double zMin(unsigned int layer) const;
//    DABoolean isAxial(unsigned int layer) const;
//    DABoolean isStereo(unsigned int layer) const;
//    DABoolean isStereoV(unsigned int layer) const ;
//    DABoolean isStereoU(unsigned int layer) const;
//
//    double radiusAtz(DABoolean& success, unsigned int ilayer, double z) const;
//    
//    const AEWireInfo& wire(unsigned int ilayer,unsigned int iwireID) const;
// Revision 06/03/99 Inga Karliner
//     add radiusNominal(unsigned int layer), phi0Nominal(unsigned int layer)
// 
// Revision 08/24/99 Inga Karliner
//     comments cleanup
// Revision 08/31/99 Inga Karliner
//     Replace polycone Rlist with DRGeomParam arrays
//         following the change in DRGeom
// Revision 09/14/99 Inga Karliner
//     Add 
//        setLayer(...)             sets    layer 
//        updateSenseWireStore(..)  updates m_layers 
//        layerNC(ilayer)           returns ref to layer for updates (private)
//     Remove obsolete functions setEast/WestEndPlate()
// 09/27/99
//     Clean up, typos    
//     add data member 
//     array of pointers to the wire survey constants pDRWireSurveyConst m_pDRWireSurveyConst;
//     add access functions wireSurveyConstants();
//------------------------------------------------------------------------ 
// 9/11/00 I.Karliner Major Change:
//    Install Dan Peterson's wire end positions rather than calculate here:
//  
//    Old: (r, phi) sense hole positions, corrected for 
//           wedding cake twist 
//           wire pull to the side (pin, wire diameters, and stereo angle)            
//          z at the EndCap corrected for bowing, bushing length
//
//    New:  read in ideal geometry's( r,phi,z)  from Dan's file, Table 14
//          with corrections for the wire pull to the side, bowing, bushing length
//          and place them w/r to the endplate pieces. 
//          See the comment preceding layerDataDPP
//-----------------------------------------------------------------------
// 9/19/00 I.Karliner
//         Add the closest field wire functions
//9/27/00  I.Karliner
//         Add wire direction to the the closest field wire functions
//10/30/00 I.Karliner
//         Add the nominal field wire radia at center (z=0) 
//         Note: there is no UP field wire above layer 47  
//         fieldRadiusNominalUp(layer) and fieldRadiusNominalDown(layer) 
//11/03/00 I.Karliner
//         pointClosestFieldWireInSensePlane: 
//         Allow search for the closest field wire in the sense plane from the entire
//         layer (was limited to radia of the hyperboloid +/-200 microns)      
// 2/01/01 I.Karliner
//        -read Table14 from constants dr3.adrgeomlayersends
//        -read LayerData from the constants dr3.adrgeomlayer
//-----------------------------------------------------------------------

// system include files

#include <utility> // for pair< >
// user include files

// forward declarations (replace these with Class declarations)
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "ADRGeom/ADRWireLayerRealistic.h"
//#include "ActiveElements/AEWireLayerCylindrical.h"
#include "ADRGeom/ADRWireSurveyConstants.h"
#include "ADRGeom/ADRLayerConstants.h"
#include "ADRGeom/ADRGeomLayersEndsConstants.h"

class ADRWireAddress;
class ADRWireLayerRealistic;
class AEWireLayerCylindrical;
class AEWireInfo;


class ADRSenseWireStore
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      typedef pair<unsigned short, unsigned short> LayerWire ;
      enum { kFirstLayer           =    1,
	     kNumberOfLayers       =   47,
	     kNumberOfAxialLayers  =   16,
             kNumberOfStereoLayers =   31, 
	     kNumberOfULayers      =   16,
             kNumberOfVLayers      =   15,
             kNumberOfWiresTotal   = 9796,
             kFirstCellInDR        =    1
      };

      // Constructors and destructor

      ADRSenseWireStore( 
			const DGConstLabVolumePtr& iEastWeddingCake1,
			const DGConstLabVolumePtr& iWestWeddingCake1,
			const DGConstLabVolumePtr& iEastWeddingCake2,
			const DGConstLabVolumePtr& iWestWeddingCake2,
			const DGConstLabVolumePtr& iEastWeddingCake3,
			const DGConstLabVolumePtr& iWestWeddingCake3,
			const DGConstLabVolumePtr& iEastWeddingCake4,
			const DGConstLabVolumePtr& iWestWeddingCake4,
			const DGConstLabVolumePtr& iEastWeddingCake5,
			const DGConstLabVolumePtr& iWestWeddingCake5,
			const DGConstLabVolumePtr& iEastWeddingCake6,
			const DGConstLabVolumePtr& iWestWeddingCake6,
			const DGConstLabVolumePtr& iEastWeddingCake7,
			const DGConstLabVolumePtr& iWestWeddingCake7,
			const DGConstLabVolumePtr& iEastWeddingCake8,
			const DGConstLabVolumePtr& iWestWeddingCake8,
			const DGConstLabVolumePtr& iEastConicalEndPlate,
			const DGConstLabVolumePtr& iWestConicalEndPlates,
			ADRWireSurveyConstants** pWireSurveyConstants,
			ADRLayerConstants** pLayerConstants,
			ADRGeomLayersEndsConstants** pGeomLayersEnds);

      virtual ~ADRSenseWireStore();
  
      void updateSenseWireStore
         (const DGConstLabVolumePtr& iEastWeddingCake1,
	  const DGConstLabVolumePtr& iEastWeddingCake2,
	  const DGConstLabVolumePtr& iEastWeddingCake3,
	  const DGConstLabVolumePtr& iEastWeddingCake4,
	  const DGConstLabVolumePtr& iEastWeddingCake5,
	  const DGConstLabVolumePtr& iEastWeddingCake6,
	  const DGConstLabVolumePtr& iEastWeddingCake7,
	  const DGConstLabVolumePtr& iEastWeddingCake8,
	  const DGConstLabVolumePtr& iEastConicalEndPlate,
	  const DGConstLabVolumePtr& iWestWeddingCake1,
	  const DGConstLabVolumePtr& iWestWeddingCake2,
	  const DGConstLabVolumePtr& iWestWeddingCake3,
	  const DGConstLabVolumePtr& iWestWeddingCake4,
	  const DGConstLabVolumePtr& iWestWeddingCake5,
	  const DGConstLabVolumePtr& iWestWeddingCake6,
	  const DGConstLabVolumePtr& iWestWeddingCake7,
	  const DGConstLabVolumePtr& iWestWeddingCake8,
	  const DGConstLabVolumePtr& iWestConicalEndPlate);

      void setEastWeddingCakeEndPlate
        ( const DGConstLabVolumePtr& iEastWeddingCake1,
	  const DGConstLabVolumePtr& iEastWeddingCake2,
	  const DGConstLabVolumePtr& iEastWeddingCake3,
	  const DGConstLabVolumePtr& iEastWeddingCake4,
	  const DGConstLabVolumePtr& iEastWeddingCake5,
	  const DGConstLabVolumePtr& iEastWeddingCake6,
	  const DGConstLabVolumePtr& iEastWeddingCake7,
	  const DGConstLabVolumePtr& iEastWeddingCake8,
	  const DGConstLabVolumePtr& iEastConicalEndPlate);
  
      void setWestWeddingCakeEndPlate
        ( const DGConstLabVolumePtr& iWestWeddingCake1,
	  const DGConstLabVolumePtr& iWestWeddingCake2,
	  const DGConstLabVolumePtr& iWestWeddingCake3,
	  const DGConstLabVolumePtr& iWestWeddingCake4,
	  const DGConstLabVolumePtr& iWestWeddingCake5,
	  const DGConstLabVolumePtr& iWestWeddingCake6,
	  const DGConstLabVolumePtr& iWestWeddingCake7,
	  const DGConstLabVolumePtr& iWestWeddingCake8,
	  const DGConstLabVolumePtr& iWestConicalEndPlate);

      // const member functions
      unsigned int numberOfLayers() const;
      unsigned int numberOfAxialLayers() const;
      unsigned int numberOfStereoLayers() const;
      unsigned int numberOfULayers() const;
      unsigned int numberOfVLayers() const;
      unsigned int numberOfWiresTotal() const;

      const unsigned int* aLayers() const ;
      const unsigned int* uLayers() const ;
      const unsigned int* vLayers() const ;

//    NOTE Range:  
//    CellID=1:9796  Layer=1:47  WireInLayer=0:NWiresInLayer-1

      LayerWire layerWire(const UInt32 CellId) const; 
      UInt32 cellID(const unsigned short Layer,
		    const unsigned short Wire) const;  

      const ADRWireSurveyConstants& wireSurveyConstants( unsigned int iCellID ) const;
                                //Note : iCellID = 1, kNumberOfWiresTotal

      const ADRLayerConstants& layerConstants( unsigned int iLayerID ) const;

      const ADRGeomLayersEndsConstants& layersEndsConstants( unsigned int iLayerID ) const;

      const AEWireLayerCylindrical& layer( unsigned int iLayerID ) const; 
      const AEWireInfo  wireMk( const ADRWireAddress& iWireAddress ) const;
      const AEWireInfo& wire( const ADRWireAddress& iWireAddress ) const;
      const AEWireInfo  wireMk( const unsigned int iCellID ) const;
      const AEWireInfo& wire( const unsigned int iCellID ) const;
 
      inline unsigned int 
      layerNumber(const AEWireLayerCylindrical& nlayer) const; 
      unsigned int 
      layerNumber(DABoolean& success, const HepPoint3D& position ) const;

  //  Closest Sense Wire
      const AEWireInfo& 
      wireObjectClosest(DABoolean& success, const HepPoint3D& iPoint,
			                      unsigned int ilayer) const;
      

//    Layer access:
  unsigned int numberOfWiresLyr(unsigned int layer) const;
  //  wire# 0: radius and phi at z=0,  with sag
  double radius(unsigned int layer) const;
  double phi0(unsigned int layer) const;
  // average radius for layer, no sag 
  double radiusNominal(unsigned int layer) const;
  // wire# 0:  phi at z=0,  NO SAG
  double phi0Nominal(unsigned int layer) const;
  double cellPhiWidth(unsigned int layer) const;
  double tanStereoAngle(unsigned int layer) const;
  double zMax(unsigned int layer) const;
  double zMin(unsigned int layer) const;
  DABoolean isAxial(unsigned int layer) const;
  DABoolean isStereo(unsigned int layer) const;
  DABoolean isStereoV(unsigned int layer) const ;
  DABoolean isStereoU(unsigned int layer) const;
  
  double radiusAtz(DABoolean& success, unsigned int ilayer, double z) const;

  //  Nominal Field Wires Radia below and above:
  //  Note: there is no UP field wire above layer 47 
  double fieldRadiusNominalUp(unsigned int layer) const;
  double fieldRadiusNominalDown(unsigned int layer) const; 
  //  Points on Closest Field Wires:
  //
  //----------------------------------------------------------------------------------+
  //  Input:   point iPoint in the field wire plane in layer ilayer.
  //           The convention is that the field wire plane belongs to the 
  //           sense wire layer above it, except for layer 16 where there 
  //           are two field wire planes, one below, one above. 
  //           ilayer = 1-47
  //  Output:  the point on the closest field wire in this wire "plane", at iPoint.z() 
  //           success = false if radius of iPoint out of range, or search fails
  //  see implementation  
      HepPoint3D 
      pointClosestFieldWireInFieldPlane
          (DABoolean& success, const HepPoint3D& iPoint,
	   unsigned int ilayer) const;

  // return point and tan vector at that point
     HepPoint3D 
      pointClosestFieldWireInFieldPlane
          (DABoolean& success, HepVector3D& tanVector,
	   const HepPoint3D& iPoint,unsigned int ilayer) const;
  //   
  //----------------------------------------------------------------------------------+
  //  Input:   a point iPoint in layer ilayer
  //           ilayer= 1,47
  //  Output:  the point on the closest field wire in this wire plane, at iPoint.z()
  //           success = false if radius of iPoint out of range, or search fails 
  //  see implementation  
      HepPoint3D 
      pointClosestFieldWireInSensePlane
          (DABoolean& success, const HepPoint3D& iPoint,
	   unsigned int ilayer) const;

  // return point and tan vector at that point
     HepPoint3D 
      pointClosestFieldWireInSensePlane
          (DABoolean& success, HepVector3D& tanVector,
	   const HepPoint3D& iPoint,unsigned int ilayer) const;

//   Wire Access:

  const AEWireInfo& wire(unsigned int ilayer, unsigned int iwireID) const;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ADRSenseWireStore();
      ADRSenseWireStore( const ADRSenseWireStore& );

      // assignment operator(s)
      const ADRSenseWireStore& operator=( const ADRSenseWireStore& );

      // private member functions
  
  // allow non_const version layer used for updates
  //      AEWireLayerCylindrical& layerNC( unsigned int iLayerID ) const;
      ADRWireLayerRealistic& layerNC( unsigned int iLayerID ) const;
      // private const member functions

      // data members

  //  pointer to array of pointers to the wire survey constants
      ADRWireSurveyConstants** m_PWireSurveyConstants;

  //  pointer to array of pointers to the layer constants
      ADRLayerConstants** m_PLayerConstants;

  //  pointer to array of pointers to the layer ends constants
      ADRGeomLayersEndsConstants** m_PLayersEndsConstants;

  //  pointer to the array of layers
  //      AEWireLayerCylindrical* m_layers;
      ADRWireLayerRealistic* m_layers;
      DGConstLabVolumePtr  m_layerEndplates[kNumberOfLayers][2];

      unsigned int m_aLayer[kNumberOfAxialLayers]; 
      unsigned int m_uLayer[kNumberOfULayers]; 
      unsigned int m_vLayer[kNumberOfVLayers];
      DABoolean m_isULayer[kNumberOfLayers];
      DABoolean m_isVLayer[kNumberOfLayers];
      unsigned short m_cellToLayer[kNumberOfWiresTotal];
      unsigned short m_firstCellInLayer[kNumberOfLayers];
      unsigned short m_lastCellInLayer[kNumberOfLayers];
      
      // static data members

};

// inline function definitions
inline
unsigned int
ADRSenseWireStore::numberOfLayers() const
{ return kNumberOfLayers; }

inline
unsigned int
ADRSenseWireStore::numberOfAxialLayers() const
{ return kNumberOfAxialLayers; }

inline
unsigned int
ADRSenseWireStore::numberOfStereoLayers() const
{ return kNumberOfStereoLayers; }

inline
unsigned int
ADRSenseWireStore::numberOfULayers() const
{ return kNumberOfULayers; }

inline
unsigned int
ADRSenseWireStore::numberOfVLayers() const
{ return kNumberOfVLayers; }

inline
unsigned int
ADRSenseWireStore::numberOfWiresTotal() const
{ return kNumberOfWiresTotal; }

inline 
unsigned int 
ADRSenseWireStore::layerNumber(const AEWireLayerCylindrical& nlayer) const
{
 const AEWireLayerCylindrical& layer1 = layer(kFirstLayer); 
 int layerdif = &nlayer - &layer1 + kFirstLayer ;
 return layerdif;
}

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRSenseWireStore.cc"
//#endif

#endif /* ADRGEOM_ADRSENSEWIRESTORE_H */












