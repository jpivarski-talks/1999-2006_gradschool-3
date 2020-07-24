#!/usr/local/bin/perl

$length = $ARGV[0];
$iteration = $ARGV[1];

$dir = "/cdat/daf4/mccann/MakeFittingWeights/tenthTry";

open( EVENTNUM, "> $dir/generate-cur-eventnum.txt" );
open( MISSEDRIFT, "> $dir/generate-cur-missedrift.txt" );
open( EDRIFT, "> $dir/generate-cur-edrift.txt" );

open( STDIN2, "<&STDIN" );

$count = 0;
$which = 0;
while( <STDIN2> )
{
#     if ( $_ =~ /Run:\s+([0-9]+)\s+Event:\s+([0-9]+)/ )
#     {
# 	$| = 1;
# 	print EVENTNUM "$1:$2\n";
# 	$| = 0;
#     }
#     if ( $_ =~ /MISSEDRIFT: (.*)$/ )
#     {
#  	$| = 1;
#  	print MISSEDRIFT "$1\n";
#  	$| = 0;
#     }
#     if ( $_ =~ /EDRIFT \(L,cT,aD,raw,expected\): (.*)$/ )
#     {
#  	$| = 1;
#  	print EDRIFT "$1\n";
#  	$| = 0;
#     }
    if ( $count == 0  &&  $which == 0 )
    {
	open( FILE, "> /cdat/tem/mccann/generate$iteration-a.txt" );
	$which = 1;
    }
    elsif ( $count == 0  &&  $which == 1 )
    {
	open( FILE, "> /cdat/tem/mccann/generate$iteration-b.txt" );
	$which = 0;
    }
    print FILE $_;
    $count++;
    if ( $count == $length )
    {
	$count = 0;
    }
}
