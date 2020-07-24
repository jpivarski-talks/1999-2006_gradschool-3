#if !defined(ALIGNMENTHISTOGRAM_ALIGNMENTHISTOGRAM_H)
#define ALIGNMENTHISTOGRAM_ALIGNMENTHISTOGRAM_H
// -*- C++ -*-
//
// Package:     AlignmentHistogram
// Module:      AlignmentHistogram
//
/**\class AlignmentHistogram AlignmentHistogram.h AlignmentHistogram/AlignmentHistogram.h
 
 Description: Suez Module that allows you to control the "event" loop

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Mon Jun  5 11:46:37 EDT 2000
// $Id: AlignmentHistogram.h,v 1.1.1.1 2001/03/07 18:03:00 llh14 Exp $
//
// Revision history
//
// $Log: AlignmentHistogram.h,v $
// Revision 1.1.1.1  2001/03/07 18:03:00  llh14
// imported AlignmentHistogram
//
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
//The data object you wish to change on each iteration
#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "BDLSVclient/DBSVAlignmentGlobal.hh"
#include "BDLSVclient/DBSVAlignmentLayer.hh"
#include "BDLSVclient/DBSVAlignmentLadder.hh"
#include "BDLSVclient/DBSVAlignmentSensor.hh"

#include "CommandPattern/Parameter.h"
// forward declarations

class AlignmentHistogram : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and 

      // ---------- Constructors and destructor ----------------
      AlignmentHistogram();
      virtual ~AlignmentHistogram();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      AlignmentHistogram( const AlignmentHistogram& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const AlignmentHistogram& operator=( const AlignmentHistogram& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      //This is the object you change every iteration
      CLEOConstantsModifiable<DBSVAlignmentGlobal>  m_SVGlobalConstants;
      CLEOConstantsModifiable<DBSVAlignmentLayer>   m_SVLayerConstants;
      CLEOConstantsModifiable<DBSVAlignmentLadder>  m_SVLadderConstants;
      CLEOConstantsModifiable<DBSVAlignmentSensor>  m_SVSensorConstants;

      //Holds the value and passes it onto the Proxy
      FIHolder< CLEOConstants<DBSVAlignmentGlobal> > m_SVGlobalConstantsHolder;
      FIHolder< CLEOConstants<DBSVAlignmentLayer> > m_SVLayerConstantsHolder;
      FIHolder< CLEOConstants<DBSVAlignmentLadder> > m_SVLadderConstantsHolder;
      FIHolder< CLEOConstants<DBSVAlignmentSensor> > m_SVSensorConstantsHolder;

      Parameter< float > m_normRphi;
      Parameter< float > m_normZ;
      Parameter< float > m_cutRphi;
      Parameter< float > m_cutZ;

      Parameter< string > m_GlobalInFile;
      Parameter< string > m_LayerInFile;      
      Parameter< string > m_LadderInFile;      
      Parameter< string > m_SensorInFile;

      Parameter< long int > m_TracksToHist;

      Parameter< DABoolean > m_OverlapToggleOff;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "AlignmentHistogram/Template/AlignmentHistogram.cc"
//#endif

#endif /* ALIGNMENTHISTOGRAM_ALIGNMENTHISTOGRAM_H */
