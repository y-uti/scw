<?hh

require 'scw.hh';

/*
 * main 関数
 */
function main(int $argc, array<string> $argv) : void
{
    if ($argc < 4) {
        print "Usage: " . $argv[0] . "filename C eta\n";
        return;
    }

    $file = $argv[1];
    list ($x, $y) = read_data($file);

    $c = floatval($argv[2]);
    $eta = floatval($argv[3]);

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

/*
 * CSV ファイルの読み込み
 */
function read_data(string $file) : (array<Vec>, array<int>)
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

    return tuple($x, $y);
}

main($argc, $argv);
