mkdir -p tests
mkdir -p workdir
f=0
tot=0
while [ $tot -lt 100000 ]; do
    ./generator > workdir/tmp.c
    if gcc workdir/gcc.c -o workdir/a.out 2>/dev/null; then
        if ./workdir/a.out < 0s.txt > workdir/gcc_out; then
            ~/Lab/parser workdir/tmp.c workdir/a.ir;
            ~/compilers-tests/irsim/build/irsim workdir/a.ir < 0s.txt > workdir/cmm_out 2>/dev/null;
            if diff workdir/gcc_out workdir/cmm_out 2>/dev/null;then
                true;
            else
                cp workdir/tmp.c tests/$f.txt
                f=$((f+1))
            fi;
            tot=$((tot+1))
            echo $f / $tot
        fi;
    fi;
done;
