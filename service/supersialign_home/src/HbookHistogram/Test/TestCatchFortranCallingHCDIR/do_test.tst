#!/usr/local/bin/tclsh 
#=======================================================================
# Program: do_test.tst
#
# Purpose: check that 
#
#
# Author: Chris Jones
#=======================================================================


#  look for a line of '---'s which designates a start of a test
puts "-------------------"
puts "test: catch fortran calling hcdir"
set results ""
if [ catch {
    set results [exec test.exe] 
    #puts $results
} caught_result ] {
    set results $caught_result
}

#puts $results

set start [string first "EMERGENCY" $results]

if {$start == -1} {
    puts "directory change catcher broken"
    return
}

set start [string first "HBook directory is incorrect \"//PAWC" $results ]
if {$start == -1} {
    puts "found unknown EMERGENCY: broken"
    return
}

puts "directory change catcher OK"
return

