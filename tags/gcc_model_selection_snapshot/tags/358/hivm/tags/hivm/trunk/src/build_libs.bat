REM build bjam
cd 3rd_party/build/boost-jam-3.1.13/
call build.bat
cd ../../../

REM copy the executable, bjam executable to my boost src directory.
move 3rd_party\build\boost-jam-3.1.13\bin.ntx86\bjam.exe 3rd_party\build\boost_1_33_1\

cd  3rd_party/build/boost_1_33_1/

REM Build program options and fileystem:
bjam --libdir=../../boost/lib --includedir=../../boost/include --with-program_options --with-filesystem "-sBUILD=debug <runtime-link>static <threading>multi" stage

REM Build unit test libraries:
bjam --libdir=../../boost/lib  --includedir=../../boost/include  --with-test "-sBUILD=debug <runtime-link>static <threading>multi" stage

cd ../../../