#!/bin/sh
# file: scripts/hardware.sh

# this file defines the names and addresses of ethernet interfaces
# on the host and the powerline device connected to each; every host 
# should have its own copy of this file so that generic scripts can 
# written and moved from one host to another without change;
#
# symbol ETH is referenced in files such as start.sh, flash.sh and
# upgrade.sh that operation on one device only; 
#
# symbols ETH1, ETH2, NIC1, NIC, PLD1 and PLD22 are referenced in 
# files such as pts.sh that operate on two devices; by convention: 
#
# interface ETH1 has address NIC1 and is connected to device PLD1;
# interface ETH2 had address NIC2 and is connected to device PLD2;
#
# to include this file in a new script:
#
# . ${SCRIPTS}/hardware.sh
#

# ====================================================================
# symbols;
# --------------------------------------------------------------------

ETH=eth1

# ====================================================================
# host interface names;
# --------------------------------------------------------------------

ETH1=eth0 
ETH2=eth1 

# ====================================================================
# host interface addresses;
# --------------------------------------------------------------------

NIC1=00:50:04:A5:D9:5A  
NIC2=00:01:03:2B:03:67  

# ====================================================================
# powerline device addresses;
# --------------------------------------------------------------------

PLD1=00:b0:52:ba:be:02
PLD2=00:b0:52:ba:be:03

# ====================================================================
# miscellaneous device addresses;
# --------------------------------------------------------------------

master=00:b0:52:da:da:01
slave1=00:b0:52:da:da:02
slave2=00:b0:52:da:da:03
slave3=00:b0:52:da:da:04
slave4=00:b0:52:dd:dd:05

