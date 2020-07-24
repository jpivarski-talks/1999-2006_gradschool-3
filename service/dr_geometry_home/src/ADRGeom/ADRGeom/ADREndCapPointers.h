#if !defined(ADRGEOM_ADRENDCAPPOINTERS_H)
#define ADRGEOM_ADRENDCAPPOINTERS_H
// -*- C++ -*-
//
// Package:     <ADRGeom>
// Module:      ADREndCapPointers
//
// Description: Holds End Caps Pointers for ADRSenseWireStore Proxy
// Usage:
//
// Author:      Adam Lyon and Inga Karliner
// $Id: ADREndCapPointers.h,v 1.1 1999/11/18 23:05:12 inga Exp $
//
// Revision history
//
// $Log: ADREndCapPointers.h,v $
// Revision 1.1  1999/11/18 23:05:12  inga
// make ADREndCapPointers
//

// system include files

// user include files
#include "DetectorGeometry/DGConstLabVolumePtr.h"

// forward declarations
// This is a helper class that just packages the end cap pointers for
// easy passing around

class ADREndCapPointers
{
  friend class ADRSenseWireStoreFactory;
  friend class ADRSenseWireStore;
public:
  
  ADREndCapPointers(const DGConstLabVolumePtr& spCake1aEast,
		    const DGConstLabVolumePtr& spCake2aEast,
		    const DGConstLabVolumePtr& spCake3aEast,
		    const DGConstLabVolumePtr& spCake4aEast,
		    const DGConstLabVolumePtr& spCake5aEast,
		    const DGConstLabVolumePtr& spCake6aEast,
		    const DGConstLabVolumePtr& spCake7aEast,
		    const DGConstLabVolumePtr& spCake8aEast,
		    const DGConstLabVolumePtr& spCake1aWest,
		    const DGConstLabVolumePtr& spCake2aWest,
		    const DGConstLabVolumePtr& spCake3aWest,
		    const DGConstLabVolumePtr& spCake4aWest,
		    const DGConstLabVolumePtr& spCake5aWest,
		    const DGConstLabVolumePtr& spCake6aWest,
		    const DGConstLabVolumePtr& spCake7aWest,
		    const DGConstLabVolumePtr& spCake8aWest,
		    const DGConstLabVolumePtr& spConicalEndPlateEast1,
		    const DGConstLabVolumePtr& spConicalEndPlateWest1);
  
  virtual ~ADREndCapPointers();
  
  // Accessors
  const DGConstLabVolumePtr& spCake1aEast() const;
  const DGConstLabVolumePtr& spCake2aEast() const;
  const DGConstLabVolumePtr& spCake3aEast() const;
  const DGConstLabVolumePtr& spCake4aEast() const;
  const DGConstLabVolumePtr& spCake5aEast() const;
  const DGConstLabVolumePtr& spCake6aEast() const;
  const DGConstLabVolumePtr& spCake7aEast() const;
  const DGConstLabVolumePtr& spCake8aEast() const;
  const DGConstLabVolumePtr& spConicalEndPlateEast1() const;
  const DGConstLabVolumePtr& spCake1aWest() const;
  const DGConstLabVolumePtr& spCake2aWest() const;
  const DGConstLabVolumePtr& spCake3aWest() const;
  const DGConstLabVolumePtr& spCake4aWest() const;
  const DGConstLabVolumePtr& spCake5aWest() const;
  const DGConstLabVolumePtr& spCake6aWest() const;
  const DGConstLabVolumePtr& spCake7aWest() const;
  const DGConstLabVolumePtr& spCake8aWest() const;
  const DGConstLabVolumePtr& spConicalEndPlateWest1() const;
  
private:
      // Constructors and destructor
  ADREndCapPointers();
  ADREndCapPointers( const ADREndCapPointers&  );
      // assignment operator(s)
  const ADREndCapPointers& operator=( const ADREndCapPointers& );

  // data members
  DGConstLabVolumePtr m_spCake1aEast ;
  DGConstLabVolumePtr m_spCake2aEast ;
  DGConstLabVolumePtr m_spCake3aEast;
  DGConstLabVolumePtr m_spCake4aEast;
  DGConstLabVolumePtr m_spCake5aEast;
  DGConstLabVolumePtr m_spCake6aEast;
  DGConstLabVolumePtr m_spCake7aEast;
  DGConstLabVolumePtr m_spCake8aEast;
  DGConstLabVolumePtr m_spConicalEndPlateEast1;
  DGConstLabVolumePtr m_spCake1aWest;
  DGConstLabVolumePtr m_spCake2aWest;
  DGConstLabVolumePtr m_spCake3aWest;
  DGConstLabVolumePtr m_spCake4aWest;
  DGConstLabVolumePtr m_spCake5aWest;
  DGConstLabVolumePtr m_spCake6aWest;
  DGConstLabVolumePtr m_spCake7aWest;
  DGConstLabVolumePtr m_spCake8aWest;
  DGConstLabVolumePtr m_spConicalEndPlateWest1;

};


// inline function definitions

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake1aEast() const 
{return m_spCake1aEast;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake2aEast() const 
{return m_spCake2aEast;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake3aEast() const 
{return m_spCake3aEast;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake4aEast() const 
{return m_spCake4aEast;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake5aEast() const 
{return m_spCake5aEast;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake6aEast() const 
{return m_spCake6aEast;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake7aEast() const 
{return m_spCake7aEast;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake8aEast() const 
{return m_spCake8aEast;}

inline const DGConstLabVolumePtr& 
ADREndCapPointers::spConicalEndPlateEast1() const 
{return m_spConicalEndPlateEast1;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake1aWest() const 
{return m_spCake1aWest;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake2aWest() const 
{return m_spCake2aWest;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake3aWest() const 
{return m_spCake3aWest;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake4aWest() const 
{return m_spCake4aWest;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake5aWest() const 
{return m_spCake5aWest;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake6aWest() const 
{return m_spCake6aWest;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake7aWest() const 
{return m_spCake7aWest;}

inline const DGConstLabVolumePtr& ADREndCapPointers::spCake8aWest() const 
{return m_spCake8aWest;}

inline const DGConstLabVolumePtr& 
ADREndCapPointers::spConicalEndPlateWest1() const 
{return m_spConicalEndPlateWest1;}

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ADRGeom/Template/ADREndCapPointers.cc"
//#endif

#endif /* ADRGEOM_ADRENDCAPPOINTERS_H */
