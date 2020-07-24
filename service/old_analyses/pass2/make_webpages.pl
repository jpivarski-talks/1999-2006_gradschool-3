#!/usr/local/bin/perl

$today = `date`;
chop( $today );

$STATSDIR = "/home/pass2/Stats";

######################################################

print "Making successfulruns.html...\n";
open( HTML, "> successfulruns.html" );

print HTML "<html><title>PASS2 auto update page (SUCCESSFUL RUNS)</title>
<body><h2>PASS2 auto update page (SUCCESSFUL RUNS)</h2></body>
<h3>Page generated $today</h3>
<pre>
";
close( HTML );

system( "/usr/local/bin/perl $STATSDIR/pass2lookup.pl -ng -ss -s 'runstatus eq \"ok\"' -f 'run | numevents | bbblumi | secperevent | l4rej | l4ratio | bharej | bharatio | logfile | startuptime | diskpath | machine | firststoptime | laststoptime | daexceptions' >> successfulruns.html" );

system( "echo \"</pre><a href=\\\"http://www.lns.cornell.edu/~bornheim/private/html/pass2/pass2.html\\\">Back to Pass2 Page</a></body></html>\" >> successfulruns.html" );

######################################################

print "Making runningruns.html...\n";
open( HTML, "> runningruns.html" );

print HTML "<html><title>PASS2 auto update page (RUNNING RUNS)</title>
<body><h2>PASS2 auto update page (RUNNING RUNS)</h2></body>
<h3>Page generated $today</h3>
<pre>
";
close( HTML );

system( "/usr/local/bin/perl $STATSDIR/pass2lookup.pl -ng -ss -s 'stillrunning ne \"done\" and stillrunning ne \"unresponsive\"' -f 'run | firststoptime | laststoptime | lastevent | logfile | machine | diskpath | startuptime | release | daexceptions | diskspaceerrors | fatalerrors' >> runningruns.html" );

system( "echo \"</pre><a href=\\\"http://www.lns.cornell.edu/~bornheim/private/html/pass2/pass2.html\\\">Back to Pass2 Page</a></body></html>\" >> runningruns.html" );

######################################################

print "Making failedruns.html...\n";
open( HTML, "> failedruns.html" );

print HTML "<html><title>PASS2 auto update page (FAILED RUNS)</title>
<body><h2>PASS2 auto update page (FAILED RUNS)</h2></body>
<h3>Page generated $today</h3>
<pre>
";
close( HTML );

system( "/usr/local/bin/perl $STATSDIR/pass2lookup.pl -ng -ss -s 'runstatus ne \"ok\" and not ( runstatus eq \"script unfinished\" and stillrunning eq \"still running\" ) and not ( runstatus eq \"script unfinished\" and stillrunning eq \"starting up\" )' -f 'run | runstatus | stillrunning | firststoptime | laststoptime | lastevent | logfile | machine | diskpath | startuptime | release | daexceptions | diskspaceerrors | fatalerrors' >> failedruns.html" );

system( "echo \"</pre><a href=\\\"http://www.lns.cornell.edu/~bornheim/private/html/pass2/pass2.html\\\">Back to Pass2 Page</a></body></html>\" >> failedruns.html" );
