git clone https://github.com/JPETTomography/JPet-HV-GUI-tool.git

cd JPet-HV-GUI-tool/

git submodule update --init --recursive

cd JPET-HV-adjust

cmake .

make

cd ..

qmake

make

./JPetGainCalibrationDBEdittingTool.exe

