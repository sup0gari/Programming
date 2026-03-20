# Cの応用
## Day1
Win32 APIを使用し、実行中のプロセスのIDと名前, 読み込んだモジュールを取得する。
### Win32 API
- `EnumProcesses`  
**実行中のPIDをすべて取得する**  
第一引数にPIDを書き込む配列(DWORD)  
第二引数は第一引数の配列のサイズ(DWORD, sizeof)  
第三引数にOSが第一引数の配列に書き込んだメモリ数を書く変数(LPDWORD)
- `OpenProcess`  
**特定のプロセスのハンドルを取得する**  
第一引数に権限の種類(DWORD)  
第二引数にハンドルを引き継ぐかどうか(BOOL)  
第三引数にどのプロセスか、PID(DWORD)
- `EnumProcessModules`  
**プロセスが読み込んだモジュールを取得する**  
第一引数にプロセスのハンドル(HANDLE)  
第二引数にモジュールを書き込む変数のアドレス(HMODULEの配列)  
第三引数に配列のサイズ(HMODULE, sizoef)  
第四引数にOSが何バイト分書き込んだかのメモリ数を書き込む変数(LPDWORD)  
- `GetModuleBaseName`  
**HMODULEをファイル名に変換する**  
第一引数にプロセスのハンドル(HANDLE)  
第二引数にモジュールの識別番号(HMODULE)  
第三引数にファイル名を書き込む配列(LPTSTR)  
第四引数に第三引数の配列に何バイト書き込むか(DWORD)
- `CloseHandle`  
**メモリリークを防ぐためにハンドルを返却する**  
第一引数に返却するハンドル(HANDLE)
### 型、関数、定数
- `DWORD`  
unsigned intと同じ、4byte
- `HMODULE`  
void *と同じ、モジュール名のポインタ
- `TCHAR`  
char, wchar_tと同じ、全角と半角文字
- `HANDLE`  
void *と同じ、ハンドルのポインタ
- `_tcsicmp`  
大文字小文字を区別しない文字列比較
- `LPDWORD`  
DWORDのポインタ
- `LPTSTR`  
TCHARの配列のポインタ
- `PROCESS_QUERY_INFORMATION`
- `PROCESS_VM_READ`

## Day2
特定のプロセスのメモリ空間に1KBを確保し、そのアドレスを表示する。
### Win32 API
- `VirtualAllocEx`  
**外部プロセスのメモリを確保する**  
第一引数にプロセスのハンドル(HANDLE)  
第二引数に確保したい場所の希望アドレス(LPVOID)  
第三引数に確保するメモリのサイズ(SIZE_T)  
第四引数に割り当ての種類。(DWORD)  
第五引数にメモリの保護属性(DWORD)  
- `VirtualFreeEx`  
**確保したメモリを開放する**
第一引数にプロセスのハンドル(HANDLE)  
第二引数に解放したい領域の先頭アドレス(LPVOID)  
第三引数に解放するサイズ(SIZE_T)  
第四引数に解放の種類(DWORD)  
- `GetLastError`  
**直前のAPIエラーコードを取得する**
### 型、関数、定数
- `_fgetts`  
標準入力から文字列を取得する
- `getchar`  
Enterキーが押されたかどうかを判定する
- `SIZE_T`  
32bit環境なら4byte, 64bit環境なら8byteと可変型  
- `MEM_COMMIT`
- `MEM_RESERVE`
- `PAGE_READWRITE`
- `MEM_RELEASE`

## Day3
`ntdll.dll`にあるWin32 APIのハンドルを取得し、先頭の数バイトを表示する。
### Win32 API
- `GetModuleHandleA`  
**読み込んでいるモジュールのベースアドレスを取得**  
第一引数にモジュール名(LPCSTR)
- `GetProcAddress`  
**読み込んでいるモジュールにある関数のアドレスを取得**  
第一引数にモジュールのベースアドレス(HMODULE)  
第二引数にモジュール内の関数名(LPCSTR)
### Window標準のシステムコール
5バイト目にSSN(syscall number)がある。SSNは4byte(DWORD)である。  
このSSNによって実行する内容が異なる。EDRなどにシステムコールをフックされている場合はこの8byteの部分が異なる可能性があるため、攻撃者は先頭8byteを書き換えてバイパスする。
ユーザーモードで動くプログラムは`ntdll.dll`を介してシステムコールする。
```assembly
mov r10,rcx   ; 0x4C 0x8B 0xD1
mov eax,<SSN> ; 0xB8 <SSN>
syscall       ; 0x0F 0x05
```

## Day4
NT HeadersでWin32 APIのアドレスを特定し、先頭数バイトを確認して、フックされているものを列挙する。
- PE(Portable Excutable)  
Windowsにおける`.exe`や`.dll`の共通フォーマットで、プログラム実行時にどこにコードがあり、どこに関数リストがあるかを知るための情報がファイルやメモリの先頭に必ず書き込まれている
1. DOS Header  
すべてのWindowsファイルの先頭にある互換性維持用のヘッダー  
`e_lfanew`という関数にNT Headersへのオフセットが記載されている  
2. NT Headers  
DLLのビット数や、関数の場所などが記されている部分  
`DataDirectory`という配列の0番目にExport Directoryという関数のアドレスリストへのベースアドレスからのオフセットが書かれている。
3. Sections  
`.text`や`.data`などの領域

## Day5
`amsi.dll`のAPIである`AmsiScanBuffer`の先頭バイトを書き換え、強制的にスキャン結果を「安全」で返す。
1. `LoadLibraryA`で`amsi.dll`のベースアドレスを取得
2. `GetProcAddress`で`AmsiScanBuffer`のアドレスを特定
3. `VirtualProtect`で書き込み権限を付与して、`memcpy`でメモリにパッチを当てる
4. `VirtualProtect`で権限を元に戻す

## Day6
自作のDLLを作成し、任意のプロセスにロードすることでそのプロセスの権限でペイロードを実行する。  
`CreateRemoteThread`というAPIを使用し、ターゲットプロセスの中に新しいスレッドを作成し、`LoadLibraryA`を実行させて自作のDLLを読み込ませる。  
DLLには実行ファイルの`main`にあたる`DllMain`というエントリポイントがある。下記dll作成コマンド。
```cmd
cl.exe /LD day6_1.c /Fe:day6_1.dll
```

## Userhook
任意のプロセスのNative apiをフックし、呼び出されたときに通知するプログラムを作成する。
### Step1
インジェクターとダミーのDLLの作成  
指定したプロセスに任意のメモリ領域を確保し、DLLを読み込ませる。
```powershell
cl.exe /LD userhook_dummy.c /link /OUT:C:\tmp\userhook_dummy.dll
cl.exe step1.c /OUT:step1.exe
```
### Step2
フックと、通知機能を実装したDLLの作成  
### Step3
Step1とStep2の統合。

## Kernel Driver
カーネルドライバーを自作する。
### Step1
デバッガへ文字列を出力するだけのカーネルドライバー  
カーネルドライバーは以下の３つが必要
- `main.c`
- `sources`
- `makefile`  
1. ビルド
```powershell
cl /c /Zi /nologo /W3 /WX- /Od /Oi /D _AMD64_ /D _KERNEL_MODE /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\km" main.c

link /NODEFAULTLIB /INCREMENTAL:NO /SUBSYSTEM:NATIVE /DRIVER /ENTRY:DriverEntry /OUT:Step1.sys main.obj "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\km\x64\ntoskrnl.lib"
```
2. ロード  
- ※必ず仮想環境でセキュアブートをオフにする
- スタートアップ設定でDSEを無効にする
```powershell
bcdedit /set testsigning on
sc.exe create Step1 type= kernel binPath= "C:\Users\sup0gari\Desktop\Step1.sys"
sc.exe start Step1
sc.exe stop Step1
```