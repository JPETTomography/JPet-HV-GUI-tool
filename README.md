Set of libraries needed for HV management in J-PET
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


Config file required for tools
==============================
The tools look for file 

	connstr.txt

in current directory (where you run the binary).
It must contain two lines

	database connection string

	HV connection string

