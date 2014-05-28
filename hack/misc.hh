<?hh

require_once 'inverse_ncdf.hh';

/*
 * $x の符号を戻す
 * $x <  0 : -1
 * $x == 0 :  0
 * $x >  0 : +1
 */
function sgn(float $x) : int
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
function norminv(float $p, float $mu, float $sigma) : float
{
    $z = inverse_ncdf($p);
    return $z !== null ? $sigma * $z + $mu : nan;
}
