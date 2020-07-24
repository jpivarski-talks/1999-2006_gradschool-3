#if !defined(MINUITINTERFACE_MIPARAMETER_H)
#define MINUITINTERFACE_MIPARAMETER_H
// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIParameter
// 
/**\class MIParameter MIParameter.h MinuitInterface/MIParameter.h

 Description: Holds parameter information used by Minuit

 Usage:
     You can get information about a parameter from MinuitInterface by
     calling MinuitInterface::parameter(). For example:
\code
     MIParameter constantTerm( theMinuitInterface->parameter(MyFcn::kConstantTerm) );
\endcode

Also, see the example code in the documentation for MinuitInterface.

MIParameters are usually constructed by MinuitInterface. You should
not have to construct one yourself. Also, you should not call the \c
set member functions. They are used by MinuitInterface. 

Note that the << operator can handle MIParameter objects for convenient 
print outs. For example:
\code
    report(INFO, kFacilityString) << myParameter;
\endcode


*/
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 15:34:33 EST 1999
// $Id: MIParameter.h,v 1.3 2002/08/16 15:24:39 cleo3 Exp $
//
// Revision history
//
// $Log: MIParameter.h,v $
// Revision 1.3  2002/08/16 15:24:39  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.2  1999/12/12 21:12:36  lyon
// Added extensive Doxygen documentation
//
// Revision 1.1.1.1  1999/12/09 15:34:50  lyon
// Imported MinuitInterface sources
//

// system include files
#include <string>

// user include files

// forward declarations
#include "MinuitInterface/MIInitialParameter.h"
#include "C++Std/iostream.h"

class MIParameter
{
      // ---------- friend classes and functions ---------------
      friend ostream& operator<<(ostream& aStream, 
				 const MIParameter& aParameter);

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MIParameter();
      MIParameter(const MIInitialParameter& initialValues);
      virtual ~MIParameter();

      // ---------- member functions ---------------------------
      /*! \internal */
      void setStart(double start);

      /*! \internal */
      void setStep(double step);

      /*! \internal */
      void setLimits(double low, double high);

      /*! \internal */
      void putResults(double value, double posErr, double negErr,
		      double parabolicErr, double globalCorrelation);

      

      // ---------- const member functions ---------------------

      //! Get the parameter's name
      inline string name() const;

      //! Get the start value
      inline double start() const;
      
      //! Get the step size
      inline double step() const;
      
      //! Get the low bound set for the parameter
      inline double lowLimit() const;

      //! Get the high bound set for the parameter
      inline double highLimit() const;

      //! Get the parameter's current value from the fit
      inline double value() const;

      //! Get the parabolic error from the fit
      inline double parabolicErr() const;

      //! Get the positive Minos error 
      inline double posErr() const;

      //! Get the negative Minos error 
      inline double negErr() const;

      //! Get the global correlation constant
      inline double globalCorrelation() const;

      //! Write out information to an output stream (used by <<)
      void dump(ostream& out) const;

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
      double m_value;
      double m_posErr;
      double m_negErr;
      double m_parabolicErr;
      double m_globalCorrelation;


      // ---------- static data members ------------------------

};

// inline function definitions
string MIParameter::name() const { return m_name; }
double MIParameter::start() const { return m_start; }
double MIParameter::step() const { return m_step; }
double MIParameter::lowLimit() const { return m_lowLimit; }
double MIParameter::highLimit() const { return m_highLimit; }
double MIParameter::value() const { return m_value; }
double MIParameter::posErr() const { return m_posErr; }
double MIParameter::negErr() const { return m_negErr; }
double MIParameter::parabolicErr() const { return m_parabolicErr; }
double MIParameter::globalCorrelation() const { return m_globalCorrelation; }

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "MinuitInterface/Template/MIParameter.cc"
//#endif

#endif /* MINUITINTERFACE_MIPARAMETER_H */
