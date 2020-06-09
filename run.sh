mkdir -p tests
mkdir -p workdir
f=0
tot=0
PARSER=~/Lab/parser
IRSIM=~/compilers-tests/irsim/build/irsim
while [ $tot -lt 100000 ]; do
    build/generator > workdir/tmp.c
    if gcc -fwrapv workdir/gcc.c -o workdir/a.out 2>/dev/null; then
        if timeout 2 workdir/a.out < workdir/0s.txt > workdir/gcc_out; then
            #filter out programs that run too slow on native
            $PARSER workdir/tmp.c workdir/a.s;
            if python3 check.py;then
                true;
            else
                cp workdir/tmp.c tests/$f.cmm
                f=$((f+1))
            fi;
            tot=$((tot+1))
            echo $f / $tot
        fi;
    else
        echo compile error;
        exit 0;
    fi;
done;
