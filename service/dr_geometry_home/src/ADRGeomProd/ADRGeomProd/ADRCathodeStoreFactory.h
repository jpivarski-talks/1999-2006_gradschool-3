#if !defined(ADRGEOMPROD_ADRCATHODESTOREFACTORY_H)
#define ADRGEOMPROD_ADRCATHODESTOREFACTORY_H
// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRCathodeStoreFactory
// 
// Description: Creates the ADRCathodeStore object from a DR DGDetectorStore
// Usage:  <usage>
//
// Author:      Adam Lyon and Inga Karliner
// Created:     Wed Nov 24, 1999
// $Id: ADRCathodeStoreFactory.h,v 1.2 2001/09/13 22:33:30 inga Exp $
//
// Revision history
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom)
//
// $Log: ADRCathodeStoreFactory.h,v $
// Revision 1.2  2001/09/13 22:33:30  inga
// read panels constants from the frame
//
// Revision 1.1.1.1  1999/12/08 17:23:08  inga
// import ADRGeomProd sources
//
// Revision 1.1  1999/11/29 18:34:30  inga
// add ADRCathodeStoreFactory
//

// system include files
#include "Experiment/Experiment.h"
#include "ADRGeom/ADRCathodeStore.h"
#include "ADRGeom/ADRCathodePointers.h"
#include "ADRGeom/ADRCathodePanelConstants.h"


// forward declarations
   class DGDetectorStore;
   class DGVolumeTubePhiSegment;
   class Frame;
//   class DGVolumeType;

// Create and maintain the ADRCathodeStore
class ADRCathodeStoreFactory
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      // ---------- Constructors and destructor ----------------
      ADRCathodeStoreFactory(const DGDetectorStore& drDetectorStore,
			     const Frame& theFrame);

      virtual ~ADRCathodeStoreFactory();

      // ---------- member functions ---------------------------
      void updateCathodeStore(const DGDetectorStore& drDetectorStore,
			      const Frame& theFrame);
      
      // ---------- const member functions ---------------------
      inline const ADRCathodeStore* cathodeStore() const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ADRCathodeStoreFactory();   //stop default
      ADRCathodeStoreFactory( const ADRCathodeStoreFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ADRCathodeStoreFactory& 
      operator=( const ADRCathodeStoreFactory& );              // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------
      ADRCathodePointers fillCathodePointers( 
				  const DGDetectorStore& detectorStore) const;

  //      const DGVolumeTubePhiSegment& getTube(

      const DGConstVolumePtr& getTube(
                            const DGConstLabVolumePtr& spCathodePanel0) const;
      //const DGConstVolumeType<DGVolumeTubePhiSegment> getTube(

      ADRCathodePanelConstants**
      getCathodePanelConst(const Frame& theFrame) const;
      // ---------- data members -------------------------------
      ADRCathodeStore* m_ADRCathodeStore;

      // ---------- static data members ------------------------

};

// inline function definitions
const ADRCathodeStore* ADRCathodeStoreFactory::cathodeStore() const
{return m_ADRCathodeStore;}


// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ADRGeomProd/Template/ADRCathodeStoreFactory.cc"
//#endif

#endif /* ADRGEOMPROD_ADRCATHODESTOREFACTORY_H */





