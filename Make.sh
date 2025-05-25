mkdir -p build
cd build
cmake ..
make

cp ccpm $HOME
cd $HOME
chmod +x ccpm
# ./ccpm