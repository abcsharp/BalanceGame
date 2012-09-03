#include "ShareHeaders.h"
#include "DrawObjects.h"

struct Record
{
	unsigned long Point,UnixTime,Gravity;
};

class AlphaEffect
{
private:
	unsigned char Value,Increment;

public:
	AlphaEffect(void);
	unsigned char& operator*(void);
	unsigned char& operator++(void);
	unsigned char operator++(int);
	unsigned char& operator--(void);
	unsigned char operator--(int);
	unsigned char& operator()(const unsigned char& Value);
	unsigned char operator()(void);

};

class Program
{
private:
	IDirect3D9* Direct3D;
	IDirect3DDevice9* Device;
	IDirect3DStateBlock9* ImageRenderState,* ModelRenderState;
	D3DLIGHT9 Light;
	D3DXVECTOR3 VecEyePoint,VecLookAtPoint,VecUp;
	D3DXMATRIX MatView,MatProjection;
	D3DPRESENT_PARAMETERS Params;
	MSXML2::IXMLDOMDocument3Ptr Setting,Ranking;
	MSXML2::IXMLDOMNodePtr Nodes[30];
	HINSTANCE Instance;
	WNDCLASSEX MainWindowClass;
	HWND MainWindow,Controls[20];
	HFONT DefaultFont;
	POINT MainWindowPos;
	Model* Ball,* Plate;
	map<string,Image*> Images;
	NumberRender* Number;
	void(Program::*Draw)(void);
	bool Window,KeyState[5],AddPoints,GameOver,IsDeviceLose;
	unsigned long FrameTime,FrameCount,GravityLong,ClearColor;
	float Gravity,DrawMode;
	double FrameFreq;
	wstring SettingPath,SaveDataPath,ShadowDataPath;
	unsigned char* PlateVertices;
	unsigned short* PlateIndex;
	void* DialogTemplate;
	int PlateType,MultiSampleLevel,VertexProcessing,SelectedMode;
	Record Records[5],Current;
	AlphaEffect Alpha;

	Program(void);
	Program(const Program& RefInstance);
	Program operator = (const Program& RefInstance);
	static LRESULT __stdcall WndProc(HWND Parent,UINT Message,WPARAM wParam,LPARAM lParam);
	static INT_PTR __stdcall SettingProc(HWND Parent,UINT Message,WPARAM wParam,LPARAM lParam);
	static INT_PTR __stdcall ScoreViewProc(HWND Parent,UINT Message,WPARAM wParam,LPARAM lParam);
	void DrawTitleMenu(void);
	void DrawGameContent(void);
	void DrawAfterGame(void);
	void ClearGameData(void);
	void GetPlateVertices(void);
	void InitViewTransform(void);
	bool Collide(D3DXVECTOR3 Start,D3DXVECTOR3 Direction,Model* Object,float* Distance,D3DXVECTOR3* Normal);
	bool FindVerticesOnPoly(unsigned long PolygonIndex,D3DXVECTOR3* VecVertices);
	D3DXVECTOR3 Slip(D3DXVECTOR3 L,D3DXVECTOR3 N);
	void InitLight(void);
	void InitPath(void);
	bool LoadSetting(void);
	bool SaveSetting(void);
	bool CreateEmptySaveData(void);
	bool ReadSaveData(void);
	bool WriteSaveData(void);
	bool ReadShadowData(wchar_t** ResultString);
	bool CreateShadowData(void);
	void DecryptSaveData(wchar_t** ResultString,unsigned DecryptSize);
	void EncryptSaveData(wchar_t** CryptSource,unsigned EncryptSize);
	bool LoadRanking(void);
	bool UpdateRanking(Record Result);
	void SetFrameRate(unsigned long FrameRate);
	unsigned long GetFrameRate(void);
	void SetGravity(unsigned long Value);
	bool InitRenderState(void);
	bool InitModels(void);
	bool InitImages(void);
	void LoadTextures(void);
	void OpenScoreViewWindow(void);
	void InitScoreViewWindow(HWND Parent);
	void InitScoreViewControls(HWND Parent);
	void OpenSettingWindow(void);
	void InitSettingWindow(HWND Parent);
	void InitSettingControls(HWND Parent);
	wstring ScoreDataToString(unsigned long Index);
	void InitImagePosition(void);
	void SetSetting(void);
	void GetSetting(void);
	bool Recover(void);

public:
	static Program* GetInstance(void);
	bool InitSetting(void);
	bool InitWindow(void);
	bool InitDirect3D(void);
	void MessageLoop(void);
	~Program(void);

};