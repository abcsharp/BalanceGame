#include "Program.h"

bool Program::CreateEmptySaveData(void)
{
	wchar_t TemplateString[]=L"\ufeff<?xml version=\"1.0\" encoding=\"utf-16\"?>\r\n"
							 L"<SaveData>\r\n"
							 L"  <HighScore1 Point=\"0\" Date=\"0\" Gravity=\"0\" />\r\n"
							 L"  <HighScore2 Point=\"0\" Date=\"0\" Gravity=\"0\" />\r\n"
							 L"  <HighScore3 Point=\"0\" Date=\"0\" Gravity=\"0\" />\r\n"
							 L"  <HighScore4 Point=\"0\" Date=\"0\" Gravity=\"0\" />\r\n"
							 L"  <HighScore5 Point=\"0\" Date=\"0\" Gravity=\"0\" />\r\n"
							 L"</SaveData>";
	DWORD ByteCount=sizeof(TemplateString);
	wchar_t* ResultString=new wchar_t[ByteCount/2];
	wcscpy(ResultString,TemplateString);
	EncryptSaveData((wchar_t**)&ResultString,ByteCount/2-1);
	HANDLE File=CreateFile(SaveDataPath.c_str(),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	bool Result=WriteFile(File,ResultString,ByteCount,&ByteCount,nullptr);
	delete[] ResultString;
	if(!Result) return false;
	if(!CloseHandle(File)) return false;
	return true;
}

bool Program::ReadSaveData(void)
{
	DWORD ByteCount;
	VARIANT_BOOL Result;
	if(!PathFileExists(SaveDataPath.c_str())){
		if(!CreateEmptySaveData()) return false;
	}
	HANDLE File=CreateFile(SaveDataPath.c_str(),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	int Count=GetFileSize(File,nullptr);
	wchar_t* Text=new wchar_t[Count/2];
	if(!ReadFile(File,Text,Count,&ByteCount,nullptr)) return false;
	DecryptSaveData(&Text,Count/2-1);
	Ranking->loadXML(Text+1,&Result);
	if(!Result) return false;
	if(Ranking->get_lastChild(&Nodes[9])<0) return false;
	if(Nodes[9]->get_firstChild(&Nodes[10])<0) return false;
	for(int i=0;i<4;i++) if(Nodes[10+i]->get_nextSibling(&Nodes[11+i])<0) return false;
	for(int i=0;i<5;i++){
		MSXML2::IXMLDOMNamedNodeMapPtr Attribute;
		if(Nodes[10+i]->get_attributes(&Attribute)<0) return false;
		if(Attribute->get_item(0,&Nodes[15+(i*3)])<0) return false;
		if(Attribute->get_item(1,&Nodes[15+(i*3)+1])<0) return false;
		if(Attribute->get_item(2,&Nodes[15+(i*3)+2])<0) return false;
	}
	if(!CreateShadowData()||!LoadRanking()) return false;
	delete[] Text;
	return true;
}

bool Program::WriteSaveData(void)
{
	unique_ptr<wchar_t[]> Temp(new wchar_t[32]);
	for(int i=0;i<5;i++){
		Nodes[15+(i*3)]->put_text(_ultow(Records[i].Point,Temp.get(),10));
		Nodes[15+(i*3)+1]->put_text(_ultow(Records[i].UnixTime,Temp.get(),10));
		Nodes[15+(i*3)+2]->put_text(_ultow(Records[i].Gravity,Temp.get(),10));
	}
	DWORD ByteCount;
	if(Ranking->save(_variant_t(ShadowDataPath.c_str()))<0) return false;
	wchar_t* Text;
	if(!ReadShadowData(&Text)){
		delete[] Text;
		return false;
	}
	int Count=wcslen(Text);
	EncryptSaveData(&Text,Count);
	HANDLE File=CreateFile(SaveDataPath.c_str(),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		nullptr,
		TRUNCATE_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	if(!WriteFile(File,Text,(Count+1)*2,&ByteCount,nullptr)) return false;
	if(!CloseHandle(File)) return false;
	delete[] Text;
	return true;
}

bool Program::ReadShadowData(wchar_t** ResultString)
{
	DWORD ByteCount;
	HANDLE File=CreateFile(ShadowDataPath.c_str(),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	int Size=GetFileSize(File,nullptr);
	*ResultString=new wchar_t[Size/2+1];
	ZeroMemory(*ResultString,Size+2);
	if(!ReadFile(File,*ResultString,Size,&ByteCount,nullptr)) return false;
	if(!CloseHandle(File)) return false;
	return true;
}

bool Program::CreateShadowData(void)
{
	HANDLE File=CreateFile(ShadowDataPath.c_str(),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	if(!CloseHandle(File)) return false;
	return true;
}

void Program::DecryptSaveData(wchar_t** ResultString,unsigned DecryptSize)
{
	wchar_t* Text=*ResultString;
	for(unsigned i=0;i<DecryptSize;i++){
		if(i%2==0||i==0) Text[i]=(~Text[i])-(DecryptSize-i);
		else Text[i]=(~Text[i])-i;
	}
	return;
}

void Program::EncryptSaveData(wchar_t** CryptSource,unsigned EncryptSize)
{
	wchar_t* ResultString=*CryptSource;
	for(unsigned i=0;i<EncryptSize;i++){
		if(i%2==0||i==0) ResultString[i]=~(ResultString[i]+(EncryptSize-i));
		else ResultString[i]=~(ResultString[i]+i);
	}
	return;
}

bool Program::LoadRanking(void)
{
	for(int i=0;i<5;i++){
		BSTR Point,Time,Gravity;
		if(Nodes[15+(i*3)]->get_text(&Point)<0) return false;
		if(Nodes[15+(i*3)+1]->get_text(&Time)<0) return false;
		if(Nodes[15+(i*3)+2]->get_text(&Gravity)<0) return false;
		Records[i].Point=_wtol(Point);
		Records[i].UnixTime=_wtol(Time);
		Records[i].Gravity=_wtol(Gravity);
	}
	return true;
}

bool Program::UpdateRanking(Record Result)
{
	int Index;
	for(Index=0;Index<=5;Index++) if(Index==5||Records[Index].Point<=Result.Point) break;
	if(Index==5) return false;
	for(int i=4;i>Index;i--){
		Records[i]=Records[i-1];
	}
	Records[Index]=Result;
	return true;
}

void Program::OpenScoreViewWindow(void)
{
	DialogBoxIndirect(Instance,(DLGTEMPLATE*)DialogTemplate,MainWindow,ScoreViewProc);
	ZeroMemory(Controls,sizeof(Controls));
	Draw=&Program::DrawTitleMenu;
	return;
}

void Program::InitScoreViewWindow(HWND Parent)
{
	RECT Rect;
	int X=200,Y=325;
	SendMessage(Parent,WM_SETICON,0,(LPARAM)MainWindowClass.hIconSm);
	GetWindowRect(MainWindow,&Rect);
	SetWindowText(Parent,L"ハイスコア");
	SetWindowPos(Parent,0,Rect.left+((Rect.right-Rect.left-X)/2),Rect.top+((Rect.bottom-Rect.top-Y)/2),X,Y,SWP_NOREPOSITION);
	InitScoreViewControls(Parent);
	for(int i=0;i<sizeof(Controls)/sizeof(HWND);i++) if(Controls[i]) SendMessage(Controls[i],WM_SETFONT,(WPARAM)DefaultFont,MAKELPARAM(true,0));
	return;
}

void Program::InitScoreViewControls(HWND Parent)
{
	wchar_t* Strings[]={L"1位",L"2位",L"3位",L"4位",L"5位"};
	RECT Rect;
	GetClientRect(Parent,&Rect);
	int i=-1;
	for(int j=0;j<sizeof(Strings)/sizeof(wchar_t*);j++){
		Controls[++i]=CreateWindow(L"Button",Strings[j],WS_CHILD|WS_VISIBLE|BS_GROUPBOX,10,11+j*55,Rect.right-20,55,Parent,(HMENU)(1000+i*2),Instance,nullptr);
		Controls[++i]=CreateWindow(L"Static",ScoreDataToString(j).c_str(),WS_CHILD|WS_VISIBLE|SS_LEFT,5,13,Rect.right-30,37,Controls[i-1],(HMENU)(1001+i*2),Instance,nullptr);
	}
	return;
}

wstring Program::ScoreDataToString(unsigned long Index)
{
	unique_ptr<wchar_t[]> Strings[3],Result(new wchar_t[256]);
	for(int i=0;i<sizeof(Strings)/sizeof(unique_ptr<wchar_t[]>);i++) Strings[i].reset(new wchar_t[32]);
	_wctime32_s(Strings[1].get(),32,(long*)&Records[Index].UnixTime);
	Strings[1].get()[24]=0;
	wsprintf(Result.get(),L"得点:%s\n日時:%s\n重力:%s",
		(Records[Index].Point)?(_ultow(Records[Index].Point,Strings[0].get(),10)):(L""),
		(Records[Index].UnixTime)?(Strings[1].get()):(L""),
		(Records[Index].Gravity)?(_ultow(Records[Index].Gravity,Strings[2].get(),10)):(L""));
	return wstring(Result.get());
}