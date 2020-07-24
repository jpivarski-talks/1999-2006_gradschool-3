#if !defined(PACKAGE_CHISQFORLAYER_H)
#define PACKAGE_CHISQFORLAYER_H
// -*- C++ -*-
//
// Package:     <ChisqForSiAlignment>
// Module:      ChisqForLayer
// 
/**\class ChisqForLayer ChisqForLayer.h package/ChisqForLayer.h

 Description: <one line class summary>
 
 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 10:00:31 EDT 2000
// $Id: ChisqForLayer.h,v 1.2 2001/02/06 20:31:25 llh14 Exp $
//
// Revision history
//
// $Log: ChisqForLayer.h,v $
// Revision 1.2  2001/02/06 20:31:25  llh14
// Added dof, and MaxEvent parameters
//
// Revision 1.1.1.1  2001/02/02 18:03:29  llh14
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
#include "BDLSVclient/DBSVAlignmentLayer.hh"

#include <fstream.h>
#include <iostream.h>

// forward declarations

class ChisqForLayer : public ChisqForSiAlignment
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum params { kDeltaX, kDeltaY, kDeltaZ, kPhiX, kPhiY, kPhiZ, kDoF };
	      

      // ---------- Constructors and destructor ----------------
      ChisqForLayer(const FIFrameIterator& iBegin, 
		    const FIFrameIterator& iEnd,       
		    CLEOConstantsModifiable<DBSVAlignmentLayer>&  
		    SVLayerConstants,
		    float normRphi, float normZ, float cutRphi, float cutZ,
		    long int maxEvents, Count layer, DABoolean varyX, 
		    DABoolean varyY, DABoolean varyZ, 
		    DABoolean varyPhiX, DABoolean varyPhiY, 
		    DABoolean varyPhiZ );

      virtual ~ChisqForLayer();

      // ---------- member functions ---------------------------

      virtual double iterate(double* values);
      virtual DABoolean selectComponent(SmallCount sensorNumber,
					       FAItem<ASiStore>& iStore);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ChisqForLayer( const ChisqForLayer& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ChisqForLayer& operator=( const ChisqForLayer& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      ofstream m_out;
      FIFrameIterator m_iBegin;
      FIFrameIterator m_iEnd;
      CLEOConstantsModifiable<DBSVAlignmentLayer>& m_SVLayerConstants;
      Count m_layer;
		 
      float m_normRphi;
      float m_normZ;
      float m_cutRphi;
      float m_cutZ;

      long int m_maxEvents;

      DABoolean m_varyX;
      DABoolean m_varyY;
      DABoolean m_varyZ;
      DABoolean m_varyPhiX;
      DABoolean m_varyPhiY;
      DABoolean m_varyPhiZ;
		
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/ChisqForLayer.cc"
//#endif

#endif /* PACKAGE_CHISQFORLAYER_H */
