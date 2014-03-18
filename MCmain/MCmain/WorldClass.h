#pragma once
#include"Headers.h"


struct OutputQueue
{
	int x,y,z;
};

class WorldClass
{
public:
	WorldClass(void);
	~WorldClass(void);

	HRESULT Initialize(ID3D11Device*,ID3D11DeviceContext*,HWND);
	HRESULT Shutdown();

	void Inputs();
	void ThreadUpdateMap();
	void OptimizeThread();

	void TurnOnAlphaRendering(ID3D11Device*,ID3D11DeviceContext*);
	void TurnOffAlphaRendering(ID3D11Device*, ID3D11DeviceContext*);

	void TurnOffZBuffer(ID3D11Device*, ID3D11DeviceContext*);
	void TurnOnZBuffer(ID3D11Device*, ID3D11DeviceContext*);

	InputClass * GetInputClass();

	void RenderFrame(ID3D11RenderTargetView *,
		ID3D11DepthStencilView *,
		D3D11_VIEWPORT& ,
		ID3D11Device *,
		ID3D11DeviceContext *);
	SkyClass * GetSkyBox();

	void RenderScene(ID3D11Device *,
		ID3D11DeviceContext *);
private:
	ButtonClass* btn;
	float skyRotation;
	int Chunks;
	int t_playerX,t_playerY,t_playerZ;
	mutex		*t_mutex;		//��ͼHash��
	mutex		*t_output;		//��ͼ������
	thread		*t_update;		//����δ̽����ͼ
	thread		*t_fastrender;	//���»�ͼ����
	bool		updateThreadSwitch;	//���µ�ͼ����
	bool		OptimizeThreadSwitch;//��ͼ���п���
	InputClass  *m_input;
	PlayerClass *m_player;
	SkyClass    *m_sky;
	CloudClass  *m_cloud;
	BlockClass	*m_block;
	DayTime		*m_daytime;
	MapGenerateClass   *m_map;
	SoundClass* m_sound;
	vector<iPointClass>RenderQueue;

//Rendering vars.
	ID3D11BlendState* AlphaRenderingEnabled;
	ID3D11BlendState* AlphaRenderingDisabled;
	ID3D11DepthStencilState* DepthStencilEnabled;
	ID3D11DepthStencilState* DepthStencilDisabled;

	Render2TextureClass colorBuffer;
	Rectangle2DClass* m_rect, *m_screenRect;
	BlurEffect efb;
	ColorshakeEffect efcs;
	HWND basic_hwnd;
};