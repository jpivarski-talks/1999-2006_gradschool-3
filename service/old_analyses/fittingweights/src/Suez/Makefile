#=============================================================================
#
# Purpose: Build a standalone executable
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
#       this file to see which symbols add to the makefile symbols. For
#       example if you just want to trace the calling routines of the function
#       'myfunc_' just once then you can issue the command
#                gmake XTRA_FFLAGS="-Wl,ymyfunc_"
#       and that option will be added to USER_FFLAGS.
#
#=============================================================================
SHELL := /bin/sh
USER_LINK := CXX
#
#----------------------------
# name of job and executable
#----------------------------

ifeq "$(JOB)" ""
  WHERE := $(shell pwd)
  JOB := $(notdir $(WHERE))
endif
EXE = ../bin/suez.exe

#----------------------------------------
# List of subdirectories containing code
#----------------------------------------

DIR_LIST := 	. \
		Class \
		Instantiate \
		$(XTRA_DIR)

#---------------------
# Set up CLEO library
#---------------------

CLEO3LIB :=	$(C3_LIB)/lib
OTHRLIB   =     $(C3_OTHER)/lib
CLEOLIB  :=     $(C_LIB)/lib
CERN3LIB :=	$(CERN3_LIB)/lib
SYSLIB	  =	$(GCC_DIR)/lib \
		$(TCL_DIR)/lib $(TK_DIR)/lib \
		/usr/local/lib

#------------------------------------
# USER, CLEO and CERN object modules
#------------------------------------

# SuezMain, UserApplication, etc. are now picked up from libSuez.a, 
# no need to explicitly link to the .o file!
#USER_OBJS :=	$(XTRA_OBJS)  UserApplication.o
USER_OBJS :=	$(XTRA_OBJS)
CLEO3_OBJS :=
OTHR_OBJS :=	
CLEO_OBJS :=	
CERN_OBJS :=

#-------------------------------------------------------
# list of include directories
# USER_INCS should be defined with respect to USER_SRC
# CLEO3_INCS should be set to C3_INC; $USER_SRC/include is implicitly included
# OTHR_INCS should be defined with respect to C3_OTHER
# CLEO_INCS should be defined with respect to C_CVSSRC
# (don't forget to leave the "." in the CLEO_INCS list)
#-------------------------------------------------------

USER_INCS :=	$(XTRA_INCS) .
CLEO3_INCS:=	$(C3_INC)
OTHR_INCS :=	.
CLEO_INCS := 	. 

#--------------------------------------------------------------
# extra fortran flags (standard and machine flags added later)
#--------------------------------------------------------------

USER_FFLAGS :=	$(XTRA_FFLAGS)

#---------------------------------------
# USER, CLEO and CERN libraries to load
#---------------------------------------
# only USER_LIBS get DEBUG flag!

# link with Suez, JobControl in USER_LIBS to pick up debug flag properly
USER_LIBS :=	$(XTRA_LIBS) \
		Suez \
		JobControl

CLEO3_LIBS :=	Processor \
		Interpreter \
		DefaultModule \
		CommandPattern \
		DynamicLoader \
		InteractiveDelivery \
		BlankRecordDelivery \
		DataStorage \
		DataDelivery \
		StorageManagement \
		DataHandler \
		DummyHistogram \
		HistogramInterface \
		ToolBox \
		Utility \
		Signal

#OTHR_LIBS :=	CLHEP
OTHR_LIBS :=
OTHR_LIBS +=	history_cleo3 readline_cleo3
CLEO_LIBS :=
#CERN_LIBS :=	packlib
CERN_LIBS :=

SYS_LIBS  =     tk$(TK_VERS) \
                tcl$(TCL_VERS) \
                X11 \
		curses

#-------------------------------------------------
# list of undefined symbols specified by the user
#-------------------------------------------------

# help gdb find some global symbols
USER_SYMS := procsel prodsel modsel

#--------------------------------------------------------------
# extra compiler/linker flags
#--------------------------------------------------------------

USER_FFLAGS     := $(XTRA_FFLAGS)
USER_CFLAGS     := $(XTRA_CFLAGS)
USER_CXXFLAGS   := $(XTRA_CXXFLAGS)

USER_LFLAGS	:=

#---------------------------------------------
# #define for cpp, define SELECT symbols here
#---------------------------------------------

USER_DEFS :=    $(XTRA_DEFS)

#--------------------------------------------------------------
# invoke standard tail to Makefile to execute compile and link
#--------------------------------------------------------------

include $(C3_GMAKE)/M.tail

#
# $Id: Makefile,v 1.83 2000/12/01 20:33:58 cdj Exp $
#
# DO NOT DELETE

