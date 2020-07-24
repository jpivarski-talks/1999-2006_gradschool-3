#if !defined(HBOOKHISTOGRAM_HIHBOOKMANAGER_H)
#define HBOOKHISTOGRAM_HIHBOOKMANAGER_H
// -*- C++ -*-
//
// Package:     <HbookHistogram>
// Module:      HIHbookManager
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Tue Feb  2 13:44:00 EST 1999
// $Id: HbookManager.h,v 1.5 1999/10/07 18:07:54 hufnagel Exp $
//
// Revision history
//
// $Log: HbookManager.h,v $
// Revision 1.5  1999/10/07 18:07:54  hufnagel
// Numbers now use float instead of double
//
// Revision 1.4  1999/10/06 20:27:22  cdj
// added setDefaultDir to Manager
//
// Revision 1.3  1999/09/29 17:55:55  cdj
// added histogram directories
//
// Revision 1.2  1999/09/10 01:12:35  mkl
// now allow booking of histograms with automatic id allocation
//
// Revision 1.1.1.1  1999/09/06 22:02:31  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

// system include files

// user include files
#include "Experiment/fortran_types.h"
#include "HistogramInterface/HIHistoManager.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HbookFile;
class HbookHistDir;

// typedef const FChar* CharList;

class HbookManager : public HIHistoManager  
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HbookManager( const char* iFileName,
		    SmallCount  iLogicalUnitNumber,
		    SmallCount  iRecordLength );

      virtual ~HbookManager();

      // ---------- member functions ---------------------------

      // Make a  sub directory
      virtual HIHistDir* mkdir( const string& iName);

      // create Hbook histograms and Ntuples
      // 1D histogram
      virtual HIHist1D* histogram( Count          iId,
				   const string&  iTitle,
				   unsigned int   NChannels,
				   float          iLowEdge,
				   float          iHighEdge,
				   float          iBitsPerChannel=0 );

      // 1D histogram with automatic id assignment
      virtual HIHist1D* histogram( const string&  iTitle,
				   unsigned int   NChannels,
				   float          iLowEdge,
				   float          iHighEdge,
				   float          iBitsPerChannel=0 );

      // 2D histogram
      virtual HIHist2D* histogram( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannelsX, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   unsigned int   iNChannelsY, 
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   float          BitsPerChannel=0 );
      
      // 2D histogram with automatic id assignment
      virtual HIHist2D* histogram( const string&  iTitle,
				   unsigned int   iNChannelsX, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   unsigned int   iNChannelsY, 
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   float          BitsPerChannel=0 );

      // profile histogram
      virtual HIHistProf* profile( Count          iId,
				   const string&  iTitle,
				   unsigned int   iNChannels, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs );

      // profile histogram with automatic id assignment
      virtual HIHistProf* profile( const string&  iTitle,
				   unsigned int   iNChannels, 
				   float          iLowEdgeX, 
				   float          iHighEdgeX,
				   float          iLowEdgeY, 
				   float          iHighEdgeY,
				   HIHistProf::DisplayErrorsAs iDisplayErrorsAs );

      // ntuple
      virtual HINtuple* ntuple(    Count          iId,
			           const string&  iTitle,
				   unsigned int   iNumberOfVariables,
				   unsigned int   iChunkSize,
				   const char* const* iLabels ) ;

      // ntuple with automatic id assignment
      virtual HINtuple* ntuple(    const string&  iTitle,
				   unsigned int   iNumberOfVariables,
				   unsigned int   iChunkSize,
				   const char* const* iLabels ) ;

      virtual HINumber* number( const string& iName,
				float         iValue );

      virtual HINumberWithErrors* numberWithErrors( 
	 const string& iName,
	 float         iValue,
	 float         iPositiveError,
	 float         iNegativeError );

      // Determines which histogram to fill corresponding
      // to a particular directory
      //virtual HIDirSwitch* mkSwitch();        

      virtual void setDefaultDir( const string& iDirectoryName );

      // ---------- const member functions ---------------------
      // Directory information
      virtual HIHistDir* defaultDir() const; // gets the default directory
      virtual HIHistDir* rootDir() const;    // gets the root directory */

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const membe functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookManager( const HbookManager& ); // stop default
      HbookManager();
      // ---------- assignment operator(s) ---------------------
      const HbookManager& operator=( const HbookManager& ); // stop default

      // ---------- private member functions -------------------
      HIHistDir* makeDefaultDir();

      // ---------- data members -------------------------------
      HbookFile& m_file;
      HbookHistDir* m_root;
      HIHistDir* m_default;

      ///keeping the name allows me to wait to create the default dir
      /// when the user first requests something from that directory
      string m_defaultDirName;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookManager.cc"
//#endif

#endif /* HBOOKHISTOGRAM_HBOOKMANAGER_H */
