#!/usr/local/bin/tclsh 
#=======================================================================
# Program: run_tests.tcl
#
# Purpose: run the tests on hbook and check to make sure everything is
#          fine
#
#
# Author: Chris Jones
#=======================================================================

set results [exec gmake -f $env(USER_SRC)/HbookHistogram/Makefile test] 
#puts $results

#  look for a line of '---'s which designates a start of a test

set start [string first "---" $results]

if {$start == -1} {
    puts "ERROR: No tests run"
    return
}

set test_results [string range $results $start end]
#puts $test_results


# loop over the lines and check for errors
set test_result_lines [split $test_results "\n"]

#puts $test_result_lines

set test_start_index -1
set index 0

set true 1
set false 0

set failed_test_indicies [list]
foreach line $test_result_lines {

#is this the start of a new test?
    if [regexp {\-\-\-+} $line] {
	set test_start_index $index
    } else {
	if [regexp broken $line] {
	    lappend failed_test_indicies $test_start_index
	}
    }
    incr index
}

#lappend failed_test_indicies 0

if { 0 != [llength $failed_test_indicies]}  {

    set next_broken_index 0
    set index 0
    foreach line $test_result_lines {
	puts $line
	if { $index == [lindex $failed_test_indicies $next_broken_index ] } {
	    puts "ERROR: test failed"
	}
	incr index
    }
} else {
    puts "tests passed"
}

