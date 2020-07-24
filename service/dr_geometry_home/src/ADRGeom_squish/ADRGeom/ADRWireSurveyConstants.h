#if !defined(ADRGEOM_ADRWIRESURVEYCONSTANTS_H)
#define ADRGEOM_ADRWIRESURVEYCONSTANTS_H
// -*- C++ -*-
//
// Package:     ADRGeom 
// Module:      ADRWireSurveyConstants
// 
// Description: Describes ADR3  Wires' Survey Constants: sense wire ends 
//              Local coordinates of the Endcaps
//              deviations from formula r=const, phi=phi0+n*(2Pi/N)
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

class ADRWireSurveyConstants
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  
  // Constructors and destructor
  ADRWireSurveyConstants();
  ADRWireSurveyConstants(unsigned int DRWireNumber,
		    double Correction_East_x_Microns,
		    double Correction_East_y_Microns,
		    double Correction_East_z_Microns,
		    double Correction_West_x_Microns,
		    double Correction_West_y_Microns,
		    double Correction_West_z_Microns);

  virtual ~ADRWireSurveyConstants();
  
  // member functions
  // const member functions 
unsigned int dRWireNumber() const ;
double correction_East_x_Microns() const;
double correction_East_y_Microns() const;
double correction_East_z_Microns() const;
double correction_West_x_Microns() const;
double correction_West_y_Microns() const;
double correction_West_z_Microns() const;

  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  //ADRWireSurveyConstants( const ADRWireSurveyConstants& ); //use default
  
  // assignment operator(s)
  //const ADRWireSurveyConstants& operator=( const ADRWireSurveyConstants& ); //use default
  
  // private member functions
  
  // private const member functions
  
  // data members  // static data members
  unsigned int m_DRWireNumber;
  double m_Correction_East_x_Microns;
  double m_Correction_East_y_Microns;
  double m_Correction_East_z_Microns;
  double m_Correction_West_x_Microns;
  double m_Correction_West_y_Microns;
  double m_Correction_West_z_Microns;
};

// inline function definitions

inline unsigned int
ADRWireSurveyConstants::dRWireNumber() const
{
   return  m_DRWireNumber;
}

inline double
ADRWireSurveyConstants::correction_East_x_Microns() const
{
   return m_Correction_East_x_Microns;
} 
inline double
ADRWireSurveyConstants::correction_East_y_Microns() const
{
   return m_Correction_East_y_Microns;
} 
inline double
ADRWireSurveyConstants::correction_East_z_Microns() const
{
   return m_Correction_East_z_Microns;
}  
inline double
ADRWireSurveyConstants::correction_West_x_Microns() const
{
   return m_Correction_West_x_Microns;
} 
inline double
ADRWireSurveyConstants::correction_West_y_Microns() const
{
   return m_Correction_West_y_Microns;
} 
inline double
ADRWireSurveyConstants::correction_West_z_Microns() const
{
   return m_Correction_West_z_Microns;
} 

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRWireSurveyConstants.cc"
//#endif

#endif /* ADRGEOM_ADRWIRESURVEYCONSTANTS_H */







