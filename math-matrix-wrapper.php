<?php

require_once 'Math/Matrix.php';

/*
 * $n 行 $m 列の零行列を作成する
 * 戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 */
function zeros($n, $m)
{
    return Math_Matrix::makeZero($n, $m);
}

/*
 * $n 行 $m 列の単位行列を作成する
 * 戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 */
function eye($n, $m)
{
    if ($n == $m) {
        return Math_Matrix::makeUnit($n);
    } else {
        $ret = Math_Matrix::makeZero($n, $m);
        $ret->add(Math_Matrix::makeUnit(min($n, $m)));
        return $ret;
    }
}

function vector($values)
{
    $nrow = count($values);
    $ncol = 1;
    $ret = Math_Matrix::makeZero($nrow, $ncol);
    for ($i = 0; $i < $nrow; ++$i) {
        $ret->setElement($i, 0, $values[$i]);
    }
    return $ret;
}

function vector_size($v)
{
    $size = $v->getSize();
    return $size[0];
}

/*
 * 行列の減算 $a - $b を計算する
 * $a, $b, および戻り値はすべて同じサイズの二次元配列
 * $array[$i][$j] が $i 行 $j 列要素
 */
function matrix_sub($a, $b)
{
    $ret = $a->cloneMatrix();
    $ret->sub($b);
    return $ret;
}

/*
 * 行列の積 $a * $b を計算する
 * $a, $b, および戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 * $a は n 行 p 列
 * $b は p 行 m 列
 * 戻り値は n 行 m 列
 */
function matrix_mul($a, $b)
{
    $ret = $a->cloneMatrix();
    $ret->multiply($b);
    return $ret;
}

/*
 * ベクトル $v の各要素にスカラー $n を足す
 * 引数 $v と戻り値は同じ長さの一次元配列
 */
function vector_scalar_add($v, $n)
{
    $size = $v->size();
    $ret = $v->cloneMatrix();
    $ret->add(Math_Matrix::makeOne($size[0], $size[1])->scale($n));
    return $ret;
}

/*
 * ベクトル $v の各要素にスカラー $n を乗じる
 * 引数 $v と戻り値は同じ長さの一次元配列
 */
function vector_scalar_mul($v, $n)
{
    $ret = $v->cloneMatrix();
    $ret->scale($n);
    return $ret;
}

/*
 * ベクトル $a と $b の和を計算する
 * 引数 $a, $b および戻り値は同じ長さの一次元配列
 */
function vector_add($a, $b)
{
    $ret = $a->cloneMatrix();
    $ret->add($b);
    return $ret;
}

/*
 * ベクトルの積による行列を計算する
 * 引数 $a, $b は一次元配列
 * 戻り値は二次元配列で $array[$i][$j] が $i 行 $j 列要素
 * 0 <= $i < count($a), 0 <= $j < count($b)
 */
function vector_mul($a, $b)
{
    $ret = $a->cloneMatrix();
    $bt = $b->cloneMatrix();
    $bt->transpose();
    $ret->multiply($bt);
    return $ret;
}

/*
 * $va と $vb の内積を計算する
 * 引数 $va, $vb は同じ長さの一次元配列
 */
function inner_product($va, $vb)
{
    $ret = $va->cloneMatrix();
    $size = $ret->getSize();
    if ($size[0] > 1) {
        $ret->transpose();
    }
    $ret->multiply($vb);
    return $ret->getElement(0, 0);
}

/*
 * 行列 $m とベクトル $v の掛け算 ($m * $v)
 * $m は n 行 p 列, 二次元配列で $m[$i][$j] が $i 行 $j 列をあらわす
 * $v は p 要素の一次元配列
 * 戻り値は n 要素の一次元配列
 */
function matrix_vector_mul($m, $v)
{
    $ret = $m->cloneMatrix();
    $ret->multiply($v);
    return $ret;
}

/*
 * ベクトル $v と行列 $m の掛け算 ($v * $m)
 * $v は p 要素の一次元配列
 * $m は p 行 n 列, 二次元配列で $m[$i][$j] が $i 行 $j 列をあらわす
 * 戻り値は n 要素の一次元配列
 */
function tvector_matrix_mul($v, $m)
{
    $ret = $v->cloneMatrix();
    $ret->transpose();
    $ret->multiply($m);
    return $ret;
}

function vector_to_string($v)
{
    return implode(',', $v->getCol(0));
}
