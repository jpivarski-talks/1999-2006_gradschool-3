#if !defined(CONSTANTSDELIVERY_CONSTANTSBINDER_H)
#define CONSTANTSDELIVERY_CONSTANTSBINDER_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      ConstantsSourceBinder
// 
// Description: 

// Usage:

// Invariants:
//
// Author:      Rich Baker
// Created:     Tues Nov 18 1997
//
// $Id: ConstantsSourceBinder.h,v 1.4 1999/06/30 23:13:12 cdj Exp $
//
// Revision history
//
// $Log: ConstantsSourceBinder.h,v $
// Revision 1.4  1999/06/30 23:13:12  cdj
// autogenerate code to create default Stream list
//
// Revision 1.3  1999/04/29 18:45:40  cdj
// updated to use new constants implementation
//
// Revision 1.2  1998/11/24 20:12:54  baker
// Change normal constructor for ConstantsSourceBinder
// to allow specification of the desired Federated
// Database.
// Added a Constructor for DBConstantsRecordInfo
// that takes a single string variable.
//
// Revision 1.1  1998/11/16 22:03:37  mkl
// renamed ConstantsBinder/Controller ConstantsSourceBinder/Controller
//
// Revision 1.5  1998/11/10 22:12:17  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.4  1998/11/04 18:55:50  baker
// Fix stupid errors so that this library actually compiles (finally!)
//
// Revision 1.3  1998/11/04 15:39:20  baker
// Continuing updates to get this library compiled
//
// Revision 1.2  1998/09/30 20:40:14  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:28:57  baker
// // September 30, Rich Baker
// // Renamed header files from former DataBaseDelivery
// // for new ConstantsDelivery library
//
// Revision 1.1.1.2  1998/01/14 20:56:56  baker
// make DataBaseDelivery library compileable
//
// Revision 1.1.1.1  1997/12/17 20:25:12  cleo3
// imported DataBaseDelivery sources
//


// system include files

// user include files
// 11/9/98 Rich Baker
// Added a Stream::Set data member to this class so that
// the streams to bind can be passed to the ConstantsSourceController
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
#include "DataDelivery/DataSourceBinder.h"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"

// stl include files

// forward declarations

// Jan 14, 1998
// Added declaration of DataSourceController (return type of member function)
class DataSourceController;

class ConstantsSourceBinder : public DataSourceBinder
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
// 11/24/98 Rich Baker
// New constructor to allow the user to specify the 
// Federated Database path to bind to, replacing the
// old constructor:
//declaration of old constructor --> ConstantsSourceBinder( 
//                               --> const Stream::Set& iBindStreams,
//                               --> const DBConstantsSelectionTag& tag );
      ConstantsSourceBinder( const string& fdbName,
			     const DBConstantsSelectionTag& tag,
			     const Stream::Set& iBindStreams );

      virtual ~ConstantsSourceBinder();

      // member functions

      // const member functions

      virtual DataSourceController*
        createDataSourceController( void ) const;

      const DBConstantsSelectionTag& selTag( void ) const;
      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ConstantsSourceBinder( const ConstantsSourceBinder& );

      // assignment operator(s)
      const ConstantsSourceBinder& operator=( const ConstantsSourceBinder& );

      // private member functions
      Stream::Set defaultStreams() const;

      // private const member functions

      // data members
      DBConstantsSelectionTag mSelectTag;

      // static data members

};

// inline function definitions

#endif /* CONSTANTSDELIVERY_CONSTANTSBINDER_H */

