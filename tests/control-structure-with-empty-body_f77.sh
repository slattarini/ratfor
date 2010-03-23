#!/bin/sh
# Copyright (C) 2010 Stefano Lattarini <stefano.lattarini@gmail.com>.
# This file is part of the Ratfor Testsuite.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Helper file, sourced by scripts checking (by compiling and running the
# generated fortran code) that ratfor control statements (if, while, for,
# etc) works correctly also with an empty body.  This file is expected to
# be sourced by test scripts after the file `rat4-testsuite-init.sh' has
# already been sourced.

test x${stmt+"set"} = x"set" \
  || testcase_HARDERROR "variable \`\$stmt' not set"

b="tst-$stmt"
# we shouldn't clobber exising files here, so try to tell the shell
# to abort on any attempt to do so (if this is supported)
(set -C) >/dev/null 2>&1 && { set -C || testcase_HARDERROR; }

case $stmt in
    # The double quoting here is *required* to avoid spurious parsing
    # errors from some shells (among them, Bash). Sigh.

    "if")
        echo "$stmt(nfunc() < 10);"  > $b-1.r
        echo "$stmt(nfunc() < 10){}" > $b-2.r
        for i in 1 2; do echo "n = n + 3" >> $b-$i.r; done
        ;;

    "while")
        echo "$stmt(nfunc() <= 3);"  > $b-1.r
        echo "$stmt(nfunc() <= 3){}" > $b-2.r
        ;;

    "do")
        (echo "do n=1,4"; echo ";")  > $b-1.r
        (echo "do n=1,4"; echo "{}") > $b-2.r
         echo "do n=1,4; ;"          > $b-3.r
         echo "do n=1,4 {}"          > $b-4.r
         echo "do n=1,4; {}"         > $b-5.r
        # in many fortran compilers, the `do' statement above behaves
        # like a `for(n=1; n<=4; n=n+1)' loop, so the value of `n' upon
        # exit may as well be `5'
        for i in 1 2 3 4 5; do echo "if(n==5) n=n-1" >> $b-$i.r; done
        ;;

    "for")
        echo "for(n = 1; n < 4; n = n + 1);"  > $b-1.r
        echo "for(n = 1; n < 4; n = n + 1){}" > $b-2.r
        ;;

    "else")
        echo "if(nfunc() > 2) n = 5; else;"   > $b-1.r
        echo "if(nfunc() > 2) n = 5; else {}" > $b-2.r
        for i in 1 2; do echo "n = n + 3" >> $b-$i.r; done
        ;;

    "if-else")
        echo "if(nfunc() > 2); else;"     > $b-1.r
        echo "if(2 > nfunc()); else {}"   > $b-2.r
        echo "if(nfunc() > 2) {} else;"   > $b-3.r
        echo "if(2 > nfunc()) {} else {}" > $b-4.r
        for i in 1 2 3 4; do echo "n = n + 3" >> $b-$i.r; done
        ;;

    "repeat")
        echo "n = 4; if(1 > 2) { repeat;   }" > $b-1.r
        echo "n = 4; if(1 > 2) { repeat {} }" > $b-2.r
        ;;

    "repeat-until")
        echo "repeat; until(nfunc() >= 4)"   > $b-1.r
        echo "repeat {} until(nfunc() >= 4)" > $b-2.r
        ;;

    "switch")
        # `switch' statement require braces
        (echo "switch(nfunc()) {}" && echo "n = n + 3") > $b.r
        ;;

    "case")
        $SED "s/^$ws0p//" > $b.r <<EOF
            100 continue
            switch(nfunc()) {
              case 1:; case 2: {} case 3:
              default: goto 200
            }
            goto 100
            200 continue

EOF
        ;;

    "default")
        i=0
        for s in " " ";" "{}" "$NEWLINE"; do
            $SED "s/^$ws0p//" > $b-$i.r <<EOF
                100 continue
                switch(nfunc()) {
                  case 1: goto 100
                  case 2: goto 100
                  case 3: goto 100
                  default:$s}
EOF
            i=`expr $i + 1`
        done
        ;;

    *)
        # CANTHAPPEN
        testcase_HARDERROR "invalid statement \`$stmt"
        ;;

esac

unset i s || : # get rid of temporary variables

cat > nfunc.r <<EOF
integer function nfunc()
    integer n
    common /counter/ n
    n = n + 1
    nfunc = n
    return
end
EOF

ls -l tst-*.r

for r in tst-*.r; do
    x=`echo $r | $SED -e 's/^tst-//' -e 's/\.r$//'`
    testprogname=`echo $stmt$x | $SED -e 's/[^a-zA-Z0-9]//g'`
    { cat nfunc.r
      echo
      $SED "s/^$ws0p//" <<EOF
        program $testprogname
        integer n, nfunc
        common /counter/ n
        n = 0
EOF
      cat $r
      echo "write(*,10) n; 10 format(I1);"
      echo end
    } > $r.tmp
    mv -f $r.tmp $r
done

# Now, do the tests.

for r in tst-*.r; do

    cat $r

    x=`echo $r | $SED 's/\.r$//'`
    f=$x.f
    exe=$x.exe

    run_RATFOR $r || testcase_FAIL "unexpected ratfor failure"
    mv stdout $f
    if test -s stderr; then
        testcase_FAIL "ratfor produced diagnostic on stderr"
        mv stderr $x--ratfor-stderr
    else
        rm -f stderr
    fi

    run_F77 $f || testcase_FAIL "cannot compile ratfor output"
    for t in stderr stdout; do
        if test -s $t; then
            mv $t $x--fortran-$t
        else
            rm -f $t
        fi
    done

    run_command timer -t 10 ./$exe \
      || testcase_FAIL "compiled binary failed"
    mv stdout $x--exe-stdout
    if test -s stderr; then
        testcase_FAIL "compiled binary produced diagnostic on stderr"
        mv stderr $x--exe-stderr
    else
        rm -f stderr
    fi

    o=`cat $x--exe-stdout` || testcase_HARDERROR
    if test x"$o" != x"4"; then
        testcase_FAIL "compiled binary produced unexpected output"
    fi

done

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
