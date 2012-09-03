#include "ShareHeaders.h"

struct Vertex
{
	float X,Y,Z,W;
	unsigned long Color;
	float U,V;
};

class DrawObject
{
protected:
	IDirect3DDevice9* Device;
	D3DXMATRIX MatScaling,MatRotation,MatWorld;
	wstring FileName;
	int ResourceID;
	
	DrawObject(void);
	DrawObject(const DrawObject& RefInstance);

public:
	D3DXVECTOR3 VecPosition,VecScaling,VecRotation;

	D3DXMATRIX& RotationMatrix(void);

	D3DXMATRIX& ScalingMatrix(void);

	D3DXMATRIX& WorldMatrix(void);

	virtual void Dispose(void)=0;
	virtual void Recover(void)=0;

	~DrawObject(void);

};

class Model : public DrawObject
{
private:
	ID3DXMesh* MeshInstance;
	IDirect3DTexture9** Textures;
	D3DMATERIAL9* Materials;
	unsigned long MaterialCount;

	Model(void);
	Model(const Model& RefInstance);
	Model operator = (const Model& RefInstance);

public:
	D3DXVECTOR3 VecDirection;

	static Model* Load(wchar_t* Name,IDirect3DDevice9* Device);
	static Model* Load(int ResourceID,IDirect3DDevice9* Device);

	virtual void Dispose(void);
	virtual void Recover(void);

	ID3DXMesh* Mesh(void);

	IDirect3DTexture9* Texture(unsigned long Index);

	D3DMATERIAL9& Material(unsigned long Index);
	unsigned long GetMaterialCount(void);

	bool Draw(void);
	~Model(void);

};

class Image : public DrawObject
{
private:
	Vertex Transformed[4];
	unsigned short Indexes[6];
	IDirect3DTexture9* TextureImage;
	double SizeX,SizeY;
	unsigned long DiffuseColor;

	Image(void);
	Image(const Image& RefInstance);
	Image operator = (const Image& RefInstance);
	void ProcessVertices(void);

public:
	static Image* Load(wchar_t* Name,IDirect3DDevice9* Device,double X,double Y);
	static Image* Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y);
	static Image* Load(wchar_t* Name,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor);
	static Image* Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor);

	virtual void Dispose(void);
	virtual void Recover(void);

	double GetSizeX(void);
	double GetSizeY(void);

	IDirect3DTexture9* Texture(void);

	void SetDiffuseColor(unsigned long Value);
	unsigned long GetDiffuseColor(void);

	void SetAlpha(unsigned char Value);

	bool Draw(void);
	~Image(void);

};

class NumberRender : public DrawObject
{
private:
	IDirect3DTexture9* NumberTexture;
	Vertex Transformed[4];
	unsigned short Indexes[6];
	unique_ptr<char[]> NumberString;
	double SizeX,SizeY,TexturePosition[13],TexturePositionY,ZeroPositionY;
	unsigned long DiffuseColor,MaxStringLength;

	NumberRender(void);
	NumberRender(const NumberRender& RefInstance);
	NumberRender operator = (const NumberRender& RefInstance);

	unsigned long Draw(void);
	void ProcessVertices(char NumberChar);

public:
	static NumberRender* Load(wchar_t* Name,IDirect3DDevice9* Deivce,double X,double Y);
	static NumberRender* Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y);
	static NumberRender* Load(wchar_t* Name,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor);
	static NumberRender* Load(int ResourceID,IDirect3DDevice9* Device,double X,double Y,unsigned long DiffuseColor);

	virtual void Dispose(void);
	virtual void Recover(void);

	double GetSizeX(void);
	double GetSizeY(void);

	unsigned long GetDrawCharCount(signed char Number);
	unsigned long GetDrawCharCount(unsigned char Number);
	unsigned long GetDrawCharCount(signed short Number);
	unsigned long GetDrawCharCount(unsigned short Number);
	unsigned long GetDrawCharCount(signed int Number);
	unsigned long GetDrawCharCount(unsigned int Number);
	unsigned long GetDrawCharCount(signed long Number);
	unsigned long GetDrawCharCount(unsigned long Number);
	unsigned long GetDrawCharCount(signed long long Number);
	unsigned long GetDrawCharCount(unsigned long long Number);
	unsigned long GetDrawCharCount(double Number,int NumberOfDecimal);

	IDirect3DTexture9* Texture(void);

	void SetDiffuseColor(unsigned long Value);
	unsigned long GetDiffuseColor(void);

	void SetAlpha(unsigned char Value);
	unsigned long Draw(signed char Number);
	unsigned long Draw(unsigned char Number);
	unsigned long Draw(signed short Number);
	unsigned long Draw(unsigned short Number);
	unsigned long Draw(signed int Number);
	unsigned long Draw(unsigned int Number);
	unsigned long Draw(signed long Number);
	unsigned long Draw(unsigned long Number);
	unsigned long Draw(signed long long Number);
	unsigned long Draw(unsigned long long Number);
	unsigned long Draw(double Number,int NumberOfDecimal);
	unsigned long Draw(wchar_t* NumberString);
	~NumberRender(void);

};