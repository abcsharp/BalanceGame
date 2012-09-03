#include "Program.h"
#include "resource.h"

bool Program::LoadSetting(void)
{
	VARIANT_BOOL Result=VARIANT_FALSE;
	DWORD ByteCount;
	if(!PathFileExists(SettingPath.c_str())){
		HANDLE File=CreateFile(SettingPath.c_str(),
			GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);
		wchar_t SettingString[]=L"\ufeff<?xml version=\"1.0\" encoding=\"utf-16\"?>\r\n"
								L"<Config>\r\n"
								L"  <Location X=\"100\" Y=\"100\" />\r\n"
								L"  <PlateType>1</PlateType>\r\n"
								L"  <Gravity>20</Gravity>\r\n"
								L"  <DrawMode>1</DrawMode>\r\n"
								L"</Config>";
		if(!WriteFile(File,SettingString,(wcslen(SettingString)+1)*2,&ByteCount,nullptr)) return false;
		if(!CloseHandle(File)) return false;
		Setting->loadXML(SettingString+1,&Result);
	}else Setting->load(_variant_t(SettingPath.c_str()),&Result);
	return (Result==VARIANT_FALSE)?(false):(true);
}

bool Program::InitSetting(void)
{
	MSXML2::IXMLDOMNamedNodeMapPtr Attribute;
	if(FAILED(Ranking.CreateInstance(__uuidof(MSXML2::DOMDocument60)))) return false;
	Ranking->put_async(VARIANT_FALSE);
	if(FAILED(Setting.CreateInstance(__uuidof(MSXML2::DOMDocument60)))) return false;
	Setting->put_async(VARIANT_FALSE);
	if(!LoadSetting()) return false;
	Setting->get_lastChild(&Nodes[0]);
	if(!Nodes[0]) return false;
	Nodes[0]->get_firstChild(&Nodes[1]);
	Nodes[1]->get_attributes(&Attribute);
	if(!Nodes[1]||!Attribute) return false;
	Attribute->get_item(0,&Nodes[2]);
	Attribute->get_item(1,&Nodes[3]);
	if(!Nodes[2]||!Nodes[3]) return false;
	BSTR Value;
	Nodes[2]->get_text(&Value);
	MainWindowPos.x=_wtoi(Value);
	Nodes[3]->get_text(&Value);
	MainWindowPos.y=_wtoi(Value);
	Nodes[1]->get_nextSibling(&Nodes[4]);
	Nodes[4]->get_nextSibling(&Nodes[5]);
	if(!Nodes[4]||!Nodes[5]) return false;
	Nodes[4]->get_text(&Value);
	PlateType=_wtoi(Value);
	if(PlateType<0&&PlateType>6) return false;
	Nodes[5]->get_text(&Value);
	SetGravity(_wtol(Value));
	Nodes[5]->get_nextSibling(&Nodes[6]);
	if(!Nodes[6]) return false;
	Nodes[6]->get_text(&Value);
	DrawMode=_wtoi(Value);
	if(DrawMode<1&&DrawMode>3) return false;
	SetFrameRate(60/DrawMode);
	if(!ReadSaveData()){
		MessageBox(nullptr,L"ハイスコア記録の読み込みに失敗しました。",L"",MB_OK|MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Program::SaveSetting(void)
{
	HRESULT hr1,hr2;
	wchar_t* Text=new wchar_t[16];
	_itow(MainWindowPos.x,Text,10);
	hr1=Nodes[2]->put_text(Text);
	_itow(MainWindowPos.y,Text,10);
	hr2=Nodes[3]->put_text(Text);
	if(hr1<0||hr2<0) return false;
	_itow(PlateType,Text,10);
	hr1=Nodes[4]->put_text(Text);
	_ltow(GravityLong,Text,10);
	hr2=Nodes[5]->put_text(Text);
	if(hr1<0||hr2<0) return false;
	_itow(DrawMode,Text,10);
	hr1=Nodes[6]->put_text(Text);
	delete[] Text;
	if(hr1<0) return false;
	hr2=Setting->save(_variant_t(SettingPath.c_str()));
	if(hr2<0) return false;
	if(!WriteSaveData()) return false;
	return true;
}

void Program::SetFrameRate(unsigned long Rate)
{
	DrawMode=60/Rate;
	FrameFreq=1000.0/Rate;
	return;
}

unsigned long Program::GetFrameRate(void)
{
	return 60/DrawMode;
}

void Program::SetGravity(unsigned long Value)
{
	GravityLong=Value;
	Gravity=Value/1000.0f;
	return;
}

void Program::OpenSettingWindow(void)
{
	int BeforeChange=PlateType;
	DialogBoxIndirect(Instance,(DLGTEMPLATE*)DialogTemplate,MainWindow,SettingProc);
	if(BeforeChange!=PlateType){
		delete Plate;
		delete[] PlateIndex;
		delete[] PlateVertices;
		Plate=Model::Load(X_TRIANGLE+PlateType,Device);
		GetPlateVertices();
	}
	ZeroMemory(Controls,sizeof(Controls));
	Alpha(5*DrawMode);
	Draw=&Program::DrawTitleMenu;
	return;
}

void Program::InitSettingWindow(HWND Parent)
{
	RECT Rect;
	int X=360,Y=270;
	SendMessage(Parent,WM_SETICON,0,(LPARAM)MainWindowClass.hIconSm);
	GetWindowRect(MainWindow,&Rect);
	SetWindowText(Parent,L"設定");
	SetWindowPos(Parent,0,Rect.left+((Rect.right-Rect.left-X)/2),Rect.top+((Rect.bottom-Rect.top-Y)/2),X,Y,SWP_NOREPOSITION);
	InitSettingControls(Parent);
	for(int i=0;i<sizeof(Controls)/sizeof(HWND);i++) if(Controls[i]) SendMessage(Controls[i],WM_SETFONT,(WPARAM)DefaultFont,MAKELPARAM(true,0));
	GetSetting();
	return;
}

void Program::InitSettingControls(HWND Parent)
{
	wchar_t* Strings[]={L"3角形",L"4角形",L"5角形",L"6角形",L"円形",L"星形",L"バッテン印型"};
	RECT Rect;
	GetClientRect(Parent,&Rect);
	int i=-1;
	Controls[++i]=CreateWindow(L"Static",L"プレートの形状",WS_CHILD|WS_VISIBLE|SS_LEFT,20,20,70,15,Parent,(HMENU)Ctrl_Static1,Instance,nullptr);
	Controls[++i]=CreateWindow(L"ComboBox",L"",WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL,20,40,120,19,Parent,(HMENU)Ctrl_ComboBox1,Instance,nullptr);
	for(int j=0;j<sizeof(Strings)/sizeof(wchar_t*);j++) SendMessage(Controls[i],CB_INSERTSTRING,-1,(LPARAM)Strings[j]);
	Controls[++i]=CreateWindow(L"Button",L"現在の動作モード",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,150,20,Rect.right-170,130,Parent,(HMENU)Ctrl_GroupBox1,Instance,nullptr);
	Controls[++i]=CreateWindow(L"Static",L"",WS_CHILD|WS_VISIBLE|SS_LEFT,5,27,Rect.right-180,85,Controls[i-1],(HMENU)Ctrl_Static2,Instance,nullptr);
	unique_ptr<wchar_t[]> ModeString(new wchar_t[256]);
	wsprintf(ModeString.get(),L"頂点処理方式:%s\n\n描画フレーム数:%dfps\n\nマルチサンプリング:%s\n\nサンプル数:%d",
		(VertexProcessing==D3DCREATE_HARDWARE_VERTEXPROCESSING)?(L"ハードウェア"):(L"ソフトウェア"),GetFrameRate(),
		(MultiSampleLevel)?(L"有効"):(L"無効"),MultiSampleLevel);
	SetWindowText(Controls[i],ModeString.get());
	Controls[++i]=CreateWindow(L"Button",L"描画フレーム数",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,20,70,120,80,Parent,(HMENU)Ctrl_GroupBox2,Instance,nullptr);
	Controls[++i]=CreateWindow(L"Button",L"60fps（デフォルト）",WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,5,15,110,15,Controls[i-1],(HMENU)Ctrl_Radio1,Instance,nullptr);
	Controls[++i]=CreateWindow(L"Button",L"30fps",WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,5,35,110,15,Controls[i-2],(HMENU)Ctrl_Radio2,Instance,nullptr);
	Controls[++i]=CreateWindow(L"Button",L"20fps",WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,5,55,110,15,Controls[i-3],(HMENU)Ctrl_Radio3,Instance,nullptr);
	Controls[++i]=CreateWindow(L"Static",L"重力（10～70）　　変更するとゲームの難易度も変化します",WS_CHILD|WS_VISIBLE|SS_LEFT,20,170,300,15,Parent,(HMENU)Ctrl_Static3,Instance,nullptr);
	Controls[++i]=CreateWindow(TRACKBAR_CLASS,L"",WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS|TBS_TOOLTIPS|TBS_DOWNISLEFT|TBS_BOTH,5,Rect.bottom-50,Rect.right-10,45,Parent,(HMENU)Ctrl_TrackBar1,Instance,nullptr);
	SendMessage(Controls[i],TBM_SETRANGE,true,MAKELONG(10,70));
	return;
}

void Program::SetSetting(void)
{
	PlateType=SendMessage(Controls[1],CB_GETCURSEL,0,0);
	for(int i=1;i<=3;i++) if(SendMessage(Controls[4+i],BM_GETCHECK,0,0)==BST_CHECKED){SetFrameRate(60/i);break;}
	SetGravity(SendMessage(Controls[9],TBM_GETPOS,0,0));
	return;
}

void Program::GetSetting(void)
{
	SendMessage(Controls[1],CB_SETCURSEL,PlateType,0);
	SendMessage(Controls[4+(int)floor(DrawMode)],BM_SETCHECK,BST_CHECKED,0);
	SendMessage(Controls[9],TBM_SETPOS,true,GravityLong);
	return;
}