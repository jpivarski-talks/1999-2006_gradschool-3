#if !defined(PACKAGE_SIGEOM_H)
#define PACKAGE_SIGEOM_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      SiGeom
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Alexander Undrus
// Created:     Fri May 22 20:51:24 EDT 1998
// $Id: SiGeom.h,v 1.4 2000/11/15 21:52:10 cdj Exp $
//
// Revision history
//
// $Log: SiGeom.h,v $
// Revision 1.4  2000/11/15 21:52:10  cdj
// removed unused parameters
//
// Revision 1.3  1999/07/25 00:58:57  undrus
//  added SiAlignedGeometryProxy.  SiGeom  produces both base and aligned geometry
//
// Revision 1.2  1999/02/08 18:06:48  cdj
// Uses new proxy registration system
//
// Revision 1.1.1.1  1998/08/18 07:42:04  cleo3
// imported SiGeom sources
//
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "ToolBox/HistogramPackage.h"
#include "CommandPattern/Parameter.h"
#include "CommandPattern/Menu.h"

// forward declarations
class ProxyBase;

class SiGeom : public Producer
{
      // ------------ friend classses and functions ---------------
friend class SiGeomDescription;

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      SiGeom( void );                      // anal1 
      virtual ~SiGeom();                   // anal5 

      // ------------ member functions ----------------------------
      /*
      float transx() const;         
      float transy() const;         
      float transz() const;         
      float phix() const;         
      float phiy() const;         
      float phiz() const;         
      */

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      // ------------ const member functions ----------------------

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      SiGeom( const SiGeom& );

      // ------------ assignment operator(s) ----------------------
      const SiGeom& operator=( const SiGeom& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------
      ProxyBase* makeSiAlignedGeometryProxy() const;

      // ------------ data members --------------------------------
      /*
      Parameter<float> m_transx;  
      Parameter<float> m_transy;  
      Parameter<float> m_transz;  
      Parameter<float> m_phix;  
      Parameter<float> m_phiy;  
      Parameter<float> m_phiz;  
      */
      // ------------ static data members -------------------------

};

// inline function definitions
/*
inline
float SiGeom::transx() const
{ return m_transx.value(); }

inline
float SiGeom::transy() const
{ return m_transy.value(); }

inline
float SiGeom::transz() const
{ return m_transz.value(); }

inline
float SiGeom::phix() const
{ return m_phix.value(); }

inline
float SiGeom::phiy() const
{ return m_phiy.value(); }

inline
float SiGeom::phiz() const
{ return m_phiz.value(); }
*/
#endif /* PACKAGE_SIGEOM_H */




