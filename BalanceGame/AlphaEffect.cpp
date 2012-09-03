#include "Program.h"

AlphaEffect::AlphaEffect(void)
{
	Value=Increment=0;
}

unsigned char& AlphaEffect::operator*(void)
{
	return Value;
}

unsigned char& AlphaEffect::operator++(void)
{
	return (((int)Value)+Increment>255)?(Value=255):(Value=Value+Increment);
}

unsigned char AlphaEffect::operator++(int)
{
	unsigned char Temp=Value;
	(((int)Value)+Increment>255)?(Value=255):(Value=Value+Increment);
	return Temp;
}

unsigned char& AlphaEffect::operator--(void)
{
	return (((int)Value)-Increment<0)?(Value=0):(Value=Value-Increment);
}

unsigned char AlphaEffect::operator--(int)
{
	unsigned char Temp=Value;
	(((int)Value)-Increment<0)?(Value=0):(Value=Value-Increment);
	return Temp;
}

unsigned char& AlphaEffect::operator()(const unsigned char& Value)
{
	return Increment=Value;
}

unsigned char AlphaEffect::operator()(void)
{
	return Increment;
}