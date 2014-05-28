<?hh

require 'matrix.hh';
require 'misc.hh';

class SCW {

    private float $c;
    private float $eta;
    private Vec $mu;
    private Mat $sigma;

    private float $alpha;
    private float $beta;
    private float $xtsx;
    private float $ymux;
    private float $phi;
    private float $vphi;

    public function __construct(float $c, float $eta)
    {
        $ndim = 1;

        $this->c = $c;
        $this->eta = $eta;
        $this->mu = vector(array_fill(0, $ndim, 0.0));
        $this->sigma = eye($ndim, $ndim);
        $this->alpha = 0.0;
        $this->beta = 0.0;
        $this->xtsx = 0.0;
        $this->ymux = 0.0;

        // phi, varphi は eta のみに依存するので先に計算する
        $this->phi = norminv($this->eta, 0.0, 1.0);
        $this->vphi = 1.0 + pow($this->phi, 2.0) / 2;
    }

    public function train(array<Vec> $xs, array<int> $ys) : void
    {
        $dim = vector_size($xs[0]);

        $this->initialize($dim);
        for ($i = 0; $i < count($xs); ++$i) {
            $x = $xs[$i];
            $y = $ys[$i];
            $this->step($x, $y);
        }
    }

    public function test(array<Vec> $xs, array<int> $ys) : void
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

    private function initialize(int $ndim) : void
    {
        $this->mu = vector(array_fill(0, $ndim, 0.0));
        $this->sigma = eye($ndim, $ndim);
    }

    private function step(Vec $x, int $y) : void
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

    private function predict(Vec $x) : int
    {
        return sgn(inner_product($this->mu, $x));
    }

    private function calc_loss(Vec $x, int $y) : float
    {
        $this->xtsx = $this->calc_xt_sigma_x($x);
        $this->ymux = $this->calc_y_mu_x($x, $y);
        $loss = $this->phi * sqrt($this->xtsx) - $this->ymux;
        return max(0.0, $loss);
    }

    private function update_mu(Vec $x, int $y) : void
    {
        $a = $this->alpha;

        $this->mu =
            vector_add(
                $this->mu,
                vector_scalar_mul(
                    matrix_vector_mul($this->sigma, $x),
                    $a * $y));
    }

    private function update_sigma(Vec $x, int $y) : void
    {
        $a = $this->alpha;
        $b = $this->beta;

        $this->sigma =
            matrix_sub(
                $this->sigma,
                $this->calc_b_sigma_x_xt_sigma($x));
    }

    private function calc_alpha(Vec $x, int $y) : float
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

    private function calc_beta(Vec $x, int $y) : float
    {
        $a = $this->alpha;

        $v = $this->xtsx;
        $u = $this->calc_u($v);

        $numer = $a * $this->phi;
        $denom = sqrt($u) + $v * $a * $this->phi;

        return $numer / $denom;
    }

    private function calc_xt_sigma_x(Vec $x) : float
    {
        return inner_product(
            tvector_matrix_mul($x, $this->sigma),
            $x);
    }

    private function calc_b_sigma_x_xt_sigma(Vec $x) : Mat
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

    private function calc_y_mu_x(Vec $x, int $y) : float
    {
        return $y * inner_product($this->mu, $x);
    }

    private function calc_u(float $v) : float
    {
        $a = $this->alpha;
        $t = $a * $v * $this->phi;

        return pow(-$t + sqrt($t * $t + 4 * $v), 2) / 4.0;
    }
}
