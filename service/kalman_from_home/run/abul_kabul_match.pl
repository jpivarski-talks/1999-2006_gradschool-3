#!/usr/local/bin/perl

%old = ();
open( OLD, "beamwall_old.log" );
while ( $line = <OLD> ) {
    if ( $line =~ /^ABUL ([0-9]{6}) ([0-9]+) ([0-9]+)$/ ) {
	$run = $1;
	$event = $2;
 	$numtracks = $3;
	@tracks = ();
    }
    elsif ( $line =~ /^KABUL (.*)$/ ) {
	@numbers = split( ' ', $1 );
	push( @tracks, [ @numbers ] );

	if ( $#tracks+1 == $numtracks ) {
	    $runevent = $run."_".$event;
	    $old{"$runevent"} = [ @tracks ];
	} # end if this was the last track entry for this event
    } # end if this is an ABUL/KABUL LINE
} # end loop over the old file

%new = ();
open( NEW, "beamwall_new.log" );
while ( $line = <NEW> ) {
    if ( $line =~ /^ABUL ([0-9]{6}) ([0-9]+) ([0-9]+)$/ ) {
	$run = $1;
	$event = $2;
 	$numtracks = $3;
	@tracks = ();
    }
    elsif ( $line =~ /^KABUL (.*)$/ ) {
	@numbers = split( ' ', $1 );
	push( @tracks, [ @numbers ] );

	if ( $#tracks+1 == $numtracks ) {
	    $runevent = $run."_".$event;
	    $new{"$runevent"} = [ @tracks ];
	} # end if this was the last track entry for this event
    } # end if this is an ABUL/KABUL LINE
} # end loop over the new file

open( NTUPLE, "> beamwall.ntuple" );
%diff = ();
foreach $k ( keys %old ) {
    die $k if ( ! defined($new{$k}) );

    @oldtracks = @{ $old{$k} };
    @newtracks = @{ $new{$k} };
    die $#oldtracks." ".$#newtracks if ( $#oldtracks != $#newtracks );

    foreach $i ( 0..$#oldtracks ) {
	@oldnumbers = @{ $oldtracks[$i] };
	@newnumbers = @{ $newtracks[$i] };
	die $#oldnumbers." ".$#newnumbers if ( $#oldnumbers != $#newnumbers );
	
	@oldrelevant = ($oldnumbers[0], $oldnumbers[1], $oldnumbers[52], $oldnumbers[53], $oldnumbers[54], 
			$oldnumbers[56], $oldnumbers[57], $oldnumbers[58], $oldnumbers[59], $oldnumbers[60], 
			$oldnumbers[17], $oldnumbers[18], $oldnumbers[19], 
			$oldnumbers[55],
			);
	
	@newrelevant = ($newnumbers[0], $newnumbers[1], $newnumbers[52], $newnumbers[53], $newnumbers[54], 
			$newnumbers[56], $newnumbers[57], $newnumbers[58], $newnumbers[59], $newnumbers[60], 
			$newnumbers[17], $newnumbers[18], $newnumbers[19], 
			$newnumbers[55],
			);

	foreach $j ( 0..$#oldrelevant ) {
	    print NTUPLE $oldrelevant[$j];
	    print NTUPLE " ";
	} # end loop over quantities of interest

	foreach $j ( 0..$#oldrelevant ) {
	    print NTUPLE $newrelevant[$j]-$oldrelevant[$j];
	    print NTUPLE " ";
	} # end loop over quantities of interest
	print NTUPLE "\n";

    } # end loop over tracks
} # end loop over events

open( KUMAC, "> beamwall_init.kumac" );
print KUMAC "nt/cr 1 'beamwall' 110 ! ! 'phi' 'cotTh' 'c' 'd0' 'z0' 's_phi' 's_cotTh' 's_c' 's_d0' 's_z0' 'px' 'py' 'pz' 'fitabrt' 'dphi' 'dcotTh' 'dc' 'dd0' 'dz0' 'ds_phi' 'ds_cotTh' 'ds_c' 'ds_d0' 'ds_z0' 'dpx' 'dpy' 'dpz' 'dfitabrt'
nt/read 1 beamwall.ntuple ! ! 50000
";
