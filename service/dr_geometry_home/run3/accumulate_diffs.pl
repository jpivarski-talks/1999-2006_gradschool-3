#!/usr/local/bin/perl

%n_pperp = ();
%s_pperp = ();
%a_pperp = ();
%b_pperp = ();

%n_redchim = ();
%s_redchim = ();
%a_redchim = ();
%b_redchim = ();

%n_redchip = ();
%s_redchip = ();
%a_redchip = ();
%b_redchip = ();

open(N, "/cdat/tem/mccann/stretchad_n_print.log");
while (<N>) {
    next if (substr($_, 0, 3) ne "FOR");
    chop;
    my @l = split(' ', $_);
    next if ($#l != 2);
    $n_pperp{$l[1]} = $l[2] if ($l[0] eq "FORPPERP");
    $n_redchim{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIM");
    $n_redchip{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIP");
}
close(N);

open(S, "/cdat/tem/mccann/stretchad_s_print.log");
while (<S>) {
    next if (substr($_, 0, 3) ne "FOR");
    chop;
    my @l = split(' ', $_);
    next if ($#l != 2);
    $s_pperp{$l[1]} = $l[2] if ($l[0] eq "FORPPERP");
    $s_redchim{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIM");
    $s_redchip{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIP");
}
close(S);

open(A, "/cdat/tem/mccann/stretchad_a_print.log");
while (<A>) {
    next if (substr($_, 0, 3) ne "FOR");
    chop;
    my @l = split(' ', $_);
    next if ($#l != 2);
    $a_pperp{$l[1]} = $l[2] if ($l[0] eq "FORPPERP");
    $a_redchim{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIM");
    $a_redchip{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIP");
}
close(A);

open(B, "/cdat/tem/mccann/stretchad_b_print.log");
while (<B>) {
    next if (substr($_, 0, 3) ne "FOR");
    chop;
    my @l = split(' ', $_);
    next if ($#l != 2);
    $b_pperp{$l[1]} = $l[2] if ($l[0] eq "FORPPERP");
    $b_redchim{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIM");
    $b_redchip{$l[1]} = $l[2] if ($l[0] eq "FORREDCHIP");
}
close(B);

open(NSPPERP, "> /cdat/dafe/mccann/tmp/ns_pperp.ntuple");
foreach my $k (keys %n_pperp) {
    next if (! defined($s_pperp{$k}));
    print NSPPERP $n_pperp{$k}." ".($n_pperp{$k} - $s_pperp{$k})."\n";
}
close(NSPPERP);

open(NAPPERP, "> /cdat/dafe/mccann/tmp/na_pperp.ntuple");
foreach my $k (keys %n_pperp) {
    next if (! defined($a_pperp{$k}));
    print NAPPERP $n_pperp{$k}." ".($n_pperp{$k} - $a_pperp{$k})."\n";
}
close(NAPPERP);

open(NBPPERP, "> /cdat/dafe/mccann/tmp/nb_pperp.ntuple");
foreach my $k (keys %n_pperp) {
    next if (! defined($b_pperp{$k}));
    print NBPPERP $n_pperp{$k}." ".($n_pperp{$k} - $b_pperp{$k})."\n";
}
close(NBPPERP);

open(NSREDCHIM, "> /cdat/dafe/mccann/tmp/ns_redchim.ntuple");
foreach my $k (keys %n_redchim) {
    next if (! defined($s_redchim{$k}));
    print NSREDCHIM $n_redchim{$k}." ".($n_redchim{$k} - $s_redchim{$k})."\n";
}
close(NSREDCHIM);

open(NAREDCHIM, "> /cdat/dafe/mccann/tmp/na_redchim.ntuple");
foreach my $k (keys %n_redchim) {
    next if (! defined($a_redchim{$k}));
    print NAREDCHIM $n_redchim{$k}." ".($n_redchim{$k} - $a_redchim{$k})."\n";
}
close(NAREDCHIM);

open(NBREDCHIM, "> /cdat/dafe/mccann/tmp/nb_redchim.ntuple");
foreach my $k (keys %n_redchim) {
    next if (! defined($b_redchim{$k}));
    print NBREDCHIM $n_redchim{$k}." ".($n_redchim{$k} - $b_redchim{$k})."\n";
}
close(NBREDCHIM);

open(NSREDCHIP, "> /cdat/dafe/mccann/tmp/ns_redchip.ntuple");
foreach my $k (keys %n_redchip) {
    next if (! defined($s_redchip{$k}));
    print NSREDCHIP $n_redchip{$k}." ".($n_redchip{$k} - $s_redchip{$k})."\n";
}
close(NSREDCHIP);

open(NAREDCHIP, "> /cdat/dafe/mccann/tmp/na_redchip.ntuple");
foreach my $k (keys %n_redchip) {
    next if (! defined($a_redchip{$k}));
    print NAREDCHIP $n_redchip{$k}." ".($n_redchip{$k} - $a_redchip{$k})."\n";
}
close(NAREDCHIP);

open(NBREDCHIP, "> /cdat/dafe/mccann/tmp/nb_redchip.ntuple");
foreach my $k (keys %n_redchip) {
    next if (! defined($b_redchip{$k}));
    print NBREDCHIP $n_redchip{$k}." ".($n_redchip{$k} - $b_redchip{$k})."\n";
}
close(NBREDCHIP);
