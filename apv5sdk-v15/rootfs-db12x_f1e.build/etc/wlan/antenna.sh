#!/bin/sh
# sysc_atha.sh
# Usage: sh sysc_atha.sh [tx rx div]
#
t=`cat /proc/sys/dev/wifi0/txantenna`
r=`cat /proc/sys/dev/wifi0/rxantenna`
d=`cat /proc/sys/dev/wifi0/diversity`
t=${1:-$t}
r=${2:-$r}
d=${3:-$d}
(sysctl -w dev.wifi0.txantenna=$t
 sysctl -w dev.wifi0.rxantenna=$r
 sysctl -w dev.wifi0.diversity=$d
)|tr '\n' ' '|sed s/dev.wifi0.//g
  echo
