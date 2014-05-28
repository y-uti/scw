<?php

function test()
{
    $n = 100;
    for ($i = 1; $i < $n; ++$i) {
        $p = $i / $n;
        $x = stats_cdf_normal($p, 0, 1, 2);
        print "$p,$x\n";
    }
}

test();
