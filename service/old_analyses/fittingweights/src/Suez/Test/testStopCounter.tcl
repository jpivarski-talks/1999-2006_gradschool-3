source_format sel AsciiSourceFormat
file in /nfs/cleo3/Offline/data/runBeginRunEvent.asc event beginrun

sink_format sel AsciiSinkFormat
set fileout fileout.asc
if { [ tcl_file exists $fileout ]==1 } { 
       echo "deleting file '$fileout'"
       tcl_file delete $fileout 
}
file out $fileout beginrun event

go 2 event 

exit
