// g++ rootntuple.cpp -I /nfs/cleo3/Offline/rel/20050721_P2/other_sources/python/include/python2.4/ `/nfs/cleo3/Offline/rel/current/other_sources/Root/bin/root-config --libs --cflags` -lHistPainter -lGX11 -lGui -lGX11TTF -lASImage -lThread -shared -o _rootntuple.so

#include "rootntuple.h"
#include <stdio.h>

///////////////////////////////////////////////////////////////////

static TThread* myThread;
static set<TCanvas*> myCanvas;

void* OneSecondLoop(void* UserArgs) {
   while (true) {
      for (set<TCanvas*>::const_iterator iter = myCanvas.begin();
	   iter != myCanvas.end();
	   ++iter) {
	 (*iter)->Update();
      }
      myThread->Sleep(1);
   }
}

///////////////////////////////////////////////////////////////////

static PyMethodDef rootntuple_RootDir_methods[] = {
   {"ls", ((PyCFunction)(rootntuple_RootDir_ls)), METH_NOARGS, "Print a directory listing in the standard ROOT form."},
   {"list", ((PyCFunction)(rootntuple_RootDir_list)), METH_NOARGS, "Return a map of the currently accessible objects with their ROOT class names."},
   {"get", ((PyCFunction)(rootntuple_RootDir_get)), METH_VARARGS, "Get an object (TDirectory or TNtuple only)."},
   {"path", ((PyCFunction)(rootntuple_RootDir_path)), METH_NOARGS, "Return the path specification to this directory."},
//   {"getpath", ((PyCFunction)(rootntuple_RootDir_getpath)), METH_VARARGS, "Return the directory specified as a string argument (e.g. \"file:/dir1/dir2\" or relative path with \"..\")."},
   {"closefile", ((PyCFunction)(rootntuple_RootDir_closefile)), METH_NOARGS, "Close the file (note that this will break all RootDir objects connected to the same file)."},
   {NULL}
};

static PyTypeObject rootntuple_RootDirType = {
   PyObject_HEAD_INIT(NULL)
   0,                         /*ob_size*/
   "_rootntuple.RootDir",        /*tp_name*/
   sizeof(rootntuple_RootDir),  /*tp_basicsize*/
   0,                         /*tp_itemsize*/
   0,                         /*tp_dealloc*/
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
   "ROOTimentary file access", /* tp_doc */
   0,		              /* tp_traverse */
   0,		              /* tp_clear */
   0,		              /* tp_richcompare */
   0,		              /* tp_weaklistoffset */
   0,                         /* tp_iter */
   0,                         /* tp_iternext */
   rootntuple_RootDir_methods,  /* tp_methods */
   0,                         /* tp_members */
   0,                         /* tp_getset */
   0,                         /* tp_base */
   0,                         /* tp_dict */
   0,                         /* tp_descr_get */
   0,                         /* tp_descr_set */
   0,                         /* tp_dictoffset */
   (initproc)rootntuple_RootDir_init, /* tp_init */
   0,                         /* tp_alloc */
   0,                         /* tp_new */
};

static PyTypeObject rootntuple_RootNtupleEntryType = {
   PyObject_HEAD_INIT(NULL)
   0,                         /*ob_size*/
   "_rootntuple.RootNtupleEntry",        /*tp_name*/
   sizeof(rootntuple_RootNtupleEntry),  /*tp_basicsize*/
   0,                         /*tp_itemsize*/
   0,                         /*tp_dealloc*/
   0,                         /*tp_print*/
   rootntuple_RootNtupleEntry_getattr,  /*tp_getattr*/
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
   "ROOT Ntuple entry (== row or event)",       /* tp_doc */
   0,		              /* tp_traverse */
   0,		              /* tp_clear */
   0,		              /* tp_richcompare */
   0,		              /* tp_weaklistoffset */
   0,                         /* tp_iter */
   0,                         /* tp_iternext */
   0,                         /* tp_methods */
   0,                         /* tp_members */
   0,                         /* tp_getset */
   0,                         /* tp_base */
   0,                         /* tp_dict */
   0,                         /* tp_descr_get */
   0,                         /* tp_descr_set */
   0,                         /* tp_dictoffset */
   (initproc)rootntuple_RootNtupleEntry_init, /* tp_init */
   0,                         /* tp_alloc */
   0,                         /* tp_new */
};

static PyMethodDef rootntuple_RootNtuple_methods[] = {
   {"range", (PyCFunction)(rootntuple_RootNtuple_range), METH_VARARGS, "Set the ntuple range with these arguments: [start,] stop[, step] or nothing to return to the defaults (0, #entries, 1)."},
   {"list", (PyCFunction)(rootntuple_RootNtuple_labels), METH_NOARGS, "Return a list of column labels."},
   {"allentries", (PyCFunction)(rootntuple_RootNtuple_allentries), METH_NOARGS, "Return the number of entries (== rows, events) in the ntuple (regardless of start/stop/step)."},
   {"entries", (PyCFunction)(rootntuple_RootNtuple_entries), METH_NOARGS, "Return the number of entries (== rows, events) in the ntuple (subject to start/stop/step)."},
   {"count", (PyCFunction)(rootntuple_RootNtuple_count), METH_NOARGS, "Count the number of entries (== rows, events) that pass the filter function (subject to start/stop/step)."},
   {"bounds", (PyCFunction)(rootntuple_RootNtuple_bounds), METH_VARARGS, "Return the minimum and maximum values (subject to filter, start/stop/step) of a list of variables."},
//   {"loadbaskets", (PyCFunction)(rootntuple_RootNtuple_loadbaskets), METH_VARARGS, "Load ntuple values into memory up to a given limit (MB) for faster access.  If followed by a list of ntuple variable names, only load those variables."},
   {"entry", (PyCFunction)(rootntuple_RootNtuple_entry), METH_VARARGS, "Get an ntuple entry by number."},
   {"map", (PyCFunction)(rootntuple_RootNtuple_map), METH_VARARGS | METH_KEYWORDS, "Map the ntuple onto a function, returning a list of output values if output=True."},
   {"plot", (PyCFunction)(rootntuple_RootNtuple_plot), METH_VARARGS | METH_KEYWORDS, "Quickly plot a variable or function with optional number of samples before binning, leftloss, rightloss, a ROOT Draw Option and a plotting frequency (can be None)."},
   {"plotbin", (PyCFunction)(rootntuple_RootNtuple_plotbin), METH_VARARGS | METH_KEYWORDS, "Quickly plot a variable or function with a specified number of bins, low edge, high edge, and possibly ROOT Draw Option and a plotting frequency (can be None)."},
   {"plotint", (PyCFunction)(rootntuple_RootNtuple_plotint), METH_VARARGS | METH_KEYWORDS, "Quickly plot a variable or function with a specified low edge and high edge as inclusive integers with one bin per integral value, and possibly ROOT Draw Option and a plotting frequency (can be None)."}, 
   {"profile", (PyCFunction)(rootntuple_RootNtuple_profile), METH_VARARGS | METH_KEYWORDS, "Quickly profile a pair of variables or function with optional number of samples before binning, leftloss, rightloss, a ROOT Draw Option and a plotting frequency (can be None)."},
   {"profilebin", (PyCFunction)(rootntuple_RootNtuple_profilebin), METH_VARARGS | METH_KEYWORDS, "Quickly profile a pair of variables or function with a specified number of bins, low edge, high edge, and possibly ROOT Draw Option and a plotting frequency (can be None)."},
   {"profileint", (PyCFunction)(rootntuple_RootNtuple_profileint), METH_VARARGS | METH_KEYWORDS, "Quickly profile a pair of variables or function with a specified low edge and high edge as inclusive integers with one bin per integral value, and possibly ROOT Draw Option and a plotting frequency (can be None)."}, 
   {"plot2", (PyCFunction)(rootntuple_RootNtuple_plot2), METH_VARARGS | METH_KEYWORDS, "Quickly plot a pair of variables or function with optional number of samples before binning, leftloss, toploss, rightloss, and bottomloss (LiTteRBox)."},
   {"plot2bin", (PyCFunction)(rootntuple_RootNtuple_plot2bin), METH_VARARGS | METH_KEYWORDS, "Quickly plot a pair of variables or function with a specified X number of bins, low edge, high edge, Y number of bins, low edge, high edge, and possibly ROOT Draw Option and a plotting frequency (can be None)."},
   {"redraw", (PyCFunction)(rootntuple_RootNtuple_redraw), METH_VARARGS, "Redraw the current histogram."},
   {"postscript", (PyCFunction)(rootntuple_RootNtuple_postscript), METH_VARARGS, "Print postscript to a file (eps or ps, depending on file extension)."},
   {"closewindow", (PyCFunction)(rootntuple_RootNtuple_close), METH_NOARGS, "Close the current histogram's window.  (Actually, you must plot something else before it really closes.)"},
   {"hist", (PyCFunction)(rootntuple_RootNtuple_hist), METH_VARARGS, "Return the current histogram as an object with a new name (optionally also set the title)."},
   {NULL}
};

static PyMemberDef rootntuple_RootNtuple_members[] = {
   {"start", T_LONG, offsetof(rootntuple_RootNtuple, start), 0, "Starting index (default is 0)."},
   {"stop", T_LONG, offsetof(rootntuple_RootNtuple, stop), 0, "One beyond the last index in an iteration."},
   {"step", T_LONG, offsetof(rootntuple_RootNtuple, step), 0, "Step size (default is 1)."},
   {"i", T_LONG, offsetof(rootntuple_RootNtuple, i), 0, "Current index position."},
   {"optimize", T_OBJECT, offsetof(rootntuple_RootNtuple, optimize), 0, "List of ntuple variables to extract (others will not be extracted!)"},
   {"filter", T_OBJECT, offsetof(rootntuple_RootNtuple, filter), 0, "Filter function to apply to each entry, passing only those that return True."},
   {"canvas_size", T_OBJECT, offsetof(rootntuple_RootNtuple, canvas_size), 0, "Tuple of width, height for the canvas used in plotting."},
   {"leftloss", T_DOUBLE, offsetof(rootntuple_RootNtuple, leftloss), 0, "Default fraction of events you are willing to lose on the left edge of auto-binned plots."},
   {"rightloss", T_DOUBLE, offsetof(rootntuple_RootNtuple, rightloss), 0, "Default fraction of events you are willing to lose on the right edge of auto-binned plots."},
   {"sample", T_INT, offsetof(rootntuple_RootNtuple, sample), 0, "Default number of good events (passing filter) used to determine binning in auto-binned plots."},
   {"padding", T_DOUBLE, offsetof(rootntuple_RootNtuple, padding), 0, "Padding fraction on the edges of auto-binned plots."},
   {"frequency", T_OBJECT, offsetof(rootntuple_RootNtuple, frequency), 0, "If None, plots will always appear after they are entirely filled; if a positive integer, plots will be redisplayed after this many good events (passing filter)."},
   {"log", T_OBJECT, offsetof(rootntuple_RootNtuple, log), 0, "If True, the y axis will be plotted in log scale for 1D and profile histograms, and the z axis will be plotted in log scale for 2D histograms."},
   {"display", T_OBJECT, offsetof(rootntuple_RootNtuple, display), 0, "If True, plots will be displayed (with ROOT graphics).  Otherwise, we just fill temporary histograms."},
   {NULL}
};

static PyTypeObject rootntuple_RootNtupleType = {
   PyObject_HEAD_INIT(NULL)
   0,                         /*ob_size*/
   "_rootntuple.RootNtuple",       /*tp_name*/
   sizeof(rootntuple_RootNtuple), /*tp_basicsize*/
   0,                         /*tp_itemsize*/
   (destructor)rootntuple_RootNtuple_dealloc, /*tp_dealloc*/
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
   "ROOT Ntuple object",      /* tp_doc */
   0,		               /* tp_traverse */
   0,		               /* tp_clear */
   0,		               /* tp_richcompare */
   0,		               /* tp_weaklistoffset */
   rootntuple_RootNtuple_iter,     /* tp_iter */
   rootntuple_RootNtuple_next,     /* tp_iternext */
   rootntuple_RootNtuple_methods,  /* tp_methods */
   rootntuple_RootNtuple_members,  /* tp_members */
   0,                         /* tp_getset */
   0,                         /* tp_base */
   0,                         /* tp_dict */
   0,                         /* tp_descr_get */
   0,                         /* tp_descr_set */
   0,                         /* tp_dictoffset */
   (initproc)rootntuple_RootNtuple_init,    /* tp_init */
   0,                         /* tp_alloc */
   0,                         /* tp_new */
};

static PyMethodDef rootntuple_RootHist_methods[] = {
   {"mean", (PyCFunction)(rootntuple_RootHist_mean), METH_NOARGS, "Return the mean and uncertainty in the mean."},
   {"rms", (PyCFunction)(rootntuple_RootHist_rms), METH_NOARGS, "Return the RMS (meaning: standard deviation) and its uncertainty."},
   {"entries", (PyCFunction)(rootntuple_RootHist_entries), METH_NOARGS, "Return the number of entries."},
   {"ouvalues", (PyCFunction)(rootntuple_RootHist_ouvalues), METH_NOARGS, "Return the values of the overflow and underlow bins."},
   {"ouerrors", (PyCFunction)(rootntuple_RootHist_ouerrors), METH_NOARGS, "Return the errors of the overflow and underlow bins."},
   {"bins", (PyCFunction)(rootntuple_RootHist_bins), METH_VARARGS, "Return the bin centers, or low edges, or high edges, depending on whether first argument is 0, -1, or 1, respectively."},
   {"values", (PyCFunction)(rootntuple_RootHist_values), METH_NOARGS, "Return the bin contents."},
   {"errors", (PyCFunction)(rootntuple_RootHist_errors), METH_NOARGS, "Return the bin errors."},
   {"redraw", (PyCFunction)(rootntuple_RootHist_redraw), METH_VARARGS, "Redraw this histogram."},
   {"postscript", (PyCFunction)(rootntuple_RootHist_postscript), METH_VARARGS, "Print postscript to a file (eps or ps, depending on file extension)."},
   {"closewindow", (PyCFunction)(rootntuple_RootHist_close), METH_NOARGS, "Close this histogram's window.  (Actually, you must plot something else before it really closes.)"},
   {NULL}
};

static PyMemberDef rootntuple_RootHist_members[] = {
   {"canvas_size", T_OBJECT, offsetof(rootntuple_RootNtuple, canvas_size), 0, "Tuple of width, height for the canvas used in plotting."},
   {"log", T_OBJECT, offsetof(rootntuple_RootHist, log), 0, "If True, the y axis will be plotted in log scale for 1D and profile histograms, and the z axis will be plotted in log scale for 2D histograms."},
   {NULL}
};

static PyTypeObject rootntuple_RootHistType = {
   PyObject_HEAD_INIT(NULL)
   0,                         /*ob_size*/
   "_rootntuple.RootHist",       /*tp_name*/
   sizeof(rootntuple_RootHist), /*tp_basicsize*/
   0,                         /*tp_itemsize*/
   (destructor)rootntuple_RootHist_dealloc, /*tp_dealloc*/
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
   "ROOT Histogram object",      /* tp_doc */
   0,		               /* tp_traverse */
   0,		               /* tp_clear */
   0,		               /* tp_richcompare */
   0,		               /* tp_weaklistoffset */
   0,			       /* tp_iter */
   0,			       /* tp_iternext */
   rootntuple_RootHist_methods,  /* tp_methods */
   rootntuple_RootHist_members,  /* tp_members */
   0,                         /* tp_getset */
   0,                         /* tp_base */
   0,                         /* tp_dict */
   0,                         /* tp_descr_get */
   0,                         /* tp_descr_set */
   0,                         /* tp_dictoffset */
   (initproc)rootntuple_RootHist_init,    /* tp_init */
   0,                         /* tp_alloc */
   0,                         /* tp_new */
};

static PyMethodDef module_methods[] = {
   {NULL}
};

///////////////////////////////////////////////////////////////////

static int rootntuple_RootDir_init(rootntuple_RootDir* self, PyObject* args) {
   char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "Pass the name of a file to read.");
      return -1;
   }

   self->f = TFile::Open(name);
   if (self->f == NULL  ||  self->f->IsOpen() == kFALSE) {
      PyErr_Format(PyExc_IOError, "Can't open file \"%s\".", name);
      return -1;
   }
      
   self->d = (TDirectory*)(self->f);

   return 0;
}

static PyObject* rootntuple_RootDir_ls(rootntuple_RootDir* self) {
   if (self->f->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   self->d->ls();
   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootDir_list(rootntuple_RootDir* self) {
   if (self->f->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   PyObject* output = PyDict_New();
   if (output == NULL) return NULL;

   TIterator* iter = self->d->GetListOfKeys()->MakeIterator();
   while (TKey* key = (TKey*)(iter->Next())) {
      TObject* obj = key->ReadObj();

      PyObject* pname = PyString_FromString(obj->GetName());
      if (pname == NULL) {
	 Py_DECREF(output);
	 return NULL;
      }

      PyObject* pclass = PyString_FromString(obj->ClassName());
      if (pclass == NULL) {
	 Py_DECREF(output);
	 Py_DECREF(pname);
	 return NULL;
      }

      if (PyDict_SetItem(output, pname, pclass) != 0) {
	 Py_DECREF(output);
	 Py_DECREF(pname);
	 Py_DECREF(pclass);
	 return NULL;
      }
   }
   return output;
}

static PyObject* rootntuple_RootDir_get(rootntuple_RootDir* self, PyObject* args) {
   if (self->f->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "Pass the name of the object you want to retrieve.");
      return NULL;
   }

   TIterator* iter = self->d->GetListOfKeys()->MakeIterator();
   while (TKey* key = (TKey*)(iter->Next())) {
      TObject* obj = key->ReadObj();

      if (strcmp(name, obj->GetName()) == 0) {
	 if (strcmp("TDirectory", obj->ClassName()) == 0) {
	    rootntuple_RootDir* pobj = PyObject_New(rootntuple_RootDir, &rootntuple_RootDirType);
	    pobj->d = (TDirectory*)(obj);
	    pobj->f = self->f;
	    return (PyObject*)(pobj);
	 }
	 else if (strcmp("TH1F", obj->ClassName()) == 0) {
	    rootntuple_RootHist* pobj = PyObject_New(rootntuple_RootHist, &rootntuple_RootHistType);
	    pobj->canvas_size = PyTuple_New(2);
	    PyTuple_SetItem(pobj->canvas_size, 0, PyInt_FromLong(500));
	    PyTuple_SetItem(pobj->canvas_size, 1, PyInt_FromLong(500));
	    pobj->canvas = NULL;
	    pobj->which = 0;
	    pobj->hist = (TH1F*)(((TH1F*)(obj))->Clone());
	    pobj->hist->SetStats(kFALSE);
	    pobj->prof = NULL;
	    pobj->hist2 = NULL;
	    pobj->options = NULL;
	    Py_INCREF(Py_False);
	    pobj->log = Py_False;

	    return (PyObject*)(pobj);
	 }
	 else if (strcmp("TProfile", obj->ClassName()) == 0) {
	    rootntuple_RootHist* pobj = PyObject_New(rootntuple_RootHist, &rootntuple_RootHistType);
	    pobj->canvas_size = PyTuple_New(2);
	    PyTuple_SetItem(pobj->canvas_size, 0, PyInt_FromLong(500));
	    PyTuple_SetItem(pobj->canvas_size, 1, PyInt_FromLong(500));
	    pobj->canvas = NULL;
	    pobj->which = 1;
	    pobj->hist = NULL;
	    pobj->prof = (TProfile*)(((TProfile*)(obj))->Clone());;
	    pobj->prof->SetStats(kFALSE);
	    pobj->hist2 = NULL;
	    pobj->options = NULL;
	    Py_INCREF(Py_False);
	    pobj->log = Py_False;

	    return (PyObject*)(pobj);
	 }
	 else if (strcmp("TH2F", obj->ClassName()) == 0) {
	    rootntuple_RootHist* pobj = PyObject_New(rootntuple_RootHist, &rootntuple_RootHistType);
	    pobj->canvas_size = PyTuple_New(2);
	    PyTuple_SetItem(pobj->canvas_size, 0, PyInt_FromLong(500));
	    PyTuple_SetItem(pobj->canvas_size, 1, PyInt_FromLong(500));
	    pobj->canvas = NULL;
	    pobj->which = 2;
	    pobj->hist = NULL;
	    pobj->prof = NULL;
	    pobj->hist2 = (TH2F*)(((TH2F*)(obj))->Clone());;
	    pobj->hist2->SetStats(kFALSE);
	    pobj->options = NULL;
	    Py_INCREF(Py_False);
	    pobj->log = Py_False;

	    return (PyObject*)(pobj);
	 }
	 else if (strcmp("TNtuple", obj->ClassName()) == 0) {
	    rootntuple_RootNtupleEntry* pentry = PyObject_New(rootntuple_RootNtupleEntry, &rootntuple_RootNtupleEntryType);
	    pentry->weight = 1.0;

	    rootntuple_RootNtuple* pobj = PyObject_New(rootntuple_RootNtuple, &rootntuple_RootNtupleType);
	    pobj->file = self->f;
	    pobj->ntuple = (TNtuple*)(obj);
	    pobj->isdouble = false;
	    pobj->i = 0;
	    pobj->start = 0;
	    pobj->stop = ((TNtuple*)(obj))->GetEntries();
	    pobj->step = 1;
	    pobj->entry = (PyObject*)(pentry);
	    Py_INCREF(Py_None);
	    pobj->optimize = Py_None;
	    Py_INCREF(Py_None);
	    pobj->filter = Py_None;
	    pobj->filtervars = NULL;
	    pobj->filtervars_size = 0;
	    pobj->canvas_size = PyTuple_New(2);
	    PyTuple_SetItem(pobj->canvas_size, 0, PyInt_FromLong(500));
	    PyTuple_SetItem(pobj->canvas_size, 1, PyInt_FromLong(500));
	    pobj->canvas = NULL;
	    pobj->which = 0;
	    pobj->hist = NULL;
	    pobj->prof = NULL;
	    pobj->hist2 = NULL;
	    pobj->options = NULL;
	    pobj->graph = NULL;
	    pobj->leftloss = 0.01;
	    pobj->rightloss = 0.01;
	    pobj->sample = 1000;
	    pobj->padding = 0.10;
	    pobj->frequency = PyInt_FromLong(1000);
	    Py_INCREF(Py_False);
	    pobj->log = Py_False;
	    Py_INCREF(Py_True);
	    pobj->display = Py_True;

	    pentry->master = pobj;

	    return (PyObject*)(pobj);
	 }
	 else if (strcmp("TNtupleD", obj->ClassName()) == 0) {
	    PyErr_SetString(PyExc_NotImplementedError, "Yeah, this really ought to read TNtupleD objects.  And it will.  Someday.");
	    return NULL;
	 }
	 else {
	    PyErr_Format(PyExc_NotImplementedError, "This version of rootntuple can't read \"%s\" types.", obj->ClassName());
	    return NULL;
	 }
      }
   }

   PyErr_Format(PyExc_ValueError, "No object named \"%s\" in this directory.", name);
   return NULL;
}

static PyObject* rootntuple_RootDir_path(rootntuple_RootDir* self) {
   if (self->f->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   return Py_BuildValue("s", self->d->GetPath());
}

// This ROOT function (TDirectory::GetDirectory) is broken.
//
// static PyObject* rootntuple_RootDir_getpath(rootntuple_RootDir* self, PyObject* args) {
//    if (self->f->IsOpen() == kFALSE) {
//       PyErr_SetString(PyExc_IOError, "The file has been closed.");
//       return NULL;
//    }

//    char* name;
//    if (!PyArg_ParseTuple(args, "s", &name)) {
//       PyErr_SetString(PyExc_TypeError, "Pass the name of a directory in the current file (e.g. \"file:/dir1/dir2\" or relative path with \"..\").");
//       return NULL;
//    }

//    TDirectory* dir = self->d->GetDirectory((const char*)(name));
//    if (dir == NULL) {
//       PyErr_Format(PyExc_IOError, "Path \"%g\" is not in this file.", name);
//       return NULL;
//    }

//    rootntuple_RootDir* obj = PyObject_New(rootntuple_RootDir, &rootntuple_RootDirType);
//    obj->d = dir;
//    obj->f = self->f;

//    return Py_BuildValue("O", obj);
// }

static PyObject* rootntuple_RootDir_closefile(rootntuple_RootDir* self) {
   if (self->f->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   self->f->Close();
   Py_INCREF(Py_None);
   return Py_None;
}

///////////////////////////////////////////////////////////////////

static int rootntuple_RootNtupleEntry_init(rootntuple_RootNtupleEntry* self, PyObject* args) {
   PyErr_SetString(PyExc_NotImplementedError, "This module does not support Ntuple creation.");
   return -1;
}

static PyObject* rootntuple_RootNtupleEntry_getattr(PyObject* thyself, char* name) {
   rootntuple_RootNtupleEntry* self = (rootntuple_RootNtupleEntry*)(thyself);
   if (self->master == NULL) {
      PyErr_SetString(PyExc_AttributeError, "The RootNtuple this entry belongs to is gone.");
      return NULL;
   }

   if (self->master->file->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   TLeaf* leaf = self->master->ntuple->GetLeaf(name);
   if (leaf != NULL) {
      if (self->master->optimize != Py_None  &&
	  self->master->ntuple->GetBranchStatus(name) == 0) {
	 PyErr_Format(PyExc_AttributeError, "Ntuple entry \"%s\" is not in the optimize list.", name);
	 return NULL;
      }

      return Py_BuildValue("d", leaf->GetValue());
   }
   
   if (strcmp(name, "_weight") == 0) return Py_BuildValue("d", self->weight);

   PyErr_Format(PyExc_AttributeError, "There is no ntuple entry \"%s\".", name);
   return NULL;
}

///////////////////////////////////////////////////////////////////

static int rootntuple_RootNtuple_init(rootntuple_RootNtuple* self, PyObject* args) {
   PyErr_SetString(PyExc_NotImplementedError, "This module does not support Ntuple creation.");
   return -1;
}

static PyObject* rootntuple_RootNtuple_range(rootntuple_RootNtuple* self, PyObject* args) {
   const char* errstring = "Pass these arguments: [start,] stop[, step] or nothing to return to the defaults (0, #entries, 1).";

   self->start = 0;
   self->stop = self->ntuple->GetEntries();
   self->step = 1;

   if (PySequence_Size(args) == 0) { }
   if (PySequence_Size(args) == 1) {
      if (!PyArg_ParseTuple(args, "l", &self->stop)) {
	 PyErr_SetString(PyExc_TypeError, errstring);
	 return NULL;
      }
   }
   else if (PySequence_Size(args) == 2) {
      if (!PyArg_ParseTuple(args, "ll", &self->start, &self->stop)) {
	 PyErr_SetString(PyExc_TypeError, errstring);
	 return NULL;
      }
   }
   else if (PySequence_Size(args) == 3) {
      if (!PyArg_ParseTuple(args, "lll", &self->start, &self->stop, &self->step)) {
	 PyErr_SetString(PyExc_TypeError, errstring);
	 return NULL;
      }
   }
   else {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }

   self->i = 0;

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_labels(rootntuple_RootNtuple* self) {
   if (self->file->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   PyObject* output = PyList_New(self->ntuple->GetNvar());
   if (output == NULL) return NULL;

   int i = 0;
   TIterator* iter = self->ntuple->GetListOfLeaves()->MakeIterator();
   while(TLeafF* leaf = (TLeafF*)(iter->Next())) {
      PyObject* str = PyString_FromString(leaf->GetName());

      if (str == NULL  ||  PyList_SetItem(output, i, str) != 0) {
	 Py_DECREF(output);
	 return NULL;
      }

      i++;
   }
   return output;
}

static PyObject* rootntuple_RootNtuple_allentries(rootntuple_RootNtuple* self) {
   return Py_BuildValue("l", self->ntuple->GetEntries());
}

static PyObject* rootntuple_RootNtuple_entries(rootntuple_RootNtuple* self) {
   return Py_BuildValue("l", (self->stop - self->start) / self->step);
}

static PyObject* rootntuple_RootNtuple_count(rootntuple_RootNtuple* self) {
   if (self->filter == Py_None) return Py_BuildValue("l", self->ntuple->GetEntries());

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   bool warned = false;
   long count = 0;
   while (true) {
      double dummy = 1.0;
      if (!rootntuple_RootNtuple_zipahead(self, &dummy, &warned)) return NULL;

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      self->i += self->step;
      count++;
   }

   return Py_BuildValue("l", count);
}

static PyObject* rootntuple_RootNtuple_bounds(rootntuple_RootNtuple* self, PyObject* args) {
   const char* errstring = "Pass a list of ntuple variables as strings.";

   PyObject* varnames;
   if (!PyArg_ParseTuple(args, "O", &varnames)  ||  !PySequence_Check(varnames)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Size(varnames) == 0) {
      return PyDict_New();
   }

   PyObject* labels = rootntuple_RootNtuple_labels(self);
   if (labels == NULL) return false;
      
   int size = PySequence_Size(varnames);
   for (int i = 0;  i < size;  i++) {
      PyObject* varname = PySequence_GetItem(varnames, i);
      if (!PyString_Check(varname)) {
	 PyErr_SetString(PyExc_TypeError, errstring);
	 Py_DECREF(labels);
	 Py_DECREF(varname);
	 return NULL;
      }
      if (!PySequence_Contains(labels, varname)) {
	 PyErr_Format(PyExc_ValueError, "Variable \"%s\" in list is not the name of an ntuple variable.", PyString_AsString(varname));
	 Py_DECREF(labels);
	 Py_DECREF(varname);
	 return false;
      }
      Py_DECREF(varname);
   }

   Py_DECREF(labels);

   char** vars = new (char*)[size];
   double* lower = new double[size];
   double* upper = new double[size];
   if (vars == NULL) {
      PyErr_Format(PyExc_MemoryError, "Couldn't allocate internal array vars[] (only %d items!)", size);
      return NULL;
   }

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   for (int i = 0;  i < size;  i++) {
      PyObject* varname = PySequence_GetItem(varnames, i);
      vars[i] = PyString_AsString(varname);
      Py_DECREF(varname);
      self->ntuple->SetBranchStatus(vars[i], 1);
   }

   bool warned = false;
   bool bound_set = false;
   while (true) {
      double dummy = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &dummy, &warned)) return NULL;
      }
      else {
	 self->ntuple->GetEntry(self->i, 0);
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      for (int i = 0;  i < size;  i++) {
	 double value = self->ntuple->GetLeaf(vars[i])->GetValue();
	 if (!bound_set  ||  value < lower[i]) lower[i] = value;
	 if (!bound_set  ||  value > upper[i]) upper[i] = value;
      }
      bound_set = true;

      self->i += self->step;
   }

   PyObject* output = PyDict_New();
   for (int i = 0;  i < size;  i++) {
      PyObject* val = Py_BuildValue("(dd)", lower[i], upper[i]);
      if (val == NULL) {
	 Py_DECREF(output);
	 return NULL;
      }
      if (PyDict_SetItemString(output, vars[i], val) != 0) {
	 Py_DECREF(output);
	 Py_DECREF(val);
	 return NULL;
      }
   }

   delete [] vars;
   delete [] upper;
   delete [] lower;

   return output;
}

// This is broken in ROOT (and not strictly necessary for us)
// 
// static PyObject* rootntuple_RootNtuple_loadbaskets(rootntuple_RootNtuple* self, PyObject* args) {
//    const char* errstring = "Pass a memory limit in megabytes (default=2000) and a list of ntuple variables as strings (default=None, meaning all).";

//    double megabytes = 2000.0;
//    PyObject* varnames = Py_None;
//    if (!PyArg_ParseTuple(args, "|dO", &megabytes, &varnames)) {
//       PyErr_SetString(PyExc_TypeError, errstring);
//       return NULL;
//    }
//    if (varnames != Py_None) {
//       if (!PySequence_Check(varnames)) {
// 	 PyErr_SetString(PyExc_TypeError, errstring);
// 	 return NULL;
//       }

//       PyObject* labels = rootntuple_RootNtuple_labels(self);
//       if (labels == NULL) return false;
      
//       int size = PySequence_Size(varnames);
//       for (int i = 0;  i < size;  i++) {
// 	 PyObject* varname = PySequence_GetItem(varnames, i);
// 	 if (!PyString_Check(varname)) {
// 	    PyErr_SetString(PyExc_TypeError, errstring);
// 	    Py_DECREF(labels);
// 	    Py_DECREF(varname);
// 	    return NULL;
// 	 }
// 	 if (!PySequence_Contains(labels, varname)) {
// 	    PyErr_Format(PyExc_ValueError, "Variable \"%s\" in list is not the name of an ntuple variable.", PyString_AsString(varname));
// 	    Py_DECREF(labels);
// 	    Py_DECREF(varname);
// 	    return false;
// 	 }
// 	 Py_DECREF(varname);
//       }

//       Py_DECREF(labels);
//    }
//    long bytes = long(floor(megabytes * 1e6));

//    long baskets = 0;
//    if (varnames == Py_None) {
//       baskets = self->ntuple->LoadBaskets(bytes);
//    }
//    else {
//       self->ntuple->SetMaxVirtualSize(bytes);
//       int size = PySequence_Size(varnames);
//       for (int i = 0;  i < size;  i++) {
// 	 PyObject* varname = PySequence_GetItem(varnames, i);
// 	 baskets += self->ntuple->GetBranch(PyString_AsString(varnames))->LoadBaskets();
// 	 Py_DECREF(varname);
//       }
//    }

//    return Py_BuildValue("i", baskets);
// }

static PyObject* rootntuple_RootNtuple_iter(PyObject* thyself) {
   rootntuple_RootNtuple* self = (rootntuple_RootNtuple*)(thyself);

   if (self->file->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   if (self->step == 0) {
      PyErr_SetString(PyExc_ValueError, "The step size may not be zero.");
      return NULL;
   }

   self->ntuple->SetBranchStatus("*", 0);

   if (self->filter != Py_None) {
      if (!PyCallable_Check(self->filter)) {
	 PyErr_SetString(PyExc_TypeError, "Filter must be a callable (function) returning only True, False, or a weight (True==1).");
	 return NULL;
      }

      PyObject* varnames;
      int size;
      if (PyObject_HasAttrString(self->filter, "varnames")) {
	 varnames = PyObject_GetAttrString(self->filter, "varnames");
	 size = PyTuple_Size(varnames);
      }
      else {
	 varnames = PyObject_GetAttrString(PyObject_GetAttrString(self->filter, "func_code"), "co_varnames");
	 size = PyInt_AsLong(PyObject_GetAttrString(PyObject_GetAttrString(self->filter, "func_code"), "co_argcount"));
      }
      if (varnames == NULL  ||  !PyTuple_Check(varnames)) {
	 PyErr_SetString(PyExc_TypeError, "Why isn't filter.func_code.co_varnames a tuple?  Is it a bug?");
	 return NULL;
      }

      PyObject* labels = rootntuple_RootNtuple_labels(self);
      if (labels == NULL) return NULL;
      
      for (int i = 0;  i < size;  i++) {
	 PyObject* varname = PyTuple_GetItem(varnames, i);
	 if (!PySequence_Contains(labels, varname)) {
	    PyErr_Format(PyExc_ValueError, "Argument \"%s\" in filter function is not an ntuple variable.", PyString_AsString(varname));
	    Py_DECREF(labels);
	    return NULL;
	 }
      }

      Py_DECREF(labels);

      if (self->filtervars != NULL) delete [] self->filtervars;
      self->filtervars_size = size;
      self->filtervars = new (char*)[self->filtervars_size];
      if (self->filtervars == NULL) {
	 PyErr_Format(PyExc_MemoryError, "Couldn't allocate internal array self->filtervars[] (only %d items!)", self->filtervars_size);
	 return NULL;
      }

      for (int i = 0;  i < size;  i++) {
	 PyObject* varname = PyTuple_GetItem(varnames, i);
	 self->filtervars[i] = PyString_AsString(varname);
	 self->ntuple->SetBranchStatus(self->filtervars[i], 1);
      }
   }

   if (self->optimize != Py_None) {
      if (!PySequence_Check(self->optimize)) {
	 PyErr_SetString(PyExc_TypeError, "Attribute \"optimize\" must be a sequence or None.");
	 return NULL;
      }

      PyObject* labels = rootntuple_RootNtuple_labels(self);
      if (labels == NULL) return NULL;

      for (int i = 0;  i < PySequence_Size(self->optimize);  i++) {
	 PyObject* obj = PySequence_GetItem(self->optimize, i);
	 if (obj == NULL) {
	    Py_DECREF(labels);
	    return NULL;
	 }
	 if (!PyString_Check(obj)) {
	    PyErr_SetString(PyExc_TypeError, "Attribute \"optimize\" must be a sequence of strings or None.");
	    Py_DECREF(labels);
	    Py_DECREF(obj);
	    return NULL;
	 }
	 char* str = PyString_AsString(obj);

	 if (!PySequence_Contains(labels, obj)) {
	    PyErr_Format(PyExc_ValueError, "Item \"%s\" in \"optimize\" is not an ntuple variable.", str);
	    Py_DECREF(labels);
	    Py_DECREF(obj);
	    return NULL;
	 }

	 self->ntuple->SetBranchStatus(str, 1);
	 Py_DECREF(obj);
      }

      Py_DECREF(labels);
   }

   self->i = self->start;
   return Py_BuildValue("O", self);
}

static bool rootntuple_RootNtuple_zipahead(rootntuple_RootNtuple* self, double* weight, bool* warned) {
   bool keepgoing = true;

   while (keepgoing) {
      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) return true;

      self->ntuple->GetEntry(self->i, 0);

      PyObject* args = PyTuple_New(self->filtervars_size);
      if (args == NULL) return false;
      for (int i = 0;  i < self->filtervars_size;  i++) {
	 PyObject* obj = PyFloat_FromDouble(self->ntuple->GetLeaf(self->filtervars[i])->GetValue());
	 if (obj == NULL  ||  PyTuple_SetItem(args, i, obj) != 0) {
	    Py_DECREF(args);
	    return false;
	 }
      }

      PyObject* result = PyEval_CallObject(self->filter, args);
      Py_DECREF(args);
      if (result == NULL) return false;
      else if (result == Py_False) {
	 self->i += self->step;
      }
      else if (result == Py_True) {
	 *weight = 1.0;
	 keepgoing = false;
      }
      else if (PyNumber_Check(result)) {
	 double tmp = PyFloat_AsDouble(result);
	 if (tmp != 0.0) {
	    *weight = tmp;
	    keepgoing = false;
	 }
	 else {
	    self->i += self->step;
	 }

	 if (warned != NULL  &&  !(*warned)) {
	    printf("Warning: this function doesn't explicitly use non-Boolean weights.  Zero will be identified as False, anything else as True.\n");
	    *warned = true;
	 }
      }
      else {
	 PyErr_SetString(PyExc_TypeError, "The filter function must return True, False, or a weight for every event.");
	 return false;
      }
   }

   return true;
}

static PyObject* rootntuple_RootNtuple_next(PyObject* thyself) {
   rootntuple_RootNtuple* self = (rootntuple_RootNtuple*)(thyself);

   if (self->file->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   if (self->step == 0) {
      PyErr_SetString(PyExc_ValueError, "The step size may not be zero.");
      return NULL;
   }

   rootntuple_RootNtupleEntry* thyentry = (rootntuple_RootNtupleEntry*)(self->entry);
   thyentry->weight = 1.0;
   if (self->filter != Py_None) {
      if (!rootntuple_RootNtuple_zipahead(self, &thyentry->weight)) return NULL;
   }

   if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) {
      PyErr_SetNone(PyExc_StopIteration);
      return NULL;
   }

   if (self->optimize != Py_None) {
      if (self->filter == Py_None) {
	 self->ntuple->GetEntry(self->i, 0);  // get just the ones you need
      }
   }
   else self->ntuple->GetEntry(self->i, 1);   // get all the variables

   self->i += self->step;
   
   return Py_BuildValue("O", self->entry);
}

static PyObject* rootntuple_RootNtuple_entry(rootntuple_RootNtuple* self, PyObject* args) {
   int i;
   if (!PyArg_ParseTuple(args, "i", &i)) {
      PyErr_SetString(PyExc_TypeError, "The entry number must be an integer.");
      return NULL;
   }

   if (i < 0  ||  i >= self->stop  ||  i >= self->ntuple->GetEntries()) {
      PyErr_Format(PyExc_KeyError, "%d is out of range.", i);
      return NULL;
   }

   if (self->file->IsOpen() == kFALSE) {
      PyErr_SetString(PyExc_IOError, "The file has been closed.");
      return NULL;
   }

   if (self->step == 0) {
      PyErr_SetString(PyExc_ValueError, "The step size may not be zero.");
      return NULL;
   }

   self->ntuple->GetEntry(i, 1);
   return Py_BuildValue("O", self->entry);
}

static PyObject* rootntuple_RootNtuple_map(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a function and, optionally, output=True/False (default=False).";

   PyObject* function;
   PyObject* output = Py_False;
   static char *kwlist[] = {"function", "output", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|O", kwlist, &function, &output)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (output != Py_True  &&  output != Py_False) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   PyObject* varnames;
   int size;
   if (PyObject_HasAttrString(function, "varnames")) {
      varnames = PyObject_GetAttrString(function, "varnames");
      size = PyTuple_Size(varnames);
   }
   else {
      varnames = PyObject_GetAttrString(PyObject_GetAttrString(function, "func_code"), "co_varnames");
      size = PyInt_AsLong(PyObject_GetAttrString(PyObject_GetAttrString(self->filter, "func_code"), "co_argcount"));
   }
   if (varnames == NULL  ||  !PyTuple_Check(varnames)) {
      PyErr_SetString(PyExc_TypeError, "Why isn't function.func_code.co_varnames a tuple?  Is it a bug?");
      return NULL;
   }

   PyObject* labels = rootntuple_RootNtuple_labels(self);
   if (labels == NULL) return NULL;
      
   for (int i = 0;  i < size;  i++) {
      PyObject* varname = PyTuple_GetItem(varnames, i);
      if (!PySequence_Contains(labels, varname)) {
	 PyErr_Format(PyExc_ValueError, "Argument \"%s\" in function is not an ntuple variable.", PyString_AsString(varname));
	 Py_DECREF(labels);
	 return NULL;
      }
   }

   Py_DECREF(labels);

   int mapvars_size = size;
   char** mapvars = new (char*)[mapvars_size];
   if (mapvars == NULL) {
      PyErr_Format(PyExc_MemoryError, "Couldn't allocate internal array mapvars[] (only %d items!)", mapvars_size);
      return NULL;
   }

   for (int i = 0;  i < size;  i++) {
      PyObject* varname = PyTuple_GetItem(varnames, i);
      mapvars[i] = PyString_AsString(varname);
      self->ntuple->SetBranchStatus(mapvars[i], 1);
   }

   bool warned = false;
   PyObject* theoutput = PyList_New(0);
   while (true) {

      if (self->filter != Py_None) {
	 double dummy;
	 if (!rootntuple_RootNtuple_zipahead(self, &dummy, &warned)) {
	    delete [] mapvars;
	    Py_DECREF(theoutput);
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      PyObject* args = PyTuple_New(mapvars_size);
      if (args == NULL) {
	 delete [] mapvars;
	 Py_DECREF(theoutput);
	 return NULL;
      }
      for (int i = 0;  i < mapvars_size;  i++) {
	 PyObject* obj = PyFloat_FromDouble(self->ntuple->GetLeaf(mapvars[i])->GetValue());
	 if (obj == NULL  ||  PyTuple_SetItem(args, i, obj) != 0) {
	    Py_DECREF(args);
	    delete [] mapvars;
	    Py_DECREF(theoutput);
	    return false;
	 }
      }

      PyObject* result = PyEval_CallObject(function, args);
      Py_DECREF(args);
      if (result == NULL) {
	 delete [] mapvars;
	 Py_DECREF(theoutput);
	 return NULL;
      }

      if (output == Py_True) {
	 if (PyList_Append(theoutput, result) != 0) {
	    delete [] mapvars;
	    Py_DECREF(theoutput);
	    Py_DECREF(result);
	    return NULL;
	 }
      }
      else {
	 Py_DECREF(result);
      }

      self->i += self->step;
   }

   if (output == Py_True) return theoutput;

   Py_INCREF(Py_None);
   return Py_None;
}

static bool rootntuple_RootNtuple_setupfunction(char*** mapvars, int* mapvars_size, rootntuple_RootNtuple* self, PyObject* function) {
   if (PyCallable_Check(function)) {
      PyObject* varnames;
      int size;
      if (PyObject_HasAttrString(function, "varnames")) {
	 varnames = PyObject_GetAttrString(function, "varnames");
	 size = PyTuple_Size(varnames);
      }
      else {
	 varnames = PyObject_GetAttrString(PyObject_GetAttrString(function, "func_code"), "co_varnames");
	 size = PyInt_AsLong(PyObject_GetAttrString(PyObject_GetAttrString(self->filter, "func_code"), "co_argcount"));
      }
      if (varnames == NULL  ||  !PyTuple_Check(varnames)) {
	 PyErr_SetString(PyExc_TypeError, "Why isn't function.func_code.co_varnames a tuple?  Is it a bug?");
	 return false;
      }

      PyObject* labels = rootntuple_RootNtuple_labels(self);
      if (labels == NULL) return false;
      
      for (int i = 0;  i < size;  i++) {
	 PyObject* varname = PyTuple_GetItem(varnames, i);
	 if (!PySequence_Contains(labels, varname)) {
	    PyErr_Format(PyExc_ValueError, "Argument \"%s\" in function is not an ntuple variable.", PyString_AsString(varname));
	    Py_DECREF(labels);
	    return false;
	 }
      }

      Py_DECREF(labels);

      *mapvars_size = size;
      *mapvars = new (char*)[*mapvars_size];
      if (*mapvars == NULL) {
	 PyErr_Format(PyExc_MemoryError, "Couldn't allocate internal array mapvars[] (only %d items!)", *mapvars_size);
	 return false;
      }

      for (int i = 0;  i < size;  i++) {
	 PyObject* varname = PyTuple_GetItem(varnames, i);
	 (*mapvars)[i] = PyString_AsString(varname);
	 self->ntuple->SetBranchStatus((*mapvars)[i], 1);
      }
   }
   else {
      PyObject* labels = rootntuple_RootNtuple_labels(self);
      if (labels == NULL) return false;
      if (!PySequence_Contains(labels, function)) {
	 PyErr_Format(PyExc_ValueError, "Argument \"%s\" in function is not an ntuple variable.", PyString_AsString(function));
	 Py_DECREF(labels);
	 return false;
      }
      Py_DECREF(labels);

      *mapvars_size = -1;
      *mapvars = new (char*)[1];
      (*mapvars)[0] = PyString_AsString(function);
      self->ntuple->SetBranchStatus((*mapvars)[0], 1);
   }
}

static bool rootntuple_RootNtuple_callfunc(float* res, rootntuple_RootNtuple* self, PyObject* function, char** mapvars, int mapvars_size) {
   if (mapvars_size != -1) {
      PyObject* args = PyTuple_New(mapvars_size);
      if (args == NULL) {
	 delete [] mapvars;
	 return false;
      }
      for (int i = 0;  i < mapvars_size;  i++) {
	 PyObject* obj = PyFloat_FromDouble(self->ntuple->GetLeaf(mapvars[i])->GetValue());
	 if (obj == NULL  ||  PyTuple_SetItem(args, i, obj) != 0) {
	    Py_DECREF(args);
	    delete [] mapvars;
	    return false;
	 }
      }

      PyObject* result = PyEval_CallObject(function, args);
      Py_DECREF(args);
      if (result == NULL) {
	 delete [] mapvars;
	 return false;
      }
      if (!PyNumber_Check(result)) {
	 PyErr_SetString(PyExc_TypeError, "The map function must return numbers.");
	 delete [] mapvars;
	 Py_DECREF(result);
	 return false;
      }
      *res = float(PyFloat_AsDouble(result));
      Py_DECREF(result);
   }
   else {
      *res = float(self->ntuple->GetLeaf(mapvars[0])->GetValue());
   }
   return true;
}

static PyObject* rootntuple_RootNtuple_plot(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or label name and, optionally, a number of good events before binning, a leftloss fraction, rightloss, an edge padding fraction, a ROOT Draw Option (default=\"\") and a draw update frequency.";

   PyObject* function;
   double leftloss = self->leftloss;
   double rightloss = self->rightloss;
   int sample = self->sample;
   double padding = self->padding;
   const char* options = "";
   const char* options2 = options;
   PyObject* freq = self->frequency;
   int frequency;
   PyObject* master = Py_None;
   long realstart = -1;
   long realstop = -1;
   long extrai = -1;
   static char *kwlist[] = {"function", "sample", "leftloss", "rightloss", "padding", "options", "frequency", "master", "realstart", "realstop", "extrai", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|idddsOOiii", kwlist, &function, &sample, &leftloss, &rightloss, &padding, &options, &freq, &master, &realstart, &realstop, &extrai)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PyString_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (leftloss < 0.  ||  rightloss < 0.  ||  leftloss + rightloss >= 1.) {
      PyErr_SetString(PyExc_ValueError, "Leftloss and rightloss must be between 0 and 1, and their sum must be less than 1.");
      return NULL;
   }
   if (sample < 1  &&  master == Py_None) {
      PyErr_SetString(PyExc_ValueError, "Sample must be a positive integer.");
      return NULL;
   }
   if (freq == Py_None) frequency = -1;
   else {
      if (!PyInt_Check(freq)  ||  PyInt_AsLong(freq) < 1) {
	 PyErr_SetString(PyExc_TypeError, "The frequency must be None or a positive integer.");
	 return NULL;
      }
      frequency = PyInt_AsLong(freq);
   }
   if (options != options2  ||  self->which != 0) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   if (self->which == 0) options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction(&mapvars, &mapvars_size, self, function)) return NULL;

   bool allint = true;
   int min, max;
   bool min_set = false, max_set = false;

   int bins;
   double low, high;
   if (master == Py_None) {
      multiset< pair<float, float> > buffer;
      long sample_counter = 0;
      while (sample_counter < sample) {

	 double weight = 1.0;
	 if (self->filter != Py_None) {
	    if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	       delete [] mapvars;
	       return NULL;
	    }
	 }

	 if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

	 if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

	 float res;
	 if (!rootntuple_RootNtuple_callfunc(&res, self, function, mapvars, mapvars_size)) return NULL;
      
	 if (!allint  ||  abs(nearbyint(res) - res) > 1e-10) allint = false;
	 else {
	    if (!min_set  ||  res < min) {
	       min_set = true;
	       min = int(nearbyint(res));
	    }
	    if (!max_set  ||  res > max) {
	       max_set = true;
	       max = int(nearbyint(res));
	    }
	 }

	 pair<float, float> tmp;
	 tmp.first = res;
	 tmp.second = weight;
	 buffer.insert(tmp);
	 sample_counter++;

	 self->i += self->step;
      }

      multiset< pair<float, float> >::const_iterator iter = buffer.begin();
      multiset< pair<float, float> >::const_iterator iter_end = buffer.end();
      multiset< pair<float, float> >::const_reverse_iterator riter = buffer.rbegin();
      multiset< pair<float, float> >::const_reverse_iterator riter_end = buffer.rend();

      for (int i = 0;  i < int(ceil(leftloss*double(sample_counter)));  i++) ++iter;
      for (int i = 0;  i < int(ceil(rightloss*double(sample_counter)));  i++) ++riter;

      low = iter->first;
      high = riter->first;

      if (low > high) {
	 double tmp = low;
	 low = high;
	 high = tmp;
      }
      if (low == high  &&  high != 0.) {
	 double tmp = high / 100.;
	 printf("Warning: only one value in prebinning sample; extending range by 1%% (%g).\n", tmp);
	 low -= tmp;
	 high += tmp;
      }
      else if (low == high) {
	 printf("Warning: only one value in prebinning sample; extending range by 1.\n");
	 low -= 1.;
	 high += 1.;
      }
      else {
	 double tmp = padding * (high - low);
	 low -= tmp;
	 high += tmp;
      }

      // http://www.fmrib.ox.ac.uk/analysis/techrep/tr00mj2/tr00mj2/node24.html
      // Scott, D. 1979. "On optimal and data-based histograms," Biometrika, 66:605-610.
      // Izenman, A. J. 1991. "Recent developments in nonparametric density estimation," Journal of the American Statistical Association, 86(413):205-224.
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-leftloss-rightloss)));  i++) ++iter;
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-leftloss-rightloss)));  i++) ++riter;

      if (realstart == -1  ||  realstop == -1  ||  extrai == -1) {
	 realstart = self->start;
	 realstop = self->stop;
	 extrai = 0;
      }

      double IQR = riter->first - iter->first;
      if (IQR > 0.) bins = int(nearbyint((high - low) / 2. / IQR * pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./3.)));
      else {
	 printf("Warning: IQR is %g, so binning is only nearly optimal.\n", IQR);
	 bins = int(nearbyint(pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./3.)));
      }

      if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
      if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
      if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
      self->which = 0;
      char strid[50];
      sprintf(strid, "%d", (int)(self));
      if (allint) self->hist = new TH1F((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", max-min+1, min-0.5, max+0.5);
      else self->hist = new TH1F((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", bins, low, high);
      self->hist->SetStats(kFALSE);

      if (self->display == Py_True) {
	 if (self->canvas == NULL) {
	    char strid[50];
	    sprintf(strid, "%d", (int)(self));
	    self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	    myCanvas.insert(self->canvas);

	    if (myThread == NULL) {
	       myThread = new TThread(OneSecondLoop, NULL);
	       myThread->Run();
	    }
	 }
	 else {
	    self->canvas->SetWindowSize(canvas_wide, canvas_high);
	    self->canvas->Resize();
	 }
      }

      for (iter = buffer.begin(); iter != iter_end;  ++iter) {
	 self->hist->Fill(iter->first, iter->second);
      }
   }
   long goodevents = 0;
   while (true) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res;
      if (!rootntuple_RootNtuple_callfunc(&res, self, function, mapvars, mapvars_size)) return NULL;

      if (master == Py_None) {
	 self->hist->Fill(res, weight);
      }
      else {
	 ((rootntuple_RootNtuple*)(master))->hist->Fill(res, weight);
      }
      self->i += self->step;
      goodevents++;
      if (frequency != -1  &&  goodevents % frequency == 0  &&  self->display == Py_True) {
	 if (master == Py_None) {
	    self->canvas->cd();
	    self->hist->Draw(options);
	    self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    self->canvas->Update();
	 }
	 else {
	    rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	    m->canvas->cd();
	    m->hist->Draw(options);
	    m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    m->canvas->Update();
	 }
      }
   }

   if (self->display == Py_True) {
      if (master == Py_None) {
	 self->canvas->cd();
	 self->hist->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 self->canvas->Update();
      }
      else {
	 rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	 m->canvas->cd();
	 m->hist->Draw(options);
	 m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 m->canvas->Update();
      }
   }

   if (allint) return Py_BuildValue("(ii)", min, max);
   else if (master == Py_None) return Py_BuildValue("(idd)", bins, low, high);
   else return Py_BuildValue("O", Py_None);
}

static PyObject* rootntuple_RootNtuple_plotbin(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or label name, a number of bins, low and high limits, and, optionally, a ROOT Draw Option (default=\"\") and a draw update frequency.";

   PyObject* function;
   int bins;
   double low, high;
   const char* options = "";
   const char* options2 = options;
   PyObject* freq = self->frequency;
   int frequency;
   PyObject* master = Py_None;
   static char *kwlist[] = {"function", "bins", "low", "high", "options", "frequency", "master", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "Oidd|sOO", kwlist, &function, &bins, &low, &high, &options, &freq, &master)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PyString_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (freq == Py_None) frequency = -1;
   else {
      if (!PyInt_Check(freq)  ||  PyInt_AsLong(freq) < 1) {
	 PyErr_SetString(PyExc_TypeError, "The frequency must be None or a positive integer.");
	 return NULL;
      }
      frequency = PyInt_AsLong(freq);
   }
   if (options != options2  ||  self->which != 0) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   if (self->which == 0) options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction(&mapvars, &mapvars_size, self, function)) return NULL;

   if (master == Py_None) {
      if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
      if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
      if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
      self->which = 0;
      char strid[50];
      sprintf(strid, "%d", (int)(self));
      self->hist = new TH1F((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", bins, low, high);
      self->hist->SetStats(kFALSE);

      if (self->display == Py_True) {
	 if (self->canvas == NULL) {
	    char strid[50];
	    sprintf(strid, "%d", (int)(self));
	    self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	    myCanvas.insert(self->canvas);

	    if (myThread == NULL) {
	       myThread = new TThread(OneSecondLoop, NULL);
	       myThread->Run();
	    }
	 }
	 else {
	    self->canvas->SetWindowSize(canvas_wide, canvas_high);
	    self->canvas->Resize();
	 }
      }
   }

   long goodevents = 0;
   while (true) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res;
      if (!rootntuple_RootNtuple_callfunc(&res, self, function, mapvars, mapvars_size)) return NULL;
      if (master == Py_None) {
	 self->hist->Fill(res, weight);
      }
      else {
	 ((rootntuple_RootNtuple*)(master))->hist->Fill(res, weight);
      }
      self->i += self->step;
      goodevents++;
      if (frequency != -1  &&  goodevents % frequency == 0  &&  self->display == Py_True) {
	 if (master == Py_None) {
	    self->canvas->cd();
	    self->hist->Draw(options);
	    self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    self->canvas->Update();
	 }
	 else {
	    rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	    m->canvas->cd();
	    m->hist->Draw(options);
	    m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    m->canvas->Update();
	 }
      }
   }

   if (self->display == Py_True) {
      if (master == Py_None) {
	 self->canvas->cd();
	 self->hist->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 self->canvas->Update();
      }
      else {
	 rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	 m->canvas->cd();
	 m->hist->Draw(options);
	 m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 m->canvas->Update();
      }
   }

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_plotint(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or label name, low and high inclusive limits, and, optionally, a ROOT Draw Option (default=\"\") and a draw update frequency.";

   PyObject* function;
   int low, high;
   const char* options = "";
   const char* options2 = options;
   PyObject* freq = self->frequency;
   int frequency;
   PyObject* master = Py_None;
   static char *kwlist[] = {"function", "low", "high", "options", "frequency", "master", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "Oii|sOO", kwlist, &function, &low, &high, &options, &freq, &master)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PyString_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (freq == Py_None) frequency = -1;
   else {
      if (!PyInt_Check(freq)  ||  PyInt_AsLong(freq) < 1) {
	 PyErr_SetString(PyExc_TypeError, "The frequency must be None or a positive integer.");
	 return NULL;
      }
      frequency = PyInt_AsLong(freq);
   }
   if (options != options2  ||  self->which != 0) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   if (self->which == 0) options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction(&mapvars, &mapvars_size, self, function)) return NULL;

   if (master == Py_None) {
      if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
      if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
      if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
      self->which = 0;
      char strid[50];
      sprintf(strid, "%d", (int)(self));
      self->hist = new TH1F((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", high-low+1, float(low)-0.5, float(high)+0.5);
      self->hist->SetStats(kFALSE);

      if (self->display == Py_True) {
	 if (self->canvas == NULL) {
	    char strid[50];
	    sprintf(strid, "%d", (int)(self));
	    self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	    myCanvas.insert(self->canvas);

	    if (myThread == NULL) {
	       myThread = new TThread(OneSecondLoop, NULL);
	       myThread->Run();
	    }
	 }
	 else {
	    self->canvas->SetWindowSize(canvas_wide, canvas_high);
	    self->canvas->Resize();
	 }
      }
   }

   long goodevents = 0;
   while (true) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res;
      if (!rootntuple_RootNtuple_callfunc(&res, self, function, mapvars, mapvars_size)) return NULL;
      if (abs(nearbyint(res) - res) > 1e-10) {
	 printf("Warning: entry %d is non-integral (%g)!\n", self->i, res);
      }

      if (master == Py_None) {
	 self->hist->Fill(res, weight);
      }
      else {
	 ((rootntuple_RootNtuple*)(master))->hist->Fill(res, weight);
      }
      self->i += self->step;
      goodevents++;
      if (frequency != -1  &&  goodevents % frequency == 0  &&  self->display == Py_True) {
	 if (master == Py_None) {
	    self->canvas->cd();
	    self->hist->Draw(options);
	    self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    self->canvas->Update();
	 }
	 else {
	    rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	    m->canvas->cd();
	    m->hist->Draw(options);
	    m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    m->canvas->Update();
	 }
      }
   }

   if (self->display == Py_True) {
      if (master == Py_None) {
	 self->canvas->cd();
	 self->hist->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 self->canvas->Update();
      }
      else {
	 rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	 m->canvas->cd();
	 m->hist->Draw(options);
	 m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 m->canvas->Update();
      }
   }

   Py_INCREF(Py_None);
   return Py_None;
}

static bool rootntuple_RootNtuple_setupfunction2(char*** mapvars, int* mapvars_size, rootntuple_RootNtuple* self, PyObject* function) {
   if (PyCallable_Check(function)) {
      PyObject* varnames;
      int size;
      if (PyObject_HasAttrString(function, "varnames")) {
	 varnames = PyObject_GetAttrString(function, "varnames");
	 size = PyTuple_Size(varnames);
      }
      else {
	 varnames = PyObject_GetAttrString(PyObject_GetAttrString(function, "func_code"), "co_varnames");
	 size = PyInt_AsLong(PyObject_GetAttrString(PyObject_GetAttrString(self->filter, "func_code"), "co_argcount"));
      }
      if (varnames == NULL  ||  !PyTuple_Check(varnames)) {
	 PyErr_SetString(PyExc_TypeError, "Why isn't function.func_code.co_varnames a tuple?  Is it a bug?");
	 return false;
      }

      PyObject* labels = rootntuple_RootNtuple_labels(self);
      if (labels == NULL) return false;
      
      for (int i = 0;  i < size;  i++) {
	 PyObject* varname = PyTuple_GetItem(varnames, i);
	 if (!PySequence_Contains(labels, varname)) {
	    PyErr_Format(PyExc_ValueError, "Argument \"%s\" in function is not an ntuple variable.", PyString_AsString(varname));
	    Py_DECREF(labels);
	    return false;
	 }
      }

      Py_DECREF(labels);

      *mapvars_size = size;
      *mapvars = new (char*)[*mapvars_size];
      if (*mapvars == NULL) {
	 PyErr_Format(PyExc_MemoryError, "Couldn't allocate internal array mapvars[] (only %d items!)", *mapvars_size);
	 return false;
      }

      for (int i = 0;  i < size;  i++) {
	 PyObject* varname = PyTuple_GetItem(varnames, i);
	 (*mapvars)[i] = PyString_AsString(varname);
	 self->ntuple->SetBranchStatus((*mapvars)[i], 1);
      }
   }
   else {
      PyObject* f1 = PySequence_GetItem(function, 0);
      if (f1 == NULL) return false;
      PyObject* f2 = PySequence_GetItem(function, 1);
      if (f2 == NULL) {Py_DECREF(f1); return false;}

      PyObject* labels = rootntuple_RootNtuple_labels(self);
      if (labels == NULL) return false;
      if (!PySequence_Contains(labels, f1)) {
	 PyErr_Format(PyExc_ValueError, "Argument \"%s\" in function is not an ntuple variable.", PyString_AsString(f1));
	 Py_DECREF(f1);
	 Py_DECREF(f2);
	 Py_DECREF(labels);
	 return false;
      }
      if (!PySequence_Contains(labels, f2)) {
	 PyErr_Format(PyExc_ValueError, "Argument \"%s\" in function is not an ntuple variable.", PyString_AsString(f2));
	 Py_DECREF(f1);
	 Py_DECREF(f2);
	 Py_DECREF(labels);
	 return false;
      }
      Py_DECREF(labels);

      *mapvars_size = -1;
      *mapvars = new (char*)[2];
      (*mapvars)[0] = PyString_AsString(f1);
      self->ntuple->SetBranchStatus((*mapvars)[0], 1);
      (*mapvars)[1] = PyString_AsString(f2);
      self->ntuple->SetBranchStatus((*mapvars)[1], 1);
      Py_DECREF(f1);
      Py_DECREF(f2);
   }
}

static bool rootntuple_RootNtuple_callfunc2(float* res1, float* res2, rootntuple_RootNtuple* self, PyObject* function, char** mapvars, int mapvars_size) {
   if (mapvars_size != -1) {
      PyObject* args = PyTuple_New(mapvars_size);
      if (args == NULL) {
	 delete [] mapvars;
	 return false;
      }
      for (int i = 0;  i < mapvars_size;  i++) {
	 PyObject* obj = PyFloat_FromDouble(self->ntuple->GetLeaf(mapvars[i])->GetValue());
	 if (obj == NULL  ||  PyTuple_SetItem(args, i, obj) != 0) {
	    Py_DECREF(args);
	    delete [] mapvars;
	    return false;
	 }
      }

      PyObject* result = PyEval_CallObject(function, args);
      Py_DECREF(args);
      if (result == NULL) {
	 delete [] mapvars;
	 return false;
      }
      if (!PyTuple_Check(result)  ||  PyTuple_Size(result) != 2) {
	 PyErr_SetString(PyExc_TypeError, "The map function must return tuples of two numbers.");
	 delete [] mapvars;
	 Py_DECREF(result);
	 return false;
      }
      PyObject* r1 = PyTuple_GetItem(result, 0);
      if (r1 == NULL  ||  !PyNumber_Check(r1)) {
	 PyErr_SetString(PyExc_TypeError, "The map function must return tuples of two numbers.");
	 delete [] mapvars;
	 Py_DECREF(result);
	 if (r1 != NULL) Py_DECREF(r1);
	 return false;
      }
      PyObject* r2 = PyTuple_GetItem(result, 1);
      if (r2 == NULL  ||  !PyNumber_Check(r2)) {
	 PyErr_SetString(PyExc_TypeError, "The map function must return tuples of two numbers.");
	 delete [] mapvars;
	 Py_DECREF(result);
	 Py_DECREF(r1);
	 if (r2 != NULL) Py_DECREF(r2);
	 return false;
      }
      *res1 = float(PyFloat_AsDouble(r1));
      *res2 = float(PyFloat_AsDouble(r2));
      Py_DECREF(result);
      Py_DECREF(r1);
      Py_DECREF(r2);
   }
   else {
      *res1 = float(self->ntuple->GetLeaf(mapvars[0])->GetValue());
      *res2 = float(self->ntuple->GetLeaf(mapvars[1])->GetValue());
   }
   return true;
}

static PyObject* rootntuple_RootNtuple_profile(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or tuple of two label names and, optionally, a number of good events before binning, a leftloss fraction, rightloss, an edge padding fraction, a ROOT Profile Type Option, a ROOT Draw Option (default=\"\") and a draw update frequency.";

   PyObject* function;
   double leftloss = self->leftloss;
   double rightloss = self->rightloss;
   int sample = self->sample;
   double padding = self->padding;
   const char* type = "";
   const char* options = "";
   const char* options2 = options;
   PyObject* freq = self->frequency;
   int frequency;
   PyObject* master = Py_None;
   long realstart = -1;
   long realstop = -1;
   long extrai = -1;
   static char *kwlist[] = {"function", "sample", "leftloss", "rightloss", "padding", "type", "options", "frequency", "master", "realstart", "realstop", "extrai", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|idddssOOiii", kwlist, &function, &sample, &leftloss, &rightloss, &padding, &type, &options, &freq, &master, &realstart, &realstop, &extrai)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PySequence_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)  &&  PySequence_Size(function) != 2) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)) {
      PyObject* f1 = PySequence_GetItem(function, 0);
      if (f1 == NULL) return NULL;
      PyObject* f2 = PySequence_GetItem(function, 1);
      if (f2 == NULL) {Py_DECREF(f1);  return NULL;}
      if (!PyString_Check(f1)  ||  !PyString_Check(f2)) {
	 PyErr_SetString(PyExc_TypeError, errstring);	
	 Py_DECREF(f1);
	 Py_DECREF(f2);
	 return NULL;
      }
      Py_DECREF(f1);
      Py_DECREF(f2);
   }
   if (leftloss < 0.  ||  rightloss < 0.  ||  leftloss + rightloss >= 1.) {
      PyErr_SetString(PyExc_ValueError, "Leftloss and rightloss must be between 0 and 1, and their sum must be less than 1.");
      return NULL;
   }
   if (sample < 1  &&  master == Py_None) {
      PyErr_SetString(PyExc_ValueError, "Sample must be a positive integer.");
      return NULL;
   }
   if (freq == Py_None) frequency = -1;
   else {
      if (!PyInt_Check(freq)  ||  PyInt_AsLong(freq) < 1) {
	 PyErr_SetString(PyExc_TypeError, "The frequency must be None or a positive integer.");
	 return NULL;
      }
      frequency = PyInt_AsLong(freq);
   }

   if (options != options2  ||  self->which != 1) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   if (self->which == 1) options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction2(&mapvars, &mapvars_size, self, function)) return NULL;

   bool allint = true;
   int min, max;
   bool min_set = false, max_set = false;

   int bins;
   double low, high;
   if (master == Py_None) {
      multiset< pair< pair<float, float>, float> > buffer;
      long sample_counter = 0;
      while (sample_counter < sample) {

	 double weight = 1.0;
	 if (self->filter != Py_None) {
	    if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	       delete [] mapvars;
	       return NULL;
	    }
	 }

	 if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

	 if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

	 float res1, res2;
	 if (!rootntuple_RootNtuple_callfunc2(&res1, &res2, self, function, mapvars, mapvars_size)) return NULL;
      
	 if (!allint  ||  abs(nearbyint(res1) - res1) > 1e-10) allint = false;
	 else {
	    if (!min_set  ||  res1 < min) {
	       min_set = true;
	       min = int(nearbyint(res1));
	    }
	    if (!max_set  ||  res1 > max) {
	       max_set = true;
	       max = int(nearbyint(res1));
	    }
	 }

	 pair<float, float> res;
	 res.first = res1;
	 res.second = res2;

	 pair< pair<float, float>, float> tmp;
	 tmp.first = res;
	 tmp.second = weight;
	 buffer.insert(tmp);
	 sample_counter++;

	 self->i += self->step;
      }

      multiset< pair< pair<float, float>, float> >::const_iterator iter = buffer.begin();
      multiset< pair< pair<float, float>, float> >::const_iterator iter_end = buffer.end();
      multiset< pair< pair<float, float>, float> >::const_reverse_iterator riter = buffer.rbegin();
      multiset< pair< pair<float, float>, float> >::const_reverse_iterator riter_end = buffer.rend();

      for (int i = 0;  i < int(ceil(leftloss*double(sample_counter)));  i++) ++iter;
      for (int i = 0;  i < int(ceil(rightloss*double(sample_counter)));  i++) ++riter;

      low = iter->first.first;
      high = riter->first.first;

      if (low > high) {
	 double tmp = low;
	 low = high;
	 high = tmp;
      }
      if (low == high  &&  high != 0.) {
	 double tmp = high / 100.;
	 printf("Warning: only one value in prebinning sample; extending range by 1%% (%g).\n", tmp);
	 low -= tmp;
	 high += tmp;
      }
      else if (low == high) {
	 printf("Warning: only one value in prebinning sample; extending range by 1.\n");
	 low -= 1.;
	 high += 1.;
      }
      else {
	 double tmp = padding * (high - low);
	 low -= tmp;
	 high += tmp;
      }

      // http://www.fmrib.ox.ac.uk/analysis/techrep/tr00mj2/tr00mj2/node24.html
      // Scott, D. 1979. "On optimal and data-based histograms," Biometrika, 66:605-610.
      // Izenman, A. J. 1991. "Recent developments in nonparametric density estimation," Journal of the American Statistical Association, 86(413):205-224.
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-leftloss-rightloss)));  i++) ++iter;
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-leftloss-rightloss)));  i++) ++riter;

      if (realstart == -1  ||  realstop == -1  ||  extrai == -1) {
	 realstart = self->start;
	 realstop = self->stop;
	 extrai = 0;
      }

      double IQR = riter->first.first - iter->first.first;
      if (IQR > 0.) bins = int(nearbyint((high - low) / 2. / IQR * pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./3.)));
      else {
	 printf("Warning: IQR is %g, so binning is only nearly optimal.\n", IQR);
	 bins = int(nearbyint(pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./3.)));
      }

      if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
      if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
      if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
      self->which = 1;
      char strid[50];
      sprintf(strid, "%d", (int)(self));
      if (allint) self->prof = new TProfile((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", max-min+1, min-0.5, max+0.5, type);
      else self->prof = new TProfile((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", bins, low, high, type);
      self->prof->SetStats(kFALSE);

      if (self->display == Py_True) {
	 if (self->canvas == NULL) {
	    char strid[50];
	    sprintf(strid, "%d", (int)(self));
	    self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	    myCanvas.insert(self->canvas);

	    if (myThread == NULL) {
	       myThread = new TThread(OneSecondLoop, NULL);
	       myThread->Run();
	    }
	 }
	 else {
	    self->canvas->SetWindowSize(canvas_wide, canvas_high);
	    self->canvas->Resize();
	 }
      }

      for (iter = buffer.begin(); iter != iter_end;  ++iter) {
	 self->prof->Fill(iter->first.first, iter->first.second, iter->second);
      }
   }

   long goodevents = 0;
   while (true) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res1, res2;
      if (!rootntuple_RootNtuple_callfunc2(&res1, &res2, self, function, mapvars, mapvars_size)) return NULL;

      if (master == Py_None) {
	 self->prof->Fill(res1, res2, weight);
      }
      else {
	 ((rootntuple_RootNtuple*)(master))->prof->Fill(res1, res2, weight);
      }
      self->i += self->step;
      goodevents++;
      if (frequency != -1  &&  goodevents % frequency == 0  &&  self->display == Py_True) {
	 if (master == Py_None) {
	    self->canvas->cd();
	    self->prof->Draw(options);
	    self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    self->canvas->Update();
	 }
	 else {
	    rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	    m->canvas->cd();
	    m->prof->Draw(options);
	    m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    m->canvas->Update();
	 }
      }
   }

   if (self->display == Py_True) {
      if (master == Py_None) {
	 self->canvas->cd();
	 self->prof->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 self->canvas->Update();
      }
      else {
	 rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	 m->canvas->cd();
	 m->prof->Draw(options);
	 m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 m->canvas->Update();
      }
   }

   if (allint) return Py_BuildValue("(ii)", min, max);
   else if (master == Py_None) return Py_BuildValue("(idd)", bins, low, high);
   else return Py_BuildValue("O", Py_None);
}

static PyObject* rootntuple_RootNtuple_profilebin(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or tuple of two label names, a number of bins, low and high limits, and, optionally, a ROOT Profile Type Option, a ROOT Draw Option (default=\"\") and a draw update frequency.";

   PyObject* function;
   int bins;
   double low, high;
   const char* type = "";
   const char* options = "";
   const char* options2 = options;
   PyObject* freq = self->frequency;
   int frequency;
   PyObject* master = Py_None;
   static char *kwlist[] = {"function", "bins", "low", "high", "type", "options", "frequency", "master", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "Oidd|ssOO", kwlist, &function, &bins, &low, &high, &type, &options, &freq, &master)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PySequence_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)  &&  PySequence_Size(function) != 2) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)) {
      PyObject* f1 = PySequence_GetItem(function, 0);
      if (f1 == NULL) return NULL;
      PyObject* f2 = PySequence_GetItem(function, 1);
      if (f2 == NULL) {Py_DECREF(f1); return NULL;}
      if (!PyString_Check(f1)  ||  !PyString_Check(f2)) {
	 PyErr_SetString(PyExc_TypeError, errstring);
	 Py_DECREF(f1);
	 Py_DECREF(f2);
	 return NULL;
      }
      Py_DECREF(f1);
      Py_DECREF(f2);
   }
   if (freq == Py_None) frequency = -1;
   else {
      if (!PyInt_Check(freq)  ||  PyInt_AsLong(freq) < 1) {
	 PyErr_SetString(PyExc_TypeError, "The frequency must be None or a positive integer.");
	 return NULL;
      }
      frequency = PyInt_AsLong(freq);
   }

   if (options != options2  ||  self->which != 1) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   if (self->which == 1) options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction2(&mapvars, &mapvars_size, self, function)) return NULL;

   if (master == Py_None) {
      if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
      if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
      if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
      self->which = 1;
      char strid[50];
      sprintf(strid, "%d", (int)(self));
      self->prof = new TProfile((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", bins, low, high, type);
      self->prof->SetStats(kFALSE);

      if (self->display == Py_True) {
	 if (self->canvas == NULL) {
	    char strid[50];
	    sprintf(strid, "%d", (int)(self));
	    self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	    myCanvas.insert(self->canvas);

	    if (myThread == NULL) {
	       myThread = new TThread(OneSecondLoop, NULL);
	       myThread->Run();
	    }
	 }
	 else {
	    self->canvas->SetWindowSize(canvas_wide, canvas_high);
	    self->canvas->Resize();
	 }
      }
   }

   long goodevents = 0;
   while (true) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res1, res2;
      if (!rootntuple_RootNtuple_callfunc2(&res1, &res2, self, function, mapvars, mapvars_size)) return NULL;
      if (master == Py_None) {
	 self->prof->Fill(res1, res2, weight);
      }
      else {
	 ((rootntuple_RootNtuple*)(master))->prof->Fill(res1, res2, weight);
      }
      self->i += self->step;
      goodevents++;
      if (frequency != -1  &&  goodevents % frequency == 0  &&  self->display == Py_True) {
	 if (master == Py_None) {
	    self->canvas->cd();
	    self->prof->Draw(options);
	    self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    self->canvas->Update();
	 }
	 else {
	    rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	    m->canvas->cd();
	    m->prof->Draw(options);
	    m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    m->canvas->Update();
	 }
      }
   }

   if (self->display == Py_True) {
      if (master == Py_None) {
	 self->canvas->cd();
	 self->prof->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 self->canvas->Update();
      }
      else {
	 rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	 m->canvas->cd();
	 m->prof->Draw(options);
	 m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 m->canvas->Update();
      }
   }

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_profileint(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or tuple of two label names, low and high inclusive limits, and, optionally, a ROOT Profile Type Option, a ROOT Draw Option (default=\"\") and a draw update frequency.";

   PyObject* function;
   int low, high;
   const char* type = "";
   const char* options = "";
   const char* options2 = options;
   PyObject* freq = self->frequency;
   int frequency;
   PyObject* master = Py_None;
   static char *kwlist[] = {"function", "low", "high", "type", "options", "frequency", "master", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "Oii|ssOO", kwlist, &function, &low, &high, &type, &options, &freq, &master)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PySequence_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)  &&  PySequence_Size(function) != 2) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)) {
      PyObject* f1 = PySequence_GetItem(function, 0);
      if (f1 == NULL) return NULL;
      PyObject* f2 = PySequence_GetItem(function, 1);
      if (f2 == NULL) {Py_DECREF(f1); return NULL;}
      if (!PyString_Check(f1)  ||  !PyString_Check(f2)) {
	 PyErr_SetString(PyExc_TypeError, errstring);
	 Py_DECREF(f1);
	 Py_DECREF(f2);
	 return NULL;
      }
      Py_DECREF(f1);
      Py_DECREF(f2);
   }
   if (freq == Py_None) frequency = -1;
   else {
      if (!PyInt_Check(freq)  ||  PyInt_AsLong(freq) < 1) {
	 PyErr_SetString(PyExc_TypeError, "The frequency must be None or a positive integer.");
	 return NULL;
      }
      frequency = PyInt_AsLong(freq);
   }

   if (options != options2  ||  self->which != 1) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   if (self->which == 1) options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction2(&mapvars, &mapvars_size, self, function)) return NULL;

   if (master == Py_None) {
      if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
      if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
      if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
      self->which = 1;
      char strid[50];
      sprintf(strid, "%d", (int)(self));
      self->prof = new TProfile((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", high-low+1, float(low)-0.5, float(high)+0.5, type);
      self->prof->SetStats(kFALSE);

      if (self->display == Py_True) {
	 if (self->canvas == NULL) {
	    char strid[50];
	    sprintf(strid, "%d", (int)(self));
	    self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	    myCanvas.insert(self->canvas);

	    if (myThread == NULL) {
	       myThread = new TThread(OneSecondLoop, NULL);
	       myThread->Run();
	    }
	 }
	 else {
	    self->canvas->SetWindowSize(canvas_wide, canvas_high);
	    self->canvas->Resize();
	 }
      }
   }

   long goodevents = 0;
   while (true) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res1, res2;
      if (!rootntuple_RootNtuple_callfunc2(&res1, &res2, self, function, mapvars, mapvars_size)) return NULL;
      if (abs(nearbyint(res1) - res1) > 1e-10) {
	 printf("Warning: entry %d is non-integral (%g)!\n", self->i, res1);
      }

      if (master == Py_None) {
	 self->prof->Fill(res1, res2, weight);
      }
      else {
	 ((rootntuple_RootNtuple*)(master))->prof->Fill(res1, res2, weight);
      }
      self->i += self->step;
      goodevents++;
      if (frequency != -1  &&  goodevents % frequency == 0  &&  self->display == Py_True) {
	 if (master == Py_None) {
	    self->canvas->cd();
	    self->prof->Draw(options);
	    self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    self->canvas->Update();
	 }
	 else {
	    rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	    m->canvas->cd();
	    m->prof->Draw(options);
	    m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	    m->canvas->Update();
	 }
      }
   }

   if (self->display == Py_True) {
      if (master == Py_None) {
	 self->canvas->cd();
	 self->prof->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 self->canvas->Update();
      }
      else {
	 rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	 m->canvas->cd();
	 m->prof->Draw(options);
	 m->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 m->canvas->Update();
      }
   }

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_plot2(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or tuple of two label names and, optionally, a number of good events, a leftloss fraction, toploss, rightloss, bottomloss, and an edge padding fraction.";

   PyObject* function;
   double leftloss = self->leftloss;
   double rightloss = self->rightloss;
   double toploss = self->leftloss;
   double bottomloss = self->rightloss;
   int sample = self->sample;
   double padding = self->padding;
   long realstart = -1;
   long realstop = -1;
   long extrai = -1;
   static char *kwlist[] = {"function", "sample", "leftloss", "toploss", "rightloss", "bottomloss", "padding", "realstart", "realstop", "extrai", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|idddddiii", kwlist, &function, &sample, &leftloss, &toploss, &rightloss, &bottomloss, &padding, &realstart, &realstop, &extrai)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PySequence_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)  &&  PySequence_Size(function) != 2) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)) {
      PyObject* f1 = PySequence_GetItem(function, 0);
      if (f1 == NULL) return NULL;
      PyObject* f2 = PySequence_GetItem(function, 1);
      if (f2 == NULL) {Py_DECREF(f1);  return NULL;}
      if (!PyString_Check(f1)  ||  !PyString_Check(f2)) {
	 PyErr_SetString(PyExc_TypeError, errstring);	
	 Py_DECREF(f1);
	 Py_DECREF(f2);
	 return NULL;
      }
      Py_DECREF(f1);
      Py_DECREF(f2);
   }
   if (leftloss < 0.  ||  rightloss < 0.  ||  leftloss + rightloss >= 1.) {
      PyErr_SetString(PyExc_ValueError, "Leftloss and rightloss must be between 0 and 1, and their sum must be less than 1.");
      return NULL;
   }
   if (toploss < 0.  ||  bottomloss < 0.  ||  toploss + bottomloss >= 1.) {
      PyErr_SetString(PyExc_ValueError, "Toploss and bottomloss must be between 0 and 1, and their sum must be less than 1.");
      return NULL;
   }
   if (sample < 1) {
      PyErr_SetString(PyExc_ValueError, "Sample must be a positive integer.");
      return NULL;
   }

   if (self->which != 2) {
      if (self->options != NULL) delete [] self->options;
      self->options = new char[5];
      strcpy(self->options, "colz");
   }

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction2(&mapvars, &mapvars_size, self, function)) return NULL;

   if (self->graph != NULL) delete self->graph;
   self->graph = new TGraph(sample);
   self->graph->SetTitle("");

   multiset<float> horiz;
   multiset<float> vert;
   vector<float> weights;
   long sample_counter = 0;
   while (sample_counter < sample) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res1, res2;
      if (!rootntuple_RootNtuple_callfunc2(&res1, &res2, self, function, mapvars, mapvars_size)) return NULL;
      
      horiz.insert(res1);
      vert.insert(res2);
      weights.push_back(weight);
      self->graph->SetPoint(sample_counter, res1, res2);

      sample_counter++;
      self->i += self->step;
   }

   if (realstart == -1  ||  realstop == -1  ||  extrai == -1) {
      realstart = self->start;
      realstop = self->stop;
      extrai = 0;
   }

   int hbins, vbins;
   double left, right, top, bottom;
   if (true) {
      multiset<float>::const_iterator iter = horiz.begin();
      multiset<float>::const_iterator iter_end = horiz.end();
      multiset<float>::const_reverse_iterator riter = horiz.rbegin();
      multiset<float>::const_reverse_iterator riter_end = horiz.rend();

      for (int i = 0;  i < int(ceil(leftloss*double(sample_counter)));  i++) ++iter;
      for (int i = 0;  i < int(ceil(rightloss*double(sample_counter)));  i++) ++riter;

      double low = *iter;
      double high = *riter;

      if (low > high) {
	 double tmp = low;
	 low = high;
	 high = tmp;
      }
      if (low == high  &&  high != 0.) {
	 double tmp = high / 100.;
	 printf("Warning: only one value in prebinning sample; extending range by 1%% (%g).\n", tmp);
	 low -= tmp;
	 high += tmp;
      }
      else if (low == high) {
	 printf("Warning: only one value in prebinning sample; extending range by 1.\n");
	 low -= 1.;
	 high += 1.;
      }
      else {
	 double tmp = padding * (high - low);
	 low -= tmp;
	 high += tmp;
      }

      int bins;
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-leftloss-rightloss)));  i++) ++iter;
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-leftloss-rightloss)));  i++) ++riter;

      double IQR = *riter - *iter;
      if (IQR > 0.) bins = int(nearbyint((high - low) / 2. / IQR * pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./4.)));
      else {
	 printf("Warning: IQR is %g, so binning is only nearly optimal.\n", IQR);
	 bins = int(nearbyint(pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./4.)));
      }

      hbins = bins;
      left = low;
      right = high;
   }
   if (true) {
      multiset<float>::const_iterator iter = vert.begin();
      multiset<float>::const_iterator iter_end = vert.end();
      multiset<float>::const_reverse_iterator riter = vert.rbegin();
      multiset<float>::const_reverse_iterator riter_end = vert.rend();

      for (int i = 0;  i < int(ceil(bottomloss*double(sample_counter)));  i++) ++iter;
      for (int i = 0;  i < int(ceil(toploss*double(sample_counter)));  i++) ++riter;

      double low = *iter;
      double high = *riter;

      if (low > high) {
	 double tmp = low;
	 low = high;
	 high = tmp;
      }
      if (low == high  &&  high != 0.) {
	 double tmp = high / 100.;
	 printf("Warning: only one value in prebinning sample; extending range by 1%% (%g).\n", tmp);
	 low -= tmp;
	 high += tmp;
      }
      else if (low == high) {
	 printf("Warning: only one value in prebinning sample; extending range by 1.\n");
	 low -= 1.;
	 high += 1.;
      }
      else {
	 double tmp = padding * (high - low);
	 low -= tmp;
	 high += tmp;
      }

      int bins;
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-bottomloss-toploss)));  i++) ++iter;
      for (int i = 0;  i < int(floor(0.25*double(sample_counter)*(1.-bottomloss-toploss)));  i++) ++riter;

      double IQR = *riter - *iter;
      if (IQR > 0.) bins = int(nearbyint((high - low) / 2. / IQR * pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./4.)));
      else {
	 printf("Warning: IQR is %g, so binning is only nearly optimal.\n", IQR);
	 bins = int(nearbyint(pow(double(sample_counter)*double(realstop - realstart)/double(self->i + extrai - realstart), 1./4.)));
      }

      vbins = bins;
      bottom = low;
      top = high;
   }

   if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
   if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
   if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
   self->which = 2;
   char strid[50];
   sprintf(strid, "%d", (int)(self));
   self->hist2 = new TH2F((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", hbins, left, right, vbins, bottom, top);
   self->hist2->SetStats(kFALSE);

   if (self->display == Py_True) {
      if (self->canvas == NULL) {
	 char strid[50];
	 sprintf(strid, "%d", (int)(self));
	 self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	 myCanvas.insert(self->canvas);

	 if (myThread == NULL) {
	    myThread = new TThread(OneSecondLoop, NULL);
	    myThread->Run();
	 }
      }
      else {
	 self->canvas->SetWindowSize(canvas_wide, canvas_high);
	 self->canvas->Resize();
      }
   }

   int i = 0;
   vector<float>::const_iterator iter_end = weights.end();
   for (vector<float>::const_iterator iter = weights.begin();
	iter != iter_end;  ++iter) {
      double x, y;
      self->graph->GetPoint(i, x, y);
      self->hist2->Fill(x, y, *iter);
      i++;
   }

   if (self->display == Py_True) {
      self->graph->GetXaxis()->SetRangeUser(left, right);
      self->graph->GetYaxis()->SetRangeUser(bottom, top);
      self->canvas->cd();
      self->graph->Draw("AP");
      self->canvas->SetLogy(0);
      self->canvas->Update();
   }

   return Py_BuildValue("(iddidd)", hbins, left, right, vbins, bottom, top);
}

static PyObject* rootntuple_RootNtuple_plot2bin(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds) {
   const char* errstring = "Pass a map function or tuple of two label names, an X number of bins, low and high limits, a Y number of bins, low and high limits, and, optionally, a ROOT Draw Option (default=\"colz\") and a draw update frequency.";

   PyObject* function;
   int binsX;
   double lowX, highX;
   int binsY;
   double lowY, highY;
   const char* options = "colz";
   const char* options2 = options;
   PyObject* freq = self->frequency;
   int frequency;
   PyObject* master = Py_None;
   static char *kwlist[] = {"function", "binsX", "lowX", "highX", "binsY", "lowY", "highY", "options", "frequency", "master", NULL};
   if (!PyArg_ParseTupleAndKeywords(args, kwds, "Oiddidd|sOO", kwlist, &function, &binsX, &lowX, &highX, &binsY, &lowY, &highY, &options, &freq, &master)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (!PyCallable_Check(function)  &&  !PySequence_Check(function)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)  &&  PySequence_Size(function) != 2) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (PySequence_Check(function)) {
      PyObject* f1 = PySequence_GetItem(function, 0);
      if (f1 == NULL) return NULL;
      PyObject* f2 = PySequence_GetItem(function, 1);
      if (f2 == NULL) {Py_DECREF(f1); return NULL;}
      if (!PyString_Check(f1)  ||  !PyString_Check(f2)) {
	 PyErr_SetString(PyExc_TypeError, errstring);
	 Py_DECREF(f1);
	 Py_DECREF(f2);
	 return NULL;
      }
      Py_DECREF(f1);
      Py_DECREF(f2);
   }
   if (freq == Py_None) frequency = -1;
   else {
      if (!PyInt_Check(freq)  ||  PyInt_AsLong(freq) < 1) {
	 PyErr_SetString(PyExc_TypeError, "The frequency must be None or a positive integer.");
	 return NULL;
      }
      frequency = PyInt_AsLong(freq);
   }

   if (options != options2  ||  self->which != 2) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   if (self->which == 2) options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   PyObject* tmp = rootntuple_RootNtuple_iter((PyObject*)(self));
   if (tmp == NULL) return NULL;
   Py_DECREF(tmp);

   char** mapvars;
   int mapvars_size;
   if (!rootntuple_RootNtuple_setupfunction2(&mapvars, &mapvars_size, self, function)) return NULL;

   if (master == Py_None) {
      if (self->hist != NULL) {delete self->hist; self->hist = NULL;}
      if (self->prof != NULL) {delete self->prof; self->prof = NULL;}
      if (self->hist2 != NULL) {delete self->hist2; self->hist2 = NULL;}
      self->which = 2;
      char strid[50];
      sprintf(strid, "%d", (int)(self));
      self->hist2 = new TH2F((string(self->ntuple->GetName()) + string("TemporaryHistogram") + string(strid)).c_str(), "", binsX, lowX, highX, binsY, lowY, highY);
      self->hist2->SetStats(kFALSE);

      if (self->display == Py_True) {
	 if (self->canvas == NULL) {
	    char strid[50];
	    sprintf(strid, "%d", (int)(self));
	    self->canvas = new TCanvas((string(self->ntuple->GetName()) + string("TemporaryCanvas") + string(strid)).c_str(), (string(self->ntuple->GetName()) + string(" ") + string(self->ntuple->GetTitle()) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
	    myCanvas.insert(self->canvas);

	    if (myThread == NULL) {
	       myThread = new TThread(OneSecondLoop, NULL);
	       myThread->Run();
	    }
	 }
	 else {
	    self->canvas->SetWindowSize(canvas_wide, canvas_high);
	    self->canvas->Resize();
	 }
      }
   }

   long goodevents = 0;
   while (true) {

      double weight = 1.0;
      if (self->filter != Py_None) {
	 if (!rootntuple_RootNtuple_zipahead(self, &weight)) {
	    delete [] mapvars;
	    return NULL;
	 }
      }

      if (self->i < 0  ||  self->i >= self->stop  ||  self->i >= self->ntuple->GetEntries()) break;

      if (self->filter == Py_None) self->ntuple->GetEntry(self->i, 0);

      float res1, res2;
      if (!rootntuple_RootNtuple_callfunc2(&res1, &res2, self, function, mapvars, mapvars_size)) return NULL;
      if (master == Py_None) {
	 self->hist2->Fill(res1, res2, weight);
      }
      else {
	 ((rootntuple_RootNtuple*)(master))->hist2->Fill(res1, res2, weight);
      }
      self->i += self->step;
      goodevents++;
      if (frequency != -1  &&  goodevents % frequency == 0  &&  self->display == Py_True) {
	 if (master == Py_None) {
	    self->canvas->cd();
	    self->hist2->Draw(options);
	    self->canvas->SetLogy(0);
	    self->canvas->SetLogz(self->log == Py_True ? 1 : 0);
	    self->canvas->Update();
	 }
	 else {
	    rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	    m->canvas->cd();
	    m->hist2->Draw(options);
	    m->canvas->SetLogy(0);
	    m->canvas->SetLogz(self->log == Py_True ? 1 : 0);
	    m->canvas->Update();
	 }
      }
   }

   if (self->display == Py_True) {
      if (master == Py_None) {
	 self->canvas->cd();
	 self->hist2->Draw(options);
	 self->canvas->SetLogy(0);
	 self->canvas->SetLogz(self->log == Py_True ? 1 : 0);
	 self->canvas->Update();
      }
      else {
	 rootntuple_RootNtuple* m = (rootntuple_RootNtuple*)(master);
	 m->canvas->cd();
	 m->hist2->Draw(options);
	 m->canvas->SetLogy(0);
	 m->canvas->SetLogz(self->log == Py_True ? 1 : 0);
	 m->canvas->Update();
      }
   }

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_redraw(rootntuple_RootNtuple* self, PyObject* args) {
   if ((self->hist == NULL && self->prof == NULL && self->hist2 == NULL)) {
      PyErr_SetString(PyExc_RuntimeError, "No histogram has ever been plotted.");
      return NULL;
   }

   const char* options = "";
   const char* options2 = options;
   if (!PyArg_ParseTuple(args, "|s", &options)) {
      PyErr_SetString(PyExc_TypeError, "ROOT Plot Options must be a string.");
      return NULL;
   }
   if (options != options2) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   if (self->canvas == NULL) {
      const char* name;
      const char* title;
      switch (self->which) {
	 case 0:
	    name = self->hist->GetName();
	    title = self->hist->GetTitle();
	    break;
	 case 1:
	    name = self->prof->GetName();
	    title = self->prof->GetTitle();
	    break;
	 case 2:
	    name = self->hist2->GetName();
	    title = self->hist2->GetTitle();
	    break;
      }

      char strid[50];
      sprintf(strid, "%d", (int)(self));
      self->canvas = new TCanvas((string(name) + string("TemporaryCanvas") + string(strid)).c_str(), (string(name) + string(" ") + string(title) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
      myCanvas.insert(self->canvas);

      if (myThread == NULL) {
	 myThread = new TThread(OneSecondLoop, NULL);
	 myThread->Run();
      }
   }
   else {
      self->canvas->SetWindowSize(canvas_wide, canvas_high);
      self->canvas->Resize();
   }

   self->canvas->cd();
   switch (self->which) {
      case 0:
	 self->hist->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 break;
      case 1:
	 self->prof->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 break;
      case 2:
	 self->hist2->Draw(options);
	 self->canvas->SetLogy(0);
	 self->canvas->SetLogz(self->log == Py_True ? 1 : 0);
	 break;
   }
   self->canvas->Update();

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_postscript(rootntuple_RootNtuple* self, PyObject* args) {
   const char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "Filename must be a string.");
      return NULL;
   }
   if (self->canvas == NULL) {
      PyErr_SetString(PyExc_RuntimeError, "You must first create a canvas (redraw or plot).");
      return NULL;
   }
   self->canvas->Print(name);

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_close(rootntuple_RootNtuple* self) {
   if (self->canvas != NULL) {
      myCanvas.erase(self->canvas);
      delete self->canvas;
      self->canvas = NULL;
   }
   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootNtuple_hist(rootntuple_RootNtuple* self, PyObject* args) {
   if (self->hist == NULL && self->prof == NULL && self->hist2 == NULL) {
      PyErr_SetString(PyExc_RuntimeError, "No histogram has ever been plotted.");
      return NULL;
   }

   const char* name;
   const char* title = NULL;
   if (!PyArg_ParseTuple(args, "s|s", &name, &title)) {
      PyErr_SetString(PyExc_TypeError, "Name and title must be strings (title is optional).");
      return NULL;
   }
   
   rootntuple_RootHist* pobj = PyObject_New(rootntuple_RootHist, &rootntuple_RootHistType);
   Py_INCREF(self->canvas_size);
   pobj->canvas_size = self->canvas_size;
   pobj->canvas = NULL;
   pobj->which = self->which;
   switch (self->which) {
      case 0:
	 pobj->hist = (TH1F*)(self->hist->Clone(name));
	 pobj->hist->SetStats(kFALSE);
	 if (title != NULL) pobj->hist->SetTitle(title);
	 else pobj->hist->SetTitle(name);
	 pobj->prof = NULL;
	 pobj->hist2 = NULL;
	 break;
      case 1:
	 pobj->hist = NULL;
	 pobj->prof = (TProfile*)(self->prof->Clone(name));
	 pobj->prof->SetStats(kFALSE);
	 if (title != NULL) pobj->prof->SetTitle(title);
	 else pobj->prof->SetTitle(name);
	 pobj->hist2 = NULL;
	 break;
      case 2:
	 pobj->hist = NULL;
	 pobj->prof = NULL;
	 pobj->hist2 = (TH2F*)(self->hist2->Clone(name));
	 pobj->hist2->SetStats(kFALSE);
	 if (title != NULL) pobj->hist2->SetTitle(title);
	 else pobj->hist2->SetTitle(name);
	 break;
   }
   if (self->options == NULL) pobj->options = NULL;
   else {
      pobj->options = new char[strlen(self->options)];
      strcpy(pobj->options, self->options);
   }
   Py_INCREF(Py_False);
   pobj->log = Py_False;

   return (PyObject*)(pobj);
}

static PyObject* rootntuple_RootNtuple_dealloc(rootntuple_RootNtuple* self) {
   ((rootntuple_RootNtupleEntry*)(self->entry))->master = NULL;
   Py_DECREF(self->entry);
   Py_DECREF(self->filter);
   Py_DECREF(self->canvas_size);
   Py_DECREF(self->frequency);
   Py_DECREF(self->log);
   if (self->filtervars != NULL) delete [] self->filtervars;
   Py_DECREF(self->optimize);
   if (self->canvas != NULL) {
      myCanvas.erase(self->canvas);
      delete self->canvas;
      self->canvas = NULL;
   }
   if (self->hist != NULL) delete self->hist;
   if (self->prof != NULL) delete self->prof;
   if (self->hist2 != NULL) delete self->hist2;
   if (self->options != NULL) delete [] self->options;
   if (self->graph != NULL) delete self->graph;
}

///////////////////////////////////////////////////////////////////

static int rootntuple_RootHist_init(rootntuple_RootHist* self, PyObject* args) {
   PyErr_SetString(PyExc_NotImplementedError, "This module does not support histogram creation.");
   return -1;
}

static PyObject* rootntuple_RootHist_mean(rootntuple_RootHist* self) {
   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);
   if (self->which == 2) hist = (TH1*)(self->hist2);

   if (self->which == 2) return Py_BuildValue("((dd)(dd))", hist->GetMean(1), hist->GetMeanError(1), hist->GetMean(2), hist->GetMeanError(2));

   return Py_BuildValue("(dd)", hist->GetMean(1), hist->GetMeanError(1));
}

static PyObject* rootntuple_RootHist_rms(rootntuple_RootHist* self) {
   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);
   if (self->which == 2) hist = (TH1*)(self->hist2);

   if (self->which == 2) return Py_BuildValue("((dd)(dd))", hist->GetRMS(1), hist->GetRMSError(1), hist->GetRMS(2), hist->GetRMSError(2));

   return Py_BuildValue("(dd)", hist->GetRMS(1), hist->GetRMSError(1));
}

static PyObject* rootntuple_RootHist_entries(rootntuple_RootHist* self) {
   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);
   if (self->which == 2) hist = (TH1*)(self->hist2);

   return Py_BuildValue("d", hist->GetEntries());
}

static PyObject* rootntuple_RootHist_ouvalues(rootntuple_RootHist* self) {
   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);
   if (self->which == 2) hist = (TH1*)(self->hist2);

   if (self->which == 2) {
      PyErr_SetString(PyExc_NotImplementedError, "Sorry, I can't find the ROOT functions to do this for 2D histograms.");
      return NULL;
   }

   return Py_BuildValue("(dd)", hist->GetBinContent(0), hist->GetBinContent(hist->GetNbinsX()+1));
}

static PyObject* rootntuple_RootHist_ouerrors(rootntuple_RootHist* self) {
   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);
   if (self->which == 2) hist = (TH1*)(self->hist2);

   if (self->which == 2) {
      PyErr_SetString(PyExc_NotImplementedError, "Sorry, I can't find the ROOT functions to do this for 2D histograms.");
      return NULL;
   }

   return Py_BuildValue("(dd)", hist->GetBinError(0), hist->GetBinError(hist->GetNbinsX()+1));
}

static PyObject* rootntuple_RootHist_bins(rootntuple_RootHist* self, PyObject* args) {
   const char* errstring = "Argument must be 0 (for bin centers), -1 (for bin low edges), or 1 (for bin high edges).";

   int which = 0;
   if (!PyArg_ParseTuple(args, "|i", &which)) {
      PyErr_SetString(PyExc_TypeError, errstring);
      return NULL;
   }
   if (which < -1  ||  which > 1) {
      PyErr_SetString(PyExc_ValueError, errstring);
      return NULL;
   }
   if (self->which == 2) {
      PyErr_SetString(PyExc_NotImplementedError, "Sorry, I can't find the ROOT functions to do this for 2D histograms.");
      return NULL;
   }

   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);

   int size = hist->GetNbinsX();
   PyObject* output = PyList_New(size);
   if (output == NULL) return NULL;
   for (int i = 0;  i < size;  i++) {
      double data;
      if (which == 0) data = hist->GetBinCenter(i+1);
      if (which == -1) data = hist->GetBinLowEdge(i+1);
      if (which == 1) data = hist->GetBinLowEdge(i+1) + hist->GetBinWidth(i+1);
      PyObject* obj = PyFloat_FromDouble(data);
      if (obj == NULL) {
	 Py_DECREF(output);
	 return NULL;
      }
      if (PyList_SetItem(output, i, obj) != 0) {
	 Py_DECREF(output);
	 Py_DECREF(obj);
	 return NULL;
      }
   }

   return output;
}

static PyObject* rootntuple_RootHist_values(rootntuple_RootHist* self) {
   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);
   if (self->which == 2) hist = (TH1*)(self->hist2);

   int size = hist->GetNbinsX();
   PyObject* output = PyList_New(size);
   if (output == NULL) return NULL;
   for (int i = 0;  i < size;  i++) {
      if (self->which < 2) {
	 PyObject* obj = PyFloat_FromDouble(hist->GetBinContent(i+1));
	 if (obj == NULL) {
	    Py_DECREF(output);
	    return NULL;
	 }
	 if (PyList_SetItem(output, i, obj) != 0) {
	    Py_DECREF(output);
	    Py_DECREF(obj);
	    return NULL;
	 }
      }
      else {
	 int sizey = hist->GetNbinsY();
	 PyObject* row = PyList_New(sizey);
	 if (row == NULL) {
	    Py_DECREF(output);
	    return NULL;
	 }
	 for (int j = 0;  j < sizey;  j++) {
	    PyObject* obj = PyFloat_FromDouble(hist->GetBinContent(i+1, j+1));
	    if (obj == NULL) {
	       Py_DECREF(output);
	       Py_DECREF(row);
	       return NULL;
	    }
	    if (PyList_SetItem(row, j, obj) != 0) {
	       Py_DECREF(output);
	       Py_DECREF(row);
	       Py_DECREF(obj);
	       return NULL;
	    }
	 }
	 if (PyList_SetItem(output, i, row) != 0) {
	    Py_DECREF(output);
	    Py_DECREF(row);
	    return NULL;
	 }
      }
   }

   return output;
}

static PyObject* rootntuple_RootHist_errors(rootntuple_RootHist* self) {
   TH1* hist;
   if (self->which == 0) hist = (TH1*)(self->hist);
   if (self->which == 1) hist = (TH1*)(self->prof);
   if (self->which == 2) hist = (TH1*)(self->hist2);

   int size = hist->GetNbinsX();
   PyObject* output = PyList_New(size);
   if (output == NULL) return NULL;
   for (int i = 0;  i < size;  i++) {
      if (self->which < 2) {
	 PyObject* obj = PyFloat_FromDouble(hist->GetBinError(i+1));
	 if (obj == NULL) {
	    Py_DECREF(output);
	    return NULL;
	 }
	 if (PyList_SetItem(output, i, obj) != 0) {
	    Py_DECREF(output);
	    Py_DECREF(obj);
	    return NULL;
	 }
      }
      else {
	 int sizey = hist->GetNbinsY();
	 PyObject* row = PyList_New(sizey);
	 if (row == NULL) {
	    Py_DECREF(output);
	    return NULL;
	 }
	 for (int j = 0;  j < sizey;  j++) {
	    PyObject* obj = PyFloat_FromDouble(hist->GetBinError(i+1, j+1));
	    if (obj == NULL) {
	       Py_DECREF(output);
	       Py_DECREF(row);
	       return NULL;
	    }
	    if (PyList_SetItem(row, j, obj) != 0) {
	       Py_DECREF(output);
	       Py_DECREF(row);
	       Py_DECREF(obj);
	       return NULL;
	    }
	 }
	 if (PyList_SetItem(output, i, row) != 0) {
	    Py_DECREF(output);
	    Py_DECREF(row);
	    return NULL;
	 }
      }
   }

   return output;
}

static PyObject* rootntuple_RootHist_redraw(rootntuple_RootHist* self, PyObject* args) {
   const char* options = "";
   const char* options2 = options;
   if (!PyArg_ParseTuple(args, "|s", &options)) {
      PyErr_SetString(PyExc_TypeError, "ROOT Plot Options must be a string.");
      return NULL;
   }
   if (options != options2) {
      if (self->options != NULL) delete self->options;
      self->options = new char[strlen(options)];
      strcpy(self->options, options);
   }
   options = self->options;

   long canvas_wide, canvas_high;
   PyObject *tmp1 = NULL, *tmp2 = NULL;
   if (!PySequence_Check(self->canvas_size)  ||
       !PyInt_Check(tmp1 = PySequence_GetItem(self->canvas_size, 0))  ||
       !PyInt_Check(tmp2 = PySequence_GetItem(self->canvas_size, 1))) {
      PyErr_SetString(PyExc_TypeError, "The attribute canvas_size must be a tuple of two integers.");
      if (tmp1 != NULL) Py_DECREF(tmp1);
      if (tmp2 != NULL) Py_DECREF(tmp2);
      return NULL;
   }
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);
   canvas_wide = PyInt_AsLong(tmp1 = PySequence_GetItem(self->canvas_size, 0));
   canvas_high = PyInt_AsLong(tmp2 = PySequence_GetItem(self->canvas_size, 1));
   Py_DECREF(tmp1);
   Py_DECREF(tmp2);

   if (self->canvas == NULL) {
      const char* name;
      const char* title;
      switch (self->which) {
	 case 0:
	    name = self->hist->GetName();
	    title = self->hist->GetTitle();
	    break;
	 case 1:
	    name = self->prof->GetName();
	    title = self->prof->GetTitle();
	    break;
	 case 2:
	    name = self->hist2->GetName();
	    title = self->hist2->GetTitle();
	    break;
      }

      char strid[50];
      sprintf(strid, "%d", (int)(self));
      self->canvas = new TCanvas((string(name) + string("TemporaryCanvas") + string(strid)).c_str(), (string(name) + string(" ") + string(title) + string(" - rootntuple")).c_str(), -1, -1, canvas_wide, canvas_high);
      myCanvas.insert(self->canvas);

      if (myThread == NULL) {
	 myThread = new TThread(OneSecondLoop, NULL);
	 myThread->Run();
      }
   }
   else {
      self->canvas->SetWindowSize(canvas_wide, canvas_high);
      self->canvas->Resize();
   }
   self->canvas->cd();

   switch (self->which) {
      case 0:
	 self->hist->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 break;
      case 1:
	 self->prof->Draw(options);
	 self->canvas->SetLogy(self->log == Py_True ? 1 : 0);
	 break;
      case 2:
	 self->hist2->Draw(options);
	 self->canvas->SetLogy(0);
	 self->canvas->SetLogz(self->log == Py_True ? 1 : 0);
	 break;
   }
   self->canvas->Update();

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootHist_postscript(rootntuple_RootHist* self, PyObject* args) {
   const char* name;
   if (!PyArg_ParseTuple(args, "s", &name)) {
      PyErr_SetString(PyExc_TypeError, "Filename must be a string.");
      return NULL;
   }
   if (self->canvas == NULL) {
      PyErr_SetString(PyExc_RuntimeError, "You must first create a canvas (redraw).");
      return NULL;
   }
   self->canvas->Print(name);

   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootHist_close(rootntuple_RootHist* self) {
   if (self->canvas != NULL) {
      myCanvas.erase(self->canvas);
      delete self->canvas;
      self->canvas = NULL;
   }
   Py_INCREF(Py_None);
   return Py_None;
}

static PyObject* rootntuple_RootHist_dealloc(rootntuple_RootHist* self) {
   Py_DECREF(self->canvas_size);
   Py_DECREF(self->log);
   
   if (self->canvas != NULL) {
      myCanvas.erase(self->canvas);
      delete self->canvas;
      self->canvas = NULL;
   }
   if (self->hist != NULL) delete self->hist;
   if (self->prof != NULL) delete self->prof;
   if (self->hist2 != NULL) delete self->hist2;
   if (self->options != NULL) delete [] self->options;
}

///////////////////////////////////////////////////////////////////

PyMODINIT_FUNC init_rootntuple(void) {
   PyObject *m;

   rootntuple_RootDirType.tp_new = PyType_GenericNew;
   if (PyType_Ready(&rootntuple_RootDirType) < 0) return;

   rootntuple_RootNtupleEntryType.tp_new = PyType_GenericNew;
   if (PyType_Ready(&rootntuple_RootNtupleEntryType) < 0) return;
 
   rootntuple_RootNtupleType.tp_new = PyType_GenericNew;
   if (PyType_Ready(&rootntuple_RootNtupleType) < 0) return;

   rootntuple_RootHistType.tp_new = PyType_GenericNew;
   if (PyType_Ready(&rootntuple_RootHistType) < 0) return;

   m = Py_InitModule3("_rootntuple", module_methods, "ROOT Ntuple access.");

   Py_INCREF(&rootntuple_RootDirType);
   PyModule_AddObject(m, "RootDir", (PyObject*)(&rootntuple_RootDirType));

   Py_INCREF(&rootntuple_RootNtupleEntryType);
   PyModule_AddObject(m, "RootNtupleEntry", (PyObject*)(&rootntuple_RootNtupleEntryType));

   Py_INCREF(&rootntuple_RootNtupleType);
   PyModule_AddObject(m, "RootNtuple", (PyObject*)(&rootntuple_RootNtupleType));

   Py_INCREF(&rootntuple_RootHistType);
   PyModule_AddObject(m, "RootHist", (PyObject*)(&rootntuple_RootHistType));

   int argc = 0;
   char **argv = NULL;
   new TApplication("ROOT TApplication", &argc, argv);
   myThread = NULL;
}

///////////////////////////////////////////////////////////////////
