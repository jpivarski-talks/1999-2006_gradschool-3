#if !defined(ADRGEOM_ADRCATHODEPANELCONSTANTS_H)
#define ADRGEOM_ADRCATHODEPANELCONSTANTS_H
// -*- C++ -*-
//
// Package:     ADRGeom 
// Module:      ADRCathodePanelConstants
// 
// Description: survey/alignment constant for 8 Cathode Panel 
//
// Usage:.
//
// Author:      Inga Karliner
// Created:     Feb 7, 2001
//
// Revision history
//
// First submission
//
// 
// system include files

// user include files

// forward declarations

class ADRCathodePanelConstants
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  
  // Constructors and destructor
  ADRCathodePanelConstants();
  ADRCathodePanelConstants(unsigned int PanelNumber,
		    double PhiMinEast,
		    double PhiMaxEast,
		    double ZOffEastPhiMin,
		    double ZOffEastPhiMax,
		    double ZShift);

  virtual ~ADRCathodePanelConstants();
  
  // member functions
  // const member functions 
  unsigned int panelNumber() const;
  double phiMinEast() const;   
  double phiMaxEast() const;
  double zOffEastPhiMin() const;
  double zOffEastPhiMax() const;
  double zShift() const;

  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  //ADRCathodePanelConstants( const ADRCathodePanelConstants& ); //use default
  
  // assignment operator(s)
  //const ADRCathodePanelConstants& operator=( const ADRCathodePanelConstants& ); //use default
  
  // private member functions
  
  // private const member functions
  
  // data members  // static data members
  unsigned int m_PanelNumber;
  double m_PhiMinEast;
  double m_PhiMaxEast;
  double m_ZOffEastPhiMin;
  double m_ZOffEastPhiMax;
  double m_ZShift;

};

// inline function definitions


inline double 
ADRCathodePanelConstants::phiMinEast() const 
{
   return m_PhiMinEast;
}    
inline  double 
ADRCathodePanelConstants::phiMaxEast() const
{
   return  m_PhiMaxEast;
} 
inline  double 
ADRCathodePanelConstants::zOffEastPhiMin() const
{
   return m_ZOffEastPhiMin;
} 
inline  double 
ADRCathodePanelConstants::zOffEastPhiMax() const
{
   return m_ZOffEastPhiMax;
} 
inline  double 
ADRCathodePanelConstants::zShift() const
{
   return m_ZShift;
}  
inline unsigned int
ADRCathodePanelConstants::panelNumber() const
{
   return  m_PanelNumber;
}

 
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRCathodePanelConstants.cc"
//#endif

#endif /* ADRGEOM_ADRADRCathodePANELCONSTANTS_H */







