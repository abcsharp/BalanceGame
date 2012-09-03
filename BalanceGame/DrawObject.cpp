#include "DrawObjects.h"

DrawObject::DrawObject(void)
{
	Device=nullptr;
	VecRotation=VecPosition=D3DXVECTOR3(0,0,0);
	VecScaling=D3DXVECTOR3(1,1,1);
	D3DXMatrixIdentity(&MatWorld);
	MatRotation=MatScaling=MatWorld;
	ResourceID=0;
	FileName=L"";
	return;
}

DrawObject::DrawObject(const DrawObject& RefInstance){}

DrawObject::~DrawObject(void)
{
	return;
}

D3DXMATRIX& DrawObject::RotationMatrix(void)
{
	return MatRotation;
}

D3DXMATRIX& DrawObject::ScalingMatrix(void)
{
	return MatScaling;
}

D3DXMATRIX& DrawObject::WorldMatrix(void)
{
	return MatWorld;
}
