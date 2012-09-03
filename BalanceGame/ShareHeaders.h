#ifndef _INC_SHARE_H_
#define _INC_SHARE_H_
#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x501
#ifdef USE_VC9
#define nullptr NULL
#endif
#import "C:/Windows/System32/msxml6.dll" named_guids raw_interfaces_only
#include <Windows.h>
#include <string>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <CommCtrl.h>
#include <Shlwapi.h>
#include <math.h>
#include <time.h>
#include <memory>
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"ComCtl32.lib")
#pragma comment(lib,"WinMM.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls'version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
using std::map;
using std::unique_ptr;
using std::pair;
using std::string;
using std::wstring;
#define Ctrl_ComboBox1 1000
#define Ctrl_Radio1    1001
#define Ctrl_Radio2    1002
#define Ctrl_Radio3    1003
#define Ctrl_GroupBox1 1004
#define Ctrl_GroupBox2 1005
#define Ctrl_Static1   1006
#define Ctrl_Static2   1007
#define Ctrl_Static3   1008
#define Ctrl_Static4   1009
#define Ctrl_TrackBar1 1010
#define Ctrl_Button1   1011
#define Ctrl_Button2   1012
#endif