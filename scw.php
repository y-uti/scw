<?php

// require 'math-matrix-wrapper.php';
require 'matrix.php';

require 'misc.php';

class SCW {

    function __construct($c, $eta)
    {
        $this->c = $c;
        $this->eta = $eta;

        // phi, varphi は eta のみに依存するので先に計算する
        $this->phi = norminv($this->eta, 0, 1);
        $this->vphi = 1 + pow($this->phi, 2) / 2;
    }

    function train($xs, $ys)
    {
        $dim = vector_size($xs[0]);

        $this->initialize($dim);
        for ($i = 0; $i < count($xs); ++$i) {
            $x = $xs[$i];
            $y = $ys[$i];
            $this->step($x, $y);
        }
    }

    function test($xs, $ys)
    {
        $total = count($xs);
        $error = 0;
        for ($i = 0; $i < count($xs); ++$i) {
            $x = $xs[$i];
            $y = $ys[$i];
            if ($this->predict($x) != $y) {
                ++$error;
            }
        }

        print "error rate = $error / $total\n";
    }

    function initialize($ndim)
    {
        $this->mu = zeros($ndim, 1);
        $this->sigma = eye($ndim, $ndim);
    }

    function step($x, $y)
    { 
        $loss = $this->calc_loss($x, $y);
        if ($loss > 0) {
            $this->alpha = $this->calc_alpha($x, $y);
            $this->beta = $this->calc_beta($x, $y);
            $this->update_mu($x, $y);
            $this->update_sigma($x, $y);
            print "mu = " . vector_to_string($this->mu) . "\n";
        }
    }

    function predict($x)
    {
        return sgn(inner_product($this->mu, $x));
    }

    function calc_loss($x, $y)
    {
        $this->xtsx = $this->calc_xt_sigma_x($x);
        $this->ymux = $this->calc_y_mu_x($x, $y);
        $loss = $this->phi * sqrt($this->xtsx) - $this->ymux;
        return max(0, $loss);
    }

    function update_mu($x, $y)
    {
        $a = $this->alpha;

        $this->mu =
            vector_add(
                $this->mu,
                vector_scalar_mul(
                    matrix_vector_mul($this->sigma, $x),
                    $a * $y));
    }

    function update_sigma($x, $y)
    {
        $a = $this->alpha;
        $b = $this->beta;

        $this->sigma =
            matrix_sub(
                $this->sigma,
                $this->calc_b_sigma_x_xt_sigma($x));
    }

    function calc_alpha($x, $y)
    {
        $v = $this->xtsx;
        $g = 1 + $this->phi * $this->phi;
        $m = $this->ymux;
        $alpha = 1 / ($v * $g) * (- $m * $this->vphi +
                                  sqrt($m * $m * pow($this->phi, 4) / 4 + $v * pow($this->phi, 2) * $g));
        $alpha = max(0, $alpha);
        $alpha = min($this->c, $alpha);

        return $alpha;
    }

    function calc_beta($x, $y)
    {
        $a = $this->alpha;

        $v = $this->xtsx;
        $u = $this->calc_u($v);

        $numer = $a * $this->phi;
        $denom = sqrt($u) + $v * $a * $this->phi;

        return $numer / $denom;
    }

    function calc_xt_sigma_x($x)
    {
        return inner_product(
            tvector_matrix_mul($x, $this->sigma),
            $x);
    }

    function calc_b_sigma_x_xt_sigma($x)
    {
        $b = $this->beta;

        return
            matrix_mul(
                matrix_mul(
                    $this->sigma,
                    vector_mul(
                        vector_scalar_mul($x, $b),
                        $x)),
                $this->sigma);
    }

    function calc_y_mu_x($x, $y)
    {
        return $y * inner_product($this->mu, $x);
    }

    function calc_u($v)
    {
        $a = $this->alpha;
        $t = $a * $v * $this->phi;

        return pow(-$t + sqrt($t * $t + 4 * $v), 2) / 4;
    }
}

function main($argc, $argv)
{
    if ($argc < 4) {
        print "Usage: " . $argv[0] . "filename C eta\n";
        return;
    }

    $file = $argv[1];
    list ($x, $y) = read_data($file);

    $c = $argv[2];
    $eta = $argv[3];

    $scw = new SCW($c, $eta);
    $scw->train($x, $y);

    if ($argc == 5) {
        $testfile = $argv[4];
        list ($tx, $ty) = read_data($testfile);
    } else {
        $tx = $x;
        $ty = $y;
    }

    $scw->test($tx, $ty);
}

function read_data($file)
{
    $data = file($file, FILE_IGNORE_NEW_LINES);

    $y = array_map(
        function ($v) { return (int) $v; },
        explode(',', $data[0]));

    array_shift($data);

    $x = array_map(
        function ($l) {
            return vector(explode(',', $l));
        }, $data);

    return array($x, $y);
}

main($argc, $argv);
