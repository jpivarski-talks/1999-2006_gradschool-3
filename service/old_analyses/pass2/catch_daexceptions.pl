#!/usr/local/bin/perl

$n = 0;
open( LS, "ls /home/pass2/Stats |" );
while( <LS> )
{
    if ( $_ =~ /status\.([0-9]+)\.pl/ )
    {
	$n = $1 if ( $n < $1 );
    }
}
$norecurse = 0;
warn "Loading data from /home/pass2/Stats/status.$n.pl...\n";
require( "/home/pass2/Stats/status.$n.pl" );
warn "Done loading data!\n";
sub numly { $a <=> $b; }
@runs = sort numly keys %log;

foreach $run ( @runs )
{
    $daexceptions = $info{$run}{"daexceptions"};
    $lastevent = $info{$run}{"lastevent"};

    if ( abs( $daexceptions - $lastevent ) < 20 )
    {
	print "run = $run, daexeptions = $daexceptions, lastevent = $lastevent\n";
    }
}
