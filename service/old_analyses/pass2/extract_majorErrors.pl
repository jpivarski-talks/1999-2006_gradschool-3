#!/usr/bin/perl

open( MAJORERRORS, "/home/pass2/Stats/majorErrors.1.txt" );
open( FEWER, "> /home/pass2/Stats/majorErrors.1.reduced.txt" );
$keepgoing = 0;
while( <MAJORERRORS> )
{
    if ( $_ eq "RunNum | Log |   Line | Line from Logfile\n" )
    {
	print FEWER $_;
    }

    elsif ( $_ eq "-------+-----+--------+-------------------------------------------------------------------------------------------\n" )
    {
	print FEWER $_;
    }

    elsif ( $_ =~ /^[0-9]{6} \|\s+([0-9]{1,3})/o )
    {
	if ( $1 > 80 )
	{
	    $keepgoing = 1;
	    print FEWER $_;
	}
	else
	{
	    $keepgoing = 0;
	}
    }

    elsif ( $keepgoing == 1 )
    {
	print FEWER $_;
    }

}
