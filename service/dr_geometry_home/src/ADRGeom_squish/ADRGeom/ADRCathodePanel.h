#if !defined(ADRGEOM_ADRCATHODEPANEL_H)
#define ADRGEOM_ADRCATHODEPANEL_H
// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRCathode Panel
// 
// Description: Base class for Cathode Panel
//
// Usage:
//   DR3 CathodePads are attached to DRGeom Cathode Panels Lab Volumes. 
//   The Lab Volumes are used to calculate the lab coordinates of the 
//   Cathode Pads.
//   Note: the panel volume type is TubePhiSegment. To get TubePhiSegment  
//         itself, see the extraction of pointers to it in ADRCathodeProxy
//
//   NOTE: Pad numbers begin with 0 on the East side.
//
// Author:      Inga Karliner
// Created:     May 7, 1999
//
// Revision history
//
// 12/11/00 I.Karliner Include survey measurements in panel placement
//          (see ADRCathodeParams)
//
// system include files

// user include files
#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
//#include "DetectorGeometry/DGVolumeTubePhiSegment.h"

#include "CLHEP/Geometry/Point3D.h"
#include "ADRGeom/ADRCathodePadInfo.h"


// forward declarations

class ADRCathodePanel
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  enum { kFirstPad    =    0};
  
  // Constructors and destructor
  ADRCathodePanel();
  ADRCathodePanel ( unsigned int iNumberOfPads,
		    const DGConstLabVolumePtr& iCathodePanel,
		    const HepPoint3D& iPoint0,
		    const HepPoint3D& iCornerEastPhiMin,
		    const HepPoint3D& iCornerEastPhiMax,
		    const HepPoint3D& iCornerWestPhiMin,
		    const HepPoint3D& iCornerWestPhiMax,
		    double iPanelRadius,
		    double iPhiMin,
		    double iPhiDelta,
		    double iHalfLengthZ,
		    double iZOffset,
		    double iPhiOffset,
		    const HepPoint3D& iOffsetPanelCenter);
  virtual ~ADRCathodePanel();
  
  // member functions
  void setNumberOfPads( unsigned int iNumberOfPads );
  void setVolume( const DGConstLabVolumePtr& iCathodePanel);

  void setPoint0(const HepPoint3D& iPoint0);
  void setCornerEastPhiMin(const HepPoint3D& iCornerEastPhiMin);
  void setCornerEastPhiMax(const HepPoint3D& iCornerEastPhiMax);
  void setCornerWestPhiMin(const HepPoint3D& iCornerWestPhiMin);
  void setCornerWestPhiMax(const HepPoint3D& iCornerWestPhiMax);

  void setPanelRadius(const double iRadius);
  void setLocalPhiMin(const double iPhiMin);
  void setPhiDelta(const double iPhiDelta); 
  void setHalfLengthZ(const double iHalfLengthZ);
  void setPanelZOffset(const double iZOffset);
  void setPanelPhiOffset(const double iPhiOffset);
  void setPanelOffsetCenter(const HepPoint3D& iOffsetPanelCenter);

  // make pad:
  ADRCathodePadInfo ADRCathodePadMk( unsigned int iPadID ) const;
  //extract pad from array made at initialization 
  const ADRCathodePadInfo& ADRCathodePad( unsigned int iPadID ) const;

  virtual void makePadsInPanel()  ; 

  //next===============
  //  const ADRCathodePadInfo& PadObjectClosest
  //  ( DABoolean& success, const HepPoint3D& iPoint ) const;
  //  unsigned int PadNumberClosest
  //  ( DABoolean& success, const HepPoint3D& iPoint ) const;
  // 
  //  inline unsigned int padNumber(const ADRCathodePadInfo& npad) const;
  //next===============
 

  // panels limits in z and phi
  //next
  double phiMin() const;
  double phiMax() const;
  double zMax() const;
  double zMin() const; 
  // edges:
  double phiMinAtZ( DABoolean& success, double z) const;
  double phiMaxAtZ( DABoolean& success,double z) const;
  double zMinAtPhi( DABoolean& success,double phi) const;
  double zMaxAtPhi( DABoolean& success, double phi) const;
     
  //next===============
  //  distance (point iPoint, cathode panel) 
  //    negative distance returned if panel outside range 
  //    (see implementation)  
  //  double pointDistance(const HepPoint3D& iPoint) const;  
  //next===============

  // const member functions
  // Cleo Lab Coordinates of the panel center and corners 
  const  HepPoint3D& point0() const;
  const  HepPoint3D& cornerEastPhiMin() const;
  const  HepPoint3D& cornerEastPhiMax() const;
  const  HepPoint3D& cornerWestPhiMin() const;
  const  HepPoint3D& cornerWestPhiMax() const;
  
  unsigned int numberOfPads() const ; 
  const  DGConstLabVolumePtr& panelVolume() const ;

  //LOCAL coordinates of the panel
  //phi is centered on 0, locaPhiMin     = -  PhiDelta/2
  //z is centered on 0,   local zmin/max = -/+halfLengthZ
  double panelRadius() const;
  double localPhiMin() const;
  double phiDelta() const ; 
  double halfLengthZ() const ;

  //offsets in z and phi 
  double phiOffset() const;
  double zOffset() const;
  const HepPoint3D& offsetCenter() const;
  // static member functions
protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
  ADRCathodePanel( const ADRCathodePanel& );

      // assignment operator(s)
  const ADRCathodePanel& operator=( const ADRCathodePanel& );

      // private member functions

      // private constx member functions

      // data members 
  //next     
  ADRCathodePadInfo* m_pads;

  unsigned int m_numberOfPads;
  DGConstLabVolumePtr m_volume;
  //local coordinates of:
  double m_panelRadius;
  double m_panelPhiMin;
  double m_panelPhiDelta;
  double m_panelHalfLengthZ;

  //offsets from nominal
  double m_panelZOffset;
  double m_panelPhiOffset;
  HepPoint3D m_panelOffsetCenter;

  //Cleo Lab Coordinates of center and corners
  HepPoint3D m_point0;
  HepPoint3D m_cornerEastPhiMin;
  HepPoint3D m_cornerEastPhiMax;
  HepPoint3D m_cornerWestPhiMin;
  HepPoint3D m_cornerWestPhiMax;
      // static data members

};

// inline function definitions

inline const DGConstLabVolumePtr&
ADRCathodePanel::panelVolume() const
{
   return m_volume;
}

inline unsigned int 
ADRCathodePanel::numberOfPads() const
{ 
  return m_numberOfPads; 
}

inline double 
ADRCathodePanel::panelRadius() const
{
  return m_panelRadius;
}

inline double 
ADRCathodePanel::localPhiMin() const
{
  return m_panelPhiMin;
}

inline double
ADRCathodePanel::phiDelta() const 
{
  return m_panelPhiDelta;
}

inline double
ADRCathodePanel::halfLengthZ() const
{
  return m_panelHalfLengthZ;
}

inline 
const HepPoint3D& 
ADRCathodePanel::offsetCenter() const
{
 return m_panelOffsetCenter;
}

inline double 
ADRCathodePanel::zOffset() const
{
return m_panelZOffset;
}

inline double 
ADRCathodePanel::phiOffset() const
{
return m_panelPhiOffset;
}
inline 
const HepPoint3D& 
ADRCathodePanel::point0() const
{
  return m_point0;
}
inline
const  HepPoint3D&
ADRCathodePanel::cornerEastPhiMin() const
{
  return  m_cornerEastPhiMin;
}

inline      
const  HepPoint3D& 
ADRCathodePanel::cornerEastPhiMax() const
{
  return m_cornerEastPhiMax ;
}
inline      
const  HepPoint3D& 
ADRCathodePanel::cornerWestPhiMin() const
{
  return m_cornerWestPhiMin;
}
inline      
const  HepPoint3D& 
ADRCathodePanel::cornerWestPhiMax() const
{
  return m_cornerWestPhiMax; 
}
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ActiveElements/Template/AEWireLayer.cc"
//#endif

#endif /* ADRGEOM_ADRCATHODEPANEL_H */
