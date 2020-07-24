default prompt off
exception continueEventLoop on

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/PDSTest

source_format sel PDSSourceFormat

file in ../rzn/$env(USER_FILE).pds
source act $env(USER_FILE) event
go

exit
