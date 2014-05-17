<?php

/*
 * $x の符号を戻す
 * $x <  0 : -1
 * $x == 0 :  0
 * $x >  0 : +1
 */
function sgn($x)
{
    if ($x < 0) {
        return -1;
    } else if ($x == 0) {
        return 0;
    } else /* if ($x > 0) */ {
        return 1;
    }
}

/*
 * 逆正規累積分布関数
 */
function norminv($p, $mu, $sigma)
{
    return stats_cdf_normal($p, $mu, $sigma, 2);
}

/*
 * 配列 $a, $b を zip する
 */
function zip($a, $b)
{
    return array_map(null, $a, $b);
}
