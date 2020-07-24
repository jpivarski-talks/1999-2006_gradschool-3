#if !defined(MINUITINTERFACE_MIINITIALPARAMETER_H)
#define MINUITINTERFACE_MIINITIALPARAMETER_H
// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIInitialParameter
// 
/**\class MIInitialParameter MIInitialParameter.h MinuitInterface/MIInitialParameter.h

 Description: Holds initial parameter information used by Minuit -- internal use only

 Usage:
    MIInitialParameters are created by MIFcn (and inherited classes)
    and are used exclusively by MinuitInterface.  They hold initial
    parameter information used to make MIParameter objects. Users
    should typically never deal with MIInitialParameters. 

    See MIParameter for results of fits.

*/
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 15:34:33 EST 1999
// $Id: MIInitialParameter.h,v 1.2 1999/12/12 21:12:36 lyon Exp $
//
// Revision history
//
// $Log: MIInitialParameter.h,v $
// Revision 1.2  1999/12/12 21:12:36  lyon
// Added extensive Doxygen documentation
//
// Revision 1.1.1.1  1999/12/09 15:34:50  lyon
// Imported MinuitInterface sources
//

// system include files

// user include files
#include <string>

// forward declarations

class MIInitialParameter
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MIInitialParameter();
      MIInitialParameter(string name, double start, double step, 
			 double low, double high);
      MIInitialParameter(string name, double start, double step);
		       
      virtual ~MIInitialParameter();

      // ---------- member functions ---------------------------      

      // ---------- const member functions ---------------------
      inline string name() const;
      inline double start() const;
      inline double step() const;
      inline double lowLimit() const;
      inline double highLimit() const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      string m_name;
      double m_start;
      double m_step;
      double m_lowLimit;
      double m_highLimit;

      // ---------- static data members ------------------------

};

// inline function definitions
string MIInitialParameter::name() const { return m_name; }
double MIInitialParameter::start() const { return m_start; }
double MIInitialParameter::step() const { return m_step; }
double MIInitialParameter::lowLimit() const { return m_lowLimit; }
double MIInitialParameter::highLimit() const { return m_highLimit; }

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "MinuitInterface/Template/MIInitialParameter.cc"
//#endif

#endif /* MINUITINTERFACE_MIINITIALPARAMETER_H */
