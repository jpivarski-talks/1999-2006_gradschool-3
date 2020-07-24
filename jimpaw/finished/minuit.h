#include <Python.h>
#include <vector>
#include "Minuit/FCNBase.h"
#include "Minuit/MnUserParameters.h"
#include "Minuit/MinuitParameter.h"
#include "Minuit/MnMigrad.h"
#include "Minuit/FunctionMinimum.h"
#include "Minuit/MnMinos.h"
#include "Minuit/MnCross.h"
#include "Minuit/MnContours.h"

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

class ExceptionDuringMinimization {
   public:
      ExceptionDuringMinimization() {}
      ~ExceptionDuringMinimization() {}
};

class MyFCN : public FCNBase {
   public:
      MyFCN(PyObject *pfcn, int npar, double up) : m_pfcn(pfcn), m_npar(npar), m_up(up), m_printmode(0) { Py_INCREF(m_pfcn); }
      ~MyFCN() { Py_DECREF(m_pfcn); }
      double operator()(const std::vector<double>& par) const;
      PyObject *pfcn() const { return m_pfcn; }
      double up() const { return m_up; }
      int printmode() const { return m_printmode; }
      void setUp(double up) { m_up = up; }
      void setPrintmode(int printmode) { m_printmode = printmode; }
      void setOriginal(std::vector<double> par) { m_original = par; }
   private:
      PyObject *m_pfcn;
      int m_npar;
      double m_up;
      int m_printmode;
      std::vector<double> m_original;
};

typedef struct {
      PyObject_HEAD
      int npar;
      MyFCN* fcn;
      MnUserParameters* upar;
      FunctionMinimum* min;
      const MnUserCovariance* ucov;
      int maxcalls;
      double tol;
      int strategy;
      int ncalls;
      PyObject* log;
      bool migrad_valid;
      bool hesse_valid;
      bool minos_valid;
} minuit_Minuit;

