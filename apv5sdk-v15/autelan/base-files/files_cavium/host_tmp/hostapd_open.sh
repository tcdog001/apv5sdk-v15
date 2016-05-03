#!/bin/sh
ifconfig ath0 down
iwpriv ath0 pure_1x 1
ifconfig ath0 up
/host_tmp/hostapd_open -dd c1

