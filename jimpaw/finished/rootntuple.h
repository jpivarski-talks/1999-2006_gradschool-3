#include <Python.h>
#include <structmember.h>
#include <vector>
#include <string>
#include <set>

#include "TROOT.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TThread.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TIterator.h"
#include "TKey.h"
#include "TNtuple.h"
#include "TLeafF.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TH2F.h"

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

typedef struct {
      PyObject_HEAD
      TDirectory* d;
      TFile* f;
} rootntuple_RootDir;

typedef struct {
      PyObject_HEAD

      TFile* file;
      TNtuple* ntuple;
      bool isdouble;

      long i;
      long start;
      long stop;
      long step;
      PyObject* entry;
      PyObject* optimize;

      PyObject* filter;
      char** filtervars;
      int filtervars_size;

      PyObject* canvas_size;
      TCanvas* canvas;
      int which;
      TH1F* hist;
      TProfile* prof;
      TH2F* hist2;
      char* options;
      TGraph* graph;

      double leftloss;
      double rightloss;
      int sample;
      double padding;
      PyObject* frequency;
      PyObject* log;
      PyObject* display;
} rootntuple_RootNtuple;

typedef struct {
      PyObject_HEAD
      rootntuple_RootNtuple* master;
      double weight;
} rootntuple_RootNtupleEntry;

typedef struct {
      PyObject_HEAD

      PyObject* canvas_size;
      TCanvas* canvas;
      int which;
      TH1F* hist;
      TProfile* prof;
      TH2F* hist2;
      char* options;

      PyObject* log;
} rootntuple_RootHist;

// RootDir methods
static int rootntuple_RootDir_init(rootntuple_RootDir* self, PyObject* args);
static PyObject* rootntuple_RootDir_ls(rootntuple_RootDir* self);
static PyObject* rootntuple_RootDir_list(rootntuple_RootDir* self);
static PyObject* rootntuple_RootDir_get(rootntuple_RootDir* self, PyObject* args);
static PyObject* rootntuple_RootDir_path(rootntuple_RootDir* self);
// static PyObject* rootntuple_RootDir_getpath(rootntuple_RootDir* self, PyObject* args);
static PyObject* rootntuple_RootDir_closefile(rootntuple_RootDir* self);

// RootNtupleEntry methods
static int rootntuple_RootNtupleEntry_init(rootntuple_RootNtupleEntry* self, PyObject* args);
static PyObject* rootntuple_RootNtupleEntry_getattr(PyObject* thyself, char* name);

// RootNtuple methods
static int rootntuple_RootNtuple_init(rootntuple_RootNtuple* self, PyObject* args);
static PyObject* rootntuple_RootNtuple_range(rootntuple_RootNtuple* self, PyObject* args);
static PyObject* rootntuple_RootNtuple_labels(rootntuple_RootNtuple* self);
static PyObject* rootntuple_RootNtuple_allentries(rootntuple_RootNtuple* self);
static PyObject* rootntuple_RootNtuple_entries(rootntuple_RootNtuple* self);
static PyObject* rootntuple_RootNtuple_count(rootntuple_RootNtuple* self);
static PyObject* rootntuple_RootNtuple_bounds(rootntuple_RootNtuple* self, PyObject* args);
// static PyObject* rootntuple_RootNtuple_loadbaskets(rootntuple_RootNtuple* self, PyObject* args);
static PyObject* rootntuple_RootNtuple_iter(PyObject* thyself);
static bool rootntuple_RootNtuple_zipahead(rootntuple_RootNtuple* self, double* weight, bool* warned = NULL);
static PyObject* rootntuple_RootNtuple_next(PyObject* thyself);
static PyObject* rootntuple_RootNtuple_entry(rootntuple_RootNtuple* self, PyObject* args);
static PyObject* rootntuple_RootNtuple_map(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static bool rootntuple_RootNtuple_setupfunction(char*** mapvars, int* mapvars_size, rootntuple_RootNtuple* self, PyObject* function);
static bool rootntuple_RootNtuple_callfunc(float* res, rootntuple_RootNtuple* self, PyObject* function, char** mapvars, int mapvars_size);
static PyObject* rootntuple_RootNtuple_plot(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static PyObject* rootntuple_RootNtuple_plotbin(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static PyObject* rootntuple_RootNtuple_plotint(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static bool rootntuple_RootNtuple_setupfunction2(char*** mapvars, int* mapvars_size, rootntuple_RootNtuple* self, PyObject* function);
static bool rootntuple_RootNtuple_callfunc2(float* res1, float* res2, rootntuple_RootNtuple* self, PyObject* function, char** mapvars, int mapvars_size);
static PyObject* rootntuple_RootNtuple_profile(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static PyObject* rootntuple_RootNtuple_profilebin(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static PyObject* rootntuple_RootNtuple_profileint(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static PyObject* rootntuple_RootNtuple_plot2(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static PyObject* rootntuple_RootNtuple_plot2bin(rootntuple_RootNtuple* self, PyObject* args, PyObject* kwds);
static PyObject* rootntuple_RootNtuple_redraw(rootntuple_RootNtuple* self, PyObject* args);
static PyObject* rootntuple_RootNtuple_postscript(rootntuple_RootNtuple* self, PyObject* args);
static PyObject* rootntuple_RootNtuple_close(rootntuple_RootNtuple* self);
static PyObject* rootntuple_RootNtuple_hist(rootntuple_RootNtuple* self, PyObject* args);
static PyObject* rootntuple_RootNtuple_dealloc(rootntuple_RootNtuple* self);

// RootHist methods
static int rootntuple_RootHist_init(rootntuple_RootHist* self, PyObject* args);
static PyObject* rootntuple_RootHist_mean(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_rms(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_entries(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_ouvalues(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_ouerrors(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_bins(rootntuple_RootHist* self, PyObject* args);
static PyObject* rootntuple_RootHist_values(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_errors(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_redraw(rootntuple_RootHist* self, PyObject* args);
static PyObject* rootntuple_RootHist_postscript(rootntuple_RootHist* self, PyObject* args);
static PyObject* rootntuple_RootHist_close(rootntuple_RootHist* self);
static PyObject* rootntuple_RootHist_dealloc(rootntuple_RootHist* self);
