#if !defined(ADRGEOM_ADRWIRELAYERREALISTIC_H)
#define ADRGEOM_ADRWIRELAYERREALISTIC_H
// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRWireLayerRealistic
// 
// Description: Derived class for DR wire layers Test
//
// Usage:
//    See AEWireLayerCylindrical first.  
//    The Realistic Wire Layer inherits from AEWireLayerCylindrical 
//
// Author:      Inga Karliner
// Created:     October 7,1999
//
// Revision history
//
// First submission

// system include files
//#include<utility>

// user include files
#include "ActiveElements/AEWireLayerCylindrical.h"
#include "ADRGeom/ADRWireSurveyConstants.h"

// forward declarations

class ADRWireLayerRealistic : public AEWireLayerCylindrical
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      ADRWireLayerRealistic(); //Needed to allow lists 
      ADRWireLayerRealistic  (
	    double iRadiusEndCap,
	    double iPhiOfFirstWireInVolume1,
	    double iPhiOfFirstWireInVolume2,
	    unsigned int iNumberOfWires,
	    double iWireSagx,
	    double iWireSagy,
	    const DGConstLabVolumePtr& iVolume1,
	    const DGConstLabVolumePtr& iVolume2,
	    ADRWireSurveyConstants** pWireSurveyConstants,
	    double iZOffSet1 = 0.,
	    double iZOffSet2 = 0.);
  
      virtual ~ADRWireLayerRealistic();

      // member functions
       virtual void setLayer(unsigned int iNumberOfWires,
			    const DGConstLabVolumePtr& iVolume1,
			    const DGConstLabVolumePtr& iVolume2,
			    double sagx,
			    double sagy, 
			    int readoutEnd,
			    double radiusEndCap,
			    double angle1,
			    double angle2,
			    double zoffset1,
			    double zoffset2,
			    double rextentMin,
			    double rextentPls,
			    DABoolean axial,
			    DABoolean stereo,
			    DABoolean stereoU,
			    DABoolean stereoV,
			    ADRWireSurveyConstants** pWireSurveyConstants);

      const ADRWireSurveyConstants& 
      wireSurveyConstants( unsigned int iwire ) const;
                                //Note : iwire = 0, (iNumberOfWires-1)

  //  Used for making and updating wires:
      virtual void makeWiresInLayer()  ;
      virtual AEWireInfo wireMk( unsigned int iWireID ) const;
      virtual void wireUpdate(unsigned int iWireID ) ;
     // layer attributes at the material

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ADRWireLayerRealistic( const ADRWireLayerRealistic& );

      // assignment operator(s)
      const ADRWireLayerRealistic& operator=( const ADRWireLayerRealistic& );

      // private member functions

      // private const member functions

      // data members
  
  //  pointer to array of pointers to the wire survey constants
      ADRWireSurveyConstants** m_PWireSurveyConstants;
      // static data members

};

// inline function definitions


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ActiveElements/Template/ADRWireLayerRealistic.cc"
//#endif

#endif /* ADRGEOM_ADRWIRELAYERREALISTIC_H */











