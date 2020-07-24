############################################################
# procedures
############################################################

######### procedure opensource  ###############
# pops the window to enter file to source

tcl_proc opensource {} {
    global bbr_fname
    toplevel .file
    
    wm title .file "Sourcing a Tcl Script"

    tcl_proc source_script filename {
        tcl_source $filename
        pack forget .file.bottom .file.entry
        .file.label configure -text "Done with sourcing"
        after 1000 destroy .file
#        openpath
    }

    frame .file.bottom
    button .file.bottom.cancel -text "Cancel" \
            -command "destroy .file"
    button .file.bottom.source -text "Source it" \
            -command {source_script $bbr_fname}
    button .file.bottom.help -text "Help" \
            -command "puts {come on, you can figure that out}"
    pack .file.bottom.help .file.bottom.cancel .file.bottom.source \
            -side left -padx 5
    pack .file.bottom -side bottom
    label .file.label -text "Filename:"
    entry .file.entry -textvariable bbr_fname
    .file.entry icursor 0
    bind .file.entry <Return> {source_script $bbr_fname}
    bind .file.entry <Double-1> {source_script $bbr_fname}
    pack .file.label .file.entry -side left
    focus .file.entry
}

######## procedure get_number  ###############
# pops a window to enter number and returns entry

tcl_proc get_number {} {
    toplevel .number
    wm title .number "How many?"
    entry .number.entry -textvariable number -width 18
    bind .number.entry <Return> \
            { eval "destroy .number" ";" "nextstop $number"} 
    pack .number.entry
    focus .number.entry
}

################################################################
# menu
################################################################


frame .menubar -relief raised -bd 2
pack .menubar  -side top -fill x
menubutton .menubar.file -text File -underline 0 -menu .menubar.file.menu
menubutton .menubar.proc -text Proc -underline 0 -menu .menubar.proc.menu
#menubutton .menubar.path -text Path -underline 0 -menu .menubar.path.menu
menubutton .menubar.stops -text Stops -underline 0 -menu .menubar.stops.menu
button .menubar.help -text help -underline 0  -command help
pack .menubar.file .menubar.proc .menubar.stops -side left   
pack .menubar.help -side right      

#focus default .

######## menuitem file

menu .menubar.file.menu

.menubar.file.menu add command -label "Source tcl script..." \
	-command opensource -underline 4
#.menubar.file.menu add command -label "Save As..." \
#	-command opensave -underline 0
.menubar.file.menu add separator
.menubar.file.menu add command -label "Exit" -command "destroy ." \
	-underline 1 -accelerator "Ctrl+d"

bind . <Control-d> "destroy ."

######## menuitem proc

menu .menubar.proc.menu

#.menubar.proc.menu add cascade -label "interact" \
#	-menu .menubar.proc.menu.list
.menubar.proc.menu add command -label "list available" -command "proc list"
.menubar.proc.menu add command -label "list selected" -command "proc lss"
.menubar.proc.menu add command -label "select" -command procselect
.menubar.proc.menu add command -label "help" -command "proc help"

######## menuitem Stops

menu .menubar.stops.menu

.menubar.stops.menu add command -label "go" -command "go"
.menubar.stops.menu add cascade -label "nextstop" -menu .menubar.stops.menu.nextstop

menu .menubar.stops.menu.nextstop

foreach number {1 5 10 50 100 500 1000 5000} {
    .menubar.stops.menu add command -label $number\
	    -command "nextstop $number"
}
.menubar.stops.menu.nextstop add command -label "number?" \
	-command "get_number"

########################
# bottom frame
########################

frame .bottom -relief ridge -borderwidth 2
button .bottom.exit -text Exit -command "destroy ." -padx 15
button .bottom.run1stop -text "process  1 stop" \
	-command "nextstop 1" -padx 15
pack .bottom.exit -side left

pack .bottom.run1stop -side right
pack .bottom -side bottom -fill x -pady 10

########################
# module list
########################

#init
#openmodlist
#openpath

#focus .

######## temp

