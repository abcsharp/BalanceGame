#include "Program.h"
#include "resource.h"

bool Program::InitWindow(void)
{
	ZeroMemory(&MainWindowClass,sizeof(WNDCLASSEX));
	MainWindowClass.cbSize=sizeof(WNDCLASSEX);
	MainWindowClass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	MainWindowClass.hCursor=LoadCursor(nullptr,IDC_ARROW);
	MainWindowClass.hInstance=Instance;
	MainWindowClass.hIcon=LoadIcon(Instance,MAKEINTRESOURCE(IDI_ICON1));
	MainWindowClass.hIconSm=(HICON)LoadImage(Instance,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	MainWindowClass.style=CS_VREDRAW|CS_HREDRAW;
	MainWindowClass.lpfnWndProc=WndProc;
	MainWindowClass.lpszClassName=L"BalanceGame";
	MainWindowClass.lpszMenuName=nullptr;
	if(!RegisterClassEx(&MainWindowClass)) return false;
	if(!(MainWindow=CreateWindow(L"BalanceGame",L"BalanceGame",
		WS_OVERLAPPEDWINDOW-WS_MAXIMIZEBOX-WS_THICKFRAME,MainWindowPos.x,MainWindowPos.y,640,480,
		nullptr,nullptr,Instance,nullptr))) return false;
	return Window=true;
}

bool Program::InitDirect3D(void)
{
	unsigned Levels[]={0,2,4,8,16};
	D3DDISPLAYMODE Display;
	Params.BackBufferCount=1;
	Params.BackBufferFormat=D3DFMT_A8R8G8B8;
	Params.EnableAutoDepthStencil=true;
	Params.AutoDepthStencilFormat=D3DFMT_D16;
	Params.Windowed=true;
	Params.SwapEffect=D3DSWAPEFFECT_DISCARD;
	if((Direct3D=Direct3DCreate9(D3D_SDK_VERSION))==nullptr) return false;
	for(int i=sizeof(Levels)/4-1;i>=0;i--){
		if(Direct3D->CheckDeviceMultiSampleType(0,D3DDEVTYPE_HAL,D3DFMT_A8R8G8B8,true,(D3DMULTISAMPLE_TYPE)Levels[i],nullptr)==D3D_OK){
			MultiSampleLevel=Levels[i];
			break;
		}
	}
	Params.MultiSampleType=(D3DMULTISAMPLE_TYPE)MultiSampleLevel;
	Direct3D->GetAdapterDisplayMode(0,&Display);
	if(Display.RefreshRate==60) Params.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;
	else Params.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
	if(Direct3D->CreateDevice(0,D3DDEVTYPE_HAL,MainWindow,VertexProcessing=D3DCREATE_HARDWARE_VERTEXPROCESSING,&Params,&Device)<0){
		if(Direct3D->CreateDevice(0,D3DDEVTYPE_HAL,MainWindow,VertexProcessing=D3DCREATE_SOFTWARE_VERTEXPROCESSING,&Params,&Device)<0){
			return false;
		}
	}
	if(!InitModels()||!InitImages()||!InitRenderState()) return false;
	GetPlateVertices();
	InitLight();
	InitViewTransform();
	return true;
}

bool Program::InitRenderState(void)
{
	if(Device->BeginStateBlock()<0) return false;
    Device->SetRenderState(D3DRS_ZENABLE,true);  
	Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	Device->SetRenderState(D3DRS_LIGHTING,true);
	Device->SetRenderState(D3DRS_AMBIENT,0x00555555);
	Device->SetRenderState(D3DRS_SPECULARENABLE,true);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	Device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
	if(Device->EndStateBlock(&ModelRenderState)<0) return false;
	if(Device->BeginStateBlock()<0) return false;
	Device->SetRenderState(D3DRS_LIGHTING,false);
	Device->SetRenderState(D3DRS_SPECULARENABLE,false);
	Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_ADD);
	Device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	Device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
	Device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	Device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	if(Device->EndStateBlock(&ImageRenderState)<0) return false;
	return true;
}

bool Program::InitModels(void)
{
	Ball=Model::Load(X_BALL,Device);
	Ball->VecPosition.y=1.0f;
	Plate=Model::Load(X_TRIANGLE+PlateType,Device);
	if(!Ball||!Plate) return false;
	return true;
}

bool Program::InitImages(void)
{
	LoadTextures();
	Number=NumberRender::Load(PNG_NUMBERS,Device,30,43,0xFFFFFFFF);
	InitImagePosition();
	return true;
}

void Program::LoadTextures(void)
{
	Images.insert(pair<string,Image*>("Title",Image::Load(PNG_TITLE,Device,363,64,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("GameStart",Image::Load(PNG_GAMESTART,Device,196,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("ViewHighScore",Image::Load(PNG_VIEWHIGHSCORE,Device,274,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("Options",Image::Load(PNG_OPTIONS,Device,156,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("Quit",Image::Load(PNG_QUIT,Device,79,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("GameOver",Image::Load(PNG_GAMEOVER,Device,325,64,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("Replay",Image::Load(PNG_REPLAY,Device,132,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("Yes",Image::Load(PNG_YES,Device,56,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("No",Image::Load(PNG_NO,Device,48,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("Arrow",Image::Load(PNG_ARROW,Device,51,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("Score",Image::Load(PNG_SCORE,Device,99,43,0xFFFFFFFF)));
	Images.insert(pair<string,Image*>("Point",Image::Load(PNG_POINT,Device,56,43,0xFFFFFFFF)));
	return;
}

void Program::InitImagePosition(void)
{
	RECT Rect;
	GetClientRect(MainWindow,&Rect);
	float XPos=300.0f,YPos=220.0f;
	Images["Title"]->VecPosition=D3DXVECTOR3((Rect.right-Images["Title"]->GetSizeX())/2,80,0);
	Images["Arrow"]->VecPosition=D3DXVECTOR3(XPos-Images["Arrow"]->GetSizeX(),YPos,0);
	Images["GameStart"]->VecPosition=D3DXVECTOR3(XPos,YPos,0);
	Images["ViewHighScore"]->VecPosition=D3DXVECTOR3(XPos,YPos+=Images["GameStart"]->GetSizeY(),0);
	Images["Options"]->VecPosition=D3DXVECTOR3(XPos,YPos+=Images["ViewHighScore"]->GetSizeY(),0);
	Images["Quit"]->VecPosition=D3DXVECTOR3(XPos,YPos+=Images["Options"]->GetSizeY(),0);
	Images["GameOver"]->VecPosition=D3DXVECTOR3(50,100,0);
	Images["Replay"]->VecPosition=D3DXVECTOR3(200,200,0);
	Images["Yes"]->VecPosition=D3DXVECTOR3(400,200,0);
	Images["No"]->VecPosition=D3DXVECTOR3(525,200,0);
	Alpha(0);
	Alpha(5*DrawMode);
	return;
}

void Program::InitPath(void)
{
	SettingPath=L"BalanceGameSetting.xml";
	SaveDataPath=L"BalanceGameSaveData.data";
	ShadowDataPath=L"Temporary_BalanceGame.xml";
	const int MaxStringLength=256;
	unique_ptr<wchar_t[]> TempString(new wchar_t[MaxStringLength]);
	const bool EndIsSplitChar=(TempString.get()[GetCurrentDirectory(MaxStringLength,TempString.get())]==L'\\')?(true):(false);
	if(!EndIsSplitChar){
		SettingPath.insert(0,L"\\");
		SaveDataPath.insert(0,L"\\");
	}
	SettingPath.insert(0,TempString.get());
	SaveDataPath.insert(0,TempString.get());
	GetTempPath(256,TempString.get());
	ShadowDataPath.insert(0,TempString.get());
	return;
}

bool Program::Recover(void)
{
	Ball->Dispose();
	Plate->Dispose();
	Number->Dispose();
	map<string,Image*>::iterator Obj=Images.begin();
	for(;Obj!=Images.end();Obj++) (*Obj).second->Dispose();
	IDirect3DSurface9* Surface;
	Device->GetRenderTarget(0,&Surface);
	Surface->Release();
	ImageRenderState->Release();
	ImageRenderState=nullptr;
	ModelRenderState->Release();
	ModelRenderState=nullptr;
	if(Device->Reset(&Params)<0) return false;
	Ball->Recover();
	Plate->Recover();
	Number->Recover();
	for(;Obj--!=Images.begin();) (*Obj).second->Recover();
	if(!InitRenderState()) return false;
	ClearGameData();
	Draw=&Program::DrawTitleMenu;
	InitImagePosition();
	return true;
}