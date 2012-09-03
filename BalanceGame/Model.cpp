#include "DrawObjects.h"

Model::Model(void)
{
	MeshInstance=nullptr;
	Textures=nullptr;
	Materials=nullptr;
	MaterialCount=0;
	return;
}

Model::Model(const Model& RefInstance){}

Model Model::operator = (const Model& RefInstance)
{
	return RefInstance;
}

Model::~Model(void)
{
	if(MeshInstance) MeshInstance->Release();
	if(Textures){
		for(unsigned i=0;i<MaterialCount;i++) if(Textures[i]) Textures[i]->Release();
		delete[] Textures;
	}
	if(Materials) delete[] Materials;
	return;
}

Model* Model::Load(wchar_t* FileName,IDirect3DDevice9* Device)
{
	unique_ptr<Model> Ptr(new Model());
	ID3DXBuffer* Mtrl;
	if(FileName&&D3DXLoadMeshFromX(FileName,
		D3DXMESH_SYSTEMMEM,
		Device,
		nullptr,
		&Mtrl,
		nullptr,
		&Ptr->MaterialCount,
		&Ptr->MeshInstance)<0) return nullptr;
	D3DXMATERIAL* Material=(D3DXMATERIAL*)Mtrl->GetBufferPointer();
	Ptr->Materials=new D3DMATERIAL9[Ptr->MaterialCount];
	Ptr->Textures=new IDirect3DTexture9*[Ptr->MaterialCount];
	for(unsigned i=0;i<Ptr->MaterialCount;i++){
		Ptr->Materials[i]=Material[i].MatD3D;
		Ptr->Materials[i].Ambient=Ptr->Materials[i].Diffuse;
		Ptr->Textures[i]=nullptr;
		if(Material[i].pTextureFilename&&lstrlenA(Material[i].pTextureFilename)>0){
			if(D3DXCreateTextureFromFileA(Device,Material[i].pTextureFilename,&Ptr->Textures[i])<0) return nullptr;
		}
	}
	Mtrl->Release();
	Ptr->Device=Device;
	Ptr->FileName=FileName;
	return Ptr.release();
}

Model* Model::Load(int ResourceID,IDirect3DDevice9* Device)
{
	unique_ptr<Model> Ptr(new Model());
	ID3DXBuffer* Mtrl;
	if(D3DXLoadMeshFromXResource(GetModuleHandle(nullptr),
		MAKEINTRESOURCEA(ResourceID),
		"XFILE",
		D3DXMESH_SYSTEMMEM,
		Device,
		nullptr,
		&Mtrl,
		nullptr,
		&Ptr->MaterialCount,
		&Ptr->MeshInstance)<0) return nullptr;
	D3DXMATERIAL* Material=(D3DXMATERIAL*)Mtrl->GetBufferPointer();
	Ptr->Materials=new D3DMATERIAL9[Ptr->MaterialCount];
	Ptr->Textures=new IDirect3DTexture9*[Ptr->MaterialCount];
	for(unsigned i=0;i<Ptr->MaterialCount;i++){
		Ptr->Materials[i]=Material[i].MatD3D;
		Ptr->Materials[i].Ambient=Ptr->Materials[i].Diffuse;
		Ptr->Textures[i]=nullptr;
		if(Material[i].pTextureFilename&&lstrlenA(Material[i].pTextureFilename)>0){
			if(D3DXCreateTextureFromFileA(Device,Material[i].pTextureFilename,&Ptr->Textures[i])<0) return nullptr;
		}
	}
	Mtrl->Release();
	Ptr->Device=Device;
	Ptr->ResourceID=ResourceID;
	return Ptr.release();
}

void Model::Dispose(void)
{
	if(MeshInstance){
		MeshInstance->Release();
		MeshInstance=nullptr;
	}
	for(int i=0;i<MaterialCount;i++){
		if(Textures[i]) Textures[i]->Release();
		Textures[i]=nullptr;
	}
	return;
}

void Model::Recover(void)
{
	ID3DXBuffer* Mtrl;
	if(!MeshInstance){
		if(ResourceID) D3DXLoadMeshFromXResource(GetModuleHandle(nullptr),MAKEINTRESOURCEA(ResourceID),"XFILE",D3DXMESH_SYSTEMMEM,Device,nullptr,&Mtrl,nullptr,nullptr,&MeshInstance);
		else if(FileName[0]) D3DXLoadMeshFromX(FileName.c_str(),D3DXMESH_SYSTEMMEM,Device,nullptr,&Mtrl,nullptr,nullptr,&MeshInstance);
	}
	D3DXMATERIAL* Material=(D3DXMATERIAL*)Mtrl->GetBufferPointer();
	for(int i=0;i<MaterialCount;i++){
		if(Material[i].pTextureFilename&&lstrlenA(Material[i].pTextureFilename)>0){
			if(D3DXCreateTextureFromFileA(Device,Material[i].pTextureFilename,&Textures[i])<0) return;
		}else Textures[i]=nullptr;
	}
	Mtrl->Release();
	return;
}

ID3DXMesh* Model::Mesh(void)
{
	return MeshInstance;
}

IDirect3DTexture9* Model::Texture(unsigned long Index)
{
	return Index>=MaterialCount?nullptr:Textures[Index];
}

D3DMATERIAL9& Model::Material(unsigned long Index)
{
	return Materials[Index];
}

bool Model::Draw(void)
{
	D3DXMatrixRotationYawPitchRoll(&MatRotation,VecRotation.y,VecRotation.x,VecRotation.z);
	D3DXMatrixScaling(&MatScaling,VecScaling.x,VecScaling.y,VecScaling.z);
	MatWorld=MatScaling*MatRotation;
	MatWorld.m[3][0]=VecPosition.x;
	MatWorld.m[3][1]=VecPosition.y;
	MatWorld.m[3][2]=VecPosition.z;
	if(Device->SetTransform(D3DTS_WORLD,&MatWorld)<0) return false;
	for(unsigned i=0;i<MaterialCount;i++){
		if(Device->SetMaterial(&Materials[i])<0) return false;
		if(Device->SetTexture(0,Textures[i])<0) return false;
		if(Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE)<0) return false;
		if(MeshInstance->DrawSubset(i)<0) return false;
	}
	return true;
}