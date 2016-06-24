GUI tool for HV management in J-PET
==================================================

Environment variables
=====================

	HVLIBDIR - HV directory from Silvermedia software source repository

How to compile
==============

	git clone https://github.com/JPETTomography/JPet-HV-GUI-tool.git
	
	cd JPet-HV-GUI-tool
	
	git submodule update --init --recursive
	
	cmake .
	
	make


Config file required for the tool
=================================
The tools look for file

	connstr.txt

in current directory (where you run the binary).
It must contain two lines

	database connection string
	
	HV connection string

Brief description of the tool
=============================

For operating with HV configuration one needs to select
in the left part of GUI tool window the needed J-PET frame
and then one of the setups corresponding to this frame.

The lowest listview in the left part of the window shows
list of configurations existing for the selected setup.
New configuration can be created and selected configuration
can be removed.
The HV values from selected configuration are shown in the
listview on the right half of the window.
"inf" values correspond to not configured channels.
These channels are not processes whed dealing with the hardware.
The HV values can be edited and the control of maximum values
is implemented. Maximum HV value is stored in the DB in the
table of photomultipliers.
Configurations also can be exported into text files and
imported from file of the same format.
