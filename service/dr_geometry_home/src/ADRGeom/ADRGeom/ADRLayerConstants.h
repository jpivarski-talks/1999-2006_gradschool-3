#if !defined(ADRGEOM_ADRLAYERCONSTANTS_H)
#define ADRGEOM_ADRLAYERCONSTANTS_H
// -*- C++ -*-
//
// Package:     ADRGeom 
// Module:      ADRLayerConstants
// 
// Description: Describes ADR3  Layer Constants
//
// Usage:.
//
// Author:      Inga Karliner
// Created:     July 27, 1999
//
// Revision history
//
// First submission
//
// 
// system include files

// user include files

// forward declarations

class ADRLayerConstants
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  
  // Constructors and destructor
  ADRLayerConstants();
  ADRLayerConstants(unsigned int DRLayerNumber,
		    unsigned int NumberOfWires,
		    int   ReadoutEnd,
		    double RadiusEndCapCM,
		    double Phi0EastInDegrees,
		    double Phi0WestInDegrees,
		    double TwistCakeInDegrees,
		    unsigned int CakeNumber_Axial,
		    unsigned int ZStepPolycone_StereoLayer,
		    double ZBowingInch,
		    double ZBushing,
		    double ZPin,
		    double Pin_WireRadiusInch,
		    double SagXMicron,
		    double SagYMicron,
		    double FieldRadiusLessEndCapCM,
		    double FieldRadiusMoreEndCapCM);

  virtual ~ADRLayerConstants();
  
  // member functions
  // const member functions 
unsigned int dRLayerNumber() const ;
unsigned int numberOfWires() const ;
int          readoutEnd() const;
double radiusEndCapCM() const;
double phi0EastInDegrees() const;
double phi0WestInDegrees() const;
double twistCakeInDegrees() const;
unsigned int cakeNumber_Axial() const;
unsigned int zStepPolycone_StereoLayer() const;
double zBowingInch() const;
double zBushing() const;
double zPin() const;
double pin_WireRadiusInch() const;
double sagXMicron() const;
double sagYMicron() const;
double fieldRadiusLessEndCapCM() const;
double fieldRadiusMoreEndCapCM() const;

  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  //ADRLayerConstants( const ADRLayerConstants& ); //use default
  
  // assignment operator(s)
  //const ADRLayerConstants& operator=( const ADRLayerConstants& ); //use default
  
  // private member functions
  
  // private const member functions
  
  // data members  // static data members
  unsigned int m_DRLayerNumber;
  unsigned int m_NumberOfWires;
  int   m_ReadoutEnd;
  double m_RadiusEndCapCM;
  double m_Phi0EastInDegrees;
  double m_Phi0WestInDegrees;
  double m_TwistCakeInDegrees;
  unsigned int m_CakeNumber_Axial;
  unsigned int m_ZStepPolycone_StereoLayer;
  double m_ZBowingInch;
  double m_ZBushing;
  double m_ZPin;
  double m_Pin_WireRadiusInch;
  double m_SagXMicron;
  double m_SagYMicron;
  double m_FieldRadiusLessEndCapCM;
  double m_FieldRadiusMoreEndCapCM;
};

// inline function definitions

inline unsigned int
ADRLayerConstants::dRLayerNumber() const
{
   return  m_DRLayerNumber;
}

inline unsigned int
ADRLayerConstants::numberOfWires() const
{
   return m_NumberOfWires ;
}
inline int
ADRLayerConstants::readoutEnd() const 
{
   return m_ReadoutEnd;
}
inline double 
ADRLayerConstants::radiusEndCapCM() const
{
   return m_RadiusEndCapCM;
}
inline double
ADRLayerConstants::phi0EastInDegrees() const
{
   return m_Phi0EastInDegrees;
}  
inline double
ADRLayerConstants::phi0WestInDegrees() const
{
   return m_Phi0WestInDegrees;
}
inline double
ADRLayerConstants::twistCakeInDegrees() const
{
   return m_TwistCakeInDegrees;
}
inline unsigned int
ADRLayerConstants::cakeNumber_Axial() const
{
   return m_CakeNumber_Axial;
} 
inline unsigned int
ADRLayerConstants::zStepPolycone_StereoLayer() const
{
   return m_ZStepPolycone_StereoLayer; 
}
inline double
ADRLayerConstants::zBowingInch() const
{
   return m_ZBowingInch;
} 
inline double
ADRLayerConstants::zBushing() const
{
  return  m_ZBushing;
    }  
inline double
ADRLayerConstants::zPin() const
{
  return m_ZPin;
    }  
inline double
ADRLayerConstants::pin_WireRadiusInch() const
{
  return m_Pin_WireRadiusInch;
    }
inline double
ADRLayerConstants::sagXMicron() const
{
return m_SagXMicron;
} 
inline double 
ADRLayerConstants::sagYMicron() const 
{
return m_SagYMicron;
}
inline double
ADRLayerConstants::fieldRadiusLessEndCapCM() const
{
return m_FieldRadiusLessEndCapCM;
} 
inline double
ADRLayerConstants::fieldRadiusMoreEndCapCM() const 
{
return m_FieldRadiusMoreEndCapCM;
}  


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRLayerConstants.cc"
//#endif

#endif /* ADRGEOM_ADRLAYERCONSTANTS_H */







