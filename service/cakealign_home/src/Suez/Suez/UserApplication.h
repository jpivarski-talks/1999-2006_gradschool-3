#if !defined(JOBCONTROL_USERAPPLICATION_H)
#define JOBCONTROL_USERAPPLICATION_H
// -*- C++ -*-
//
// Package:     JobControl
// Module:      UserApplication
// 
// Description: This class must be provided by the user of
//      in order to build an application. It must define
//      the modules that are to form the basis of the application.
///
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Thu Mar 20 15:13:08 EST 1997
// $Id: UserApplication.h,v 1.2 2000/03/14 23:26:04 mkl Exp $
//
// Revision history (at end of file)
//

// system include files

// user include files


// forward declarations
class JobControl;
class Interpreter;

class UserApplication
{
      // friend classses and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      UserApplication( JobControl* jobControl );
      virtual ~UserApplication();

      // member functions
      DABoolean runEmbeddedCommands( Interpreter& interpreter );

      // const member functions
      virtual DABoolean interactiveMode() const;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      UserApplication( const UserApplication& );

      // assignment operator(s)
      const UserApplication& operator=( const UserApplication& );

      // private member functions

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

// ------------------------------------------------------
// Revision history
//
// $Log: UserApplication.h,v $
// Revision 1.2  2000/03/14 23:26:04  mkl
// make OSF1 compiler happy (such a better compiler)
//
// Revision 1.1  2000/03/14 20:55:23  mkl
// new UserApplication functionality: new embeddedCommand and interactiveMode methods for running Suez in standalone mode in online
//
// Revision 1.2  1997/07/11 06:55:57  mkl
// // New <Package>/<File>.h structure
// // Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/03/27 06:53:57  mkl
// imported sources.
//

#endif /* JOBCONTROL_USERAPPLICATION_H */
