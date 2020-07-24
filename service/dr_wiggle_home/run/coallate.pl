#!/usr/local/bin/perl

%mc0_rphi = ();
%mc1_rphi = ();
%mc10_rphi = ();
%mc100_rphi = ();

%mc0_z = ();
%mc1_z = ();
%mc10_z = ();
%mc100_z = ();

open(MC0, "/cdat/tem/mccann/runover_mc0.log");
while(<MC0>) {
    next if ( substr($_,0,29) ne " %% INFO-Processor.DRWiggle: " );
    if ( $_ =~ /RPHI ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc0_rphi{$1} = [$2, $3, $4];
    } elsif ( $_ =~ /Z ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc0_z{$1} = [$2, $3, $4];
    }
}
close(MC0);

open(MC1, "/cdat/tem/mccann/runover_mc1.log");
while(<MC1>) {
    next if ( substr($_,0,29) ne " %% INFO-Processor.DRWiggle: " );
    if ( $_ =~ /RPHI ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc1_rphi{$1} = [$2, $3, $4];
    } elsif ( $_ =~ /Z ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc1_z{$1} = [$2, $3, $4];
    }
}
close(MC1);

open(MC10, "/cdat/tem/mccann/runover_mc10.log");
while(<MC10>) {
    next if ( substr($_,0,29) ne " %% INFO-Processor.DRWiggle: " );
    if ( $_ =~ /RPHI ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc10_rphi{$1} = [$2, $3, $4];
    } elsif ( $_ =~ /Z ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc10_z{$1} = [$2, $3, $4];
    }
}
close(MC10);

open(MC100, "/cdat/tem/mccann/runover_mc100.log");
while(<MC100>) {
    next if ( substr($_,0,29) ne " %% INFO-Processor.DRWiggle: " );
    if ( $_ =~ /RPHI ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc100_rphi{$1} = [$2, $3, $4];
    } elsif ( $_ =~ /Z ([0-9\.]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+) ([0-9\-\+\.eE]+)/ ) {
	$mc100_z{$1} = [$2, $3, $4];
    }
}
close(MC100);

open(RPHI, "> ../rzn/mcdiff_rphi.ntuple");
$count = 0;
foreach $key ( keys %mc0_rphi ) {
    next if ( ! defined($mc1_rphi{$key})   ||
	      ! defined($mc10_rphi{$key})  ||
	      ! defined($mc100_rphi{$key})    );
    last if ( $count > 1500 );
    $count++;

    print RPHI "$mc0_rphi{$key}->[0] $mc0_rphi{$key}->[1] $mc0_rphi{$key}->[2] ".
	(($mc0_rphi{$key}->[0]) - ($mc1_rphi{$key}->[0]))." ".
	(($mc0_rphi{$key}->[1]) - ($mc1_rphi{$key}->[1]))." ".
	(($mc0_rphi{$key}->[2]) - ($mc1_rphi{$key}->[2]))." ".
	(($mc0_rphi{$key}->[0]) - ($mc10_rphi{$key}->[0]))." ".
	(($mc0_rphi{$key}->[1]) - ($mc10_rphi{$key}->[1]))." ".
	(($mc0_rphi{$key}->[2]) - ($mc10_rphi{$key}->[2]))." ".
	(($mc0_rphi{$key}->[0]) - ($mc100_rphi{$key}->[0]))." ".
	(($mc0_rphi{$key}->[1]) - ($mc100_rphi{$key}->[1]))." ".
	(($mc0_rphi{$key}->[2]) - ($mc100_rphi{$key}->[2]))."\n";
}
close(RPHI);

open(Z, "> ../rzn/mcdiff_z.ntuple");
$count = 0;
foreach $key ( keys %mc0_z ) {
    next if ( ! defined($mc1_z{$key})   ||
	      ! defined($mc10_z{$key})  ||
	      ! defined($mc100_z{$key})    );
    last if ( $count > 1500 );
    $count++;

    print Z "$mc0_z{$key}->[0] $mc0_z{$key}->[1] $mc0_z{$key}->[2] ".
	(($mc0_z{$key}->[0]) - ($mc1_z{$key}->[0]))." ".
	(($mc0_z{$key}->[1]) - ($mc1_z{$key}->[1]))." ".
	(($mc0_z{$key}->[2]) - ($mc1_z{$key}->[2]))." ".
	(($mc0_z{$key}->[0]) - ($mc10_z{$key}->[0]))." ".
	(($mc0_z{$key}->[1]) - ($mc10_z{$key}->[1]))." ".
	(($mc0_z{$key}->[2]) - ($mc10_z{$key}->[2]))." ".
	(($mc0_z{$key}->[0]) - ($mc100_z{$key}->[0]))." ".
	(($mc0_z{$key}->[1]) - ($mc100_z{$key}->[1]))." ".
	(($mc0_z{$key}->[2]) - ($mc100_z{$key}->[2]))."\n";
}
close(Z);
