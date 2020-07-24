#if !defined(ADRGEOMPROD_ADRSENSEWIRESTOREFACTORY_H)
#define ADRGEOMPROD_ADRSENSEWIRESTOREFACTORY_H
// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRSenseWireStoreFactory
// 
//Description: Creates the ADRSenseWireStore object from a DR DGDetectorStore
// Usage:
//
// Author:      Adam Lyon and Inga Karliner
//
// Revision history
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom)
//  2/6/01 I.Karliner Add getGeomLayer, getGeomLayerEnds
// 
// $Log: ADRSenseWireStoreFactory.h,v $
// Revision 1.2  2001/02/06 23:25:30  inga
// Add getGeomLayer, getGeomLayerEnds
//
// Revision 1.1.1.1  1999/12/08 17:23:08  inga
// import ADRGeomProd sources
//
// Revision 1.1  1999/11/18 23:04:03  inga
// make ADRSenseWireStoreFactory
//

// system include files

// user include files
#include "ADRGeom/ADRSenseWireStore.h"
#include "ADRGeom/ADREndCapPointers.h"

// forward declarations
   class DGDetectorStore;
   class Frame;

// Create and maintain the ADRSenseWireStore
class ADRSenseWireStoreFactory
{
      // ---------- friend classes and functions ---------------
   friend class ADRSenseWireProxy;

   public:
      // ---------- constants, enums and typedefs --------------
      // ---------- Constructors and destructor ----------------
      ADRSenseWireStoreFactory(const DGDetectorStore& drDetectorStore,
                               const Frame& theFrame);
      virtual ~ADRSenseWireStoreFactory();

     // ---------- member functions ---------------------------
  void updateSenseWireStore(const DGDetectorStore& drDetectorStore);
 
  // ---------- const member functions ---------------------
  
  ADRSenseWireStore* senseWireStore() const;
  
  // ---------- static member functions --------------------
  
protected:
  // ---------- protected member functions -----------------
  
  // ---------- protected const member functions -----------
  
private:
  // ---------- Constructors and destructor ----------------
  
  ADRSenseWireStoreFactory();  //stop default
  ADRSenseWireStoreFactory( 
	     const ADRSenseWireStoreFactory& ); // stop default

  // ---------- assignment operator(s) ---------------------
  const ADRSenseWireStoreFactory& operator=(const ADRSenseWireStoreFactory& );
 // stop default
  
  // ---------- private member functions -------------------
  
  // ---------- private const member functions -------------
  ADRWireSurveyConstants** 
  getWireSurveyConstants(
			 const Frame& theFrame) const;
  
  ADREndCapPointers 
  fillEndCapPointers( const DGDetectorStore& detectorStore) const;

  ADRLayerConstants** 
  getGeomLayer(
	       const Frame& theFrame) const;

  ADRGeomLayersEndsConstants**
  getGeomLayerEnds(
		   const Frame& theFrame) const;
  
  // ---------- data members -------------------------------
  ADRSenseWireStore* m_ADRSenseWireStore;
   
      // ---------- static data members ------------------------

};

inline ADRSenseWireStore* 
ADRSenseWireStoreFactory::senseWireStore() const
{return m_ADRSenseWireStore;}


// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ADRGeomProd/Template/ADRSenseWireStoreFactory.cc"
//#endif

#endif /* ADRGEOMPROD_ADRSENSEWIRESTOREFACTORY_H */

