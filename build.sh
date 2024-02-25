make clean
make depen
make main.out

BUILD="$(pwd)/build"
DEPENDENCIES="$(pwd)/build/dependencies"

echo $BUILD

echo $DEPENDENCIES
mkdir $BUILD
mkdir $DEPENDENCIES

# move build and dependant files into respective directories

mv $(pwd)/main.out $BUILD

mv $(pwd)/*.o $DEPENDENCIES
