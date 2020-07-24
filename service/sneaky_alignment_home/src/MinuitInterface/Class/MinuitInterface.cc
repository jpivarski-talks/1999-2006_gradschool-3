// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MinuitInterface
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 16:14:51 EST 1999
// $Id: MinuitInterface.cc,v 1.6 2003/01/29 22:00:39 cleo3 Exp $
//
// Revision history
//
// $Log: MinuitInterface.cc,v $
// Revision 1.6  2003/01/29 22:00:39  cleo3
// use pointer to function type rather than void * when declaring FORTRAN interface
//
// Revision 1.5  2000/07/13 15:21:41  lyon
// rumMigrad now returns command status ; flush fortran buffers so printouts appear correctly ; prescale minuit command error message
//
// Revision 1.4  2000/06/21 14:03:45  lyon
// Report to the user if there is a fit failure, instead of an assertion
//
// Revision 1.3  1999/12/12 21:12:33  lyon
// Added extensive Doxygen documentation
//
// Revision 1.2  1999/12/12 04:23:36  lyon
// Added Doxygeniated documenation
//
// Revision 1.1.1.1  1999/12/09 15:34:51  lyon
// Imported MinuitInterface sources
//

#include "Experiment/Experiment.h"
#include "Experiment/report.h"
#include <stdio.h>
#include <assert.h>

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "MinuitInterface/MinuitInterface.h"
#include "ToolBox/ReportControl.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "MinuitInterface.MinuitInterface" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MinuitInterface.cc,v 1.6 2003/01/29 22:00:39 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

MinuitInterface* MinuitInterface::m_instance = 0;

typedef void (* MIFCNType)(FInteger*, FDouble*, FDouble*, FDouble*, FInteger*, void*);

extern "C" 
{

  // The FCN function
  void MIFCNInternal(FInteger* npar, FDouble* grad, FDouble* fval, 
	     FDouble* xval, FInteger* iflag, void* futil);

  // initialize Minuit
  void mninit_(FInteger* in, FInteger* out, FInteger* save);
  
  // Specify parameters
  void mnparm_(FInteger* num, const FChar* name, 
	       FDouble* start, FDouble* step, 
	       FDouble* lowLimit, FDouble* highLimit, 
	       FInteger* ok, FLength clen);

  // Execute a minuit command
  void mnexcm_(void* fcn, const FChar* command, FInteger* param,
	       FInteger* nparam, FInteger* ierr, void* futil, FLength len);

  // Execuate a minuit command as a big string
  void mncomd_(MIFCNType fcn, const FChar* command, FInteger* icond, void* futil,
	       FLength len);

  // Get parameter information
  void mnpout_(FInteger* num, FChar* name, FDouble* value, FDouble* err,
	       FDouble* low, FDouble* high, FInteger* iparam, FLength clen);
  
  // Get minuit status information
  void mnstat_(FDouble* fmin, FDouble* fedn, FDouble* errdef, 
	       FInteger* npari, FInteger* nparx, FInteger* istat);

  // Get parameter error information
  void mnerrs_(FInteger* num, FDouble* eplus, FDouble* eneg, 
	       FDouble* eparab, FDouble* globcc);

  // Get the covariance matrix
  void mnemat_(FDouble* minuitEMAT, FInteger* ndim);

  // Go into interactive mode
  void mnintr_(MIFCNType fcn, void* futil);

  // Flush to get output out
  void flush_( const FInteger& iUnit ) ;
}

//
// constructors and destructor
//
// This is a private constructor since class is singleton
MinuitInterface::MinuitInterface() :
  m_fcn(0),
  m_parameters(0),
  m_covMatrix(0),
  m_diagLevel(kMute)
{
  // Initialize Minuit Fortran
  FInteger inputunit = 5;
  FInteger outputunit = 6;
  FInteger saveunit = 7;
  mninit_(&inputunit, &outputunit, &saveunit);

  // Set the diagnostics level in Minuit
  char buf[80];
  sprintf(buf, "SET PRI %d", m_diagLevel);
    runMinuitCommand(buf);

  flush();
}

// MinuitInterface::MinuitInterface( const MinuitInterface& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MinuitInterface::~MinuitInterface()
{

  // Do we already have parameters?
  if ( m_parameters ) 
  {
    // Delete them
    delete m_parameters;
  }  

  // Delete the covariance matrix
  if ( m_covMatrix ) delete m_covMatrix;
}


//
// assignment operators
//
// const MinuitInterface& MinuitInterface::operator=( const MinuitInterface& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

// Load the function to minimize.
/*!
  This registers your Fcn object with MinuitInterface. If an old Fcn is
  already registered, it is replaced by the new one and any
  parameters and error matrices held internally by Minuit are
  discarded. You are still responsible for memory of and within the
  Fcn object.
  \param theFcn The Fcn object you want Minuit to use.
*/
void MinuitInterface::loadFcn(MIFcn& theFcn)
{
  m_fcn = &theFcn;

  // Tell fcn the current diag level
  m_fcn->setDiagLevel(m_diagLevel);

  // Do we already have parameters?
  if ( m_parameters != 0 ) 
  {
    // Delete them
    delete m_parameters;
  }
  
  // Make an empty parameter list
  m_parameters = new STL_VECTOR(MIParameter)();

  // Make parameters from the initial parameters in the function
  const STL_VECTOR(MIInitialParameter)* ips = m_fcn->initialParameters();

  // Make sure we have initial parameters
  if ( ips == 0 ) 
  {
    report(EMERGENCY, kFacilityString) << "FCN gave me a null pointer "
				       << "for the initial parameter list."
				       << " That's very bad!" 
				       << endl;
    assert(ips != 0);
  }

  // Loop over initial parameters, making full parameters
  for ( unsigned int i = 0; i < ips->size(); ++i )
  {

    const MIInitialParameter& ip = (*ips)[i];

    // Make a parameter and add it to our list
    m_parameters->push_back( ip );
    
    // Tell minuit about it
    setMinuitParameter(i+1);

  }
    
}

// Change the start value of a parameter (we assume that parameter 1 has 
//  paramIndex of 0 !!)
/*!
  When you load your Fcn, Minuit registers your initial
  parameters (MIInitialParameter) defined in the Fcn
  object. You may override the default start value of the
  parameter with this member function. Furthermore, if you want
  to fix a parameter to a certain value, use this member
  function to set that value, if you do not want the default.
  
  \param paramIndex The parameter index number (starts at 0, so
  the first parameter has index=0). USE THE ENUM's in your Fcn object.
  \param startValue The starting value of the parameter.
*/
void MinuitInterface::changeParameterStart(unsigned int paramIndex, 
					   double startValue)
{

  int paramNum = ++paramIndex;

  checkParamNum(paramNum);
  
  (*m_parameters)[paramIndex].setStart(startValue);
  setMinuitParameter(paramNum);
}

// Change step
/*!
  \sa changeParameterStart
*/
void MinuitInterface::changeParameterStep(unsigned int paramIndex, 
					  double step)
{

  int paramNum = ++paramIndex;

  checkParamNum(paramNum);
  
  (*m_parameters)[paramIndex].setStep(step);
  setMinuitParameter(paramNum);
}

// Change limits
/*!
  \sa changeParameterStart
*/
void MinuitInterface::changeParameterLimits(unsigned int paramIndex, 
					    double low, double high)
{
  int paramNum = ++paramIndex;
  
  checkParamNum(paramNum);
  
  (*m_parameters)[paramIndex].setLimits(low, high);
  setMinuitParameter(paramNum);
}

// Fix a parameter to its start value
/*!
  Use changeParameterStart
  to set the start value, if you do not want the default.
  \sa changeParameterStart
*/
void MinuitInterface::fixParameter(unsigned int paramIndex)
{
  
  int paramNum = ++paramIndex;
  
  checkParamNum(paramNum);

  // Tell minuit to fix the parameter
  char buf[80];
  sprintf(buf, "FIX %d", paramNum);;
  
  runMinuitCommand(buf);
}

// Release a parameter
/*!
  \sa changeParameterStart
*/
void MinuitInterface::releaseParameter(unsigned int paramIndex)
{
  int paramNum = ++paramIndex;
  
  checkParamNum(paramNum);

  // Tell minuit to release the parameter
  char buf[80];
  sprintf(buf, "RELEASE %d", paramNum);;
  
  runMinuitCommand(buf);
}

// Interactive mode
/*!
  Calling this member function will cause Minuit to go into
  interactive mode, accepting commands from the keyboard. You
  can run Migrad, Minos, or most any other Minuit function to
  your heart's content. The only thing you must NOT do is change 
  the number of parameters involved in the fit (but fixing and
  releasing is fine). Exit interactive with the "Exit"
  command. The Minuit parameter values and errors are then
  transferred to C++, so you can get obtain that information
  with calls to parameter and covarianceMatrix.
  \sa parameter covarianceMatrix
*/
void MinuitInterface::interact()
{
  checkForFcn();

  report(WARNING, kFacilityString) << "Warning: Some actions you take "
				   << "while in " 
				   << "interactive mode will not\n"
				   << "be noticed by the C++ program.\n"
				   << "Do NOT add or remove parameters! "
				   << "(fixing and releasing are ok)\n"
				   << "Use EXIT to return from interactive "
				   << "mode."
				   << endl;

  // Begin interactive mode
  void* futil = 0;
  mnintr_(MIFCNInternal, futil);

  // Send any results to the parameters
  resultsToParameters();
}

// Run MiGrad and store results
/*!
  Run the MiGrad minimization. This will seek values of the
  parameters that minimizes the function in the Fcn. Parabolic
  errors will also be determined. To get the minimization
  results, use the parameter member function (and perhaps
  covarianceMatrix). This function returns the Minuit return value
  of the MIGRAD call. 0 means all is well. Parameters are filled regardless
  of the return status.
  \sa parameter covarianceMatrix
*/
int MinuitInterface::runMigrad()
{
  checkForFcn();

  string comm("MIGRAD");

  // Run the command
  int ok = runMinuitCommand(comm);

  // Put the results into the parameters
  resultsToParameters();

  return ok;
}

// Run Simplex and store results
/*!
  Run the Simplex minimization. This will seek values of the
  parameters that minimizes the function in the Fcn. Unlike MIGRAD,
  SIMPLEX doesn't calculate gradients, so no errors will be
  determined. Also, Minuit's interpretation of tolerance is 100 times
  less stringent for SIMPLEX than Minuit, so it is advisable to give
  one explicitly. Unless you have redefined UP to be != 1.0, SIMPLEX
  will stop when it reaches an EDM of 0.1 * tolerance.  Because
  maxcalls comes before tolerance in the argument list, you'll have to
  specify that too. This function returns the Minuit return value of
  the call: 0 means all is well. Parameters are filled regardless of
  the return status.
  \sa parameter
*/
int MinuitInterface::runSimplex(int maxcalls, double tolerance)
{
  checkForFcn();

  char args[80];
  sprintf(args, " %d %g", maxcalls, tolerance);

  string comm("SIMPLEX");
  comm += args;

  // Run the command
  int ok = runMinuitCommand(comm);

  // Put the results into the parameters
  resultsToParameters();

  return ok;
}

// Run MINOS error analysis
/*!
  Run the Minos error determination. This will determine
  asymptotic errors on the parameters. Typical usage is to run
  MiGrad first, then Minos, and then obtain the parameters. See
  the example in the comments above. This function returns the Minuit
  return value of the MINOS call. 0 means that all is well. Parameters
  are filled regardless of the return status.
  \sa runMigrad parameter covarianceMatrix
*/
int MinuitInterface::runMinos()
{
  checkForFcn();

  string comm("MINOS");
  
  // Run the command
  int ok = runMinuitCommand(comm);
  
  // Put the results into the parameters
  resultsToParameters();

  return ok;
}

// Run a minuit command (without parameters)
int MinuitInterface::runMinuitCommand(string comm)
{
  

  static ReportControl repCon ( ALERT,
				kFacilityString,
				"Minuit Command Failed: " , // message preface
				5,           // geometric prescale factor
				200  ) ;  // plateau


  FInteger icond;
  void* futil = 0;

  mncomd_(MIFCNInternal, comm.c_str(), &icond, futil, comm.length() );

  flush();

  if ( icond != 0 )
  {
    if( repCon.nCalls() < repCon.plateau() )
    {
      repCon() << comm << ", Return Code = " << icond << endl;
    }
  }

  return icond;
  
}

// Change the diagnostics level
/*!
  Set the diagnostics level (print out level) for Minuit and
  your Fcn object. Use the enum diagLevels above.
  \sa diagLevels
*/
void MinuitInterface::setDiagLevel(int level)
{
  m_diagLevel = level;

  char buf[80];
  
  sprintf(buf, "SET PRI %d", level);
  
  // Set the diagnostics level in Minuit
  string comm(buf);
  
  runMinuitCommand(comm);

  // Set the diagnostics level within the fcn if there is one
  if ( m_fcn != 0 ) m_fcn->setDiagLevel(level);
}

// Put the results of a minimization or error seeking into the parameters
void MinuitInterface::resultsToParameters()
{

  int nParams = m_parameters->size();

  // Loop over the parameters
  for ( unsigned int i = 0 ; i < nParams ; ++i )
  {

    // Get information about this parameter
    FInteger num = i+1;
    FChar name[80];
    FDouble value, err, low, high;
    FInteger iparam;
    FDouble eplus, eneg, eparab, globcc;

    FInteger len = 79;
    
    mnpout_(&num, name, &value, &err, &low, &high, &iparam, len);

    mnerrs_(&num, &eplus, &eneg, &eparab, &globcc);

    // Fill the parameter information
    (*m_parameters)[i].putResults(value, eplus, eneg, eparab, globcc);

  } // for (over parameters)

  // Now get the covariance matrix
  // Do we already have a covariance matrix?
  if ( m_covMatrix )
  {
    // Junk it
    delete m_covMatrix;
  }

  // Make a new covariance matrix of the correct size
  m_covMatrix = new HepMatrix(nParams, nParams);
  
  // Make a space for minuit to copy the matrix
  double* minuitEMAT = new double[nParams*nParams];

  // Get the matrix from minuit
  mnemat_(minuitEMAT, &nParams);

  // Convert the workspace matrix to the HepMatrix
  // Be sure to get the ordering right!
  unsigned int j = 0;
  for ( unsigned int l = 0; l < nParams; ++l )
  {
    for ( unsigned int m = 0; m < nParams; ++m )
    {
      // Yes - should be m,l to get Fortran ordering
      (*m_covMatrix)[m][l] = minuitEMAT[j];
      ++j;
    }
  }

  // Delete the workspace (since newed with [], must delete[])
  delete[] minuitEMAT;
}

// Get the Covariance matrix (a copy)
/*!
  \return A HepMatrix object (see $C3_INC/CLHEP/Matrix/Matrix.h)
  with the covariance matrix. You
  can get matrix elements easily with the [] notation and the
  Fcn enums. See the example in the comments above.
*/
HepMatrix MinuitInterface::covarianceMatrix() const
{
  // Return a deep copy
  return *m_covMatrix;
}

// Get minuit statistics
MIStats MinuitInterface::minuitStats() const
  /*!
    \return A MIStats object with information about the status of 
    Minuit.
  */
{
  
  FDouble fmin, fedn, errdef;
  FInteger npari, nparx, istat;
  
  mnstat_(&fmin, &fedn, &errdef, &npari, &nparx, &istat);

  // Make a MIStats object and return
  return MIStats(fmin, fedn, errdef, npari, nparx, istat);
}

// Get results from the fit
/*!
  \param paramIndex The index of the desired parameter (use the
  Fcn's enum).
  \return An MIParameter object of the desired parameter. A
  copy is returned, so the object is still valid if you register 
  another Fcn with MinuitInterface.
*/
MIParameter MinuitInterface::parameter(int paramIndex) const
{
  checkParamNum(paramIndex+1);

  return (*m_parameters)[paramIndex];
}

// Check if this parameter number is legal
void MinuitInterface::checkParamNum(unsigned int paramNum) const
{
  checkForFcn();

  if ( paramNum < 1 || paramNum > m_parameters->size() )
  {
    report(EMERGENCY, kFacilityString) << "Parameter Number " 
				       << paramNum << " is invalid" << endl;
    assert(false);
  }
}

// Check if we have an FCN loaded
void MinuitInterface::checkForFcn() const
{
  if ( m_fcn == 0 ) 
  {
    report(EMERGENCY, kFacilityString) << "There is no FCN loaded!" 
				       << endl;
    assert(false);
  }
}


// Flush the output buffer
void MinuitInterface::flush() const
{
  FInteger lun = 6;
  flush_(lun);
}

// Set Minuit parameter with MNPARM function  
// (num starts at 1)
void MinuitInterface::setMinuitParameter(unsigned int paramNum)
{
  int ok;
  int i = paramNum - 1;

  FInteger o_num = paramNum;
  string o_name = (*m_parameters)[i].name();
  FDouble o_start = (*m_parameters)[i].start();
  FDouble o_step = (*m_parameters)[i].step();
  FDouble o_lowLimit = (*m_parameters)[i].lowLimit();
  FDouble o_highLimit = (*m_parameters)[i].highLimit();
  
  mnparm_( &o_num, o_name.c_str(), &o_start, &o_step, &o_lowLimit, 
	   &o_highLimit, &ok, o_name.length() );

  if ( ok != 0 )
  {
    report(EMERGENCY, kFacilityString) << "Cannot set parameter " << paramNum
				       << " return code was " << ok << endl;
    assert(ok == 0);
  }
}

// Return the fcn
/*!
  This member function returns a pointer to the Fcn object that
  MinuitInterface currently has registered. You shouldn't have
  to call this function under normal circumstances.
*/
MIFcn* MinuitInterface::fcn() const
{
  return m_fcn;
}

// static member functions

// Get the one and only Minuit instance
/*!
  Use this member function to get the instance of
  MinuitInterface in the system. See the example in the
  comments above.
*/
MinuitInterface* MinuitInterface::instance()
{
  // Do we already have a Minuit Interface?
  if ( m_instance == 0 )
    m_instance = new MinuitInterface; // Make it

  // Return the one and only instance
  return m_instance;
}





