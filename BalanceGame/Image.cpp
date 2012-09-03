#include "DrawObjects.h"

Image::Image(void)
{
	TextureImage=nullptr;
	unsigned short Temp[]={0,1,2,0,2,3};
	memcpy(Indexes,Temp,sizeof(unsigned short)*6);
	SizeX=0;
	SizeY=0;
	return;
}

Image::Image(const Image& RefInstance){}

Image Image::operator = (const Image& RefInstance)
{
	return RefInstance;
}

Image::~Image(void)
{
	if(TextureImage) TextureImage->Release();
	return;
}

Image* Image::Load(wchar_t* Name,IDirect3DDevice9* Device,double X,double Y)
{
	return Load(Name,Device,X,Y,0);
}

Image* Image::Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y)
{
	return Load(ResourceID,Device,X,Y,0);
}

Image* Image::Load(wchar_t* Name,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor)
{
	unique_ptr<Image> Ptr(new Image());
	if(Name&&D3DXCreateTextureFromFile(Device,Name,&Ptr->TextureImage)<0) return nullptr;
	D3DSURFACE_DESC Desc;
	if(Ptr->TextureImage->GetLevelDesc(0,&Desc)<0) return nullptr;
	Vertex Temp[]={
		{0.0f,0.0f,0.0f,1.0f,0,              0.0f,               0.0f},
		{0.0f,0.0f,0.0f,1.0f,0,(X+0.5)/Desc.Width,               0.0f},
		{0.0f,0.0f,0.0f,1.0f,0,(X+0.5)/Desc.Width,(Y+0.5)/Desc.Height},
		{0.0f,0.0f,0.0f,1.0f,0,              0.0f,(Y+0.5)/Desc.Height}
	};
	memcpy(Ptr->Transformed,Temp,sizeof(Vertex)*4);
	Ptr->DiffuseColor=DiffuseColor;
	Ptr->SizeX=X;
	Ptr->SizeY=Y;
	Ptr->Device=Device;
	Ptr->FileName=Name;
	return Ptr.release();
}

Image* Image::Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor)
{
	unique_ptr<Image> Ptr(new Image());
	if(D3DXCreateTextureFromResource(Device,GetModuleHandle(nullptr),MAKEINTRESOURCE(ResourceID),&Ptr->TextureImage)<0) return nullptr;
	D3DSURFACE_DESC Desc;
	if(Ptr->TextureImage->GetLevelDesc(0,&Desc)<0) return nullptr;
	Vertex Temp[]={
		{0.0f,0.0f,0.0f,1.0f,0,              0.0f,               0.0f},
		{0.0f,0.0f,0.0f,1.0f,0,(X+0.5)/Desc.Width,               0.0f},
		{0.0f,0.0f,0.0f,1.0f,0,(X+0.5)/Desc.Width,(Y+0.5)/Desc.Height},
		{0.0f,0.0f,0.0f,1.0f,0,              0.0f,(Y+0.5)/Desc.Height}
	};
	memcpy(Ptr->Transformed,Temp,sizeof(Vertex)*4);
	Ptr->DiffuseColor=DiffuseColor;
	Ptr->SizeX=X;
	Ptr->SizeY=Y;
	Ptr->Device=Device;
	Ptr->ResourceID=ResourceID;
	return Ptr.release();
}

void Image::Dispose(void)
{
	if(TextureImage){
		TextureImage->Release();
		TextureImage=nullptr;
	}
	return;
}

void Image::Recover(void)
{
	if(!TextureImage){
		if(ResourceID) D3DXCreateTextureFromResource(Device,GetModuleHandle(nullptr),MAKEINTRESOURCE(ResourceID),&TextureImage);
		else if(FileName[0]) D3DXCreateTextureFromFile(Device,FileName.c_str(),&TextureImage);
	}
	return;
}

double Image::GetSizeX(void)
{
	return SizeX;
}

double Image::GetSizeY(void)
{
	return SizeY;
}

IDirect3DTexture9* Image::Texture(void)
{
	return TextureImage;
}

void Image::SetDiffuseColor(unsigned long Value)
{
	DiffuseColor=Value;
	return;
}

unsigned long Image::GetDiffuseColor(void)
{
	return DiffuseColor;
}

void Image::SetAlpha(unsigned char Value)
{
	DiffuseColor=(DiffuseColor&0x00FFFFFF)|(((unsigned long)Value)<<24);
	return;
}

void Image::ProcessVertices(void)
{
	for(int i=0;i<sizeof(Transformed)/sizeof(Vertex);i++){
		double X=(i==1||i==2)?(SizeX):(0.0),Y=(i>=2)?(SizeY):(0.0);
		Transformed[i].X=X*MatWorld.m[0][0]+X*MatWorld.m[1][0]+X*MatWorld.m[2][0]+MatWorld.m[3][0];
		Transformed[i].Y=Y*MatWorld.m[0][1]+Y*MatWorld.m[1][1]+Y*MatWorld.m[2][1]+MatWorld.m[3][1];
		Transformed[i].Z=MatWorld.m[3][2];
		Transformed[i].Color=DiffuseColor;
	}
	return;
}

bool Image::Draw(void)
{
	D3DXMatrixRotationYawPitchRoll(&MatRotation,VecRotation.y,VecRotation.x,VecRotation.z);
	D3DXMatrixScaling(&MatScaling,VecScaling.x,VecScaling.y,VecScaling.z);
	MatWorld=MatScaling*MatRotation;
	MatWorld.m[3][0]=VecPosition.x;
	MatWorld.m[3][1]=VecPosition.y;
	MatWorld.m[3][2]=VecPosition.z;
	ProcessVertices();
	if(Device->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)<0) return false;
	if(Device->SetTexture(0,TextureImage)<0) return false;
	if(Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,4,2,Indexes,D3DFMT_INDEX16,Transformed,sizeof(Vertex))<0) return false;
	return true;
}