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
- `EnumProcessModule`  
**プロセスが読み込んだモジュールを取得する**  
第一引数にプロセスのハンドル(HANDLE)  
第二引数にモジュールを書き込む変数のアドレス(HMODULEの配列)  
第三引数に配列のサイズ(DWORD, sizoef)  
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
情報を聞く権限
- `PROCESS_VM_READ`  
メモリを読み取る権限