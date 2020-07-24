#if !defined(ADRGEOM_ADRCATHODEPADINFO_H)
#define ADRGEOM_ADRCATHODEPADINFO_H
// -*- C++ -*-
//
// Package:     ADRGeom 
// Module:      ADRCathodePadInfo
// 
// Description: Describes a Cathode Pad 
//
// Usage:.
//
// Author:      Inga Karliner
// Created:     May 6, 1999
//
// Revision history
//
// First submission
//
// 
// system include files

// user include files
#include "CLHEP/Geometry/Point3D.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"

// forward declarations

class ADRCathodePadInfo
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  
  // Constructors and destructor
  ADRCathodePadInfo();
  ADRCathodePadInfo(
		    const HepPoint3D& iPoint0,
		    const HepPoint3D& iCornerEastPhiMin,
		    const HepPoint3D& iCornerEastPhiMax,
		    const HepPoint3D& iCornerWestPhiMin,
		    const HepPoint3D& iCornerWestPhiMax,
		    double iradius,
		    double iPhiMin,
		    double iPhiDelta,
		    double iHalfLengthZ);

  virtual ~ADRCathodePadInfo();
  
  // member functions

  void setPoint0(const HepPoint3D& iPoint0);
  void setCornerEastPhiMin(const HepPoint3D& iCornerEastPhiMin);
  void setCornerEastPhiMax(const HepPoint3D& iCornerEastPhiMax);
  void setCornerWestPhiMin(const HepPoint3D& iCornerWestPhiMin);
  void setCornerWestPhiMax(const HepPoint3D& iCornerWestPhiMax);

  void setRadius(const double radius);
  void setLocalPadPhiMin(const double iPhiMin);
  void setPadPhiDelta(const double iPhiDelta); 
  void setPadHalfLengthZ(const double iHalfLengthZ);

  // pads limits in z and phi
  double padGlobalPhiMin() const;
  double padGlobalPhiMax() const;
  double padGlobalZMax() const;
  double padGlobalZMin() const;
  
  // edges:
  double padPhiMinAtZ(  DABoolean& success,double z) const;
  double padPhiMaxAtZ(  DABoolean& success,double z) const;
  double padZMinAtPhi(  DABoolean& success,double phi) const;
  double padZMaxAtPhi(  DABoolean& success,double phi) const;

  // const member functions
  // Cleo Lab Coordinates of the pad center and corners 
  const  HepPoint3D& point0() const;
  const  HepPoint3D& cornerEastPhiMin() const;
  const  HepPoint3D& cornerEastPhiMax() const;
  const  HepPoint3D& cornerWestPhiMin() const;
  const  HepPoint3D& cornerWestPhiMax() const;
 
  //LOCAL coordinates of the pad
  double radius() const;
  double localPadPhiMin() const;
  double padPhiDelta() const ; 
  double padHalfLengthZ() const ;
  
  
  //  distance (point iPoint, cathode pad) 
  //    negative distance returned if pad outside range 
  //    (see implementation)  
  //      double pointDistance(const HepPoint3D& iPoint) const;
  
  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  //ADRCathodePadInfo( const ADRCathodePadInfo& ); //use default
  
  // assignment operator(s)
  //const ADRCathodePadInfo& operator=( const ADRCathodePadInfo& ); //use default
  
  // private member functions
  
  // private const member functions
  
  // data members
  //local coordinates of:
  double m_radius;
  double m_padPhiMin;
  double m_padPhiDelta;
  double m_padHalfLengthZ;  
//Cleo Lab Coordinates of center and corners
  HepPoint3D m_point0;
  HepPoint3D m_cornerEastPhiMin;
  HepPoint3D m_cornerEastPhiMax;
  HepPoint3D m_cornerWestPhiMin;
  HepPoint3D m_cornerWestPhiMax;

  // static data members

};

// inline function definitions
inline
double 
ADRCathodePadInfo::radius() const
{
   return m_radius;
}

inline
double 
ADRCathodePadInfo::padHalfLengthZ() const
{
   return m_padHalfLengthZ;
}

inline
double 
ADRCathodePadInfo::padPhiDelta() const
{
   return m_padPhiDelta;
}
inline
double 
ADRCathodePadInfo::localPadPhiMin() const
{
   return m_padPhiMin;
}
inline
const HepPoint3D& 
ADRCathodePadInfo::point0() const
{
   return m_point0;
}

inline
const HepPoint3D& 
ADRCathodePadInfo::cornerEastPhiMin()  const
{
   return m_cornerEastPhiMin;
}

inline
const HepPoint3D& 
ADRCathodePadInfo::cornerEastPhiMax()  const
{
   return m_cornerEastPhiMax;
}

inline
const HepPoint3D& 
ADRCathodePadInfo::cornerWestPhiMin()  const
{
   return m_cornerWestPhiMin;
}

inline
const HepPoint3D& 
ADRCathodePadInfo::cornerWestPhiMax()  const
{
   return m_cornerWestPhiMax;
}



//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRCathodePadInfo.cc"
//#endif

#endif /* ADRGEOM_ADRCATHODEPADINFO_H */







