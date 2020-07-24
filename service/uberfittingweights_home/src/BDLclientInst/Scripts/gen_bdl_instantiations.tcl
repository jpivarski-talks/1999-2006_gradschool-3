#!/usr/local/bin/tclsh 
#=======================================================================
# Program: gen_bdl_instantiations
#
# Purpose: generate all the files needed to instantiation the
#          templates used to extract constants from the Frame
#
# How to Use: the script takes two argument.
#             First: directory to look for its files
#             Second:  a file containing a table of constants names, 
#             stream that holds the constant, and optionally the file that
#             must be included to find the stream definition.  If the first 
#             line of the file begins with '#' then that line is ignored
#
# Author: Chris Jones
#=======================================================================

#----------------------------------------------------------------
#
# Configuration variables
#
#----------------------------------------------------------------

# the skeleton file used to build all the instantiate files
set instSkelFileName "T_constname.cc.skel"

# the directory that contains the instantiation files
set instDirName "Instantiate"
 

#----------------------------------------------------------------
#
# Set up a table that is filled with data we have obtained from
#  our input file
#
#----------------------------------------------------------------

# open directory
set directory [ lindex $argv 0 ]

set instSkelFileName "$directory/Scripts/$instSkelFileName"
set instDirName "$directory/$instDirName"

# open the file that contains the table
set tableFile [ open [ lindex $argv 1 ] r ]

# bring in our parsing routines and global variables
source "$env(USER_SRC)/BDLclientInst/Scripts/parse_file.tcl"

# parse the file and put the info into a list
set table [ parse_file $tableFile ]

close $tableFile

# Now that we have our data organized, we need to generate code

#----------------------------------------------------------------
#
# create all the instantiation files
#
#----------------------------------------------------------------
foreach constInfo $table {
    set constName [lindex $constInfo $tableColumnConstName]
    set streamName [lindex $constInfo $tableColumnStreamName]
    set streamFile [lindex $constInfo $tableColumnStreamFile]
    set seqPre ""
    if [ string match "non" [lindex $constInfo $tableColumnSeq] ] {
	set seqPre "nonseq"
    }

    set instSkelFile [open $instSkelFileName r]
    set instFile [open "$instDirName/T_$constName.cc" w]
    while { [ gets $instSkelFile line ] >= 0} {
	regsub -all "_streamfile_" $line $streamFile line
	regsub -all "_constname_" $line $constName line 
	regsub -all "_streamname_" $line $streamName line
	regsub -all "_seqpre_" $line $seqPre line
	puts $instFile $line
    }
    close $instSkelFile
    close $instFile
}
