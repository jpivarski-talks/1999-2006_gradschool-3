#!/bin/sh
find . -name "*.h" -print | awk '{split($1,a,"/"); if(a[4]) print "a2ps -f11 -R -1 -o "a[4]".ps "$0""; if(!a[4]&&a[3]) print "a2ps -f11 -R -1 -o "a[3]".ps "$0""}' |/bin/sh
find . -name "*.cc" -print | awk '{split($1,a,"/"); if(a[4]) print "a2ps -f11 -R -1 -o "a[4]".ps "$0""; if(!a[4]&&a[3]) print "a2ps -f11 -R -1 -o "a[3]".ps "$0""}' |/bin/sh
