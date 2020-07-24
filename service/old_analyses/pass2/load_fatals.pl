#!/usr/local/bin/perl

$STATSDIR = "/home/pass2/Stats";

$n = 0;
open( LS, "ls $STATSDIR |" );
while( <LS> )
{
    if ( $_ =~ /majorErrors\.([0-9]+)\.pl/ )
    {
	$n = $1 if ( $n < $1 );
    }
}
$s = 3;
@majorerrors = ();

foreach $i ( reverse( 1..$n ) )
{
    if ( -e "$STATSDIR/majorErrors.$i.pl" )
    {
	warn "Loading fatal error log from $STATSDIR/majorErrors.$i.pl...\n";
	open( MERRS, "$STATSDIR/majorErrors.$i.pl" );
	$started = 0;
	while( $text = <MERRS> )
	{
	    eval( $text ) if ( $started == 1 );
	    $started = 1 if ( $text eq "# START MAJORERRORS\n" );
	    $started = 0 if ( $text eq "# STOP MAJORERRORS\n" );
	}
	close( MERRS );
    }
}

1;
