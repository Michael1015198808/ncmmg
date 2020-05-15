A naive cmm code generator (with an automatically testing tool.)

### Usage
To compile the code generator, type

    make

To start testing, type

    ./run.sh

(you need to comment out outputs in irsim:main.cc, because this run.sh uses a naive diff.)

(All test data that you're wrong will be copied into tests/)

### Grammers
Grammers that will be tested
- Arithmetic without division (Because divided by zero is hard to solve)
- declare and airhtmetics of array.
- if and if-else.
- while
