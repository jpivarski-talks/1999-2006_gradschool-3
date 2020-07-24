#if !defined(ADRGEOM_ADRCATHODEPARAMS_H)
#define ADRGEOM_ADRCATHODEPARAMS_H
// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRCathodeParams
// 
// Description: definitions of constants to be used in ADR3 Cathodes
//
// Usage:
//    <usage>
//
// Author:      Inga Karliner
// Created:     May 17, 1999
//
// Revision history
//
// 12/11/00 I.Karliner: Survey measurements of 8 panels were added
//                      PhiMinEast,     PhiMaxEast, 
//                      ZOffEastPhiMin, ZOffEastPhiMax  
//                      together with the overall shift kADCathodeZShift
//  2/09/01 I.Karliner: allow non-const parameters[[kNumberOfPanels]
//                      kADCathodeZShift[kNumberOfPanels] instead of
//                      one overall z-shift
// system include files

// user include files

// forward declarations

class ADRCathodeParams
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      
      enum
      {
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
 
      static const double kADRCathodePitch;
      static const double kADRCathodePadLength;
      static const double kPanelMeasuredDelMin;
      static const double kPanelMeasuredDelMax; 
      static double kPanelMeasuredPhiMinEast[kNumberOfPanels]; 
      static double kPanelMeasuredPhiMaxEast[kNumberOfPanels];
      static double kPanelMeasuredZOffEastPhiMin[kNumberOfPanels]; 
      static double kPanelMeasuredZOffEastPhiMax[kNumberOfPanels]; 
      static double kADCathodeZShift[kNumberOfPanels];

      // ---------- Constructors and destructor ----------------
      ADRCathodeParams();
      virtual ~ADRCathodeParams();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ADRCathodeParams( const ADRCathodeParams& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ADRCathodeParams& operator=( const ADRCathodeParams& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/ADRCathodeParams.cc"
//#endif

#endif /* PACKAGE_ADRCathodeParams_H */
