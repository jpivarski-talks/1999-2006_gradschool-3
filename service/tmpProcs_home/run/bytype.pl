#!/usr/local/bin/perl

%peak = ();
%cont = ();
%scan = ();
%high = ();
open(PEAK, "peakruns.dat");
open(CONT, "contruns.dat");
open(SCAN, "scanruns.dat");
open(HIGH, "highruns.dat");
while (my $run = <PEAK>) {
    chop($run);
    $peak{$run} = 1;
}
while (my $run = <CONT>) {
    chop($run);
    $cont{$run} = 1;
}
while (my $run = <SCAN>) {
    chop($run);
    $scan{$run} = 1;
}
while (my $run = <HIGH>) {
    chop($run);
    $high{$run} = 1;
}
close(PEAK);
close(CONT);
close(SCAN);
close(HIGH);

foreach $x ("meanredchi.dat", "meancutredchi.dat", "fracredchi.dat", 
	    "meanerrcotth.dat", "meancuterrcotth.dat", "fracerrcotth.dat", 
	    "meanerrz0.dat", "meancuterrz0.dat", "fracerrz0.dat") {
    open(X, $x);
    open(PEAK, "> peak_$x");
    open(CONT, "> cont_$x");
    open(SCAN, "> scan_$x");
    open(HIGH, "> high_$x");
    open(OTHER, "> other_$x");
    while (my $line = <X>) {
	chop($line);
	my ($run, $data) = split(/\s+/, $line);
	print PEAK "$run $data\n" if (defined($peak{$run}));
	print CONT "$run $data\n" if (defined($cont{$run}));
	print SCAN "$run $data\n" if (defined($scan{$run}));
	print HIGH "$run $data\n" if (defined($high{$run}));
	print OTHER "$run $data\n" if (!defined($peak{$run})  &&
				       !defined($cont{$run})  &&
				       !defined($scan{$run})  &&
				       !defined($high{$run}));
    }
    close(PEAK);
    close(CONT);
    close(SCAN);
    close(HIGH);
    close(OTHER);
    close(X);
}

