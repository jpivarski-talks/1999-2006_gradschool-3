#if !defined(ADRGEOM_ADRGEOMLAYERSENDSCONSTANTS_H)
#define ADRGEOM_ADRGEOMLAYERSENDSCONSTANTS_H
// -*- C++ -*-
//
// Package:     ADRGeom 
// Module:      ADRGeomLayersEndsConstants
// 
// Description: (x,y,z ) of the ADR3 sense wire ends in ideal geometry
//              in the Lab, including sense wire positions, wire pull
//              (in r and phi) due to stereo angle, bushings, pins, and 
//              countertwist of the wedding cake pieces.
//
// Usage:.
//
// Author:      Inga Karliner
// Created:     Sept 11, 2000
//
// Revision history
//
// First submission
//
// 
// system include files

// user include files

// forward declarations

class ADRGeomLayersEndsConstants
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  
  // Constructors and destructor
  ADRGeomLayersEndsConstants();
  ADRGeomLayersEndsConstants(unsigned int DRLayerNumber,
		    double Radius_MM,
		    double X_WestMM,
		    double Y_WestMM,
		    double Z_WestMM,
		    double X_EastMM,
		    double Y_EastMM);

  virtual ~ADRGeomLayersEndsConstants();
  
  // member functions
  // const member functions 
unsigned int dRLayerNumber() const ;
double radius0MM() const;
double x0_WestMM() const;
double y0_WestMM() const;
double z0_WestMM() const;
double x0_EastMM() const;
double y0_EastMM() const;

  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  //ADRGeomLayersEndsConstants( const ADRGeomLayersEndsConstants& ); //use default
  
  // assignment operator(s)
  //const ADRGeomLayersEndsConstants& operator=( const ADRGeomLayersEndsConstants& ); //use default
  
  // private member functions
  
  // private const member functions
  
  // data members  // static data members
  unsigned int m_DRLayerNumber;
  double m_radius0MM ;
  double m_x0_WestMM ;
  double m_y0_WestMM ;
  double m_z0_WestMM ;
  double m_x0_EastMM ;
  double m_y0_EastMM ;
};

// inline function definitions

inline unsigned int
ADRGeomLayersEndsConstants::dRLayerNumber() const
{
   return  m_DRLayerNumber;
}

inline double
ADRGeomLayersEndsConstants::radius0MM() const
{
   return m_radius0MM;
}
inline double
ADRGeomLayersEndsConstants::x0_WestMM() const
{
   return m_x0_WestMM;
} 
inline double
ADRGeomLayersEndsConstants::y0_WestMM() const
{
   return m_y0_WestMM;
} 
inline double
ADRGeomLayersEndsConstants::z0_WestMM() const
{
   return m_z0_WestMM;
} 
inline double
ADRGeomLayersEndsConstants::x0_EastMM() const
{
   return m_x0_EastMM;
} 

inline double
ADRGeomLayersEndsConstants::y0_EastMM() const
{
   return m_y0_EastMM;
} 
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRGeomLayersEndsConstants.cc"
//#endif

#endif /* ADRGEOM_ADRGEOMLAYERSENDSCONSTANTS_H */







