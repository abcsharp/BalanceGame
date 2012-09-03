#include "Program.h"
#include "resource.h"

Program::Program(void)
{
	Direct3D=nullptr;
	Device=nullptr;
	Setting=Ranking=nullptr;
	Ball=Plate=nullptr;
	PlateVertices=nullptr;
	PlateIndex=nullptr;
	Number=nullptr;
	ImageRenderState=ModelRenderState=nullptr;
	ZeroMemory(&Nodes,sizeof(Nodes));
	ZeroMemory(&Light,sizeof(Light));
	ZeroMemory(&Params,sizeof(Params));
	ZeroMemory(&Current,sizeof(Current));
	Window=GameOver=AddPoints=IsDeviceLose=FrameCount=0;
	ClearColor=0xFF000000;
	SelectedMode=-1;
	Alpha(0);
	DefaultFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	int Size=sizeof(DLGTEMPLATE)+sizeof(unsigned short)*4;
	DialogTemplate=malloc(Size);
	ZeroMemory(DialogTemplate,Size);
	ZeroMemory(KeyState,sizeof(KeyState));
	DLGTEMPLATE Template={WS_OVERLAPPEDWINDOW-WS_THICKFRAME-WS_MINIMIZEBOX-WS_MAXIMIZEBOX};
	memcpy(DialogTemplate,&Template,sizeof(DLGTEMPLATE));
	Draw=&Program::DrawTitleMenu;
	VecEyePoint=D3DXVECTOR3(0,1,-1);
	VecLookAtPoint=D3DXVECTOR3(0,0,0);
	VecUp=D3DXVECTOR3(0,1,0);
	InitPath();
	Instance=GetModuleHandle(nullptr);
	CoInitialize(nullptr);
	return;
}

Program::Program(const Program& RefInstance){}

Program Program::operator = (const Program& RefInstance)
{
	return Program();
}

Program::~Program(void)
{
	DeleteFile(ShadowDataPath.c_str());
	Setting=nullptr;
	Ranking=nullptr;
#ifndef USE_VC9
	for(auto Obj=Images.begin();Obj!=Images.end();Obj++)
#else 
	for(map<string,Image*>::iterator Obj=Images.begin();Obj!=Images.end();Obj++)
#endif
	delete Obj->second;
	for(int i=0;i<sizeof(Nodes)/sizeof(MSXML2::IXMLDOMNodePtr);i++) Nodes[i]=nullptr;
	if(Direct3D) Direct3D->Release();
	if(Device) Device->Release();
	if(Ball) delete Ball;
	if(Plate) delete Plate;
	if(ImageRenderState) ImageRenderState->Release();
	if(ModelRenderState) ModelRenderState->Release();
	if(PlateVertices) delete[] PlateVertices;
	if(PlateIndex) delete[] PlateIndex;
	if(Number) delete Number;
	free(DialogTemplate);
	CoUninitialize();
	return;
}

Program* Program::GetInstance(void)
{
	static Program* ClassInstance=new Program();
	return ClassInstance;
}

LRESULT __stdcall Program::WndProc(HWND Parent,UINT Message,WPARAM wParam,LPARAM lParam)
{
	static Program* Game=Program::GetInstance();
	switch(Message){
	case WM_KEYDOWN:
		if(Game->Draw!=&Program::DrawGameContent){
			if(wParam<VK_SELECT&&wParam>VK_HOME){
				Game->KeyState[wParam-VK_LEFT]=true;
			}else if(wParam==VK_RETURN) Game->KeyState[4]=true;
		}
		if(Game->IsDeviceLose){
			if(Game->Recover()) Game->IsDeviceLose=false;
			else SendMessage(Game->MainWindow,WM_CLOSE,0,0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		Game->Window=false;
		RECT Rect;
		GetWindowRect(Parent,&Rect);
		Game->MainWindowPos.x=Rect.left;
		Game->MainWindowPos.y=Rect.top;
		if(!Game->SaveSetting()){
			MessageBox(Parent,L"Ý’è‚Ì•Û‘¶‚ÉŽ¸”s‚µ‚Ü‚µ‚½B",L"",MB_OK|MB_ICONWARNING);
			PostQuitMessage(0);
		}
	default:
		return DefWindowProc(Parent,Message,wParam,lParam);
	}
	return 0;
}

INT_PTR __stdcall Program::SettingProc(HWND Parent,UINT Message,WPARAM wParam,LPARAM lParam)
{
	static Program* Game=Program::GetInstance();
	switch (Message)
	{
	case WM_INITDIALOG:
		Game->InitSettingWindow(Parent);
		return (INT_PTR)true;
	case WM_CLOSE:
		Game->SetSetting();
		EndDialog(Parent,0);
		break;
	}
	return (INT_PTR)false;
}

INT_PTR __stdcall Program::ScoreViewProc(HWND Parent,UINT Message,WPARAM wParam,LPARAM lParam)
{
	static Program* Game=Program::GetInstance();
	switch(Message){
	case WM_INITDIALOG:
		Game->InitScoreViewWindow(Parent);
		return (INT_PTR)true;
	case WM_CLOSE:
		EndDialog(Parent,0);
		break;
	}
	return (INT_PTR)false;
}

void Program::MessageLoop(void)
{
	unsigned long Count=1;
	//Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	MSG Msg;
	ZeroMemory(&Msg,sizeof(Msg));
	UpdateWindow(MainWindow);
	ShowWindow(MainWindow,SW_SHOW);
	timeBeginPeriod(1);
	FrameTime=timeGetTime();
	while(Msg.message!=WM_QUIT){
		if(PeekMessage(&Msg,0,0,0,PM_REMOVE)){
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}else if(Window){
			while((double)(timeGetTime()-FrameTime)<FrameFreq*Count) Sleep(1);
			Device->SetTransform(D3DTS_VIEW,&MatView);
			Device->SetTransform(D3DTS_PROJECTION,&MatProjection);	
			Device->Clear(0,nullptr,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,ClearColor,1.0f,0);
			(this->*Draw)();
			if(Count==GetFrameRate()){
				IsDeviceLose=(Device->TestCooperativeLevel()==D3DERR_DEVICENOTRESET)?(true):(false);
				FrameTime=timeGetTime();
				Count=0;
			}
			++Count;
		}
	}
	timeEndPeriod(1);
	return;
}

void Program::GetPlateVertices(void)
{
	ID3DXMesh* Mesh=Plate->Mesh();
	int IndexBufferSize,VertexBufferSize;
	void* LockedVertexBuffer=nullptr,* LockedIndexBuffer=nullptr;
	IDirect3DIndexBuffer9* IndexBuffer=nullptr;
	IDirect3DVertexBuffer9* VertexBuffer=nullptr;
	D3DINDEXBUFFER_DESC IndexBufferDesc;
	D3DVERTEXBUFFER_DESC VertexBufferDesc;
	Mesh->GetIndexBuffer(&IndexBuffer);
	Mesh->GetVertexBuffer(&VertexBuffer);
	IndexBuffer->GetDesc(&IndexBufferDesc);
	VertexBuffer->GetDesc(&VertexBufferDesc);
	IndexBufferSize=IndexBufferDesc.Size;
	VertexBufferSize=VertexBufferDesc.Size;
	PlateVertices=new unsigned char[VertexBufferSize/sizeof(unsigned char)];
	PlateIndex=new unsigned short[IndexBufferSize/sizeof(unsigned short)];
	VertexBuffer->Lock(0,0,&LockedVertexBuffer,D3DLOCK_READONLY);
	memcpy(PlateVertices,LockedVertexBuffer,VertexBufferSize);
	VertexBuffer->Unlock();
	IndexBuffer->Lock(0,0,&LockedIndexBuffer,D3DLOCK_READONLY);
	memcpy(PlateIndex,LockedIndexBuffer,IndexBufferSize);
	IndexBuffer->Unlock();
	VertexBuffer->Release();
	IndexBuffer->Release();
	return;
}

void Program::ClearGameData(void)
{
	GameOver=false;
	AddPoints=false;
	Light.Diffuse.r=Light.Diffuse.g=Light.Diffuse.b=0.0f;
	Current.Gravity=Current.Point=Current.UnixTime=0;
	SelectedMode=-1;
	FrameCount=0;
	Ball->VecPosition=D3DXVECTOR3(0,1,0);
	Ball->VecDirection=D3DXVECTOR3(0,0,0);
	Plate->VecRotation=D3DXVECTOR3(0,0,0);
	KeyState[0]=KeyState[1]=KeyState[2]=KeyState[3]=KeyState[4]=false;
	return;
}