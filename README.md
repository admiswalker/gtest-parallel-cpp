# sample_separate-compilation-of-googletest

このサンプルでは，GoogleTest で記述されたテストファイルごとにコンパイルして，
エントリーポイントとなる main.cpp よりまとめて実行する．

テストファルが長大になった場合に，分割コンパイルでコンパイル時間を短縮することが目的である．
また，OpenMP で並列化しており，コンパイル単位で並列実行するようにしてある．

## 注意点

エラー自体は popen の戻り値から取得しており，エラーの発生に気が付かない可能性は低いが，
エラーの詳細な数は文字列置換でカウントしているので，googletest のバージョンアップすると，正常に動作しなくなる可能性がある．

## 参考資料

- [Google Test ではテストの並列実行をサポートしていますか？](http://opencv.jp/googletestdocs/FAQ.html#faq-does-google-test-support-in-parallel)

