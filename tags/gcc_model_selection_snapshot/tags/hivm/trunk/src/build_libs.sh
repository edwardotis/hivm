echo build bjam
cd 3rd_party/build/boost-jam-3.1.13/
chmod 775 build.sh
./build.sh
cd ../../../

echo copy the executable, bjam executable to my boost src directory.
mv 3rd_party/build/boost-jam-3.1.13/bin.linuxx86/bjam 3rd_party/build/boost_1_33_1/

cd  3rd_party/build/boost_1_33_1/

echo Build program options and fileystem:
./bjam --libdir=../../boost/lib --includedir=../../boost/include --with-program_options --with-filesystem "-sBUILD=debug <runtime-link>static <threading>multi" stage

echo Build unit test libraries:
./bjam --libdir=../../boost/lib  --includedir=../../boost/include  --with-test "-sBUILD=debug <runtime-link>static dynamic <threading>multi" stage

cd ../../../
