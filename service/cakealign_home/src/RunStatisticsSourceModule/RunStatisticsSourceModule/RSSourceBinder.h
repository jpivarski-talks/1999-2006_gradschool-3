#if !defined(RUNSTATISTICSSOURCEMODULE_RSSOURCEBINDER_H)
#define RUNSTATISTICSSOURCEMODULE_RSSOURCEBINDER_H
// -*- C++ -*-
//
// Package:     RunStatisticsSourceModule
// Module:      RSSourceBinder
// 
/**\class RSSourceBinder RSSourceBinder.h RunStatisticsSourceModule/RSSourceBinder.h

 Description: Factory for RSSourceController

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Sat Jan 29 10:03:17 EST 2000
// $Id: RSSourceBinder.h,v 1.1.1.1 2000/01/29 20:47:36 cdj Exp $
//
// Revision history
//
// $Log: RSSourceBinder.h,v $
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files

// user include files
#include "DataDelivery/DataSourceBinder.h"
#include "RunStatisticsSourceModule/RSSourceController.h"

// forward declarations

class RSSourceBinder : public DataSourceBinder
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      RSSourceBinder( const Stream::Set& iBindStreams ) :
	 DataSourceBinder( RSSourceController::kDataSourceID, "", iBindStreams) {}
      //virtual ~RSSourceBinder();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      DataSourceController* createDataSourceController() const {
	 return new RSSourceController( boundStreams() );
      }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      RSSourceBinder( const RSSourceBinder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const RSSourceBinder& operator=( const RSSourceBinder& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "RunStatisticsSourceModule/Template/RSSourceBinder.cc"
//#endif

#endif /* RUNSTATISTICSSOURCEMODULE_RSSOURCEBINDER_H */
