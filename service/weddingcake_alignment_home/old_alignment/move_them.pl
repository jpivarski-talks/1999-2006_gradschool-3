#!/usr/local/bin/perl

open( MOVE, $ARGV[0] );
%move = ();
while( <MOVE> )
{
    if ( $_ =~ /ring ([1-8]) (east|west) (x|y|rotation) should be moved by ([0-9\.\-\+eE]+) microns/ )
    {
	$move{$1}{$2}{$3} = $4;
    }
}
close( MOVE );

# Keep this for explicit unit conversion
foreach $ring ( 1..8 )
{
    foreach $side ( "west", "east" )
    {
	foreach $dir ( "x", "y", "rotation" )
	{
	    $move{$ring}{$side}{$dir} /= 25600;
# 	    print "$ring, $side, $dir: $move{$ring}{$side}{$dir}\n";
	}
    }
}

open( OLD, $ARGV[1] );
while( <OLD> )
{
    if ( $_ =~ /^\s*([0-9]{1,2})\s+([0-9\.\-\+eE]+)\s+([0-9\.\-\+eE]+)\s+([0-9\.\-\+eE]+)\s+([0-9\.\-\+eE]+)\s+([0-9\.\-\+eE]+)\s+([0-9\.\-\+eE]+)\s*$/ && $1 >= 3 )
    {
	$index = $1;  $x = $2; $y = $3; $z = $4; $phix = $5; $phiy = $6; $phiz = $7;
	$side = ( $index > 10 ? "east" : "west" );
	$ring = ( $side eq "west" ? $index - 2 : $index - 10 );
	$x += $move{$ring}{$side}{"x"};
	$y += $move{$ring}{$side}{"y"};
	$phiz += $move{$ring}{$side}{"rotation"};
	print "$index $x $y $z $phix $phiy $phiz\n";
    }
    else
    {
	print $_;
    }
}
