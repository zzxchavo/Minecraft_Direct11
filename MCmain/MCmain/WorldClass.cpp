#include "WorldClass.h"

static float my = 1.0f,my2 = 1.0f;
BoneClass bone1, bone2 , bone3;

WorldClass::WorldClass(void)
{
//BlockClass-based object
	SAFE_CREATE(m_block ,BlockClass);
	SAFE_CREATE(m_sky, SkyClass(580.0f));
	SAFE_CREATE(m_cloud , CloudClass);
//End 
	SAFE_CREATE(btn,ButtonClass(200,200,100,30));
	SAFE_CREATE(m_sound , SoundClass);
	SAFE_CREATE(m_input, InputClass);
	SAFE_CREATE(m_player, PlayerClass);
	SAFE_CREATE(m_map , MapGenerateClass);
	SAFE_CREATE(m_rect,Rectangle2DClass);
	SAFE_CREATE(m_screenRect, Rectangle2DClass);
	SAFE_CREATE(m_daytime, DayTime);
//mutexs & threads
	SAFE_CREATE(t_mutex , mutex);
	SAFE_CREATE(t_output ,mutex);
	updateThreadSwitch = true;
	OptimizeThreadSwitch = true;
//End thread operation.
//Ohter initialize operations.
	skyRotation = 0.0f;
	Chunks = 50;
	t_playerX = 0;
	t_playerY = 0;
	t_playerZ = 0;
}

HRESULT WorldClass::Shutdown()
{
	HRESULT hr = S_OK;
	SHADERMANAGER.ShutDown();
	EFFECTMANAGER.Shutdown();
	return hr;
}
WorldClass::~WorldClass(void)
{
	if(t_update)
	{
		updateThreadSwitch = false;
		t_update ->join();
		delete t_update;
		t_update = NULL;
	}
	if(t_fastrender)
	{
		OptimizeThreadSwitch = false;
		t_fastrender ->join();
		delete t_fastrender;
		t_fastrender = NULL;
	}
	SAFE_DELETE(t_mutex);
	SAFE_DELETE(t_output);
	SAFE_DELETE(m_sky);
	SAFE_DELETE(m_cloud);
	SAFE_DELETE(m_input);
	SAFE_DELETE(m_block);
	SAFE_DELETE(m_rect);
	SAFE_DELETE(m_screenRect);
	SAFE_DELETE(m_daytime);

	SAFE_DELETE(btn);
// Delete ShaderClass objects
}

void WorldClass::ThreadUpdateMap()
{
	while(updateThreadSwitch)
	{
		t_mutex->lock();
//		m_map->GenerateArea(0,0,50);
		t_mutex->unlock();
	}
}

void WorldClass::OptimizeThread()
{
	while(OptimizeThreadSwitch)
	{
		t_output->lock();
		int moves[][3]=
		{
			{ 1, 0, 0},
			{ 0, 1, 0},
			{ 0, 0, 1},
			{-1, 0, 0},
			{ 0,-1, 0},
			{ 0, 0,-1},
		};
		iPointClass start;
		start._XYZ[0] = m_player->GetX();
		start._XYZ[1] = m_player->GetY();
		start._XYZ[2] = m_player->GetZ();
		t_output->unlock();
	}
}
HRESULT WorldClass::Initialize(ID3D11Device* device,ID3D11DeviceContext* context,HWND hwnd)
{

	HRESULT hr=S_OK;
	srand(time(NULL));
	basic_hwnd = hwnd;	
	
//back color buffer operations!!!!!!!!!!!!!!!!!!!!!!!!!
	D3D11_INPUT_ELEMENT_DESC fogLayout[3] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		, { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = 3;
//Creating BlendBuffer
	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(blend_desc));
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = device->CreateBlendState(&blend_desc, &AlphaRenderingEnabled);
	if (FAILED(hr))
	{
		MessageBox(NULL,L"Creating BlendState Failed!",L"Error - WorldClass",MB_OK);
	}
	blend_desc.RenderTarget[0].BlendEnable = false;
	hr = device->CreateBlendState(&blend_desc, &AlphaRenderingDisabled);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Creating BlendState Failed!", L"Error - WorldClass", MB_OK);
	}


	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc,sizeof(depthDesc));
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = true;
	depthDesc.StencilReadMask = 0xff;
	depthDesc.StencilWriteMask = 0xff;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	MSG_RETURN(device->CreateDepthStencilState(&depthDesc,&DepthStencilEnabled),"depthstencil");

	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.DepthEnable = false;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = true;
	depthDesc.StencilReadMask = 0xff;
	depthDesc.StencilWriteMask = 0xff;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = device->CreateDepthStencilState(&depthDesc, &DepthStencilDisabled);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "", "", MB_OK);
	}

// objects initialization

	MSG_RETURN(m_player->Initialize(device, context, m_map),"player");
	m_player->SetStep(0.167f);
//	m_player->SetStep(5.0f);
//	m_map->GenerateArea(0, 0, 50);
	if (FAILED(m_sky->Initialize(device, context)))
		return E_FAIL;
	if (FAILED(m_block->Initialize(device, context)))
		return E_FAIL;

	SHADERMANAGER.Initialize(device, context);
	SHADERMANAGER.AddShader(device, context, "basic", L"shaders/fog/fog.vs", L"shaders/fog/fog.ps",fogLayout,3);
	SHADERMANAGER.AddShader(device, context, "skybox", L"shaders/skybox/skybox.vs", L"shaders/skybox/skybox.ps", fogLayout, 3);
	SHADERMANAGER.AddShader(device, context, "cloud", L"shaders/skybox/cloud.vs", L"shaders/skybox/cloud.ps", fogLayout, 3);
	SHADERMANAGER.AddShader(device, context, "tex2d", L"shaders/Tex2D/t2d.vs", L"shaders/Tex2D/t2d.ps", fogLayout, 3);
	SHADERMANAGER.AddShader(device, context, "horizontal", L"shaders/Blur/horizontal.vs", L"shaders/Blur/horizontal.ps", fogLayout, 3);
	SHADERMANAGER.AddShader(device, context, "vertical", L"shaders/Blur/vertical.vs", L"shaders/Blur/vertical.ps", fogLayout, 3);
	SHADERMANAGER.AddShader(device, context, "gui", L"shaders/GUI/button.vs", L"shaders/GUI/button.ps", fogLayout, 3);
	SHADERMANAGER.AddShader(device, context, "colorshake", L"shaders/colorshake/colorshake.vs", L"shaders/colorshake/colorshake.ps", fogLayout, 3);

	EFFECTMANAGER.AddEffect(device, context, "blur");
	EFFECTMANAGER.AddEffect(device, context, "colorshake");

	MSG_RETURN(m_cloud->Initialize(device, context), "cloud");
	MSG_RETURN(DefinesAndTextures::GetInstance()->Initialize(device, context), "textures");
	MSG_RETURN(m_rect->Initialize(device, context, 800, 600, 200, 200), "m_rect");
	MSG_RETURN(m_screenRect->Initialize(device, context, 800, 600, 800, 600), "m_screenRect");
	MSG_RETURN(m_daytime->Initialize(device, context), "m_daytime");
	MSG_RETURN(colorBuffer.Initialize(device, context), "buffer");

	m_player->SetPosition(device,context,0.0f, 63.0f, 0.0f);
	
//	PlaySound(L"sound/calm.wav", NULL, SND_LOOP | SND_ASYNC);
//End
	bone1.SetName("hehe");
	bone2.SetName("haha");
	bone3.SetName("xixi");
	bone1.BindWithAsParent(&bone2);
	bone2.BindWithAsParent(&bone3);
	bone1.SetPosition(0.0f, 65.0f, 0.0f);
	bone2.SetPosition(0.0f, 65.0f, 10.0f);
	bone3.SetPosition(0.0f, 65.0f,15.0f);
	efb.Initialize(device, context);
	efcs.Initialize(device, context);
	btn->Initialize(device, context);
	return true;
}

SkyClass * WorldClass::GetSkyBox()
{
	return m_sky;
}

InputClass * WorldClass::GetInputClass()
{
	return m_input;
}

void WorldClass::TurnOffZBuffer(ID3D11Device * device,ID3D11DeviceContext * context)
{
	context->OMSetDepthStencilState(DepthStencilDisabled,1);
}

void WorldClass::TurnOnZBuffer(ID3D11Device * device,ID3D11DeviceContext * context)
{
	context->OMSetDepthStencilState(DepthStencilEnabled, 1);
}

void WorldClass::RenderFrame(ID3D11RenderTargetView * rendertarget,
	ID3D11DepthStencilView * depthstencil,
	D3D11_VIEWPORT& viewport,
	ID3D11Device * device,
	ID3D11DeviceContext * context)
{
	
	m_daytime->UpdateTime(device,context);
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	if(m_input->GetKeyState(VK_LEFT)) m_player->GetCamera()->AddRotationY(-1);
	if(m_input->GetKeyState(VK_RIGHT)) m_player->GetCamera()->AddRotationY( 1);
	if(m_input->GetKeyState(VK_DOWN)) m_player->GetCamera()->AddRotationX( 1);
	if(m_input->GetKeyState(VK_UP)) m_player->GetCamera()->AddRotationX(-1);
	if(m_input->GetKeyState(VK_SPACE))  m_player->GetCamera()->FlyUp();
	if(m_input->GetKeyState(VK_SHIFT))  m_player->GetCamera()->DiveDown();
	if(m_input->GetKeyState('W'))
		m_player->GetCamera()->MoveForward();
	if(m_input->GetKeyState('A'))
		m_player->GetCamera()->MoveLeft();
	if(m_input->GetKeyState('D'))
		m_player->GetCamera()->MoveRight();
	if(m_input->GetKeyState('S'))
		m_player->GetCamera()->MoveBackward();

	m_player->CheckCollision(device,context,m_map);
	m_player->GetCamera()->UpdateCamera(device, context);
	printf("%f\t%f\t%f\t%d\n", m_player->GetCamera()->GetX(), m_player->GetCamera()->GetY(), m_player->GetCamera()->GetZ(), m_daytime->GetTime());
//Set Render Target::Set the render target to be the render to texture

	colorBuffer.SetRenderTarget(device, context, depthstencil);
	colorBuffer.ClearRenderTarget(device, context, depthstencil);
	RenderScene(device, context);
	TurnOffZBuffer(device, context);

// Reset the render target back to the original back buffer and not the render to texture anymore.
//	efb.UseEffect(device, context, depthstencil, colorBuffer.GetShaderResourceView());
//	efcs.UseEffect(device, context, depthstencil, efb.GetShaderResourceView());
	EFFECTMANAGER.Render(device, context, depthstencil, colorBuffer.GetShaderResourceView());

	context->OMSetRenderTargets(1, &rendertarget, depthstencil);
	SHADERMANAGER.UseShader(device, context, "tex2d");
	context->PSSetShaderResources(0, 1, &(EFFECTMANAGER.GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);

/*
	SHADERMANAGER.UseShader(device,context,"horizontal");
	context->PSSetShaderResources(0, 1, &(colorBuffer.GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);
	context->ClearRenderTargetView(rendertarget, ClearColor);
	context->ClearDepthStencilView(depthstencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

	SHADERMANAGER.UseShader(device,context,"vertical");
	context->PSSetShaderResources(0, 1, &(horizon.GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);

	SHADERMANAGER.UseShader(device,context,"horizontal");
	context->PSSetShaderResources(0, 1, &(colorBuffer.GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);

	SHADERMANAGER.UseShader(device,context,"colorshake"); 
	context->PSSetShaderResources(0, 1, &(colorBuffer.GetShaderResourceView()));
	m_screenRect->Render(device, context, 0, 0);

	SHADERMANAGER.UseShader(device,context,"tex2d");
	context->PSSetShaderResources(0, 1, &(colorBuffer.GetShaderResourceView()));
	m_rect->Render(device, context, 0, 0);
*/

//	btn->Render(device, context);
	TurnOnZBuffer(device, context);
}
void WorldClass::RenderScene(ID3D11Device * device,
	ID3D11DeviceContext * context)
{
	SHADERMANAGER.UseShader(device,context,"skybox");
	MCTextures[m_daytime->GetSkyboxTexture()]->PSBindTexture(device, context);
	m_sky->Render(device, context);
	m_sky->SetPosition(m_player->GetCamera()->GetX(),
		m_player->GetCamera()->GetY(),
		m_player->GetCamera()->GetZ(),
		device,
		context);
	m_sky->SetRotation(0.0f, skyRotation, skyRotation, device, context);
	skyRotation -= 0.0001f;
	if(skyRotation < -360.0f) skyRotation += 360.0f;
	context->Draw(36, 0);

	SHADERMANAGER.UseShader(device, context, "cloud");
	TurnOnAlphaRendering(device, context);

	MCTextures[CLOUD]->PSBindTexture(device, context);
	m_cloud->Draw(device, context, m_block);

	SHADERMANAGER.UseShader(device, context, "basic");
	//	bone1.SetYawPitchRoll([&]()->float{return ::my += 0.003f; }(), 0.0f, 0.0f);
	//	bone2.SetYawPitchRoll([&]()->float{return ::my2 += 0.03f; }(), 0.0f, 0.0f);

	D3DXVECTOR3 bone1ret = bone1.GetPosition();
	D3DXVECTOR3 bone2ret = bone2.GetPosition();
	D3DXVECTOR3 bone3ret = bone3.GetPosition();
	printf("%f\t%f\t%f\t%d\n", m_player->GetCamera()->GetX(), m_player->GetCamera()->GetY(), m_player->GetCamera()->GetZ(), m_daytime->GetTime());
	//	printf("\t%f\t%f\t%f\n\t%f\t%f\t%f\n\t%f\t%f\t%f\n", bone1ret[0], bone1ret[1], bone1ret[2],
	//		bone2ret[0], bone2ret[1], bone2ret[2], bone3ret[0], bone3ret[1], bone3ret[2]);


	//	DrawLine(m_block, device, context, bone1ret[0], bone1ret[1], bone1ret[2],
	//		bone2ret[0], bone2ret[1], bone2ret[2]);
	//	DrawLine(m_block, device, context, bone2ret[0], bone2ret[1], bone2ret[2],
	//		bone3ret[0], bone3ret[1], bone3ret[2]);

	m_block->Render(device, context);
	for (int i = -50; i<50; i++)
	for (int j = -50; j<50; j++)
	for (int my = -20; my<50; my++)
	{
		int tx = m_map->GetData(i, my, j);
		//			printf("%d ",tx);
		if (tx != 0 && m_player->GetCamera()->CheckBox(i, my, j, 0.5))
		{
			MCTextures[tx]->PSBindTexture(device, context);
			m_block->SetTransparency(device, context, 1.0f);
			m_block->SetRotation(0.0f, 0.0f, 0.0f, device, context);
			m_block->SetPosition(i, my, j, device, context);
			context->Draw(36, 0);
		}
	}
	TurnOffAlphaRendering(device, context);
	return ;
}

void WorldClass::TurnOnAlphaRendering(ID3D11Device* device,ID3D11DeviceContext* context)
{
	float rgba[] = {1.0f,1.0f,1.0f,1.0f};
	context->OMSetBlendState(AlphaRenderingEnabled,rgba,0xffffffff);
}

void WorldClass::TurnOffAlphaRendering(ID3D11Device* device, ID3D11DeviceContext* context)
{
	float rgba[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(AlphaRenderingDisabled, rgba, 0xffffffff);
}