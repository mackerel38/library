# verify

公開問題へ提出するコードを`verify/<サービス名>_<問題名>.cpp`へ置きます。
`oj-verify`で自動接続する既存コードは`*.test.cpp`の名前を維持します。

一つのverifierは、原則として一つのライブラリ部品を直接検証します。
`.verify-helper/config.toml`でGNU++23と`-Icp`を明示し、verification-helper既定のC++17へ戻らず、`cp`をinclude rootとして解決できるようにします。

新規の`*.cpp`は提出用コードとGNU++23コンパイル確認までをこちらで行い、judgeへの提出とAC確認は利用者が行います。
AC結果を受け取るまではdocsの状態を`judge AC未確認`のままにします。

手動提出が必要な問題は`verify/manual.md`のチェックリストで管理します。
