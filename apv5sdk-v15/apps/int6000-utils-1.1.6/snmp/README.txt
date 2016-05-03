Intellon INT6x00 SNMP Demonstration
===================================
IMPORTANT NOTE: This package is simply a demonstration of how an SNMP
sub-agent can be used to manage an Intellon device. It is not a
complete SNMP agent, and it is not supported by Intellon.

Overview
========
This SNMP sub-agent currently acquires a list of Intellon devices
by broadcasting a Version Info Request MME every three seconds
on the interface specified by the -i command line option (default:
eth1). The devices that respond have their MAC address, Hardware ID,
and firmware identification strings inserted into a row in a table
on the SNMP daemon, one row per device.

If a SNMP SET is performed on the deviceReset entry, the device
corresponding to that row will be reset. Any integer value can be
written to cause a reset.

See the Usage section for examples.

Toolkit Integration
-------------------
The Intellon Linux Toolkit is used to enable the Intellon SNMP subagent
to communicate with INT6x00 devices (see the snmp directory). The
Intellon Linux API can also be used to perform the same functionality
(see the snmp2 directory).

The itln_subagent.c file is the starting point of the subagent. Here
the subagent registers itself with the SNMP daemon, initializes
and registers its tables and scalars with SNMPD, and starts a timer
(used for periodic querying of the network).

There are primarily two ways to extend the functionality of the
current subagent; you can allow more values to be read from the INT6x00
devices, and/or allow more values to be written to the INT6x00 devices.

To allow more values to be read, changes in what the subagent
retrieves from each device could be added to the timer function
in itln_subagent.c. After the list of devices currently on the
network is retrieved, other MMEs could be sent to obtain additional
information. This information could then be added to the SNMP table
(itln_subagent.c demonstrates how to do this for the device type,
MAC address, and firmware revision).

To allow for more values to be written to the device, changes will need
to be made to the intellon.c file. It is here that the appropriate
function calls to either the Linux Toolkit or API can be added
so that they will be called on a SNMP 'set' request. Interacting
with a device on a SNMP 'set' request is demonstrated by the reset
device functionality that is currently implemented. This section of
intellon.c could be extended, for instance, to allow a SNMP 'set' on
the MAC address to change the MAC address of a remote INT6x00 device.

For more general information on the API
provided by net-snmp, see the tutorials at
http://net-snmp.sourceforge.net/wiki/index.php/Tutorials. For examples
on using the Linux Toolkit functions, see int6k. For examples on
using the Linux API, see int6k2.

Installation
============
The NET-SNMP snmpd package must be installed to make use of the
Intellon SNMP sub-agent. This package can be acquired from the NET-SNMP
website (http://net-snmp.sourceforge.net/).

The Intellon NET-SNMP sub-agent has been tested with NET-SNMP version
5.4.2.1.

For those unfamiliar with the NET-SNMP package, the following files
have been included in the int6000-utils-linux/snmp/ directory:

net-snmpd.sh:
	This script will extract, build, and install the NET-SNMP
	package. You must have the gzipped tar available in the same
	directory as the script for it to do its job. This can be
	obtained from the net-snmp sourceforge page. Version 5.4.2.1
	is required.

snmpd-example.conf:
	This is an example snmpd configuration that can be copied to
	/usr/local/share/snmp/snmpd.conf. Its provides no limitations
	on which hosts can connect to and modify the SNMP data. As a
	result, this file should only be used if the security
	implications are understood. See the snmpd.conf manual page
	for more information (run: man snmpd.conf). Be sure to enable
	agentx support in this file, since the Intellon subagent
	requires agentx support to function.

NOTES: Previous installations of NET SNMP can cause conflicts with a
new installation. Ensure old snmpd.conf files and NET SNMP libraries
are removed if the snmpd daemon does not work.

After installing snmpd (with or without the help of net-snmpd.sh) and
its required configuration, a "make install" can be performed in the
int6000-utils-linux/snmp/ directory. This will install the Intellon
sub-agent and the Intellon MIBs. This must be performed after the
installation of NET SNMP, since the sub-agent relies on NET SNMP
libraries.

Usage
=====

Overview
--------
Before the sub-agent can be used, the snmpd daemon must be running.
This is accomplished as shown below:

	# snmpd
	# itln_subagent

The sub-agent does not currently execute as a daemon. It also has
noisy debugging code in it.

Examples
--------
These examples assume that the read/write community name is "public"
and that the SNMP daemon is running on the same machine that the
examples are executed on.

$ snmpwalk -m ALL -v 2c -c public localhost intellon
	Show all informarmation under the intellon OID.

$ snmpget -v 2c -c public localhost IHPAV-OPERATIONS-MIB::deviceMacAddress.0
	This will GET the MAC address of the first device instance in
	the SNMP table if one exists.

$ snmpset -m ALL -v 2c -c public localhost IHPAV-OPERATIONS-MIB::deviceReset.0 i 0
	This will reset the Intellon device in the SNMP table at row 0.
