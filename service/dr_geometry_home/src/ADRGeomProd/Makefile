#gmake MAKEFILEDEP=n if you DON'T WANT $MAKEFILE as a dependency
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
# if you want your shared processor to have a distinctive name other
# than "<LIBNAME>.so", specify it here. E.g. MyLibProd
SHARMODNAME := ADRGeomProd

#----------------------------------------
# List of subdirectories containing code
#----------------------------------------

DIR_LIST :=	. Class Fortran Instantiate $(XTRA_DIR)

#---------------------
# Set up CLEO library
#---------------------

CLEO3LIB        := $(C3_LIB)/lib
OTHRLIB		:= $(C3_OTHER)/lib
#CLEOLIB         := $(C_LIB)/lib
#CERN3LIB 	:= $(CERN3_LIB)/lib
COMMLIB   	:= $(COMM_LIB)/lib
SYSLIB   	:=

#------------------------------------
# USER, CLEO and CERN object modules
#------------------------------------

USER_OBJS :=	$(XTRA_OBJS)
CLEO3_OBJS :=	
OTHR_OBJS :=	
#CLEO_OBJS :=	
#CERN_OBJS :=	
COMM_OBJS :=	

#-------------------------------------------------------
# list of include directories
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
CLEO3_INCS :=	$(C3_INC)
COMM_INCS =	$(CONSTANTS_INCS) 
#OTHR_INCS :=	$(C3_OTHER)

#---------------------------------------
# USER, CLEO and CERN libraries to load
#---------------------------------------

#USER_LIBS  :=   ADRGeom ActiveElements
CLEO3_LIBS :=   Processor \
                CommandPattern \
                DRGeom ADRGeom ActiveElements \
                DetectorGeometry \
		BDLclientInst \
                CleoDB  \
		DataHandler \
		Utility

OTHR_LIBS  :=	CLHEP
#CLEO_LIBS  :=
CERN_LIBS  :=
#CERN_LIBS  := packlib mathlib                    
COMM_LIBS  = $(CONSTANTS_LIBS)                    
SYS_LIBS   :=                     

#-------------------------------------------------
# list of undefined symbols
#-------------------------------------------------

USER_SYMS 	:= $(XTRA_SYMS)

#
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

USER_DEFS     := $(XTRA_DEFS)

#--------------------------------------------------------------
# invoke standard tail to Makefile to execute compile and link
#--------------------------------------------------------------


include $(C3_GMAKE)/M.tail

# DO NOT DELETE





