GIU tool for HV management in J-PET
===================================

How to use
==========

Compile and install

	git clone https://github.com/JPETTomography/JPet-HV-GUI-tool.git

	cd JPet-HV-GUI-tool/

	git submodule update --init --recursive

	cd JPET-HV-adjust

	cmake .

	make

	cd ..

	qmake

	make

	vi HVTool.exe.txt

write postgres sql connection string in the first line and libhv connection string in the second line and save the file



Run the application

	./HVTool.exe



Environment variables
=====================

	HVLIBDIR - HV directory from Silvermedia software source repository

