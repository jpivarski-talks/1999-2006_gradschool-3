#=======================================================================
# Program: parse_file.tcl
#
# Purpose: parse a constants info file and create a list of its data
#
# How to Use: source this script in another tcl job
#             input:
#                put an open file in the variable 'tableFile'
#             ouput:
#                puts a list of the data in variable 'table'
#
# Author: Chris Jones
#=======================================================================

#
# Variables used to describe how the data is organized in the file
#
set tableColumnConstName   0
set tableColumnSeq         1
set tableColumnOnOff       2
set tableColumnStreamName  3
set tableColumnStreamFile  4

proc parse_file { tableFile } {

global tableColumnConstName tableColumnSeq tableColumnOnOff tableColumnStreamName tableColumnStreamFile
#----------------------------------------------------------------
#
# Configuration variables
#
#----------------------------------------------------------------

# Default stream include file
    set defaultStreamIncFile "DataHandler/Stream.h"
    
#----------------------------------------------------------------
#
# Set up a table that is filled with data we have obtained from
#  our input file
#
#----------------------------------------------------------------

# loop over each line in the tableFile and search for table entries
#  and places those entries in table
    set table [list]
    while { [gets $tableFile line ] >= 0 } {
#
	if [ string match "\#*" $line ] {
#         this line is a comment, ignore it
	} else {
	    set tableRow ""
	    set tempRow [ split $line]
#           remove any empty lists created by extra spaces between table
#           entries
	    foreach entry $tempRow {
		if { 0 < [llength $entry] } {
		    lappend tableRow $entry
		}
	    }

	    if { $tableColumnStreamFile == [llength $tableRow] } {
		lappend tableRow $defaultStreamIncFile
	    }

#           only add to table if list is not empty
	    if { 0 < [llength $tableRow] } {
		lappend table $tableRow
	    }
	}
#this is useful for debugging
    }
    puts $table
    return $table
}
