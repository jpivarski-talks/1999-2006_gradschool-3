#if !defined(CALIBRATEDDATA_CALIBRATEDCATHODEHIT_H)
#define CALIBRATEDDATA_CALIBRATEDCATHODEHIT_H
// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      CalibratedCathodeHit
// 
// Description: Calibrated Cathode data, including cluster parameters
//
// Usage:
//    <usage>
//
// Author:      Dawn Isabel
// Created:     Wed Jul  1 14:47:52 EDT 1998
// $Id: CalibratedCathodeHit.h,v 1.6 2000/03/03 15:13:39 lyon Exp $
//
// Revision history
//
// $Log: CalibratedCathodeHit.h,v $
// Revision 1.6  2000/03/03 15:13:39  lyon
// Storage helper stuff
//
// Revision 1.5  1999/03/19 17:41:13  jjt
// Implement operator<< for all devices.
//
// Revision 1.4  1998/12/15 18:53:58  jjt
// Modify print() to eliminate compiler warnings
//
// Revision 1.3  1998/08/18 19:17:54  wsun
// added phi position to CalibratedCathodeHit
//
// Revision 1.2  1998/08/11 23:26:28  cdj
// fixed typo
//
// Revision 1.1  1998/08/07 17:53:23  isabel
// class for calibrated cathode hits
//

// system include files

// user include files
#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "CalibratedData/CalibratedHit.h"

// forward declarations

class CalibratedCathodeHit : public CalibratedHit
{
      // ---------- friend classes and functions ---------------
      
   public:
      // ---------- constants, enums and typedefs --------------
      typedef double PicoCoul;
      typedef long QualHit;
      typedef double Meters;
      
      
      // ---------- Constructors and destructor ----------------
      CalibratedCathodeHit(UInt16 peakPad,
			   UInt16 numberOfPads,
			   PicoCoul totalCharge,
			   PicoCoul peakCharge,
			   CalibratedHit::Identifier id,
			   Meters z = 0., 
			   double phiPosition = 0.,
			   double weight = 0.,
			   double width = 0.,
			   QualHit qualHit = 0 );
      
      CalibratedCathodeHit( const CalibratedCathodeHit& );
      virtual ~CalibratedCathodeHit();
      
      // ---------- member functions ---------------------------
      inline         UInt16           peakPad       () const;
      inline         UInt16           numberOfPads  () const;
      inline virtual PicoCoul         totalCharge   () const;
      inline virtual PicoCoul         peakCharge    () const;
      inline virtual Meters           z             () const;
      inline virtual double           phi           () const;
      inline virtual double           weight        () const;
      inline virtual double           width         () const;
      inline         QualHit          qualHit       () const;
      inline         DeviceType       deviceType    () const;

      inline void setZ         (Meters z);
      inline void setPhi       (double aPhi);
      inline void setWeight    (double aWeight);
      inline void setWidth     (double aWidth);
      inline void setQualHit   (QualHit aQualHit);
      void printCCH(ostream&) const;

      // Comparison operator (checks Identifier)
      DABoolean operator==(const CalibratedCathodeHit& rhs) const;
      DABoolean operator!=(const CalibratedCathodeHit& rhs) const;
  
      
      
   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions ----------- 
      // ---------- assignment operator(s) ---------------------
      const CalibratedCathodeHit& operator=( const CalibratedCathodeHit& ); // stop default
      
   private:
      // ---------- Constructors and destructor ----------------
      CalibratedCathodeHit();
      
      // ---------- private member functions -------------------
      
      // ---------- private const member functions -------------
      
      // ---------- data members -------------------------------
      UInt16      m_peakPad;
      UInt16      m_numberOfPads;
      PicoCoul    m_totalCharge;
      PicoCoul    m_peakCharge;
      Meters      m_z;
      double      m_phi ;
      double      m_weight;
      double      m_width;
      QualHit     m_qualHit;
      
      // ---------- static data members ------------------------
      
};

// inline function definitions

inline ostream& operator<< (ostream& os, const CalibratedCathodeHit& hit)
{ hit.printCCH(os);
  return os;
}

inline UInt16
CalibratedCathodeHit::peakPad() const
{
   return m_peakPad;
}

inline UInt16
CalibratedCathodeHit::numberOfPads() const
{
   return m_numberOfPads;
}

inline CalibratedCathodeHit::PicoCoul
CalibratedCathodeHit::totalCharge() const
{
   return m_totalCharge;
}

inline CalibratedCathodeHit::PicoCoul
CalibratedCathodeHit::peakCharge() const
{
   return m_peakCharge;
}

inline CalibratedCathodeHit::Meters
CalibratedCathodeHit::z() const
{
   return m_z ;
}

inline double
CalibratedCathodeHit::phi() const
{
   return m_phi;
}

inline double
CalibratedCathodeHit::weight() const
{
   return m_weight;
}

inline double
CalibratedCathodeHit::width() const
{
   return m_width;
}

inline CalibratedCathodeHit::QualHit
CalibratedCathodeHit::qualHit() const
{
   return m_qualHit;
}

inline CalibratedCathodeHit::DeviceType
CalibratedCathodeHit::deviceType() const
{
   return Cathode;
}

//functions for changing select data members

inline void
CalibratedCathodeHit::setZ(CalibratedCathodeHit::Meters z)
{
   m_z = z;
}

inline void
CalibratedCathodeHit::setPhi(double aPhi)
{
   m_phi = aPhi;
}

inline void
CalibratedCathodeHit::setWeight(double aWeight)
{
   m_weight = aWeight;
}

inline void
CalibratedCathodeHit::setWidth(double aWidth)
{
   m_width = aWidth;
}

inline void
CalibratedCathodeHit::setQualHit(CalibratedCathodeHit::QualHit aQualHit)
{
   m_qualHit = aQualHit;
}

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "CalibratedData/Template/CalibratedCathodeHit.cc"
//#endif

#endif /* CALIBRATEDDATA_CALIBRATEDCATHODEHIT_H */
