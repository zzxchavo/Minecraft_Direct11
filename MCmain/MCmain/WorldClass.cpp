#include "WorldClass.h"

static float my = 1.0f,my2 = 1.0f;
BoneClass bone1, bone2 , bone3;

WorldClass::WorldClass(void)
{
//shader objcets
	CREATE_VERTEX_SHADER(skyboxVS);
	CREATE_PIXEL_SHADER(skyboxPS);

	CREATE_VERTEX_SHADER(basicVS);
	CREATE_PIXEL_SHADER(basicPS); 

	CREATE_VERTEX_SHADER(cloudVS);
	CREATE_PIXEL_SHADER(cloudPS);
//End shader objects


//BlockClass-based object
	SAFE_CREATE(m_block ,BlockClass);
	SAFE_CREATE(m_sky, SkyClass(580.0f));
	SAFE_CREATE(m_cloud , CloudClass);
//End 

	SAFE_CREATE(m_sound , SoundClass);
	SAFE_CREATE(m_input, InputClass);
	SAFE_CREATE(m_player, PlayerClass);
	SAFE_CREATE(m_map , MapGenerateClass);
	SAFE_CREATE(m_rect,Rectangle2DClass);
//mutexs & threads
	SAFE_CREATE(t_mutex , mutex);
	SAFE_CREATE(t_output ,mutex);
	updateThreadSwitch = true;
	OptimizeThreadSwitch = true;
//End thread operation.
//Ohter initialize operations.

	Chunks = 50;
	t_playerX = 0;
	t_playerY = 0;
	t_playerZ = 0;
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

// Delete ShaderClass objects
	DELETESHADER(basicVS);
	DELETESHADER(basicPS);

	DELETESHADER(skyboxVS);
	DELETESHADER(skyboxPS);

	DELETESHADER(cloudVS);
	DELETESHADER(cloudPS);
//End
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
HRESULT WorldClass::Initialize(ID3D11Device * device,ID3D11DeviceContext * context,HWND hwnd)
{

	HRESULT hr=S_OK;
	srand(time(NULL));
	basic_hwnd = hwnd;

	D3D11_INPUT_ELEMENT_DESC fogLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(fogLayout);
	hr = basicVS->Initialize(device,context,L"shaders/fog/fog.vs","VSEntry","vs_5_0",fogLayout,numElements);
	if(FAILED(hr))
	{
		MessageBox(NULL,L"VS Failed",L"Error - World Class",MB_OK);
		return hr;
	}
	hr = skyboxVS->Initialize(device,context,L"shaders/skybox/skybox.vs","VSEntry","vs_5_0",fogLayout,numElements);
	if(FAILED(hr))
	{
		MessageBox(NULL,L"VS Failed",L"Error - World Class",MB_OK);
		return hr;
	}
	hr = basicPS->Initialize(device,context,L"shaders/fog/fog.ps","PSEntry","ps_5_0");
	if(FAILED(hr))
	{
		MessageBox(NULL,L"PS Failed",L"Error - World Class",MB_OK);
		return hr;
	}

	hr = skyboxPS->Initialize(device,context,L"shaders/skybox/skybox.ps","PSEntry","ps_5_0");
	if(FAILED(hr))
	{
		MessageBox(NULL,L"PS Failed",L"Error - World Class",MB_OK);
		return hr;
	}
	hr = cloudVS->Initialize(device, context, L"shaders/skybox/cloud.vs", "VSEntry","vs_5_0",fogLayout,numElements);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"VS Failed", L"Error - WorldClass", MB_OK);
		return hr;
	}
	hr = cloudPS->Initialize(device, context, L"shaders/skybox/cloud.ps", "PSEntry", "ps_5_0");
	if (FAILED(hr))
	{
		MessageBox(NULL, L"PS Failed", L"Error - WorldClass", MB_OK);
		return hr;
	}
	if(!m_sky->Initialize(device,context))
		return E_FAIL;
	if(!m_block->Initialize(device,context))
		return E_FAIL;

//back color buffer operations!!!!!!!!!!!!!!!!!!!!!!!!!

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

	hr = device->CreateDepthStencilState(&depthDesc,&DepthStencilEnabled);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,"","",MB_OK);
	}

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

	hr = m_player->Initialize(device, context, m_map);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "error", "player", MB_OK);
		return false;
	}
	m_player->SetStep(0.167f);
//	m_map->GenerateArea(0, 0, 50);
	hr = m_cloud->Initialize(device, context);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "error", "cloud", MB_OK);
		return false;
	}
	hr = MCTextures.Initialize(device,context);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "error", "textures", MB_OK);
		return false;
	}
	hr = m_rect->Initialize(device,context);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "error", "m_rect", MB_OK);
		return false;
	}
	m_player->SetPosition(device,context,0.0f, 63.0f, 0.0f);
	
//	int max_height = (*m_map)[m_map->BKDHash(0, 0)].Max_height + 3;
//	m_player->SetPosition(device,context,0, max_height, 0);
//	printf("%d %d\n", (*m_map)[m_map->BKDHash(0, 0)].Max_height + 1,(int)m_player->GetCamera()->GetY());
	PlaySound(L"sound/calm.wav", NULL, SND_LOOP | SND_ASYNC);
//End
	bone1.SetName("hehe");
	bone2.SetName("haha");
	bone3.SetName("xixi");
	bone1.BindWithAsParent(&bone2);
	bone2.BindWithAsParent(&bone3);
	bone1.SetPosition(0.0f, 65.0f, 0.0f);
	bone2.SetPosition(0.0f, 65.0f, 10.0f);
	bone3.SetPosition(0.0f, 65.0f,15.0f);
	hr = colorBuffer.Initialize(device, context, 800, 600, 0.1f, 600.0f);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"", L"", MB_OK);
		return hr;
	}
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
	
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(rendertarget, ClearColor);
	context->ClearDepthStencilView(depthstencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//Set Render Target::Set the render target to be the render to texture
//	colorBuffer.SetRenderTarget(device, context);

	//clear the render to texture
//	colorBuffer.ClearRenderTarget(device, context, ClearColor);

	if (m_input->GetKeyState(VK_LEFT)) m_player->GetCamera()->AddRotationY(-1);
	if(m_input->GetKeyState(VK_RIGHT)) m_player->GetCamera()->AddRotationY( 1);
	if(m_input->GetKeyState(VK_DOWN)) m_player->GetCamera()->AddRotationX( 1);
	if(m_input->GetKeyState(VK_UP)) m_player->GetCamera()->AddRotationX(-1);
	if(m_input->GetKeyState(VK_SPACE))  m_player->GetCamera()->FlyUp();
	if(m_input->GetKeyState(VK_SHIFT))  m_player->GetCamera()->DiveDown();
	if(m_input->GetKeyState('W'))
	{
		m_player->GetCamera()->MoveForward();
//		printf("w");
	}
	if(m_input->GetKeyState('A'))
	{
		m_player->GetCamera()->MoveLeft();
//		printf("a");
	}
	if(m_input->GetKeyState('D'))
	{
		m_player->GetCamera()->MoveRight();
//		printf("d");
	}
	if(m_input->GetKeyState('S'))
	{
		m_player->GetCamera()->MoveBackward();
//		printf("s");
	}
	m_player->CheckCollision(device,context,m_map);
	m_player->GetCamera()->UpdateCamera(device, context);

	context->VSSetShader(cloudVS->GetVertexShader(), NULL, 0);
	context->PSSetShader(cloudPS->GetPixelShader(), NULL, 0);

	TurnOnAlphaRendering(device, context);
	
	MCTextures[CLOUD]->PSBindTexture(device,context);
	m_cloud->Draw(device,context,m_block);

	context->VSSetShader( basicVS->GetVertexShader(), NULL, 0 );
	context->PSSetShader( basicPS->GetPixelShader(), NULL, 0 );

//	bone1.SetYawPitchRoll([&]()->float{return ::my += 0.003f; }(), 0.0f, 0.0f);
	bone2.SetYawPitchRoll([&]()->float{return ::my2 += 0.03f; }(), 0.0f, 0.0f);

	D3DXVECTOR3 bone1ret = bone1.GetPosition();
	D3DXVECTOR3 bone2ret = bone2.GetPosition();
	D3DXVECTOR3 bone3ret = bone3.GetPosition();
	printf("\t%f\t%f\t%f\n", m_player->GetCamera()->GetX(), m_player->GetCamera()->GetY(), m_player->GetCamera()->GetZ());
/*	printf("\t%f\t%f\t%f\n\t%f\t%f\t%f\n\t%f\t%f\t%f\n", bone1ret[0], bone1ret[1], bone1ret[2],
		bone2ret[0], bone2ret[1], bone2ret[2], bone3ret[0], bone3ret[1], bone3ret[2]);

	
	DrawLine(m_block, device, context, bone1ret[0], bone1ret[1], bone1ret[2],
		bone2ret[0], bone2ret[1], bone2ret[2]);
	DrawLine(m_block, device, context, bone2ret[0], bone2ret[1], bone2ret[2],
		bone3ret[0], bone3ret[1], bone3ret[2]);
*/
	m_block->Render(device,context);
	for(int i=-50;i<50;i++)
		for(int j=-50;j<50;j++)
		for (int my = -20;my<50;my++)
		{
			int tx = (*m_map).GetData(i, my, j);
//			printf("%d ",tx);
			if(tx!=0&&m_player->GetCamera()->CheckBox(i,my,j,0.5))
			{
				MCTextures[COBBLE_STONE]->PSBindTexture(device,context);
				m_block->SetTransparency(device, context, 1.0f);
				m_block->SetRotation(0.0f, 0.0f, 0.0f, device, context);
				m_block->SetPosition(i,my,j,device,context);
				context->Draw(36,0);
			}
		}
	TurnOffAlphaRendering(device, context);

	context->VSSetShader( skyboxVS->GetVertexShader(), NULL, 0 );
	context->PSSetShader( skyboxPS->GetPixelShader(), NULL, 0 );

	MCTextures[SKYBOX_DAY]->PSBindTexture(device,context);
	m_sky ->Render(device,context);
	m_sky ->SetPosition(m_player->GetCamera()->GetX(),
		m_player->GetCamera()->GetY(),
		m_player->GetCamera()->GetZ(),
		device,
		context);
	context ->Draw(36, 0);
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	context->OMSetRenderTargets(1, &rendertarget, depthstencil);
	// Reset the viewport back to the original.
	context->RSSetViewports(1, &viewport);
	TurnOffZBuffer(device, context);
	m_rect->Render(device,context);
	context->Draw(0, 12);
	TurnOnZBuffer(device, context);
///DownSampleTexture
//	m_DownSampleTexure.SetRenderTarget(device, context);
//	m_DownSampleTexure.ClearRenderTarget(device,context,ClearColor);
	;
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