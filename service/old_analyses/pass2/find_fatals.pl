#!/usr/local/bin/perl

$any = 0;
%interesting = ();
if ( $#ARGV == 0  && $ARGV[0] =~ /^all$/io )
{
    $any = 1;
}
else
{
    foreach my $run ( @ARGV )
    {
	$interesting{$run} = 1;
    }
}

$STATSDIR = "/home/pass2/Stats";
sub byrunlog
{
    my ( $arun, $alog, $amessage, $alines ) = @{ $a };
    my ( $brun, $blog, $bmessbge, $blines ) = @{ $b };

    my $cmp = $arun <=> $brun;
    return $cmp if ( $cmp != 0 );

    return $blog <=> $alog;
}
require( "$STATSDIR/load_fatals.pl" );
@majorerrors = sort byrunlog @majorerrors;


$prevrun = "";
foreach my $me ( @majorerrors )
{
    my ( $run, $log, $message, $lines ) = @{ $me };

    if ( $any == 1  ||  defined( $interesting{$run} ) )
    {
	print "======== RUN $run from LOG $log ==================================\n"
	    if ( $run != $prevrun );

	my @l = @{ $lines };
	my $s = "s";
	$s = "" if ( $#l == 0 );
	print "Found on line$s " . join( " ", @l ) . ":\n";
	print "    $message\n";
    }
    $prevrun = $run;
}
