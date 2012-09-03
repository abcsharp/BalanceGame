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
			}else MessageBox(nullptr,L"Direct3D�f�o�C�X�̏������Ɏ��s���܂����B\n"
									 L"���̃R���s���[�^�[�ł̓Q�[���𓮂������߂�\n"
									 L"�K�v�Ȑ��\�𖞂����Ă��Ȃ��\��������܂��B\n"
									 L"��荂���\�ȃR���s���[�^�[�ŃQ�[�����s�����������߂��܂��B",
									 L"",
									 MB_OK|MB_ICONWARNING);
		}else MessageBox(nullptr,L"�E�B���h�E�̏������Ɏ��s���܂����B",L"",MB_OK|MB_ICONWARNING);
	}else MessageBox(nullptr,L"�ݒ�̓ǂݍ��݂Ɏ��s���܂����B",L"",MB_OK|MB_ICONWARNING);
	ReleaseMutex(Mutex);
	return 0;
}