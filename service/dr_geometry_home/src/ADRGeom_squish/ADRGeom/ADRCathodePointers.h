#if !defined(ADRGEOM_ADRCATHODECATHODEPOINTERS_H)
#define ADRGEOM_ADRCATHODECATHODEPOINTERS_H
// -*- C++ -*-
//
// Package:     <ADRGeom>
// Module:      ADRCathodeCathodePointers
// 
// Description: Holds Volume Pointers for DR Cathode
//
// Usage:    <usage>
//
// Author:      Adam Lyon and Inga Karliner
// Created:     Wed Nov 24 1999
// $Id: ADRCathodePointers.h,v 1.4 2001/09/13 22:18:59 inga Exp $
//
// Revision history
//
// $Log: ADRCathodePointers.h,v $
// Revision 1.4  2001/09/13 22:18:59  inga
// add friend class ADRCathodeStoreFactory, ADRCathodeStore
//
// Revision 1.3  2001/08/23 19:05:32  cleo3
// allowed default copy constructor (I have no idea how this compiled without it)
//
// Revision 1.2  2000/12/11 21:15:26  inga
// add cathode panels survey
//
// Revision 1.1  1999/11/29 18:33:38  inga
// add ADRCathodePointers 
// 12/11/00 I. Karliner
//   Add pointer DGConstLabVolumePtr  m_spEastConicalEndCap 
// 02/09/01 I.Karliner
//  Add friend class ADRCathodeStoreFactory, ADRCathodeStore 

// system include files
#include "Experiment/Experiment.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"

// forward declarations

// This is a helper class that just packages the cathode panel pointers
class ADRCathodePointers
{
  friend class ADRCathodeStoreFactory;
  friend class ADRCathodeStore;
  public:
  
    ADRCathodePointers(const DGConstLabVolumePtr& spCathodePanel0,
		       const DGConstLabVolumePtr& spCathodePanel1,
		       const DGConstLabVolumePtr& spCathodePanel2,
		       const DGConstLabVolumePtr& spCathodePanel3,
		       const DGConstLabVolumePtr& spCathodePanel4,
		       const DGConstLabVolumePtr& spCathodePanel5,
		       const DGConstLabVolumePtr& spCathodePanel6,
		       const DGConstLabVolumePtr& spCathodePanel7,
		       const DGConstLabVolumePtr& spConicalEndPlateEast);

    virtual ~ADRCathodePointers();

    // Accessors
    const DGConstLabVolumePtr& spCathodePanel0() const;
    const DGConstLabVolumePtr& spCathodePanel1() const;
    const DGConstLabVolumePtr& spCathodePanel2() const;
    const DGConstLabVolumePtr& spCathodePanel3() const;
    const DGConstLabVolumePtr& spCathodePanel4() const;
    const DGConstLabVolumePtr& spCathodePanel5() const;
    const DGConstLabVolumePtr& spCathodePanel6() const;
    const DGConstLabVolumePtr& spCathodePanel7() const;
    const DGConstLabVolumePtr& spConicalEndPlateEast() const;

  private:
      // Constructors and destructor
  ADRCathodePointers();
  //ADRCathodePointers( const ADRCathodePointers&  ); //allow default
      // assignment operator(s)
  //const ADRCathodePointers& operator=( const ADRCathodePointers& ); //allow default

  // data memebers
    DGConstLabVolumePtr m_spCathodePanel0;
    DGConstLabVolumePtr m_spCathodePanel1;
    DGConstLabVolumePtr m_spCathodePanel2;
    DGConstLabVolumePtr m_spCathodePanel3;
    DGConstLabVolumePtr m_spCathodePanel4;
    DGConstLabVolumePtr m_spCathodePanel5;
    DGConstLabVolumePtr m_spCathodePanel6;
    DGConstLabVolumePtr m_spCathodePanel7;
    DGConstLabVolumePtr m_spConicalEndPlateEast;

};


// inline function definitions

inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel0() const
{return m_spCathodePanel0;}
  
inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel1() const
{return m_spCathodePanel1;}

inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel2() const
{return m_spCathodePanel2;}

inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel3() const
{return m_spCathodePanel3;}

inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel4() const
{return m_spCathodePanel4;}

inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel5() const
{return m_spCathodePanel5;}

inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel6() const
{return m_spCathodePanel6;}

inline const DGConstLabVolumePtr& ADRCathodePointers::spCathodePanel7() const
{return m_spCathodePanel7;}

inline const DGConstLabVolumePtr& ADRCathodePointers::spConicalEndPlateEast() const
{return m_spConicalEndPlateEast;}

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ADRGeom/Template/ADRCathodeCathodePointers.cc"
//#endif

#endif /* ADRGEOM_ADRCATHODECathodePointers_H */



