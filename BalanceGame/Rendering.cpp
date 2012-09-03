#include "Program.h"

bool Program::Collide(D3DXVECTOR3 Start,D3DXVECTOR3 Direction,Model* Object,float* Distance,D3DXVECTOR3* Normal)
{
	int Hit=false;
	D3DXMATRIX MatWorld;
	ID3DXMesh* Mesh=Object->Mesh();
	D3DXVec3Normalize(&Direction,&Direction);

	D3DXMatrixInverse(&MatWorld,nullptr,&Object->WorldMatrix());
	D3DXVec3TransformCoord(&Start,&Start,&MatWorld);

	unsigned long PolygonIndex;
	D3DXIntersect(Mesh,&Start,&Direction,&Hit,&PolygonIndex,nullptr,nullptr,Distance,nullptr,nullptr);
	if(Hit){
		D3DXVECTOR3 VecVertex[3];
		FindVerticesOnPoly(PolygonIndex,VecVertex);
		D3DXPLANE Plane;
		D3DXPlaneFromPoints(&Plane,&VecVertex[0],&VecVertex[1],&VecVertex[2]);
		*Normal=D3DXVECTOR3(Plane.a,Plane.b,Plane.c);
		return true;
	}
	return false;
}

bool Program::FindVerticesOnPoly(unsigned long PolygonIndex,D3DXVECTOR3* VecVertices)
{
	ID3DXMesh* Mesh=Plate->Mesh();
	unsigned long Stride=Mesh->GetNumBytesPerVertex(),VertexNumber=Mesh->GetNumVertices(),PolygonNumber=Mesh->GetNumFaces();
	float* FloatVetices=nullptr;

	FloatVetices=(float*)&PlateVertices[Stride*PlateIndex[PolygonIndex*3]];
	VecVertices[0].x=FloatVetices[0];
	VecVertices[0].y=FloatVetices[1];
	VecVertices[0].z=FloatVetices[2];

	FloatVetices=(float*)&PlateVertices[Stride*PlateIndex[PolygonIndex*3+1]];
	VecVertices[1].x=FloatVetices[0];
	VecVertices[1].y=FloatVetices[1];
	VecVertices[1].z=FloatVetices[2];

	FloatVetices=(float*)&PlateVertices[Stride*PlateIndex[PolygonIndex*3+2]];
	VecVertices[2].x=FloatVetices[0];
	VecVertices[2].y=FloatVetices[1];
	VecVertices[2].z=FloatVetices[2];
	return true;
}

D3DXVECTOR3 Program::Slip(D3DXVECTOR3 L,D3DXVECTOR3 N)
{
	D3DXVECTOR3 S;
	//S=L-(N*L)/(|N|^2)*N
	S=L-((D3DXVec3Dot(&N,&L))/(pow(D3DXVec3Length(&N),2)))*N;
	return S;
}

void Program::InitViewTransform(void)
{
	D3DXMatrixLookAtLH(&MatView,&VecEyePoint,&VecLookAtPoint,&VecUp);
	D3DXMatrixPerspectiveFovLH(&MatProjection,D3DX_PI/4,4.0f/3.0f,0.1f,100.0f);
	return;
}

void Program::InitLight(void)
{
	D3DXVECTOR3 VecDirection(1,-1,1);
	Light.Type=D3DLIGHT_DIRECTIONAL;
	Light.Diffuse.r=0.0f;
	Light.Diffuse.g=0.0f;
	Light.Diffuse.b=0.0f;    
	Light.Specular.r=1.0f;
	Light.Specular.g=1.0f;
	Light.Specular.b=1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction,&VecDirection);
	Light.Range=200.0f;
	return;
}

void Program::DrawTitleMenu(void)
{
	(SelectedMode==0||SelectedMode==3)?(Alpha--):(Alpha++);
	Device->BeginScene();
	ImageRenderState->Apply();
	if(KeyState[1]){
		KeyState[1]=false;
		if(floor(Images["Arrow"]->VecPosition.y-Images["Arrow"]->GetSizeY())>=Images["GameStart"]->VecPosition.y)
			Images["Arrow"]->VecPosition.y=floor(Images["Arrow"]->VecPosition.y)-Images["Arrow"]->GetSizeY();
	}
	if(KeyState[3]){
		KeyState[3]=false;
		if(floor(Images["Arrow"]->VecPosition.y+Images["Arrow"]->GetSizeY())<=Images["Quit"]->VecPosition.y)
			Images["Arrow"]->VecPosition.y=floor(Images["Arrow"]->VecPosition.y)+Images["Arrow"]->GetSizeY();
	}
	if(KeyState[4]){
		KeyState[4]=false;
		SelectedMode=((int)floor(Images["Arrow"]->VecPosition.y)-Images["GameStart"]->VecPosition.y)/Images["Arrow"]->GetSizeY();
		if(GetKeyState(VK_SHIFT)&0x80){
			MARGINS Margins={-1};
			HRESULT(__stdcall*DwmFunction)(HWND,MARGINS*)=nullptr;
			DwmFunction=(HRESULT(__stdcall*)(HWND,MARGINS*))GetProcAddress(GetModuleHandle(L"dwmapi.dll"),"DwmExtendFrameIntoClientArea");
			if(DwmFunction) if(DwmFunction(MainWindow,&Margins)>=0) ClearColor=0;
			SelectedMode=-1;
		}
		if(SelectedMode==1) Draw=&Program::OpenScoreViewWindow;
		else if(SelectedMode==2) Draw=&Program::OpenSettingWindow;
		else if(SelectedMode==3) Alpha(10*DrawMode);
	}
	if(*Alpha==0&&SelectedMode!=-1){
		if(SelectedMode==0){
			Draw=&Program::DrawGameContent;
			ClearGameData();
		}
		else if(SelectedMode==3) SendMessage(MainWindow,WM_CLOSE,0,0);
	}
	Images["Arrow"]->SetAlpha(*Alpha);
	Images["GameStart"]->SetAlpha(*Alpha);
	Images["ViewHighScore"]->SetAlpha(*Alpha);
	Images["Options"]->SetAlpha(*Alpha);
	Images["Quit"]->SetAlpha(*Alpha);
	Images["Title"]->SetAlpha(*Alpha);
	Images["Arrow"]->Draw();
	Images["GameStart"]->Draw();
	Images["ViewHighScore"]->Draw();
	Images["Options"]->Draw();
	Images["Quit"]->Draw();
	Images["Title"]->Draw();
	Device->EndScene();
	Device->Present(nullptr,nullptr,nullptr,nullptr);
	return;
}

void Program::DrawGameContent(void)
{
	Device->SetLight(0,&Light);
	Device->LightEnable(0,true);
	(SelectedMode!=-1)?(Alpha--):(Alpha++);
	if(*Alpha!=255&&*Alpha!=0){
		Light.Diffuse.r=Light.Diffuse.g=Light.Diffuse.b=*Alpha/255.0f;
		Images["Score"]->SetAlpha(*Alpha);
		Number->SetAlpha(*Alpha);
		Images["Point"]->SetAlpha(*Alpha);
	}else if(!SelectedMode&&*Alpha==0){
		Current.Gravity=GravityLong;
		_time32((long*)&Current.UnixTime);
		UpdateRanking(Current);
		Current.Gravity=Current.Point=Current.UnixTime=0;
		SelectedMode=-1;
		Images["Arrow"]->VecPosition=D3DXVECTOR3(Images["Yes"]->VecPosition.x-Images["Arrow"]->GetSizeX(),200,0);
		Draw=&Program::DrawAfterGame;
	}
	Ball->VecDirection=D3DXVECTOR3(0,0,0);

	if(GetForegroundWindow()==MainWindow){
		float FrameGravity=Gravity*DrawMode;
		int MoveFlag;
		if(MoveFlag=(((bool)(GetKeyState(VK_UP)&0x80))-((bool)(GetKeyState(VK_DOWN)&0x80)))) Plate->VecRotation.x=Plate->VecRotation.x+FrameGravity*MoveFlag;
		if(MoveFlag=(((bool)(GetKeyState(VK_LEFT)&0x80))-((bool)(GetKeyState(VK_RIGHT)&0x80)))) Plate->VecRotation.z=Plate->VecRotation.z+FrameGravity*MoveFlag;
	}
	Device->BeginScene();
	ModelRenderState->Apply();
	Plate->Draw();
	Ball->Draw();
	ImageRenderState->Apply();
	Images["Score"]->Draw();
	Number->VecPosition.x=Images["Score"]->GetSizeX();
	Images["Point"]->VecPosition.x=Number->VecPosition.x+Number->Draw(Current.Point)*Number->GetSizeX();
	Images["Point"]->Draw();
	Device->EndScene();
	Device->Present(nullptr,nullptr,nullptr,nullptr);
	float Distance=0;
	D3DXVECTOR3 VecNormal,VecGravity(0,(-Gravity+0.005)*DrawMode,0);
	Ball->VecDirection+=VecGravity;
	if(Collide(Ball->VecPosition,Ball->VecDirection,Plate,&Distance,&VecNormal)){
		if(Distance<=0.1){
			AddPoints=true;
			D3DXVec3TransformCoord(&VecNormal,&VecNormal,&Plate->RotationMatrix());
			Ball->VecDirection=Slip(Ball->VecDirection,VecNormal);
		}
	}else AddPoints=false;
	Ball->VecPosition+=Ball->VecDirection;	
	D3DXVECTOR3 VecPerp=-VecNormal;
	if(Collide(Ball->VecPosition,VecPerp,Plate,&Distance,&VecNormal)&&Distance<=0.1){	
		VecNormal*=(0.07-Distance);
		Ball->VecPosition+=VecNormal;
	}
	if(AddPoints){
		if(FrameCount>=6/((unsigned long)DrawMode)){
			FrameCount=0;
			Current.Point+=(D3DXVec3Length(&Ball->VecDirection)/DrawMode)*5000.0f;
		}
	}else{
		if(Current.Point&&!GameOver){
			GameOver=true;
			FrameCount=0;
		}else if(GameOver&&FrameCount==GetFrameRate()/2) SelectedMode=0;
	}
	++FrameCount;
	return;
}

void Program::DrawAfterGame(void)
{
	(SelectedMode!=-1)?(Alpha--):(Alpha++);
	Device->BeginScene();
	ImageRenderState->Apply();
	if(KeyState[0]){
		KeyState[0]=false;
		if(floor(Images["Arrow"]->VecPosition.x)-125>=Images["Yes"]->VecPosition.x-Images["Arrow"]->GetSizeX()) Images["Arrow"]->VecPosition.x=floor(Images["Arrow"]->VecPosition.x)-125;
	}
	if(KeyState[2]){
		KeyState[2]=false;
		if(floor(Images["Arrow"]->VecPosition.x)+125<=Images["No"]->VecPosition.x-Images["Arrow"]->GetSizeX()) Images["Arrow"]->VecPosition.x=floor(Images["Arrow"]->VecPosition.x)+125;
	}
	if(KeyState[4]){
		KeyState[4]=false;
		SelectedMode=((int)floor(Images["Arrow"]->VecPosition.x)-(Images["Yes"]->VecPosition.x-Images["Arrow"]->GetSizeX()))/125;
	}
	if(*Alpha==0&&SelectedMode!=-1){
		if(SelectedMode) Draw=&Program::DrawTitleMenu;
		else Draw=&Program::DrawGameContent;
		SelectedMode=-1;
		Images["Arrow"]->VecPosition=Images["GameStart"]->VecPosition;
		Images["Arrow"]->VecPosition.x-=Images["Arrow"]->GetSizeX();
		ClearGameData();
	}
	Images["GameOver"]->SetAlpha(*Alpha);
	Images["Replay"]->SetAlpha(*Alpha);
	Images["Yes"]->SetAlpha(*Alpha);
	Images["No"]->SetAlpha(*Alpha);
	Images["Arrow"]->SetAlpha(*Alpha);
	Images["GameOver"]->Draw();
	Images["Replay"]->Draw();
	Images["Yes"]->Draw();
	Images["No"]->Draw();
	Images["Arrow"]->Draw();
	Device->EndScene();
	Device->Present(nullptr,nullptr,nullptr,nullptr);
	return;
}