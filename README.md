# ModelTree

## Installation

    git clone git@github.com:PeterParfenov/ModelTree.git
    cd ModelTree
    mkdir build
    cd build
    cmake ../src/
    make
    
## Usage

Converting urqmd data (test.f14) into root-file (from the build directory):

    ./Converters/urqmd2tc -i test.f14 -o output.root
