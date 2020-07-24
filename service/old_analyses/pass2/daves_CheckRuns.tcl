
module sel DBModule
database in /nfs/cleo3/database/data12/db event beginrun startrun endrun
database read_collection mp2-bhabha
database run $env(RUNNO) $env(RUNNO3)
#database read_collection mp2-hadron

for {set i 0} {$i <= 200} {incr i} {
stream act beginrun
go 1 beginrun 
stream act beginrun event
go 1 event
}

quit
