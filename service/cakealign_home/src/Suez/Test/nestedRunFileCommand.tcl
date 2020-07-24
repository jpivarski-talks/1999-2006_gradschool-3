echo "(1) here in first level run_file file"
echo "(2) now calling nested file"
run_file $env(C3_CVSSRC)/Suez/Test/nestedRunFileCommand2.tcl
echo "(4) after calling nested file"
