scw
===

PHP での SCW の実装

「第一回機械学習アルゴリズム実装会」で作ったものです。

http://connpass.com/event/6178/


### 実行方法

```
$ php scw.php digits_train.csv 1 0.5 digits_test.csv
```

引数の意味は以下のとおりです。

- digits_train.csv = 学習データ
- 1 = C
- 0.5 = eta
- digits_test.csv = テストデータ
