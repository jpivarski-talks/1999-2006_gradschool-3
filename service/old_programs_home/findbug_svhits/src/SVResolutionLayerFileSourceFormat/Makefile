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
SHARMODNAME := $(JOB)

#----------------------------------------
# List of subdirectories containing code
#----------------------------------------

DIR_LIST :=	. Class Instantiate $(XTRA_DIR)

#---------------------
# Set up CLEO library
#---------------------

CLEO3LIB 	:= $(C3_LIB)/lib
OTHRLIB 	:= $(C3_OTHER)/lib
CLEOLIB		:= $(C_LIB)/lib
CERN3LIB	:=
COMMLIB         := $(COMM_LIB)/lib
SYSLIB		:=

#------------------------------------
# USER, CLEO and CERN object modules
#------------------------------------

USER_OBJS :=	$(XTRA_OBJS)
OTHR_OBJS :=	
CLEO_OBJS :=	
CERN_OBJS :=	

#-------------------------------------------------------
# list of include directories
# USER_INCS should be defined with respect to USER_SRC
# OTHR_INCS should be defined with respect to C3_OTHER
# CLEO_INCS should be defined with respect to C_CVSSRC
# (don't forget to leave the "." in the CLEO_INCS list)
#-------------------------------------------------------

USER_INCS  :=	$(XTRA_INCS) . 
CLEO3_INCS :=	$(C3_INC)
OTHR_INCS  :=	.
#CLEO_INCS  :=	.

COMM_INCS = $(CONSTANTS_INCS)

#---------------------------------------
# USER, CLEO and CERN libraries to load
#---------------------------------------

USER_LIBS :=
OTHR_LIBS :=	
CLEO3_LIBS:=	DataDelivery \
		DataHandler \
		ToolBox Utility

CLEO_LIBS :=	CLHEP
#CERN_LIBS  :=
COMM_LIBS = $(CONSTANTS_LIBS)

#-------------------------------------------------
# list of undefined symbols
#-------------------------------------------------

USER_SYMS :=	$(XTRA_SYMS)

#--------------------------------------------------------------
# extra compiler/linker flags
#--------------------------------------------------------------

USER_FFLAGS     := $(XTRA_FFLAGS)
USER_CFLAGS     := $(XTRA_CFLAGS)
USER_CXXFLAGS   := $(XTRA_CXXFLAGS)


USER_LFLAGS     := $(XTRA_LFLAGS)

#---------------------------------------------
# #define for cpp, define SELECT symbols here
#---------------------------------------------

USER_DEFS :=    $(XTRA_DEFS)

#--------------------------------------------------------------
# invoke standard tail to Makefile to execute compile and link
#--------------------------------------------------------------

#uncomment the following if you want to add a test constant file
# to the official data directory
#--------------------------------------------------------------
# Copy test.svresolutionlayer from main directory to $(C3_DATA) directory
#--------------------------------------------------------------
#
#ifeq "$(C3_DATA)" ""
#C3_DATA := $(C3_LIB)/data
#endif
#
#default: 
#production : $(C3_DATA)/test.svresolutionlayer
#clean : clean_text_constants
#clean_text_constants :
#	rm -f $(C3_DATA)/test.svresolutionlayer
#
#$(C3_DATA)/test.svresolutionlayer : $(USER_SRC)/$(JOB)/test.svresolutionlayer
#	cp $< $@ 

include $(C3_GMAKE)/M.tail

#
# $Id: Makefile,v 1.1.1.1 2001/05/22 16:30:01 mccann Exp $
#
# DO NOT DELETE
