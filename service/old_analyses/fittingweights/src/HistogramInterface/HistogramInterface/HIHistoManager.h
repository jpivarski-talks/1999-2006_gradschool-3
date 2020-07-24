#if !defined(HISTOGRAMINTERFACE_HIHISTOMANAGER_H)
#define HISTOGRAMINTERFACE_HIHISTOMANAGER_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistoManager
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Tue Oct 27 14:17:02 EST 1998
// $Id: HIHistoManager.h,v 1.7 1999/10/07 18:05:06 hufnagel Exp $
//
// Revision history
//
// $Log: HIHistoManager.h,v $
// Revision 1.7  1999/10/07 18:05:06  hufnagel
// Numbers now use float instead of double
//
// Revision 1.6  1999/10/06 20:25:26  cdj
// added setDefaultDir to Manager interface
//
// Revision 1.5  1999/09/29 17:52:04  cdj
// added histogram directories
//
// Revision 1.4  1999/09/10 01:11:18  mkl
// now allow booking of histograms with automatic id allocation
//
// Revision 1.3  1999/09/07 18:53:31  mkl
// new histogram interface
//
// Revision 1.2  1999/02/23 19:51:55  cdj
// ntuple takes a const char* const* instead of a const char*
//
// Revision 1.1.1.1  1999/02/16 22:07:04  cdj
// first submission
//

// system include files
#include <string>

// user include files
#include "HistogramInterface/HIHistProf.h"

// forward declarations
class HIHist1D;
class HIHist2D;
class HIHistProf;
class HINtuple;
class HINumber;
class HINumberWithErrors;
class HIHistDir;
class HIDirSwitch;


class HIHistoManager
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIHistoManager(); // stop default  
      virtual ~HIHistoManager(); 
      // ---------- member functions ---------------------------

      // Make a  sub directory
      virtual HIHistDir* mkdir( const string& iName) = 0;

      // create generic histograms and ntuples
      // 1D histogram
      virtual HIHist1D* histogram( Count          iId, 
				   const string&  iTitle,
				   unsigned int   iNChannels, 
				   float          iLowEdge,
				   float          iHighEdge,
				   float          iBitsPerChannel=0) =0;

      // 1D histogram with automatic id assignment
      virtual HIHist1D* histogram( const string&  iTitle,
				   unsigned int   iNChannels, 
				   float          iLowEdge,
				   float          iHighEdge,
				   float          iBitsPerChannel=0) =0;

      // 2D histogram
      virtual HIHist2D* histogram( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannelsX, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   unsigned int   iNChannelsY, 
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   float          BitsPerChannel=0) =0;
      
      // 2D histogram with automatic id assignment
      virtual HIHist2D* histogram( const string&  iTitle,
				   unsigned int   iNChannelsX, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   unsigned int   iNChannelsY, 
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   float          BitsPerChannel=0) =0;
      
      // profile histogram
      virtual HIHistProf* profile( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannels, 
				   float iLowEdgeX, 
				   float iHighEdgeX,
				   float iLowEdgeY, 
				   float iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) =0;

      // profile histogram with automatic id assignment
      virtual HIHistProf* profile( const string&  iTitle,
				   unsigned int   iNChannels, 
				   float iLowEdgeX, 
				   float iHighEdgeX,
				   float iLowEdgeY, 
				   float iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) =0;

      // ntuple
      virtual HINtuple* ntuple(    Count          iId,
			           const string&  iTitle,
				   unsigned int   iSizeOfEntry,
				   unsigned int   iChunkSize,
				   const char* const* iLabels) =0; 

      // ntuple histogram with automatic id assignment
      virtual HINtuple* ntuple(    const string&  iTitle,
				   unsigned int   iSizeOfEntry,
				   unsigned int   iChunkSize,
				   const char* const* iLabels) =0; 

      virtual HINumber* number( const string& iName,
				float        iValue ) =0;

      virtual HINumberWithErrors* numberWithErrors( 
	 const string& iName,
	 float        iValue,
	 float        iPositiveError,
	 float        iNegativeError ) =0;

      // Determines which histogram to fill corresponding
      // to a particular directory
      //virtual HIDirSwitch* mkSwitch() = 0;        
      
      /// should only be called by Suez internals
      virtual void setDefaultDir( const string& iDirectoryName ) = 0;

      // ---------- const member functions ---------------------
      // Directory information
      virtual HIHistDir* defaultDir() const = 0; // gets the default directory
      virtual HIHistDir* rootDir() const = 0;    // gets the root directory */


      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
   
      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIHistoManager( const HIHistoManager& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIHistoManager& operator=( const HIHistoManager& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHistoManager.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHISTOMANAGER_H */








