<?hh

require_once 'inverse_ncdf.hh';

function test() : void
{
    $n = 100;
    for ($i = 1; $i < $n; ++$i) {
        $p = (float) ($i / $n);
        $x = inverse_ncdf($p);
        print "$p,$x\n";
    }
}

test();
