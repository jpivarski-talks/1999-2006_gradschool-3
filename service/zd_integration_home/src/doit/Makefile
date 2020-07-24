#=============================================================================
#
# Purpose: Build a user library with name doit_pro_nozfiles
#       THIS MAKES THE PRODUCTION VERSION OF DOIT
#
# How to use:
#
#   1.) Copy this file to your build area. (Please don't build code in your
#       source area!)
#
#   2.) This makefile includes "M.tail", which contains all the "meat".
#       (suffix rules, platform dependencies etc.)
#
#   3.) Dependencies (ala makedepend) are done via a ".depend" file 
#       which gets created and include by "M.tail".
#
#   4.) Make sure the environment variable USER_SRC is set to the
#       directory *above* the source code:
#         e.g. "setenv USER_SRC /home/mkl/work/cleo3/cvssrc"
#
#   5.) To compile the library, do"
#         unix> gmake CXX=<compiler>
#         e.g. "gmake CXX=cxx"
#
#   6.) The library will appear by default as "../lib/lib<directory>.a", 
#
#   7.) To compile and link in debug mode, use "DEBUG=y":
#          e.g. "gmake CXX=cxx DEBUG=y"
#        This will result in an appended "_g" to the library name.
#
#   8.) You can OVERRIDE any symbol used in a makefile by specifying the new
#       symbol on the command line, for example, 
#          e.g. "gmake USER_OBJS=myfunc.o"
#       Please remember that this will OVERRIDE any value specified in the 
#       Makefile. If you just which to add extra information to the current
#       makefile value the 'XTRA_????' symbols have been created. Look through
#       this file to see which symbols add to the makefile symbols. 
#
#=============================================================================
SHELL := /bin/sh
USER_LINK := CXX
#
#----------------------------
# name of job and executable
#----------------------------

ifeq "$(JOB)" ""
  WHERE := $(shell pawd)
  JOB := $(notdir $(WHERE))
endif
EXE = 

#----------------------------------------
# List of subdirectories containing code
#----------------------------------------

# A list of the base doit libraries (no diagonstics - these are added
#  by the doityomama make file -- note that diagdoit has
# some routines that are used by regular doit)
DOIT_DIRS := 	cdtrack cf chargdiv control \
		diagdoit \
		dukdef duklseq duseq eventcontrol extendtrak \
		fitter geomcalc hitcorrct hitlist hitsontrak index \
		initializ kuip multibunch namelist \
		process runfil seedtrack sf sf_data_access sfseq svd \
		trakstak ttab utility vfind cleoks_util

DIR_LIST :=	$(DOIT_DIRS) $(XTRA_DIR)

#		diagdoit diaghbook \


#-------------------------------------------------------
# list of include directories:
#   $USER_SRC/include is implicitly included!
#   USER_INCS should be defined with respect to USER_SRC
#   CLEO3_INCS should be defined with respect to C3_INC 
#   CLEO_INCS should be defined with respect to C_CVSSRC
#   CERN_INCS should be defined with respect to CERNSRC
#   COMM_INCS should be defined with respect to COMM_INC
#   OTHR_INCS should be fully specified!
# Only uncommented settings are used!
#-------------------------------------------------------

USER_INCS :=	$(XTRA_INCS) cdoffcal Klmn3Fort .
CLEO3_INCS:=	.
#CLEO_INCS := 	. 
#CERN_INCS :=	.
#COMM_INCS =	$(CONSTANTS_INCS)
#OTHR_INCS :=	$(C3_OTHER)

#--------------------------------------------------------------
# extra compiler flags
#--------------------------------------------------------------

USER_FFLAGS   := $(XTRA_FFLAGS)
USER_CFLAGS   := $(XTRA_CFLAGS)
USER_CXXFLAGS := $(XTRA_CXXFLAGS)

#---------------------------------------------
# #define for cpp, define SELECT symbols here
#---------------------------------------------

USER_DEFS := -DCLEO_CLEO3 -DCLEO_NOCOMDOC -DCLEO_SFDUET \
	     -DCLEO_SFLLIS \
	     -DCLEO_SFZFIT \
	     -DCLEO_SFNOZFILES \
	     $(XTRA_DEFS)

#             -DCLEO_DUFIND_HIT_DUMP \
#             -DCLEO_DUFIND_ANALYSIS \


ifeq "$(OS_NAME)" "SunOS"
USER_DEFS += -DCLEO_SUNOS	
endif

#--------------------------------------------------------------
# invoke standard tail to Makefile to execute compile and link
#--------------------------------------------------------------


include $(C3_GMAKE)/M.tail

# --------------------------------------------------------------------
# if you need something special done for a particular file, put here:
#
#$(LIBRARY)(special_file.o) : $(USER_SRC)/<Library>/Class/special_file.cc
#	$(CXX) $(CXXFLAGS) $(GUNG_FIX) $(CLEO_DEFS) $(LIST_INCS) $(CXX_INCS) -c $<
#	@$(AR) r $@ $%
#	@$(RM) $%
# --------------------------------------------------------------------

# $Id: Makefile,v 1.7 2003/03/05 18:40:28 dpp Exp $
#
# DO NOT DELETE
