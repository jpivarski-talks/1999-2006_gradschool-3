#if !defined(ADRGEOM_ADRCATHODESTORE_H)
#define ADRGEOM_ADRCATHODESTORE_H
// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRCathodeStore
// 
// Description: Class that holds the ADR3 Cathode
//
// Usage:
//---------------------------------------------------------------------
// Access functions: 
//    numberOfPanels     
//---------------------------------------------------------------------
//
// Author:      Inga Karliner 
// Revision history
//
// 7/9/99 Inga Karliner  
//  Add 
//  padObjectClosest()        Input phi,z; Output: Success, padID, pad object 
//  panelObjectClosest()      Input phi,z; Output: Success, panel object
//  panelNumberClosest()      Input phi,z; Output: Success, panel number 
//  padInPanelNumberClosest() Input phi,z; Output: Success, pad in panel #
//  Note:  if Success=false the results are not to be used.    
//         (z out of range -otherwise it should not happen) 
// 12/11/00 I. Karliner
//   Add pointer DGConstLabVolumePtr  m_EastConicalEndCap 
// 4/24/01 I.Karliner
//   Change over to DGConstLabVolumePtr m_spEastConicalEndCapPiece2

// system include files

#include <utility> // for pair< >
// user include files

// forward declarations 
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "ADRGeom/ADRCathodePanel.h"
#include "ADRCathodePanelConstants.h"
// next =========
//#include "ADRGeom/ADRCathodePadInfo.h"


class ADRCathodeStore
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
  //  typedef pair<unsigned short, unsigned short> PanelPad;
  enum { kFirstPadID           =    1,
	 kFirstPanel           =    0,
	 kNumberOfPanels       =    8,
	 kNumberOfPadsInPanel  =  192,
	 kNumberOfPadsTotal    = 1536,
         kPanel0               =    1,
         kPanel1               =    2,
         kPanel2               =    3,
         kPanel3               =    4,
         kPanel4               =    5,
         kPanel5               =    6,
         kPanel6               =    7,
         kPanel7               =    8
  };
  
      // Constructors and destructor

  ADRCathodeStore( 		  
		  const DGConstLabVolumePtr& iCathodePanel0, 
		  const DGConstLabVolumePtr& iCathodePanel1,
		  const DGConstLabVolumePtr& iCathodePanel2,
		  const DGConstLabVolumePtr& iCathodePanel3,
		  const DGConstLabVolumePtr& iCathodePanel4,
		  const DGConstLabVolumePtr& iCathodePanel5,
		  const DGConstLabVolumePtr& iCathodePanel6,
		  const DGConstLabVolumePtr& iCathodePanel7,
		  double iCathodePanelRadius,
		  double iCathodePanelPhiMin,
		  double iCathodePanelPhiDelta,
		  double iCathodePanelHalfLengthZ,
		  const DGConstLabVolumePtr& iConicalEndPlateEastPiece2,
		  ADRCathodePanelConstants** pCathodePanelConstants
		  );
  virtual ~ADRCathodeStore();
  
  // member functions
  
  void setCathode( 		  
		  const DGConstLabVolumePtr& iCathodePanel0, 
		  const DGConstLabVolumePtr& iCathodePanel1,
		  const DGConstLabVolumePtr& iCathodePanel2,
		  const DGConstLabVolumePtr& iCathodePanel3,
		  const DGConstLabVolumePtr& iCathodePanel4,
		  const DGConstLabVolumePtr& iCathodePanel5,
		  const DGConstLabVolumePtr& iCathodePanel6,
		  const DGConstLabVolumePtr& iCathodePanel7,
		  double iCathodePanelRadius,
		  double iCathodePanelPhiMin,
		  double iCathodePanelPhiDelta,
		  double iCathodePanelHalfLengthZ,
		  const DGConstLabVolumePtr& iConicalEndPlateEastPiece2,
		  ADRCathodePanelConstants** pCathodePanelConstants
		  );	
  
  // const member functions
  unsigned int numberOfPanels() const;
  unsigned int numberOfPadsInPanel() const;
  unsigned int numberOfPadsTotal() const;

  //    NOTE Range:  
  //    PadID=1:1536    
  //    Panel = 0:7  PadInPanel=0:191 
// 
  unsigned short panelOfPadID(const unsigned int PadId) const;
  unsigned short padInPanelOfPadID(const unsigned int PadId) const;
  unsigned int   padID(const unsigned short Panel,
		       const unsigned short Pad) const;    

  const ADRCathodePanel& panel( unsigned int iPanel ) const;
  
  inline unsigned int 
  panelNumber(const ADRCathodePanel& npanel) const; 

  //-----------------------------------------------------------
  //NOTE: User must check success 
  //      if Success=false the results are not to be used.    
  //         (z out of range -otherwise it should not happen)     
  //------------------------------------------------------------
  // If success: panel object for phi, z 
  //    defaults to Panel 0
  const ADRCathodePanel& 
  panelObjectClosest(DABoolean& success, double phi, double z) const;

  //-----------------------------------------------------------
  //NOTE: User must check success
  //      if Success=false the results are not to be used.    
  //         (z out of range -otherwise it should not happen)       
  //------------------------------------------------------------ 
  // If success: panel (0-7) for phi, z  
  //    defaults to Panel 0
  unsigned int 
  panelNumberClosest(DABoolean& success,  double phi, double z ) const;

  //-----------------------------------------------------------
  //NOTE: User must check success
  //      if Success=false the results are not to be used.    
  //         (z out of range -otherwise it should not happen)       
  //------------------------------------------------------------
  // If success: returns pad object and padId = (1-1536) for phi, z
  //    defaults to padId = 0 (non-existent)  
  const ADRCathodePadInfo& 
  padObjectClosest(DABoolean& success, 
		    unsigned int& padId, double phi, double z) const; 

  //-----------------------------------------------------------
  //NOTE: User must check success
  //      if Success=false the results are not to be used.    
  //         (z out of range -otherwise it should not happen)  
  //------------------------------------------------------------ 
  // If success: pad in panel (0-191) for phi, z; 
  //    defaults to pad 0     
  unsigned int
  padInPanelNumberClosest(DABoolean& success, double phi, double z) const; 

  const ADRCathodePanelConstants& panelConstants(unsigned int iPanelID) const;

  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  ADRCathodeStore();
  ADRCathodeStore( const ADRCathodeStore& );
  
  // assignment operator(s)
  const ADRCathodeStore& operator=( const ADRCathodeStore& );
  
  // private member functions

  // private const member functions
  // in perfect geometry this is the closest panel for phi,z:
  // used by padObjectClosest, etc
  const ADRCathodePanel& 
  panelObjectClosestPerfect(DABoolean& success, double phi, double z) const;
  unsigned int 
  panelNumberClosestPerfect(DABoolean& success,  double phi, double z) const;  
  // data members
 
  //  pointer to array of pointers to the panel constants
  ADRCathodePanelConstants** m_PCathodePanelConstants;

  ADRCathodePanel* m_panels;
  DGConstLabVolumePtr  m_panelSupport[kNumberOfPanels];

  DGConstLabVolumePtr  m_EastConicalEndCapPiece2;
  

  //panel data in LOCAL panel coordinates
  double m_panelRadius;
  double m_panelPhiMin;
  double m_panelPhiDelta;
  double m_panelHalfLengthZ;
  

// next =========
  unsigned short m_padToPanel[kNumberOfPadsTotal];
      
      // static data members

};

// inline function definitions
inline
unsigned int
ADRCathodeStore::numberOfPanels() const
{ return kNumberOfPanels; }

inline
unsigned int
ADRCathodeStore::numberOfPadsTotal() const
{ return kNumberOfPadsTotal; }

inline 
unsigned int 
ADRCathodeStore::numberOfPadsInPanel() const
{ return kNumberOfPadsInPanel; }

inline 
unsigned int 
ADRCathodeStore::panelNumber(const ADRCathodePanel& npanel) const
{
 const ADRCathodePanel& panel1 =panel(kFirstPanel); 
 int paneldif = &npanel - &panel1 + kFirstPanel ;
 return paneldif;
}

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRCathodeStore.cc"
//#endif

#endif /* ADRGEOM_ADRCATHODESTORE_H */












