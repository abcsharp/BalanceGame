#include "DrawObjects.h"

NumberRender::NumberRender(void)
{
	Device=nullptr;
	NumberTexture=nullptr;
	MaxStringLength=32;
	NumberString.reset(new char[MaxStringLength]);
	unsigned short TempIndex[]={0,1,2,0,2,3};
	memcpy(Indexes,TempIndex,sizeof(unsigned short)*6);
	Vertex TempVertex[]={
		{0.0f,0.0f,0.0f,1.0f,0,0.0f,0.0f},
		{0.0f,0.0f,0.0f,1.0f,0,0.0f,0.0f},
		{0.0f,0.0f,0.0f,1.0f,0,0.0f,0.0f},
		{0.0f,0.0f,0.0f,1.0f,0,0.0f,0.0f}
	};
	memcpy(Transformed,TempVertex,sizeof(Vertex)*4);
	SizeX=0;
	SizeY=0;
	TexturePositionY=0;
	ZeroPositionY=0;
	DiffuseColor=0;
	return;
}

NumberRender::NumberRender(const NumberRender& RefInstance){}

NumberRender NumberRender::operator = (const NumberRender& RefInstance)
{
	return RefInstance;
}

NumberRender::~NumberRender(void)
{
	if(NumberTexture) NumberTexture->Release();
	return;
}

NumberRender* NumberRender::Load(wchar_t* Name,IDirect3DDevice9* Device,double X,double Y)
{
	return Load(Name,Device,X,Y,0);
}

NumberRender* NumberRender::Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y)
{
	return Load(ResourceID,Device,X,Y,0);
}

NumberRender* NumberRender::Load(wchar_t* Name,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor)
{
	unique_ptr<NumberRender> Ptr(new NumberRender());
	if(Name&&D3DXCreateTextureFromFile(Device,Name,&Ptr->NumberTexture)<0) return nullptr;
	D3DSURFACE_DESC Desc;
	if(Ptr->NumberTexture->GetLevelDesc(0,&Desc)<0) return nullptr;
	for(int i=0;i<13;i++) Ptr->TexturePosition[i]=((X*i)+0.5)/((double)Desc.Width);
	Ptr->TexturePositionY=(Y+0.5)/((double)Desc.Height);
	Ptr->ZeroPositionY=0.5/Y;
	Ptr->DiffuseColor=DiffuseColor;
	Ptr->SizeX=X;
	Ptr->SizeY=Y;
	Ptr->Device=Device;
	Ptr->FileName=Name;
	return Ptr.release();
}

NumberRender* NumberRender::Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor)
{
	unique_ptr<NumberRender> Ptr(new NumberRender());
	if(D3DXCreateTextureFromResource(Device,GetModuleHandle(nullptr),MAKEINTRESOURCE(ResourceID),&Ptr->NumberTexture)<0) return nullptr;
	D3DSURFACE_DESC Desc;
	if(Ptr->NumberTexture->GetLevelDesc(0,&Desc)<0) return nullptr;
	for(int i=0;i<13;i++) Ptr->TexturePosition[i]=((X*i)+0.5)/((double)Desc.Width);
	Ptr->TexturePositionY=(Y+0.5)/((double)Desc.Height);
	Ptr->ZeroPositionY=0.5/Y;
	Ptr->DiffuseColor=DiffuseColor;
	Ptr->SizeX=X;
	Ptr->SizeY=Y;
	Ptr->Device=Device;
	Ptr->ResourceID=ResourceID;
	return Ptr.release();
}

void NumberRender::Dispose(void)
{
	if(NumberTexture){
		NumberTexture->Release();
		NumberTexture=nullptr;
	}
	return;
}

void NumberRender::Recover(void)
{
	if(!NumberTexture){
		if(ResourceID) D3DXCreateTextureFromResource(Device,GetModuleHandle(nullptr),MAKEINTRESOURCE(ResourceID),&NumberTexture);
		else if(FileName[0]) D3DXCreateTextureFromFile(Device,FileName.c_str(),&NumberTexture);
	}
	return;
}

double NumberRender::GetSizeX(void)
{
	return SizeX;
}

double NumberRender::GetSizeY(void)
{
	return SizeY;
}

unsigned long NumberRender::GetDrawCharCount(signed char Number)
{
	_ltoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(unsigned char Number)
{
	_ultoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(signed short Number)
{
	_ltoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(unsigned short Number)
{
	_ultoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(signed int Number)
{
	_ltoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(unsigned int Number)
{
	_ultoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(signed long Number)
{
	_ltoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(unsigned long Number)
{

	_ultoa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(signed long long Number)
{
	_i64toa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(unsigned long long Number)
{
	_ui64toa(Number,NumberString.get(),10);
	return strlen(NumberString.get());
}

unsigned long NumberRender::GetDrawCharCount(double Number,int NumberOfDecimal)
{
	int Count,DecPosition,Sign;
	if(!NumberOfDecimal) NumberOfDecimal++;
	_fcvt_s(NumberString.get(),sizeof(char)*MaxStringLength,Number,NumberOfDecimal,&DecPosition,&Sign);
	Count=strlen(NumberString.get())+1;
	if(DecPosition<=0) Count-=DecPosition-1;
	if(Sign) Count++;
	return Count;
}

IDirect3DTexture9* NumberRender::Texture(void)
{
	return NumberTexture;
}

void NumberRender::SetDiffuseColor(unsigned long Value)
{
	DiffuseColor=Value;
	return;
}

unsigned long NumberRender::GetDiffuseColor(void)
{
	return DiffuseColor;
}

void NumberRender::SetAlpha(unsigned char Value)
{
	DiffuseColor=(DiffuseColor&0x00FFFFFF)|(((unsigned long)Value)<<24);
	return;
}

void NumberRender::ProcessVertices(char NumberChar)
{
	int Index=0;
	switch(NumberChar){
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		Index=NumberChar-'0';break;
	case '-':
		Index=10;break;
	case '.':
		Index=11;break;
	}
	for(int i=0;i<4;i++){
		double X=(i==1||i==2)?(SizeX):(0.0),Y=(i>=2)?(SizeY):(0.0);
		Transformed[i].X=X*MatWorld.m[0][0]+X*MatWorld.m[1][0]+X*MatWorld.m[2][0]+MatWorld.m[3][0];
		Transformed[i].Y=Y*MatWorld.m[0][1]+Y*MatWorld.m[1][1]+Y*MatWorld.m[2][1]+MatWorld.m[3][1];
		Transformed[i].Z=MatWorld.m[3][2];
		Transformed[i].Color=DiffuseColor;
		Transformed[i].U=TexturePosition[(i==0||i==3)?(Index):(Index+1)];
		Transformed[i].V=(i>=2)?(TexturePositionY):(ZeroPositionY);
	}
	return;
}

unsigned long NumberRender::Draw(void)
{
	D3DXMatrixRotationYawPitchRoll(&MatRotation,VecRotation.y,VecRotation.x,VecRotation.z);
	MatWorld=MatRotation;
	MatWorld.m[3][0]=VecPosition.x;
	MatWorld.m[3][1]=VecPosition.y;
	MatWorld.m[3][2]=VecPosition.z;
	if(Device->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)<0) return 0;
	if(Device->SetTexture(0,NumberTexture)<0) return 0;
	int CharCount=strlen(NumberString.get());
	for(int i=0;i<CharCount;i++){
		ProcessVertices(NumberString.get()[i]);
		if(Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,4,2,Indexes,D3DFMT_INDEX16,Transformed,sizeof(Vertex))<0) return 0;
		MatWorld.m[3][0]+=SizeX;
	}
	return CharCount;
}

unsigned long NumberRender::Draw(signed char Number)
{
	_ltoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(unsigned char Number)
{
	_ultoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(signed short Number)
{
	_ltoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(unsigned short Number)
{
	_ultoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(signed int Number)
{
	_ltoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(unsigned int Number)
{
	_ultoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(signed long Number)
{
	_ltoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(unsigned long Number)
{
	_ultoa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(signed long long Number)
{
	_i64toa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(unsigned long long Number)
{
	_ui64toa(Number,NumberString.get(),10);
	return Draw();
}

unsigned long NumberRender::Draw(double Number,int NumberOfDecimal)
{
	int DecPosition,Sign;
	if(!NumberOfDecimal) NumberOfDecimal++;
	_fcvt_s(NumberString.get(),sizeof(char)*MaxStringLength,Number,NumberOfDecimal,&DecPosition,&Sign);
	string Result=NumberString.get();
	if(DecPosition!=Result.length()){
		if(DecPosition>0) Result.insert(DecPosition,".");
		else{
			memset(NumberString.get(),'0',MaxStringLength);
			NumberString.get()[-DecPosition]=0;
			Result.insert(0,NumberString.get());
			Result.insert(0,"0.");
		}
	}
	if(Sign) Result.insert(0,"-");
	strcpy(NumberString.get(),Result.c_str());
	return Draw();
}

unsigned long NumberRender::Draw(wchar_t* NumberString)
{
	wcstombs(this->NumberString.get(),NumberString,MaxStringLength);
	return Draw();
}