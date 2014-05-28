<?hh

newtype Vec = array<float>;
newtype Mat = array<Vec>;

/*
 * $n 行 $m 列の零行列を作成する
 * 戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 */
function zeros(int $n, int $m) : Mat
{
    return array_fill(0, $n, array_fill(0, $m, 0.0));
}

/*
 * $n 行 $m 列の単位行列を作成する
 * 戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 */
function eye(int $n, int $m) : Mat
{
    $ret = zeros($n, $m);
    for ($i = 0; $i < min($n, $m); ++$i) {
        $ret[$i][$i] = 1.0;
    }
    return $ret;
}

function vector(array<float> $values) : Vec
{
    return $values;
}

function vector_size(Vec $v) : int
{
    return count($v);
}

/*
 * 行列の減算 $a - $b を計算する
 * $a, $b, および戻り値はすべて同じサイズの二次元配列
 * $array[$i][$j] が $i 行 $j 列要素
 */
function matrix_sub(Mat $a, Mat $b) : Mat
{
    $n = count($a);
    $m = count($a[0]);

    $result = array_fill(0, $n, array_fill(0, $m, 0));
    for ($i = 0; $i < $n; ++$i) {
        for ($j = 0; $j < $m; ++$j) {
            $result[$i][$j] = $a[$i][$j] - $b[$i][$j];
        }
    }

    return $result;
}

/*
 * 行列の積 $a * $b を計算する
 * $a, $b, および戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 * $a は n 行 p 列
 * $b は p 行 m 列
 * 戻り値は n 行 m 列
 */
function matrix_mul(Mat $a, Mat $b) : Mat
{
    $n = count($a);
    $p = count($a[0]); // == count($b);
    $m = count($b[0]);

    $result = array_fill(0, $n, array_fill(0, $m, 0));
    for ($i = 0; $i < $n; ++$i) {
        for ($j = 0; $j < $m; ++$j) {
            for ($k = 0; $k < $p; ++$k) {
                $result[$i][$j] += $a[$i][$k] * $b[$k][$j];
            }
        }
    }

    return $result;
}

/*
 * ベクトル $v の各要素にスカラー $n を足す
 * 引数 $v と戻り値は同じ長さの一次元配列
 */
function vector_scalar_add(Vec $v, float $n) : Vec
{
    return array_map(
        function ($vi) use ($n) { return $vi + $n; }, $v);
}

/*
 * ベクトル $v の各要素にスカラー $n を乗じる
 * 引数 $v と戻り値は同じ長さの一次元配列
 */
function vector_scalar_mul(Vec $v, float $n) : Vec
{
    return array_map(
        function ($vi) use ($n) { return $vi * $n; }, $v);
}

/*
 * ベクトル $a と $b の和を計算する
 * 引数 $a, $b および戻り値は同じ長さの一次元配列
 */
function vector_add(Vec $a, Vec $b) : Vec
{
    return array_map(
        function ($ai, $bi) { return (float)$ai + (float)$bi; }, $a, $b);
}

/*
 * ベクトルの積による行列を計算する
 * 引数 $a, $b は一次元配列
 * 戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 * 0 <= $i < count($a), 0 <= $j < count($b)
 */
function vector_mul(Vec $a, Vec $b) : Mat
{
    $n = count($a);
    $m = count($b);

    $result = array_fill(0, $n, array_fill(0, $m, 0));
    for ($i = 0; $i < $n; ++$i) {
        for ($j = 0; $j < $m; ++$j) {
            $result[$i][$j] = $a[$i] * $b[$j];
        }
    }

    return $result;
}

/*
 * $va と $vb の内積を計算する
 * 引数 $va, $vb は同じ長さの一次元配列
 */
function inner_product(Vec $va, Vec $vb) : float
{
    return array_sum(
        array_map(
            function ($a, $b) { return (float)$a * (float)$b; },
            $va,
            $vb));
}

/*
 * 行列 $m とベクトル $v の掛け算 ($m * $v)
 * $m は n 行 p 列, 二次元配列で $m[$i][$j] が $i 行 $j 列をあらわす
 * $v は p 要素の一次元配列
 * 戻り値は n 要素の一次元配列
 */
function matrix_vector_mul(Mat $m, Vec $v) : Vec
{
    $n = count($m);
    $p = count($m[0]); // == count($v)

    $result = array_fill(0, $n, 0);
    for ($i = 0; $i < $n; ++$i) {
        for ($j = 0; $j < $p; ++$j) {
            $result[$i] += $m[$i][$j] * $v[$j];
        }
    }

    return $result;
}

/*
 * ベクトル $v と行列 $m の掛け算 ($v * $m)
 * $v は p 要素の一次元配列
 * $m は p 行 n 列, 二次元配列で $m[$i][$j] が $i 行 $j 列をあらわす
 * 戻り値は n 要素の一次元配列
 */
function tvector_matrix_mul(Vec $v, Mat $m) : Vec
{
    $n = count($m[0]);
    $p = count($m); // == count($v);

    $result = array_fill(0, $n, 0);
    for ($i = 0; $i < $n; ++$i) {
        for ($j = 0; $j < $p; ++$j) {
            $result[$i] += $v[$j] * $m[$j][$i];
        }
    }

    return $result;
}

function vector_to_string(Vec $v) : string
{
    return implode(',', $v);
}
