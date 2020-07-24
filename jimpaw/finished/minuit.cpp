// g++ minuit.cpp -I /usr/include/python2.4 -I Minuit-1_5_2/ Minuit-1_5_2/src/*.o -shared -o _minuit.so

#include "minuit.h"

static PyObject *PyExc_MinuitError;

static int minuit_Minuit_init(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   PyObject *pfcn;
   int npar;
   double up;
   PyObject *pnames;
   std::vector<const char*> names;
   
   char errorstring[256] = "Initialize _Minuit with callable, int number of parameters, up (1. for chi**2, 0.5 for -loglike), tuple of names (strings).";

   if (!PyArg_ParseTuple(args, "OidO", &pfcn, &npar, &up, &pnames)) {
      PyErr_SetString(PyExc_TypeError, errorstring);
      return -1;
   }
   if (!PyCallable_Check(pfcn)) {
      PyErr_SetString(PyExc_TypeError, errorstring);
      return -1;
   }
   if (!PyTuple_Check(pnames)) {
      PyErr_SetString(PyExc_TypeError, errorstring);
      return -1;
   }
   if (PyTuple_Size(pnames) != npar) {
      PyErr_SetString(PyExc_TypeError, errorstring);
      return -1;
   }
   for (int i = 0;  i < npar;  i++) {
      PyObject *obj = PyTuple_GetItem(pnames, i);
      if (obj == NULL) return -1;
      if (!PyString_Check(obj)) {
	 PyErr_SetString(PyExc_TypeError, errorstring);
	 return -1;
      }
      names.push_back(PyString_AsString(obj));
   }
   if (npar < 1) {
      PyErr_SetString(PyExc_ValueError, "A constant function can't be minimized!");
      return -1;
   }

   self->npar = npar;
   self->fcn = new MyFCN(pfcn, npar, up);
   self->upar = new MnUserParameters();
   for (int i = 0;  i < npar;  i++) {
      self->upar->add(names[i], 0., 1.);
   }
   self->min = NULL;
   self->ucov = NULL;
   self->maxcalls = 0;
   self->tol = 0.1;
   self->strategy = 1;
   self->ncalls = 0;
   self->log = PyList_New(0);
   self->migrad_valid = false;
   self->hesse_valid = false;
   self->minos_valid = false;

   return 0;
};

static int minuit_Minuit_dealloc(minuit_Minuit* self) {
   if (self->fcn) delete self->fcn;
   if (self->upar) delete self->upar;
   if (self->min) delete self->min;
   Py_DECREF(self->log);
}

static PyObject* minuit_Minuit_value(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 return Py_BuildValue("d", self->upar->value(i));
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_error(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 return Py_BuildValue("d", self->upar->error(i));
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_eps(minuit_Minuit* self) {
   MnMachinePrecision mp;
   return Py_BuildValue("d", mp.eps());
}

static PyObject* minuit_Minuit_npar(minuit_Minuit* self) {
   return Py_BuildValue("i", self->npar);
}

static PyObject* minuit_Minuit_maxcalls(minuit_Minuit* self) {
   return Py_BuildValue("i", self->maxcalls);
}

static PyObject* minuit_Minuit_tol(minuit_Minuit* self) {
   return Py_BuildValue("d", self->tol);
}

static PyObject* minuit_Minuit_strategy(minuit_Minuit* self) {
   return Py_BuildValue("i", self->strategy);
}

static PyObject* minuit_Minuit_fcn(minuit_Minuit* self) {
   return self->fcn->pfcn();
}

static PyObject* minuit_Minuit_up(minuit_Minuit* self) {
   return Py_BuildValue("d", self->fcn->up());
}

static PyObject* minuit_Minuit_ncalls(minuit_Minuit* self) {
   return Py_BuildValue("i", self->ncalls);
}

static PyObject* minuit_Minuit_printmode(minuit_Minuit* self) {
   return Py_BuildValue("i", self->fcn->printmode());
}

static PyObject* minuit_Minuit_log(minuit_Minuit* self) {
   Py_INCREF(self->log);
   return self->log;
}

static PyObject* minuit_Minuit_fval(minuit_Minuit* self) {
   if (self->min != NULL) {
      return Py_BuildValue("d", self->min->fval());
   }
   else {
      PyErr_SetString(PyExc_MinuitError, "You must run MIGRAD first.");
      return NULL;
   }
}

static PyObject* minuit_Minuit_edm(minuit_Minuit* self) {
   if (self->min != NULL) {
      return Py_BuildValue("d", self->min->edm());
   }
   else {
      PyErr_SetString(PyExc_MinuitError, "You must run MIGRAD first.");
      return NULL;
   }
}

static PyObject* minuit_Minuit_covariance(minuit_Minuit* self) {
   if (self->ucov == NULL) {
      PyErr_SetString(PyExc_MinuitError, "You must successfully execute MIGRAD or HESSE first.");
      return NULL;
   }
   int size = self->npar;

   PyObject *cov = PyList_New(size);
   if (cov == NULL) return NULL;

   int I = 0;  // index over all parameters
   int i = 0;  // index counting only floating parameters
   while (I < size) {
      PyObject *row = PyList_New(size);
      if (row == NULL) {
	 Py_DECREF(cov);
	 return NULL;
      }

      int J = 0;  // index over all parameters
      int j = 0;  // index counting only floating parameters
      while (J < size) {
	 double num;
	 if (self->min->userParameters().parameter(I).isFixed()  ||
	     self->min->userParameters().parameter(J).isFixed()) {
	    num = 0.;
	 }
	 else {
	    num = (*self->ucov)(i, j);
	 }
	 PyObject *elem = PyFloat_FromDouble(num);

	 if (elem == NULL  ||  PyList_SetItem(row, J, elem) != 0) {
	    Py_DECREF(row);
	    Py_DECREF(cov);
	    return NULL;
	 }

	 if (!self->min->userParameters().parameter(J).isFixed()) j++;
	 J++;
      }

      if (PyList_SetItem(cov, I, row) != 0) {
	 Py_DECREF(row);
	 Py_DECREF(cov);
	 return NULL;
      }

      if (!self->min->userParameters().parameter(I).isFixed()) i++;
      I++;
   }

   return cov;
}

static PyObject* minuit_Minuit_covariance_floating(minuit_Minuit* self) {
   if (self->ucov == NULL) {
      PyErr_SetString(PyExc_MinuitError, "You must successfully execute MIGRAD or HESSE first.");
      return NULL;
   }
   int size = self->min->userParameters().variableParameters();

   PyObject *cov = PyList_New(size);
   if (cov == NULL) return NULL;

   int i = 0;  // index counting only floating parameters
   while (i < size) {
      PyObject *row = PyList_New(size);
      if (row == NULL) {
	 Py_DECREF(cov);
	 return NULL;
      }

      int j = 0;  // index counting only floating parameters
      while (j < size) {
	 PyObject *elem = PyFloat_FromDouble((*self->ucov)(i, j));
	 if (elem == NULL  ||  PyList_SetItem(row, j, elem) != 0) {
	    Py_DECREF(row);
	    Py_DECREF(cov);
	    return NULL;
	 }

	 j++;
      }

      if (PyList_SetItem(cov, i, row) != 0) {
	 Py_DECREF(row);
	 Py_DECREF(cov);
	 return NULL;
      }

      i++;
   }

   return cov;
}

static PyObject* minuit_Minuit_globalcc(minuit_Minuit* self) {
   if (self->ucov == NULL) {
      PyErr_SetString(PyExc_MinuitError, "You must successfully execute MIGRAD or HESSE first.");
      return NULL;
   }
   int size = self->npar;

   PyObject *cov = PyList_New(size);
   if (cov == NULL) return NULL;

   int I = 0;  // index over all parameters
   int i = 0;  // index counting only floating parameters
   while (I < size) {
      PyObject *row = PyList_New(size);
      if (row == NULL) {
	 Py_DECREF(cov);
	 return NULL;
      }

      int J = 0;  // index over all parameters
      int j = 0;  // index counting only floating parameters
      while (J < size) {
	 double num;
	 if (self->min->userParameters().parameter(I).isFixed()  ||
	     self->min->userParameters().parameter(J).isFixed()) {
	    num = 0.;
	 }
	 else {
	    num = (*self->ucov)(i, j) / sqrt((*self->ucov)(i, i)) / sqrt((*self->ucov)(j, j));
	 }
	 PyObject *elem = PyFloat_FromDouble(num);

	 if (elem == NULL  ||  PyList_SetItem(row, J, elem) != 0) {
	    Py_DECREF(row);
	    Py_DECREF(cov);
	    return NULL;
	 }

	 if (!self->min->userParameters().parameter(J).isFixed()) j++;
	 J++;
      }

      if (PyList_SetItem(cov, I, row) != 0) {
	 Py_DECREF(row);
	 Py_DECREF(cov);
	 return NULL;
      }

      if (!self->min->userParameters().parameter(I).isFixed()) i++;
      I++;
   }

   return cov;
}

static PyObject* minuit_Minuit_globalcc_floating(minuit_Minuit* self) {
   if (self->ucov == NULL) {
      PyErr_SetString(PyExc_MinuitError, "You must successfully execute MIGRAD or HESSE first.");
      return NULL;
   }
   int size = self->min->userParameters().variableParameters();

   PyObject *cov = PyList_New(size);
   if (cov == NULL) return NULL;

   int i = 0;  // index counting only floating parameters
   while (i < size) {
      PyObject *row = PyList_New(size);
      if (row == NULL) {
	 Py_DECREF(cov);
	 return NULL;
      }

      int j = 0;  // index counting only floating parameters
      while (j < size) {
	 PyObject *elem = PyFloat_FromDouble((*self->ucov)(i, j) / sqrt((*self->ucov)(i, i)) / sqrt((*self->ucov)(j, j)));
	 if (elem == NULL  ||  PyList_SetItem(row, j, elem) != 0) {
	    Py_DECREF(row);
	    Py_DECREF(cov);
	    return NULL;
	 }

	 j++;
      }

      if (PyList_SetItem(cov, i, row) != 0) {
	 Py_DECREF(row);
	 Py_DECREF(cov);
	 return NULL;
      }

      i++;
   }

   return cov;
}

//////////////////////////////////////////////////////////////

static PyObject* minuit_Minuit_setValue(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   double num;
   if (!PyArg_ParseTuple(args, "sd", &name, &num)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter and its new value.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 self->upar->setValue(i, num);

	 char item[256];
	 sprintf(item, "User set parameter \"%s\" value to %g.", name, num);
	 PyObject *pstring = PyString_FromString(item);
	 if (PyList_Append(self->log, pstring) != 0) return NULL;

	 return Py_None;
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_setError(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   double num;
   if (!PyArg_ParseTuple(args, "sd", &name, &num)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter and its new uncertainty.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 self->upar->setError(i, num);

	 char item[256];
	 sprintf(item, "User set parameter \"%s\" error to %g.", name, num);
	 PyObject *pstring = PyString_FromString(item);
	 if (PyList_Append(self->log, pstring) != 0) return NULL;

	 return Py_None;
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_fix(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 self->upar->fix(i);

	 char item[256];
	 sprintf(item, "User fixed parameter \"%s\".", name);
	 PyObject *pstring = PyString_FromString(item);
	 if (PyList_Append(self->log, pstring) != 0) return NULL;

	 return Py_None;
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_release(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 self->upar->release(i);

	 char item[256];
	 sprintf(item, "User released parameter \"%s\".", name);
	 PyObject *pstring = PyString_FromString(item);
	 if (PyList_Append(self->log, pstring) != 0) return NULL;

	 return Py_None;
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_setUpper(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   double num;
   if (!PyArg_ParseTuple(args, "sd", &name, &num)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter and its new upper limit.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 self->upar->setUpperLimit(i, num);

	 char item[256];
	 sprintf(item, "User set parameter \"%s\" upper limit to %g.", name, num);
	 PyObject *pstring = PyString_FromString(item);
	 if (PyList_Append(self->log, pstring) != 0) return NULL;

	 return Py_None;
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_setLower(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   double num;
   if (!PyArg_ParseTuple(args, "sd", &name, &num)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter and its new lower limit.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 self->upar->setLowerLimit(i, num);

	 char item[256];
	 sprintf(item, "User set parameter \"%s\" lower limit to %g.", name, num);
	 PyObject *pstring = PyString_FromString(item);
	 if (PyList_Append(self->log, pstring) != 0) return NULL;

	 return Py_None;
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_unlimit(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "You must pass the name of a parameter.");
      return NULL;
   }

   for (int i = 0;  i < self->npar;  i++) {
      if (strcmp(name, self->upar->name(i)) == 0) {
	 self->upar->removeLimits(i);

	 char item[256];
	 sprintf(item, "User removed limits from parameter \"%s\".", name);
	 PyObject *pstring = PyString_FromString(item);
	 if (PyList_Append(self->log, pstring) != 0) return NULL;

	 return Py_None;
      }
   }
   
   PyErr_Format(PyExc_ValueError, "Unrecognized parameter name: \"%s\".", name);
   return NULL;
}

static PyObject* minuit_Minuit_setUp(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   double num;
   if (!PyArg_ParseTuple(args, "d", &num)) {
      PyErr_SetString(PyExc_TypeError, "Up must be a number.");
      return NULL;
   }

   char item[256];
   sprintf(item, "User set up to %g", num);
   PyObject *pstring = PyString_FromString(item);
   if (PyList_Append(self->log, pstring) != 0) return NULL;

   self->fcn->setUp(num);
   return Py_None;
}

static PyObject* minuit_Minuit_setPrintmode(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   int num;
   if (!PyArg_ParseTuple(args, "i", &num)  ||  num < 0  ||  num > 2) {
      PyErr_SetString(PyExc_TypeError, "Printmode must be 0 (nothing), 1 (values), 2 (differences).");
      return NULL;
   }

   self->fcn->setPrintmode(num);
   return Py_None;
}

static PyObject* minuit_Minuit_setMaxcalls(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   int num;
   if (!PyArg_ParseTuple(args, "i", &num)) {
      PyErr_SetString(PyExc_TypeError, "Maxcalls must be an integer.");
      return NULL;
   }
   self->maxcalls = num;

   char item[256];
   sprintf(item, "User set maxcalls to %d.", num);
   PyObject *pstring = PyString_FromString(item);
   if (PyList_Append(self->log, pstring) != 0) return NULL;

   return Py_None;
}

static PyObject* minuit_Minuit_setTol(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   double num;
   if (!PyArg_ParseTuple(args, "d", &num)) {
      PyErr_SetString(PyExc_TypeError, "Tol must be a number.");
      return NULL;
   }
   self->tol = num;

   char item[256];
   sprintf(item, "User set tol to %g.", num);
   PyObject *pstring = PyString_FromString(item);
   if (PyList_Append(self->log, pstring) != 0) return NULL;

   return Py_None;
}

static PyObject* minuit_Minuit_setStrategy(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   int num;
   if (!PyArg_ParseTuple(args, "i", &num)  ||  num < 0  ||  num > 2) {
      PyErr_SetString(PyExc_TypeError, "Strategy must be 0 (fast), 1 (medium), or 2 (accurate).");
      return NULL;
   }
   self->strategy = num;

   char item[256];
   sprintf(item, "User set strategy to %d.", num);
   PyObject *pstring = PyString_FromString(item);
   if (PyList_Append(self->log, pstring) != 0) return NULL;

   return Py_None;
}

//////////////////////////////////////////////////////////////

bool state_okay(minuit_Minuit* self, bool check_mutable = false, std::vector<double>* par = NULL) {
   int floatable = 0;
   for (int i = 0;  i < self->npar;  i++) {
      double value = self->upar->value(i);
      if (par != NULL) {
	 value = (*par)[i];
      }

      if (self->upar->parameter(i).hasLowerLimit()  &&
	  value <= self->upar->parameter(i).lowerLimit()) {
	 char errorstring[256];
	 sprintf(errorstring, "Value of parameter \"%s\" (%g) is below its lower limit (%g)", self->upar->name(i), value, self->upar->parameter(i).lowerLimit());
	 PyErr_Format(PyExc_MinuitError, errorstring);
	 return false;
      }

      if (self->upar->parameter(i).hasUpperLimit()  &&
	  value >= self->upar->parameter(i).upperLimit()) {
	 char errorstring[256];
	 sprintf(errorstring, "Value of parameter \"%s\" (%g) is above its upper limit (%g)", self->upar->name(i), value, self->upar->parameter(i).upperLimit());
	 PyErr_Format(PyExc_MinuitError, errorstring);
	 return false;
      }

      if (!self->upar->parameter(i).isFixed()) floatable++;
   }

   if (check_mutable) {
      if (floatable < 1) {
	 PyErr_SetString(PyExc_MinuitError, "You can't minimize if all parameteres are fixed!");
	 return false;
      }
   }

   return true;
}

static PyObject* minuit_Minuit_call(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
   std::vector<double> par;

   if (PyTuple_Check(args)  &&  PyTuple_Size(args) == 0) {
      for (int i = 0;  i < self->npar;  i++) {
	 par = self->upar->params();
      }
      if (!state_okay(self, false, &par)) return NULL;
   }
   else if (PyTuple_Check(args)  &&  PyTuple_Size(args) == self->npar) {
      for (int i = 0;  i < self->npar;  i++) {
	 PyObject *obj = PyTuple_GetItem(args, i);
	 if (obj == NULL) return NULL;
	 if (!PyNumber_Check(obj)) {
	    PyErr_SetString(PyExc_TypeError, "Function arguments must all be numbers.");
	    return NULL;
	 }
	 par.push_back(PyFloat_AsDouble(obj));
      }
      if (!state_okay(self, false, &par)) return NULL;
   }
   else {
      char errorstring[256];
      sprintf(errorstring, "The function takes exactly %d arguments.", self->npar);
      PyErr_SetString(PyExc_TypeError, errorstring);
      return NULL;
   }

   int tmpprintmode = self->fcn->printmode();
   self->fcn->setPrintmode(0);
   double result = (*self->fcn)(par);
   self->fcn->setPrintmode(tmpprintmode);

   self->ncalls++;
   
   return Py_BuildValue("d", result);
}

static PyObject* minuit_Minuit_migrad(minuit_Minuit* self) {
   if (!state_okay(self, true, NULL)) return NULL;

   self->migrad_valid = false;
   MnMigrad migrad(*self->fcn, *self->upar, self->strategy);

   if (self->fcn->printmode() > 0) {
      self->fcn->setOriginal(self->upar->params());
      switch (self->fcn->printmode()) {
	 case 1:
	    printf("      Result | Parameter values\n");
	    break;
	 case 2:
	    printf("      Result | Differences in parameter values from initial\n");
	    break;
      }
   }

   if (self->min != NULL) {
      delete self->min;
      self->min = NULL;
   }
   if (self->ucov != NULL) {
      self->ucov = NULL;
   }
   try {
      self->min = new FunctionMinimum(migrad(self->maxcalls, self->tol));
   }
   catch (ExceptionDuringMinimization theException) {
      self->min = NULL;
      return NULL;
   }

   self->ncalls += self->min->nfcn();
   for (int i = 0;  i < self->npar;  i++) {
      self->upar->setValue(i, self->min->userParameters().value(i));
      self->upar->setError(i, self->min->userParameters().error(i));
   }

   char state[256];
   if (self->min->isValid()) {
      sprintf(state, "MIGRAD is successful.");
      self->migrad_valid = true;
   }
   else if (self->min->hasReachedCallLimit()) {
      sprintf(state, "MIGRAD has reached its call limit of %d", self->maxcalls);
   }
   else if (self->min->isAboveMaxEdm()) {
      sprintf(state, "MIGRAD reached an EDM of %g, which is above the requested maximum of 0.001*tol*up = 0.001 * %g * %g = %g.", self->min->edm(), self->tol, self->fcn->up(), 0.001 * self->tol * self->fcn->up());
   }
   else if (!self->min->hasPosDefCovar()) {
      sprintf(state, "MIGRAD reached a point of non-positive curvature (covariance matrix is not positive definite).");
   }
   else {
      sprintf(state, "MIGRAD failed.");
   }

   if (self->min->hasValidCovariance()) {
      self->ucov = &self->min->userCovariance();
   }

   PyObject *pstring = PyString_FromString(state);
   if (PyList_Append(self->log, pstring) != 0) return NULL;
   return pstring;
}

// static PyObject* minuit_Minuit_hesse(minuit_Minuit* self) {
//    if (!state_okay(self, false, NULL)) return NULL;
//    self->migrad_valid = false;

// }

// static PyObject* minuit_Minuit_minos(minuit_Minuit* self, PyObject *args, PyObject *kwds) {
//    if (!state_okay(self, false, NULL)) return NULL;
// }

static PyObject* theiter(PyObject* self) {
   return self;
}

static PyObject* thenext(PyObject* self) {
   return Py_BuildValue("d", 5.);
}

static PyObject* thelen(minuit_Minuit* self) {
   return Py_BuildValue("i", 10);
}

static PyMethodDef minuit_Minuit_methods[] = {
   {"value", (PyCFunction)(minuit_Minuit_value), METH_VARARGS, "Return the value of a given parameter (pass parameter name)."},
   {"error", (PyCFunction)(minuit_Minuit_error), METH_VARARGS, "Return the uncertainty in a given parameter (pass parameter name)."},
   {"eps", (PyCFunction)(minuit_Minuit_eps), METH_NOARGS, "Return the assumed machine precision."},
   {"npar", (PyCFunction)(minuit_Minuit_npar), METH_NOARGS, "Return the number of (free + fixed) parameters in the function."},
   {"maxcalls", (PyCFunction)(minuit_Minuit_maxcalls), METH_NOARGS, "Return the maximum number of function calls a MINUIT process is allowed to make (0 = unlimited)."},
   {"tol", (PyCFunction)(minuit_Minuit_tol), METH_NOARGS, "Return the chosen tolerance (minimization will end when EDM is 0.001*tol*up)."},
   {"strategy", (PyCFunction)(minuit_Minuit_strategy), METH_NOARGS, "Return the chosen MINUIT strategy (0 = fast, 1 = medium, 2 = accurate)."},
   {"fcn", (PyCFunction)(minuit_Minuit_fcn), METH_NOARGS, "Return a reference to the function."},
   {"up", (PyCFunction)(minuit_Minuit_up), METH_NOARGS, "Return the chosen error definition (1. for chi**2, 0.5 for -loglike)."},
   {"ncalls", (PyCFunction)(minuit_Minuit_ncalls), METH_NOARGS, "Return the number of times the function has been called."},
   {"printmode", (PyCFunction)(minuit_Minuit_printmode), METH_NOARGS, "Return the chosen printmode (0 = nothing, 1 = values, 2 = differences)."},
   {"log", (PyCFunction)(minuit_Minuit_log), METH_NOARGS, "Return a list of MINUIT output messages."},
   {"fval", (PyCFunction)(minuit_Minuit_fval), METH_NOARGS, "Return the function minimum value."},
   {"edm", (PyCFunction)(minuit_Minuit_edm), METH_NOARGS, "Return the estimated distance to the minimum (EDM)."},
   {"covariance", (PyCFunction)(minuit_Minuit_covariance), METH_NOARGS, "Return the covariance matrix."},
   {"covariance_floating", (PyCFunction)(minuit_Minuit_covariance_floating), METH_NOARGS, "Return the covariance matrix of floating parameters."},
   {"globalcc", (PyCFunction)(minuit_Minuit_globalcc), METH_NOARGS, "Return the global correlation coefficients matrix."},
   {"globalcc_floating", (PyCFunction)(minuit_Minuit_globalcc_floating), METH_NOARGS, "Return the global correlation coefficients matrix of floating parameters."},
   ///////////////////////////////////////////////////////////////
   {"setValue", (PyCFunction)(minuit_Minuit_setValue), METH_VARARGS, "Set the value of a given parameter (pass parameter name and its new value)."},
   {"setError", (PyCFunction)(minuit_Minuit_setError), METH_VARARGS, "Set the uncertainty (step size) of a given parameter (pass parameter name and its new uncertainty)."},
   {"fix", (PyCFunction)(minuit_Minuit_fix), METH_VARARGS, "Fix the value of a given parameter (pass parameter name) so that it doesn't float in the fit."},
   {"release", (PyCFunction)(minuit_Minuit_release), METH_VARARGS, "Release the value of a given parameter (pass parameter name) so that it will float in the fit."},
   {"setLower", (PyCFunction)(minuit_Minuit_setLower), METH_VARARGS, "Set the lower limit that is allowed for a given parameter (pass parameter name and limit)."},
   {"setUpper", (PyCFunction)(minuit_Minuit_setUpper), METH_VARARGS, "Set the upper limit that is allowed for a given parameter (pass parameter name and limit)."},
   {"unlimit", (PyCFunction)(minuit_Minuit_unlimit), METH_VARARGS, "Remove limits on a parameter (pass parameter name)."},
   {"setUp", (PyCFunction)(minuit_Minuit_setUp), METH_VARARGS, "Set the chosen error definition (1. for chi**2, 0.5 for -loglike)."},
   {"setPrintmode", (PyCFunction)(minuit_Minuit_setPrintmode), METH_VARARGS, "Set the printmode (0 = nothing, 1 = values, 2 = differences)."},
   {"setMaxcalls", (PyCFunction)(minuit_Minuit_setMaxcalls), METH_VARARGS, "Set the maximum number of function calls a MINUIT process is allowed to make (0 = unlimited)."},
   {"setTol", (PyCFunction)(minuit_Minuit_setTol), METH_VARARGS, "Set the tolerance (minimization will end when EDM is 0.001*tol*up)."},
   {"setStrategy", (PyCFunction)(minuit_Minuit_setStrategy), METH_VARARGS, "Set the MINUIT strategy (0 = fast, 1 = medium, 2 = accurate)."},
   ///////////////////////////////////////////////////////////////
   {"call", (PyCFunction)(minuit_Minuit_call), METH_VARARGS, "Call the function with the current values (no arguments) or with a supplied argument list."},
   {"migrad", (PyCFunction)(minuit_Minuit_migrad), METH_NOARGS, "Minimize the function with MIGRAD."},
   {"__len__", (PyCFunction)(thelen), METH_NOARGS, "whatever."},
   {NULL}
};

static PyTypeObject minuit_MinuitType = {
   PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_minuit.Minuit",          /*tp_name*/
    sizeof(minuit_Minuit),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)minuit_Minuit_dealloc,    /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Minuit minimizer",        /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    theiter,		               /* tp_iter */
    thenext,		               /* tp_iternext */
    minuit_Minuit_methods,     /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)minuit_Minuit_init,    /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};


//////////////////////////////////////////////////////////////////////

double MyFCN::operator()(const std::vector<double>& par) const {
   PyObject* arglist = PyTuple_New(m_npar);
   if (arglist == NULL) throw ExceptionDuringMinimization();
   for (int i = 0;  i < m_npar;  i++) {
      PyObject *obj = PyFloat_FromDouble(par[i]);
      if (obj == NULL  ||  PyTuple_SetItem(arglist, i, obj) != 0) {
	 Py_DECREF(arglist);
	 throw ExceptionDuringMinimization();
      }
   }

   PyObject* result = PyEval_CallObject(m_pfcn, arglist);
   Py_DECREF(arglist);
   if (result == NULL) throw ExceptionDuringMinimization();
   
   double res;
   if (!PyArg_Parse(result, "d", &res)) {
      PyErr_SetString(PyExc_TypeError, "The function must return a single number.");
      Py_DECREF(result);
      throw ExceptionDuringMinimization();
   }

   if (m_printmode > 0) {
      switch (m_printmode) {
	 case 1:
	    printf("%12g |", res);
	    for (int i = 0;  i < m_npar;  i++) printf(" %12g", par[i]);
	    printf("\n");
	    break;
	 case 2:
	    printf("%12g |", res);
	    for (int i = 0;  i < m_npar;  i++) printf(" %12g", par[i]-m_original[i]);
	    printf("\n");
	    break;
      }
   }

   Py_DECREF(result);
   return res;
}

//////////////////////////////////////////////////////////////////////

static PyMethodDef module_methods[] = {
   {NULL}
};

PyMODINIT_FUNC init_minuit(void) {
   PyObject *m;

   minuit_MinuitType.tp_new = PyType_GenericNew;
   if (PyType_Ready(&minuit_MinuitType) < 0) return;

   m = Py_InitModule3("_minuit", module_methods, "Minuit minimizer interface.");
   Py_INCREF(&minuit_MinuitType);
   PyModule_AddObject(m, "Minuit", (PyObject*)(&minuit_MinuitType));

   PyExc_MinuitError = PyErr_NewException("_minuit.MinuitError", NULL, NULL);
   if (PyExc_MinuitError == NULL) return;
   Py_INCREF(PyExc_MinuitError);
   PyModule_AddObject(m, "MinuitError", PyExc_MinuitError);
}
