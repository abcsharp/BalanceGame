#include "ShareHeaders.h"
#include "Program.h"

unique_ptr<Program> Game;

int __stdcall wWinMain(HINSTANCE,HINSTANCE,LPTSTR,int)
{
	HANDLE Mutex=CreateMutex(nullptr,true,L"BalanceGame");
	if(GetLastError()==ERROR_ALREADY_EXISTS) return 0;
	Game.reset(Program::GetInstance());
	if(Game->InitSetting()){
		if(Game->InitWindow()){
			if(Game->InitDirect3D()){
				Game->MessageLoop();
			}else MessageBox(nullptr,L"Direct3Dデバイスの初期化に失敗しました。\n"
									 L"このコンピューターではゲームを動かすために\n"
									 L"必要な性能を満たしていない可能性があります。\n"
									 L"より高性能なコンピューターでゲームを行う事をお勧めします。",
									 L"",
									 MB_OK|MB_ICONWARNING);
		}else MessageBox(nullptr,L"ウィンドウの初期化に失敗しました。",L"",MB_OK|MB_ICONWARNING);
	}else MessageBox(nullptr,L"設定の読み込みに失敗しました。",L"",MB_OK|MB_ICONWARNING);
	ReleaseMutex(Mutex);
	return 0;
}