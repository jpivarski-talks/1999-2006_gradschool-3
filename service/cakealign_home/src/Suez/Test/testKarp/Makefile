#=============================================================================
#
# Purpose: Link a user program written to use SCAFFOLD
#
# Note: *This is a prototype!* It follows the usual Makefile layout but
#	You need to make sure you have the environment variables SCAF_SRC
#       and SCAF_LIB set to the correct areas.  On the Wilson cluster
#       this can be done by:
#           export SCAF_SRC=/home/axp/sjp/cxx/src
#           export SCAF_LIB=/home/axp/sjp/cxx      ( for bash/ksh users )
#
#           setenv SCAF_SRC=/home/axp/sjp/cxx/src
#           setenv SCAF_LIB=/home/axp/sjp/cxx      ( for csh/tcsh users )
#
# For the User:
# -------------
#   Here is a quick crib on how to use this file. It is assumed you have
#   a directory 'build' on the machine you wish to compile on. Also it
#   is assumed all your source code exists in and below one
#   directory. This directory is assumed to be $USER_SRC/<name>.
#       (a) Copy this file into the directory build/<name> and name your
#           copy of this file 'Makefile'.
#           (Provided the environment variable JOB is not set, the file
#           will pick up the job <name> from the directory. If you want
#           to build an executable with a different <name> then add
#           'JOB=<new_name>' to the gmake command line. See note below
#           about command line overrides.)
#       (b) Make sure you set USER_OBJS to be that name of the object
#           module that contains the PROGRAM or main() code. This means
#           it will not be deleted after it has been put into the
#           library and will be used to build the executable.
#       (c) If you have any of your own extra libraries add them to the
#           USER_LIBS definition and any include area should be added to
#           USER_INCS. The user libraries should be located in either
#           the build/lib or ~/lib directories.
#       (d) If you have code stored in subdirectories below the <name>
#           directory include the subdirectories in the variable DIR_LIST.
#       (e) Check that environment variable USER_SRC is set to the
#           directory *above* the source code. Note: this can be an NFSD
#           mounted directory, for instance on the AXP/OSF USER_SRC
#           might be defined as
#                /home/ds/soft/src
#           and the source code and include files are stored in and
#           below the directory
#                          /home/ds/soft/src/<name>
#           (This means you do not have to move the source between platforms)
#       (f) Execute the command:
#                gmake
#
#   The executable should appear in build/bin/<name> and any code compiled
#   will be in build/lib/lib<name>.a
#
#   To compile and link a debug version execute the line:
#                gmake DEBUG=y
#     this will create a debug version of any users code in
#     build/lib/lib<name>_g.a and link to user debug libraries.
#
#   Once you have run 'gmake' once, if you all you do is edit your
#     source code then you can get quicker performance by using the
#     command line:
#                gmake production
#     However if you have added or removed files, changed any file
#     dependencies or edited the Makefile itself then you need to issue
#     the 'gmake' command on its own (see step (f)).
#
#   You can override any symbol used in a makefile by specifying the new
#     symbol on the 'gmake' command line, for example
#                gmake USER_OBJS=myfunc.o
#     It is important to remember that this approach OVERRIDES the makefile
#     value. IF you just which to add extra information to the current
#     makefile value the 'XTRA_????' symbols have been created. Look through
#     this file to see which symbols add to the makefile symbols.
#
#=============================================================================
SHELL := /bin/sh
CXX := g++
USER_LINK := CXX
#
#----------------------------
# name of job and executable
#----------------------------

ifeq "$(JOB)" ""
  WHERE := $(shell pwd)
  JOB := $(notdir $(WHERE))
endif
EXE := ../bin/$(JOB)

#----------------------------------------
# List of subdirectories containing code
#----------------------------------------

DIR_LIST :=	../Class $(XTRA_DIR)

#---------------------
# Set up CLEO library
#---------------------

CLEOLIB :=	$(C_LIB)/lib
OTHRLIB :=	/nile02/tmp1/hepvis/shared_libs/g++

#------------------------------------
# USER, CLEO and CERN object modules
#------------------------------------

USER_OBJS :=	test.o \
		$(XTRA_OBJS)

OTHR_OBJS :=	

CLEO_OBJS :=	

CERN_OBJS :=	

#-------------------------------------------------------
# list of include directories
# USER_INCS should be defined with respect to USER_SRC
# CLEO_INCS should be defined with respect to C_CVSSRC
# (don't forget to leave the "." in the CLEO_INCS list)
#-------------------------------------------------------

USER_INCS :=	. $(XTRA_INCS)

OTHR_INCS :=	$(C3_INC)

CLEO_INCS :=	.

#---------------------------------------
# USER, CLEO and CERN libraries to load
#---------------------------------------

#USER_LIBS :=	KarpHandler 	DriverHandler 	DataHandler	$(XTRA_LIBS)
USER_LIBS :=	DataHandler	$(XTRA_LIBS)

#OTHR_LIBS :=	DataStream	Switchboard	DataBase
OTHR_LIBS :=	ToolBox 	Utility


CLEO_LIBS :=	ftknlib	lsqfit	ceid	cdcc	qqlib	evcl \
		ccc	track

NILE_LIBS := $(shell test -e $(C3_GMAKE)/nile.libs && \
		cat $(C3_GMAKE)/nile.libs)

CERN_LIBS :=	packlib

#-------------------------------------------------
# list of undefined symbols specified by the user
#-------------------------------------------------

USER_SYMS :=	clrwpk_ $(XTRA_SYMS)

#--------------------------------------------------------
# force loading of these routines from DVLIB not DVDUMMY
#--------------------------------------------------------

DRIVER_SYMS :=	dvluns_	dvgrun_ rpcmd_

#-----------------------------------
# make up list of undefined symbols
#-----------------------------------

CLEO_SYMS :=	$(USER_SYMS)	$(DRIVER_SYMS)

#--------------------------------------------------------------
# extra fortran flags (standard and machine flags added later)
#--------------------------------------------------------------

ifeq "$(strip $(CLIB))" ""
  CLIB:=clib
endif
USER_FFLAGS :=	$(XTRA_FFLAGS)

#---------------------------------------------
# #define for cpp, define SELECT symbols here
#---------------------------------------------

USER_DEFS :=	-D$(CLIB)=$(CLIB)	-DSJP_STL	$(XTRA_DEFS)

#--------------------------------------------------------------
# invoke standard tail to Makefile to execute compile and link
#--------------------------------------------------------------

include $(C3_GMAKE)/M.tail

#
# $Id: Makefile,v 1.2 1998/06/26 14:03:27 cleo3 Exp $
#
# DO NOT DELETE
