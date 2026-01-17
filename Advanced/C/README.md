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