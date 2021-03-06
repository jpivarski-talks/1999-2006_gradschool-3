# ;-*-Mode: makefile-*-
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
NOLIB :=y
#
#----------------------------
# name of job and executable
#----------------------------

ifeq "$(JOB)" ""
  WHERE := $(shell pawd)
  JOB := $(notdir $(WHERE))
endif
JOB := HbookHistogram
EXE = test.exe

#----------------------------------------
# List of subdirectories containing code
#----------------------------------------
ifeq "$(TEST_DIR)" ""
  WHERE := $(shell pawd)
  TEST_DIR := $(notdir $(WHERE))
endif

DIR_LIST := 	Test/$(TEST_DIR) $(XTRA_DIR)

#-------------------------------------------------------
# Set up libraries paths:
# (Only uncommented settings are used!)
#-------------------------------------------------------
CLEO3LIB :=	$(C3_LIB)/lib
#OTHRLIB   =     $(C3_OTHER)/lib
#CLEOLIB  :=     $(C_LIB)/lib
CERN3LIB :=	$(CERN3_LIB)/lib
#COMMLIB  :=	$(COMM_LIB)/lib
#SYSLIB	  =	$(GCC_DIR)/lib \
#		$(TCL_DIR)/lib $(TK_DIR)/lib \
#		/usr/local/lib

#------------------------------------
# USER, CLEO and CERN object modules
#------------------------------------

# SuezMain is now picked up from libSuez.a, 
# no need to explicitly link to the .o file!
USER_OBJS :=	test.o $(XTRA_OBJS)
CLEO3_OBJS:=	
OTHR_OBJS :=	
CLEO_OBJS :=	
CERN_OBJS :=	
COMM_OBJS :=	

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

USER_INCS :=	$(XTRA_INCS) .
CLEO3_INCS:=	.
#CLEO_INCS := 	. 
CERN_INCS :=	.
#COMM_INCS =	$(CONSTANTS_INCS)
#OTHR_INCS :=	$(C3_OTHER)

#---------------------------------------
# USER, CLEO and CERN libraries to load
#---------------------------------------
# only USER_LIBS get DEBUG flag!

USER_LIBS :=	$(XTRA_LIBS)

CLEO3_LIBS :=
CLEO3_LIBS :=	HbookHistogram \
		LoadFortranCommons \
		DummyHistogram \
		HistogramInterface \
		ToolBox \
		Utility

#OTHR_LIBS :=	CLHEP
#OTHR_LIBS +=
#CLEO_LIBS :=
CERN_LIBS :=	packlib

#COMM_LIBS =   $(CONSTANTS_LIBS)

#SYS_LIBS  = 

#-------------------------------------------------
# list of undefined symbols specified by the user
#-------------------------------------------------

USER_SYMS 	:= get_size_pawc_ $(XTRA_SYMS)

#--------------------------------------------------------------
# extra compiler/linker flags
#--------------------------------------------------------------

USER_FFLAGS     := $(XTRA_FFLAGS)
USER_CFLAGS     := $(XTRA_CFLAGS)
USER_CXXFLAGS   := $(XTRA_CXXFLAGS)

USER_LFLAGS	:= $(XTRA_LDFLAGS)

#---------------------------------------------
# #define for cpp, define SELECT symbols here
#---------------------------------------------

USER_DEFS 	:= $(XTRA_DEFS)

#--------------------------------------------------------------
# invoke standard tail to Makefile to execute compile and link
#--------------------------------------------------------------
dummy : default
production : ../lib
../lib :
	ln -s ../lib ../lib

include $(C3_GMAKE)/M.tail

#
# $Id: Makefile,v 1.2 2000/10/17 15:30:14 savinov Exp $
#
# DO NOT DELETE
