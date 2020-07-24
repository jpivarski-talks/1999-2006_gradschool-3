#if !defined(PACKAGE_CHISQFORLADDER_H)
#define PACKAGE_CHISQFORLADDER_H
// -*- C++ -*-
//
// Package:     <ChisqForSiAlignment>
// Module:      ChisqForLadder
// 
/**\class ChisqForLadder ChisqForLadder.h package/ChisqForLadder.h

 Description: <one line class summary>
 
 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 10:00:31 EDT 2000
// $Id: ChisqForLadder.h,v 1.2 2001/02/06 20:31:25 llh14 Exp $
//
// Revision history
//
// $Log: ChisqForLadder.h,v $
// Revision 1.2  2001/02/06 20:31:25  llh14
// Added dof, and MaxEvent parameters
//
// Revision 1.1.1.1  2001/02/02 18:03:28  llh14
// imported ChisqForSiAlignment
//

// system include files

// user include files
#include "ChisqForSiAlignment/ChisqForSiAlignment.h"

#include "MinuitInterface/MIFcn.h"

#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "FrameAccess/FATable.h"

#include "HelixIntersection/HIIntersectionSurface.h"

//The data object you wish to change on each iteration
#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "BDLSVclient/DBSVAlignmentLadder.hh"

#include <fstream.h>
#include <iostream.h>

// forward declarations

class ChisqForLadder : public ChisqForSiAlignment
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum params { kDeltaX, kDeltaY, kDeltaZ, kPhiX, kPhiY, kPhiZ, kDoF};

      // ---------- Constructors and destructor ----------------
      ChisqForLadder(const FIFrameIterator& iBegin, 
		     const FIFrameIterator& iEnd,       
		     CLEOConstantsModifiable<DBSVAlignmentLadder>&  
		     SVLadderConstants,
		     float normRphi, float normZ, float cutRphi, float cutZ,
		     long int maxEvents, Count ladder, DABoolean varyX, 
		     DABoolean varyY, DABoolean varyZ, DABoolean varyPhiX, 
		     DABoolean varyPhiY, DABoolean varyPhiZ );

      virtual ~ChisqForLadder();

      // ---------- member functions ---------------------------

      virtual double iterate(double* values);
      inline virtual DABoolean selectComponent(SmallCount sensorNumber,
					       FAItem<ASiStore>& iStore);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ChisqForLadder( const ChisqForLadder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ChisqForLadder& operator=( const ChisqForLadder& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      ofstream m_out;
      FIFrameIterator m_iBegin;
      FIFrameIterator m_iEnd;
      CLEOConstantsModifiable<DBSVAlignmentLadder>& m_SVLadderConstants;
      Count m_LadderNumber;
      Count m_compCtr;

      float m_normRphi;
      float m_normZ;
      float m_cutRphi;
      float m_cutZ;	 

      DABoolean m_varyX;
      DABoolean m_varyY;
      DABoolean m_varyZ;
      DABoolean m_varyPhiX;
      DABoolean m_varyPhiY;
      DABoolean m_varyPhiZ;
			
      long int m_maxEvents;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/ChisqForLadder.cc"
//#endif

#endif /* PACKAGE_CHISQFORLADDER_H */
