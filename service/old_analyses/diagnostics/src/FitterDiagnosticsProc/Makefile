#=============================================================================
#
# Purpose: Link a Processor/Producer
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
#   5.) To link a shared library, do"
#         unix> gmake CXX=<compiler> shared_module
#         e.g. "gmake CXX=cxx shared_module"
#
#   6.) The library will appear by default as "../lib/lib<directory>.a", 
#       and the linked shared library as "../shlib/<directory>.so".
#       If you want the shared library name to have a different name,
#       specify that name with "SHARMODNAME" below.
#
#   7.) To compile and link in debug mode, use "DEBUG=y":
#          e.g. "gmake CXX=cxx DEBUG=y shared_module"
#        This will result in an appended "_g" to library and 
#       shared library names.
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

# if you want your shared processor to have a distinctive name other
# than "<LIBNAME>.so", specify it here. E.g. MyLibProd
SHARMODNAME := FitDiagProc

#----------------------------------------
# List of subdirectories containing code
#----------------------------------------

DIR_LIST :=	. Class Instantiate $(XTRA_DIR)

#---------------------
# Set up CLEO library
#---------------------

CLEO3LIB	:= $(C3_LIB)/lib
OTHRLIB		:= $(C3_OTHER)/lib
CLEOLIB		:= $(C_LIB)/lib
CERN3LIB 	:= $(CERN3_LIB)/lib
COMMLIB   	:= $(COMM_LIB)/lib
SYSLIB   	:=

#------------------------------------
# USER, CLEO and CERN object modules
#------------------------------------

USER_OBJS :=	$(XTRA_OBJS)
CLEO3_OBJS :=	
OTHR_OBJS :=	
CLEO_OBJS :=	
CERN_OBJS :=	
COMM_OBJS :=	

#-------------------------------------------------------
# list of include directories
# USER_INCS should be defined with respect to USER_SRC
# CLEO3_INCS should be set to C3_INC; $USER_SRC/include is implicitly included
# OTHR_INCS should be defined with respect to C3_OTHER
# CLEO_INCS should be defined with respect to C_CVSSRC
# CERN_INCS should be defined with respect to CERNSRC
# (don't forget to leave the "." in the CLEO_INCS list)
#-------------------------------------------------------

USER_INCS  :=	$(XTRA_INCS) . 
CLEO3_INCS :=	$(C3_INC)
OTHR_INCS  :=	.
CLEO_INCS  :=	.
CERN_INCS  :=	.
COMM_INCS  :=	$(COMM_INC)

#---------------------------------------
# USER, CLEO and CERN libraries to load
#---------------------------------------

USER_LIBS  :=
#CLEO3_LIBS :=   ProxyBind \
#		Processor \
#		TrackDelivery \
#                CommandPattern \
#		NavigationProd \
#		Navigation \
#		FastMCProcessor \
#		FastMCTaggerProc \
#		HelixIntersection \
#		CalibratedData \
#		TrackFinder \
#		TrackRoot \
#		KinematicTrajectory \
#                CleoDB \
#		DataHandler \
#		ProxyDict \
#		ToolBox \
#		Utility

# ReconTracks C3trProducer C3tr 
CLEO3_LIBS :=   Processor \
		CommandPattern \
		FastMCProcessor FastMCTaggerProc \
		KalmanFilter \
		HelixIntersection \
		Navigation \
		TrackShowerMatching \
		TrackFitter TrackFinder \
		SiHits ASiStorePro SiGeom \
		ADRGeom DRGeom ActiveElements CDOffCal \
		CalibratedData \
		TrackDelivery TrackRoot \
		DetectorGeometry KinematicTrajectory  \
		C3cc MagField BeamEnergy \
                CleoDB \
		ProxyBind \
		DataHandler \
		ToolBox \
		Utility 

OTHR_LIBS  :=	CLHEP
CLEO_LIBS  :=
CERN_LIBS  :=
CERN_LIBS  := mathlib packlib 
COMM_LIBS  :=
SYS_LIBS   :=

#-------------------------------------------------
# list of undefined symbols
#-------------------------------------------------

USER_SYMS 	:= $(XTRA_SYMS)

#--------------------------------------------------------------
# extra compiler/linker flags
#--------------------------------------------------------------

USER_FFLAGS     := $(XTRA_FFLAGS)
USER_CFLAGS     := $(XTRA_CFLAGS)
USER_CXXFLAGS   := $(XTRA_CXXFLAGS)

USER_LFLAGS	:= $(XTRA_LFLAGS)

#---------------------------------------------
# #define for cpp, define SELECT symbols here
#---------------------------------------------

USER_DEFS 	:= $(XTRA_DEFS)

#--------------------------------------------------------------
# invoke standard tail to Makefile to execute compile and link
#--------------------------------------------------------------

include $(C3_GMAKE)/M.tail

#
# $Id: Makefile,v 1.5 2002/02/26 16:53:34 wsun Exp $
#
# DO NOT DELETE

