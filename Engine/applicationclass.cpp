////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_Light = 0;
	m_projectileCount = 0;
	m_projectileCountDelta = 0;
	m_projectileCountTorret = 0;
	m_projectileCountBoss = 0;
	enemyBlockStep = false;
	resetGame = false;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Bitmap01 = 0;
	m_Bitmap02 = 0;
	m_Bitmap03 = 0;

}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	
	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	m_baseStaticMatrix = baseViewMatrix;

	// Set the initial position of the camera.
	cameraX = 500.0f;
	cameraY = 5.0f;
	cameraZ = 0.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	int textureCount = 5;
	WCHAR** filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/grass.jpg";
	filenames[1] = L"../Engine/data/soil.jpg";
	filenames[2] = L"../Engine/data/rock.jpg";
	filenames[3] = L"../Engine/data/snow.jpg";
	filenames[4] = L"../Engine/data/alpha.jpg";

	result = m_Terrain->Initialize(m_Direct3D->GetDevice(), "../Engine/data/heightmap02.bmp", filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.
	m_Water = new TerrainClass;
	if (!m_Water)
	{
		return false;
	}

	// Initialize the terrain object.
	textureCount = 3;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/waterDiffuse.jpg";
	filenames[1] = L"../Engine/data/waterNormal.png";
	filenames[2] = L"../Engine/data/waterSpecular.png";

	result = m_Water->Initialize(m_Direct3D->GetDevice(), "../Engine/data/waterHeightmap.bmp", filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Skydome = new SkydomeClass;
	if (!m_Skydome)
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/skydome/Skydome01.jpg";
	filenames[1] = L"../Engine/data/skydome/Skydome02.jpg";

	// Initialize the model object.
	result = m_Skydome->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[0] = new BillboardClass;
	if (!m_Billboard[0])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/bush/Bush_1.png";
	filenames[1] = L"../Engine/data/bush/Bush_1Op.png";

	// Initialize the model object.
	result = m_Billboard[0]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[1] = new BillboardClass;
	if (!m_Billboard[1])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/bush/Bush_2.png";
	filenames[1] = L"../Engine/data/bush/Bush_2Op.png";

	// Initialize the model object.
	result = m_Billboard[1]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard[2] = new BillboardClass;
	if (!m_Billboard[2])
	{
		return false;
	}

	textureCount = 2;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/bush/Bush_3.png";
	filenames[1] = L"../Engine/data/bush/Bush_3Op.png";

	// Initialize the model object.
	result = m_Billboard[2]->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);
	m_Position->SetPositionDelta(500.0f, 5.0f, 1000.0f);
	m_Position->SetRotationDelta(0.0f, 180.0f, 0.0f);

	m_Position->SetPositionTorret(490.0f, 3.0f, 900.0f);
	m_Position->SetRotationTorret(0.0f, 180.0f, 0.0f);

	m_Position->SetPositionEnemyBoss(590.0f, -30.0f, 1350.0f);
	m_Position->SetRotationEnemyBoss(0.0f, 0.0f, 0.0f);

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_SkydomeShader = new SkydomeShaderClass;
	if (!m_SkydomeShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_SkydomeShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_WaterShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_BillboardShader = new BillboardShaderClass;
	if (!m_BillboardShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_BillboardShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_GroupShader = new GroupShaderClass;
	if (!m_GroupShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_GroupShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_DiffuseShader = new DiffuseShaderClass;
	if (!m_DiffuseShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_DiffuseShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}


	m_stadistics = new stadistics;
	m_stadistics->SetDeltalife(3);

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/HUDS/HUDBalckComet_Status.png", 768, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap01 = new BitmapClass;
	if (!m_Bitmap01)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Bitmap01->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/HUDS/HUDBalckComet_Status1.png", 768, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap02 = new BitmapClass;
	if (!m_Bitmap02)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Bitmap02->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/HUDS/HUDBalckComet_HBar.png", 384, 128);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap03 = new BitmapClass;
	if (!m_Bitmap03)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Bitmap03->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/HUDS/HUDBalckComet_MBar.png", 384, 128);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Projectile = new SphereClass;
	if (!m_Projectile)
	{
		return false;
	}

	// Create the model object.
	m_ProjectileDelta = new SphereClass;
	if (!m_ProjectileDelta)
	{
		return false;
	}

	textureCount = 1;
	filenames = new WCHAR*[textureCount];
	filenames[0] = L"../Engine/data/waterDiffuse.jpg";

	// Initialize the model object.
	result = m_Projectile->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model object.
	result = m_ProjectileDelta->Initialize(m_Direct3D->GetDevice(), filenames, textureCount);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	
	m_Cup = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/cup", "cup.obj");
	m_Principal = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/principal", "principal.obj");
	m_delta = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/enemy/delta", "delta.obj");
	m_boss = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/enemy/b_wing", "b_wing.obj");
	m_car01 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/cars/01", "fCars.obj");
	m_car02 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/cars/02", "fCars01.obj");
	m_building01 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/01", "building01.obj");
	m_building01_1 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/01", "building01.obj");
	m_building02 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/02", "building02.obj");
	m_building03 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/03", "building03.obj");
	m_building04 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/04", "building04.obj");
	m_building04_1 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/04", "building04.obj");
	m_building05 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/05", "building05.obj");
	m_building06 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/06", "building06.obj");
	m_building06_1 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/06", "building06.obj");
	m_building06_2 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/06", "building06.obj");
	m_building06A = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/building/07", "building06A.obj");
	m_floor01 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/floor/01", "floor.obj");
	m_floor02 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/floor/02", "floor02.obj");
	m_torret = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/enemy/torret", "torret.obj");
	m_pups = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Pups/01", "pups01.obj");
	m_pups02 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Pups/01", "pups01.obj");
	m_pups03 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Pups/01", "pups01.obj");
	m_pups04 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Pups/02", "pups02.obj");
	m_pups05 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Pups/02", "pups02.obj");
	m_pups06 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/Pups/02", "pups02.obj");
	m_proyectileLaser = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/laser/03", "laser.obj");
	m_proyectileQuad = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/laser/01", "laserQuad.obj");
	m_proyectileQuad01 = Model::ObjToModel(m_Direct3D->GetDevice(), "../Engine/data/laser/02", "laserQuad01.obj");
	

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	globalSphere = new BoundingSphere(D3DXVECTOR3(500.0f, 0.0f, 500.0f), 1000.0f);
	BSVisionTorret = new BoundingSphere(D3DXVECTOR3(490.0f, 3.0f, 900.0f), 50.0f);
	BSVisionEnemyBoss = new BoundingSphere(D3DXVECTOR3(500.0f, 30.0f, 1350.0f), 150.0f);
	m_ProVel = 20;
	m_ProVelDelta = 20;
	m_ProVelTorret = 30;
	m_ProVelBoss = 40;
	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap01)
	{
		m_Bitmap01->Shutdown();
		delete m_Bitmap01;
		m_Bitmap01 = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap02)
	{
		m_Bitmap02->Shutdown();
		delete m_Bitmap02;
		m_Bitmap02 = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap03)
	{
		m_Bitmap03->Shutdown();
		delete m_Bitmap03;
		m_Bitmap03 = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;


	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Render the graphics.
	result = RenderGraphics();
	if(!result)
	{
		return false;
	}

	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result, type;
	float posX, posY, posZ, rotX, rotY, rotZ, AuposX, AuposY, AuposZ;

	int mouseX, mouseY;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsLeftPressed(); //A
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed(); //D
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsUpPressed(); //W
	//m_Position->MoveForward(keyDown);
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsDownPressed(); //S
	//m_Position->MoveBackward(keyDown);
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsResetPressed();
	resetGame = keyDown;

	keyDown = m_Input->IsXPressed();
	//m_Position->MoveUpward(keyDown);
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsZPressed();
	//m_Position->MoveDownward(keyDown);
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);
	
	keyDown = m_Input->IsCPressed();
	m_Position->ChangeCameraType(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);
	m_Position->GetCameraType(type);
	m_Position->GetPosition(AuposX, AuposY, AuposZ);



	m_Input->GetMouseLocation(mouseX, mouseY);

	// Set the position of the camera.
	m_Terrain->GetHeightAtPosition(AuposX, AuposZ, AuposY);

	if (posY <= AuposY) {
		m_Camera->SetPosition(posX, AuposY, posZ);
	}
	else {
		m_Camera->SetPosition(posX, posY, posZ);
	}
	//m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(mouseY, mouseX, 0);
	m_Camera->SetCameraType(type);

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	static int pro = 0;
	keyDown = m_Input->IsLeftClickPressed();

	if (keyDown && pro == 0)
	{
		m_projectileCount++;

		m_projectilePos.push_back(D3DXVECTOR3(posX, posY, posZ));
		m_projectileRot.push_back(D3DXVECTOR3(rotX, rotY, rotZ));
		pro = m_ProVel;
	}
	else
	{
		pro--;
		if (pro <= 0) pro = 0;
	}



	return true;
}


bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn off back face culling.
	m_Direct3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_Direct3D->TurnZBufferOff();

	D3DXVECTOR3 cameraPosition;
	cameraPosition = m_Camera->GetPosition();
	//movimientos del Skydome
	D3DXMATRIX SkyScale, SkyTrans, SkyDomeMatrix;
	D3DXMatrixScaling(&SkyScale, 1, 1, 1);
	//D3DXMatrixRotationAxis(&SkyRot, &D3DXVECTOR3(1, 1, 0), rotation * 0.5);
	D3DXMatrixTranslation(&SkyTrans, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	SkyDomeMatrix = SkyScale * SkyTrans;
	m_Skydome->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_SkydomeShader->Render(m_Direct3D->GetDeviceContext(), m_Skydome->GetIndexCount(), SkyDomeMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Skydome->GetTexture(), m_Skydome->GetTextureCount());
	if (!result)
	{
		return false;
	}
	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();


	// Render the terrain buffers.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTextures(), m_Terrain->GetTextureCount());

	if (!result)
	{
		return false;
	}

	// Render the billboard buffers.
	m_Billboard[0]->Render(m_Direct3D->GetDeviceContext());

	// Render the billboard using the billboard shader.
	for (int i = 260; i < 400; i += 40) {
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[0]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(479, 0, i)), viewMatrix, projectionMatrix,
			m_Billboard[0]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}
	for (int i = 550; i < 760; i += 30) {
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[0]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(i, 0, 300)), viewMatrix, projectionMatrix,
			m_Billboard[0]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}


	// Render the terrain buffers.
	m_Billboard[1]->Render(m_Direct3D->GetDeviceContext());

	for (int i = 640; i < 730; i += 40) {
		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[1]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(479, 0, i)), viewMatrix, projectionMatrix,
			m_Billboard[1]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}
	for (int i = 530; i < 800; i += 30) {
		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[1]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(i, 0, 908)), viewMatrix, projectionMatrix,
			m_Billboard[1]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}

	// Render the terrain buffers.
	m_Billboard[2]->Render(m_Direct3D->GetDeviceContext());

	for (int i = 550; i < 760; i += 30) {
		// Render the terrain using the terrain shader.
		result = m_BillboardShader->Render(m_Direct3D->GetDeviceContext(), m_Billboard[2]->GetIndexCount(), BillboardClass::GenerateWorldMatrix(m_Camera->GetPosition(), D3DXVECTOR3(i, 0, 200)), viewMatrix, projectionMatrix,
			m_Billboard[2]->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 10);
		if (!result)
		{
			return false;
		}
	}



	D3DXMATRIX trans, scale, cupWorldMatrix;
	D3DXMatrixTranslation(&trans, 80, 5, 20);
	D3DXMatrixScaling(&scale, 5, 5, 5);
	cupWorldMatrix = scale * trans;
	m_Cup->UpdateColliders(cupWorldMatrix);
	for (auto &group : *m_Cup->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), cupWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//Buildngs
	D3DXMATRIX b01trans, b01scale, b01WorldMatrix;
	D3DXMatrixTranslation(&b01trans, 650, 30, 380);
	D3DXMatrixScaling(&b01scale, 1, 1, 1);
	b01WorldMatrix = b01scale * b01trans;
	m_building01->UpdateColliders(b01WorldMatrix);
	for (auto &group : *m_building01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b01_1trans, b01_1scale, b01_1WorldMatrix;
	D3DXMatrixTranslation(&b01_1trans, 570, 35, 410);
	D3DXMatrixScaling(&b01_1scale, 1, 1, 1);
	b01_1WorldMatrix = b01_1scale * b01_1trans;
	m_building01_1->UpdateColliders(b01_1WorldMatrix);
	for (auto &group : *m_building01_1->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b01_1WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b02trans, b02scale, b02WorldMatrix;
	D3DXMatrixTranslation(&b02trans, 540, 30, 450);
	D3DXMatrixScaling(&b02scale, 1, 1, 1);
	b02WorldMatrix = b02scale * b02trans;
	m_building02->UpdateColliders(b02WorldMatrix);
	for (auto &group : *m_building02->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b02WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}


	D3DXMATRIX b03trans, b03scale, b03WorldMatrix;
	D3DXMatrixTranslation(&b03trans, 465, 60, 820);
	D3DXMatrixScaling(&b03scale, 1, 1, 1);
	b03WorldMatrix = b03scale * b03trans;
	m_building03->UpdateColliders(b03WorldMatrix);
	for (auto &group : *m_building03->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b03WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b04trans, b04scale, b04WorldMatrix;
	D3DXMatrixTranslation(&b04trans, 540, 30, 820);
	D3DXMatrixScaling(&b04scale, 1, 1, 1);
	b04WorldMatrix = b04scale * b04trans;
	m_building04->UpdateColliders(b04WorldMatrix);
	for (auto &group : *m_building04->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b04WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b04_1trans, b04_1scale, b04_1WorldMatrix;
	D3DXMatrixTranslation(&b04_1trans, 465, 30, 600);
	D3DXMatrixScaling(&b04_1scale, 1, 1, 1);
	b04_1WorldMatrix = b04_1scale * b04_1trans;
	m_building04_1->UpdateColliders(b04_1WorldMatrix);
	for (auto &group : *m_building04_1->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b04_1WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b05trans, b05scale, b05WorldMatrix;
	D3DXMatrixTranslation(&b05trans, 462, 30, 500);
	D3DXMatrixScaling(&b05scale, 1, 1, 1);
	b05WorldMatrix = b05scale * b05trans;
	m_building05->UpdateColliders(b05WorldMatrix);
	for (auto &group : *m_building05->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b05WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06trans, b06scale, b06WorldMatrix;
	D3DXMatrixTranslation(&b06trans, 540, 17, 550);
	D3DXMatrixScaling(&b06scale, 1, 1, 1);
	b06WorldMatrix = b06scale * b06trans;
	m_building06->UpdateColliders(b06WorldMatrix);
	for (auto &group : *m_building06->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06_1trans, b06_1scale, b06_1WorldMatrix;
	D3DXMatrixTranslation(&b06_1trans, 540, 17, 590);
	D3DXMatrixScaling(&b06_1scale, 1, 1, 1);
	b06_1WorldMatrix = b06_1scale * b06_1trans;
	m_building06_1->UpdateColliders(b06_1WorldMatrix);
	for (auto &group : *m_building06_1->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06_1WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06_2trans, b06_2scale, b06_2WorldMatrix;
	D3DXMatrixTranslation(&b06_2trans, 540, 17, 630);
	D3DXMatrixScaling(&b06_2scale, 1, 1, 1);
	b06_2WorldMatrix = b06_2scale * b06_2trans;
	m_building06_2->UpdateColliders(b06_2WorldMatrix);
	for (auto &group : *m_building06_2->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06_2WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX b06Atrans, b06Ascale, b06AWorldMatrix;
	D3DXMatrixTranslation(&b06Atrans, 600, 17, 380);
	D3DXMatrixScaling(&b06Ascale, 1, 1, 1);
	b06AWorldMatrix = b06Ascale * b06Atrans;
	m_building06A->UpdateColliders(b06AWorldMatrix);
	for (auto &group : *m_building06A->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), b06AWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//Floor
	D3DXMATRIX f02trans, f02scale, f02WorldMatrix;
	D3DXMatrixTranslation(&f02trans, 500, 1.2, 210);
	D3DXMatrixScaling(&f02scale, 1, 1, 1);
	f02WorldMatrix = f02scale * f02trans;
	m_floor02->UpdateColliders(f02WorldMatrix);
	for (auto &group : *m_floor02->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f02WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f01trans, f01scale, f01WorldMatrix;
	D3DXMatrixTranslation(&f01trans, 500, 1.2, 350);
	D3DXMatrixScaling(&f01scale, 1, 1, 1);
	f01WorldMatrix = f01scale * f01trans;
	m_floor01->UpdateColliders(f01WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f03trans, f03scale, f03WorldMatrix;
	D3DXMatrixTranslation(&f03trans, 500, 1.2, 490);
	D3DXMatrixScaling(&f03scale, 1, 1, 1);
	f03WorldMatrix = f03scale * f03trans;
	m_floor01->UpdateColliders(f03WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f03WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f04trans, f04scale, f04WorldMatrix;
	D3DXMatrixTranslation(&f04trans, 500, 1.2, 630);
	D3DXMatrixScaling(&f04scale, 1, 1, 1);
	f04WorldMatrix = f04scale * f04trans;
	m_floor01->UpdateColliders(f04WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f04WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f05trans, f05scale, f05WorldMatrix;
	D3DXMatrixTranslation(&f05trans, 500, 1.2, 770);
	D3DXMatrixScaling(&f05scale, 1, 1, 1);
	f05WorldMatrix = f05scale * f05trans;
	m_floor01->UpdateColliders(f05WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f05WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	D3DXMATRIX f06trans, f06scale, f06WorldMatrix;
	D3DXMatrixTranslation(&f06trans, 500, 1.2, 910);
	D3DXMatrixScaling(&f06scale, 1, 1, 1);
	f06WorldMatrix = f06scale * f06trans;
	m_floor01->UpdateColliders(f06WorldMatrix);
	for (auto &group : *m_floor01->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f06WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//Floor Special
	D3DXMATRIX f07trans, f07scale, f07rotate, f07WorldMatrix;
	D3DXMatrixTranslation(&f07trans, 587, 1.2, 350);
	D3DXMatrixScaling(&f07scale, 1, 1, 1);
	D3DXMatrixRotationY(&f07rotate, 80.1);
	f07WorldMatrix = f07scale * f07rotate * f07trans;
	m_floor02->UpdateColliders(f07WorldMatrix);
	for (auto &group : *m_floor02->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), f07WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//Power ups
	static bool renderPUps01 = true;
	if (renderPUps01) {
		D3DXMATRIX putrans, puscale, purotate, puWorldMatrix;

		static float pUpMove = 0;
		pUpMove += .1;
		float A = sin(pUpMove) * .2;

		D3DXMatrixTranslation(&putrans, 500, 5 + A, 900);
		D3DXMatrixScaling(&puscale, 3, 3, 3);
		//D3DXMatrixRotationY(&deltarotate, 80.1);
		puWorldMatrix = puscale* putrans;
		m_pups->UpdateColliders(puWorldMatrix);
		for (auto &group : *m_pups->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), puWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	//Power ups 02
	static bool renderPUps02 = true;
	if (renderPUps02) {
		D3DXMATRIX putrans02, puscale02, purotate02, puWorldMatrix02;

		static float pUpMove = 0;
		pUpMove += .1;
		float A = sin(pUpMove) * .2;

		D3DXMatrixTranslation(&putrans02, 509, 11 + A, 1106);
		D3DXMatrixScaling(&puscale02, 3, 3, 3);
		//D3DXMatrixRotationY(&deltarotate, 80.1);
		puWorldMatrix02 = puscale02* putrans02;
		m_pups02->UpdateColliders(puWorldMatrix02);
		for (auto &group : *m_pups02->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), puWorldMatrix02, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	//Power ups 03
	static bool renderPUps03 = true;
	if (renderPUps03) {
		D3DXMATRIX putrans03, puscale03, purotate03, puWorldMatrix03;

		static float pUpMove = 0;
		pUpMove += .1;
		float A = sin(pUpMove) * .2;

		D3DXMatrixTranslation(&putrans03, 512, 11 + A, 451);
		D3DXMatrixScaling(&puscale03, 3, 3, 3);
		//D3DXMatrixRotationY(&deltarotate, 80.1);
		puWorldMatrix03 = puscale03* putrans03;
		m_pups03->UpdateColliders(puWorldMatrix03);
		for (auto &group : *m_pups03->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), puWorldMatrix03, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	//Power ups 04
	static bool renderPUps04 = true;
	if (renderPUps04) {
		D3DXMATRIX putrans04, puscale04, purotate04, puWorldMatrix04;

		static float pUpMove = 0;
		pUpMove += .1;
		float A = sin(pUpMove) * .2;

		D3DXMatrixTranslation(&putrans04, 500, 17 + A, 720);
		D3DXMatrixScaling(&puscale04, 3, 3, 3);
		//D3DXMatrixRotationY(&deltarotate, 80.1);
		puWorldMatrix04 = puscale04* putrans04;
		m_pups04->UpdateColliders(puWorldMatrix04);
		for (auto &group : *m_pups04->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), puWorldMatrix04, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	//Power ups 05
	static bool renderPUps05 = true;
	if (renderPUps05) {
		D3DXMATRIX putrans05, puscale05, purotate05, puWorldMatrix05;

		static float pUpMove = 0;
		pUpMove += .1;
		float A = sin(pUpMove) * .2;

		D3DXMatrixTranslation(&putrans05, 490, 11 + A, 625);
		D3DXMatrixScaling(&puscale05, 3, 3, 3);
		//D3DXMatrixRotationY(&deltarotate, 80.1);
		puWorldMatrix05 = puscale05* putrans05;
		m_pups05->UpdateColliders(puWorldMatrix05);
		for (auto &group : *m_pups05->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), puWorldMatrix05, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	//Power ups 06
	static bool renderPUps06 = true;
	if (renderPUps06) {
		D3DXMATRIX putrans06, puscale06, purotate06, puWorldMatrix06;

		static float pUpMove = 0;
		pUpMove += .1;
		float A = sin(pUpMove) * .2;

		D3DXMatrixTranslation(&putrans06, 494, 17 + A, 850);
		D3DXMatrixScaling(&puscale06, 3, 3, 3);
		//D3DXMatrixRotationY(&deltarotate, 80.1);
		puWorldMatrix06 = puscale06* putrans06;
		m_pups06->UpdateColliders(puWorldMatrix06);
		for (auto &group : *m_pups06->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), puWorldMatrix06, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	//other cars
	
	//D3DXMATRIX car01trans, car01scale, car01rotate, car01WorldMatrix;
	static float posXcar, posYcar, posZcar ;
	if ( !m_Position->GetAnimationCarControl(1) ) {
		m_Position->GetPositionCar(posXcar, posYcar, posZcar, 3);
	}
	if (posXcar <= -67 && !m_Position->GetAnimationCarControl(1)) {
		m_Position->ChangeAnimationCarControl(1);
		m_Position->ChangeAnimationCarControl(0);
	}
	D3DXMATRIX car01trans, car01scale, car01WorldMatrix;
	D3DXMatrixTranslation(&car01trans, 590 + posXcar, 3 + posYcar, 352 + posZcar);
	D3DXMatrixScaling(&car01scale, 1, 1, 1);
	//D3DXMatrixRotationY(&car01rotate, 80.1);
	//car01WorldMatrix = car01scale * car01rotate * car01trans;
	car01WorldMatrix = car01scale * car01trans;

	if (!m_Position->GetAnimationCarControl(0)) {
		
		m_car01->UpdateColliders(car01WorldMatrix);
		for (auto &group : *m_car01->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), car01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}
	else {
		m_car02->UpdateColliders(car01WorldMatrix);
		for (auto &group : *m_car02->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), car01WorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}


	//Enemy
	float posXDelta, posYDelta, posZDelta, rotXDelta, rotYDelta, rotZDelta;
	static bool enproDelta = true;
	//Get the enemy point position/rotation.
	if (enproDelta) {
		m_Position->GetPositionDelta(posXDelta, posYDelta, posZDelta);
		m_Position->GetRotationDelta(rotXDelta, rotYDelta, rotZDelta);

		D3DXMATRIX deltatrans, deltascale, deltarotate, deltaWorldMatrix;
		D3DXMatrixTranslation(&deltatrans, posXDelta, posYDelta, posZDelta);
		D3DXMatrixScaling(&deltascale, 1, 1, 1);
		//D3DXMatrixRotationY(&deltarotate, 80.1);
		deltaWorldMatrix = deltascale* deltatrans;
		m_delta->UpdateColliders(deltaWorldMatrix);
		for (auto &group : *m_delta->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), deltaWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}


	//Enemy Torret
	float posXtorret, posYtorret, posZtorret, rotXtorret, rotYtorret, rotZtorret;
	static bool enproTorret = true, seeVisionTorret = false;
	//Get the enemy point position/rotation.
	m_Position->GetPositionTorret(posXtorret, posYtorret, posZtorret);
	m_Position->GetRotationTorret(rotXtorret, rotYtorret, rotZtorret);

	if (enproTorret) {
		D3DXMATRIX torrettrans, torretscale, torretrotate, torretWorldMatrix;
		D3DXMatrixTranslation(&torrettrans, posXtorret, posYtorret, posZtorret);
		D3DXVECTOR3 positionObjetive = D3DXVECTOR3(0, 0, 0);
		m_Position->GetPosition(positionObjetive.x, positionObjetive.y, positionObjetive.z);
		float anguloObjetive = atan2(posXtorret - positionObjetive.x, posZtorret - positionObjetive.z)*(180 / D3DX_PI);
		D3DXMatrixScaling(&torretscale, 1, 1, 1);
		D3DXMatrixRotationY(&torretrotate, anguloObjetive);
		torretWorldMatrix = torretscale* torretrotate * torrettrans;
		m_torret->UpdateColliders(torretWorldMatrix);
		for (auto &group : *m_torret->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), torretWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}

	//Boss
	float posXBoss, posYBoss, posZBoss, rotXBoss, rotYBoss, rotZBoss;
	static bool bossRender = false, bossRenderPanel = false, ctrlVisionTorret = false;
	m_Position->GetPositionEnemyBoss(posXBoss, posYBoss, posZBoss);
	m_Position->GetRotationEnemyBoss(rotXBoss, rotYBoss, rotZBoss);



	if (m_Principal->GetBS()->Collides(BSVisionEnemyBoss) && ((m_stadistics->GetBosslife()) > 0.0f)) {
		bossRender = true;
	}
	else {
		bossRender = false;
	}

	static float AnimXBoss, AnimYBoss, AnimZBoss;
	if (bossRender) {
		
		if (!m_Position->GetAnimationBossControl(1)) {
			m_Position->GetPositionAnimationBoss(AnimXBoss, AnimYBoss, AnimZBoss, 1);
		}
		if (AnimYBoss >= 30 && !m_Position->GetAnimationBossControl(1)) {
			m_Position->ChangeAnimationBossControl(1);
			m_Position->ChangeAnimationBossControl(0);
		}
		if (m_Position->GetAnimationBossControl(0)) {
			static float waterMov = 0;
			waterMov += .1;
			float A = sin(waterMov) * .5;
			AnimYBoss = AnimYBoss + A;
		}

		D3DXMATRIX Bosstrans, Bossscale, Bossrotate, BossWorldMatrix;
		D3DXMatrixTranslation(&Bosstrans, posXBoss + AnimXBoss, posYBoss + AnimYBoss, posZBoss + AnimZBoss);
		D3DXMatrixScaling(&Bossscale, 1, 1, 1);
		D3DXMatrixRotationY(&Bossrotate, rotYBoss);
		BossWorldMatrix = Bossscale* Bosstrans;
		m_boss->UpdateColliders(BossWorldMatrix);
		for (auto &group : *m_boss->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), BossWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}
	}
	
	//Principal
	//m_Position->MoveForward(true);
	static bool defeatRender = true;
	D3DXMATRIX transla, rotat;
	
	D3DXVECTOR3 HPosition;
	HPosition = m_Camera->GetPosition();

	static D3DXMATRIX tempprincipalWorldMatrix, principalWorldMatrix;
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	float yaw, pitch, roll;

	m_Position->GetPosition(position.x, position.y, position.z);
	m_Position->GetRotation(rotation.x, rotation.y, rotation.z);

	pitch = rotation.x * 0.0174532925f;
	yaw = (rotation.y + 180.0) * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	m_Terrain->GetHeightAtPosition(position.x, position.z, position.y);
	if (HPosition.y <= position.y) {
		D3DXMatrixTranslation(&transla, position.x, position.y, position.z);
	}
	else {
		D3DXMatrixTranslation(&transla, position.x, HPosition.y, position.z);
	}

	D3DXMatrixRotationYawPitchRoll(&rotat, yaw, 0, 0);



	tempprincipalWorldMatrix = rotat * transla;

	m_Principal->UpdateColliders(tempprincipalWorldMatrix);

	if (!m_Principal->Collides(m_Cup)) {
		principalWorldMatrix = tempprincipalWorldMatrix;
	}
	else
	{
		tempprincipalWorldMatrix = tempprincipalWorldMatrix;
	}

	for (auto &group : *m_Principal->GetGroupList())
	{
		group.Render(m_Direct3D->GetDeviceContext());
		result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), principalWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
		if (!result)
		{
			return false;
		}
	}

	//m_Projectile->Render(m_Direct3D->GetDeviceContext());

	for (int i = 0; i < m_projectileCount; i++)
	{
		D3DXMATRIX rotatPro, proWorldMatrix;

		D3DXVECTOR3 move = D3DXVECTOR3(0, 0, 10);

		pitch = m_projectileRot[i].x * 0.0174532925f;
		yaw = m_projectileRot[i].y * 0.0174532925f;
		roll = m_projectileRot[i].z * 0.0174532925f;

		D3DXMatrixRotationYawPitchRoll(&rotatPro, yaw, 0, 0);

		D3DXVECTOR4 transformed;
		D3DXVec3Transform(&transformed, &move, &rotatPro);

		m_projectilePos[i].x += transformed.x;
		m_projectilePos[i].y += transformed.y;
		m_projectilePos[i].z += transformed.z;

		D3DXMatrixTranslation(&proWorldMatrix, m_projectilePos[i].x, m_projectilePos[i].y, m_projectilePos[i].z);

		if (!globalSphere->Collides(new BoundingSphere(m_projectilePos[i], 5)))
		{
			m_projectileCount--;
			m_projectilePos.erase(m_projectilePos.begin() + i);
			m_projectileRot.erase(m_projectileRot.begin() + i);
		}
		else if (m_delta->GetBS()->Collides(new BoundingSphere(m_projectilePos[i], 5))) {
			m_projectileCount--;
			m_projectilePos.erase(m_projectilePos.begin() + i);
			m_projectileRot.erase(m_projectileRot.begin() + i);
			m_stadistics->SetDeltalifeMinus();
			if ((m_stadistics->GetDeltalife()) <= 0.0f) {
				enproDelta = false;
			}
		}
		else if (m_boss->GetBS()->Collides(new BoundingSphere(m_projectilePos[i], 5))) {
			m_projectileCount--;
			m_projectilePos.erase(m_projectilePos.begin() + i);
			m_projectileRot.erase(m_projectileRot.begin() + i);
			m_stadistics->SetBosslifeMinus();
			if ((m_stadistics->GetBosslife()) <= 0.0f) {
				bossRenderPanel = true;
				bossRender = false;
			}
		}
		else if (m_torret->GetBS()->Collides(new BoundingSphere(m_projectilePos[i], 5))) {
			m_projectileCount--;
			m_projectilePos.erase(m_projectilePos.begin() + i);
			m_projectileRot.erase(m_projectileRot.begin() + i);
			m_stadistics->SetTorretlifeMinus();
			if ((m_stadistics->GetTorretlife()) <= 0.0f) {
				enproTorret = false;
			}
		}

		/*result = m_DiffuseShader->Render(m_Direct3D->GetDeviceContext(), m_Projectile->GetIndexCount(), proWorldMatrix, viewMatrix, projectionMatrix,
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Projectile->GetTexture());*/
		for (auto &group : *m_proyectileQuad->GetGroupList())
		{
			group.Render(m_Direct3D->GetDeviceContext());
			result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), proWorldMatrix, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
			if (!result)
			{
				return false;
			}
		}

		if (!result)
		{
			return false;
		}
	}

	static int proDelta = 0;
	//Enemy Projectil
	if (enproDelta == true) {
		if (proDelta == 0) {
			m_projectileCountDelta++;

			m_projectilePosDelta.push_back(D3DXVECTOR3(posXDelta, posYDelta, posZDelta));
			m_projectileRotDelta.push_back(D3DXVECTOR3(rotXDelta, rotYDelta, rotZDelta));
			proDelta = m_ProVelDelta;
		}
		else
		{
			proDelta--;
			if (proDelta <= 0) proDelta = 0;
		}

		//m_ProjectileDelta->Render(m_Direct3D->GetDeviceContext());

		for (int i = 0; i < m_projectileCountDelta; i++)
		{
			D3DXMATRIX rotatProDelta, proWorldMatrixDelta;

			D3DXVECTOR3 moveDelta = D3DXVECTOR3(0, 0, 5);

			pitch = m_projectileRotDelta[i].x * 0.0174532925f;
			yaw = m_projectileRotDelta[i].y * 0.0174532925f;
			roll = m_projectileRotDelta[i].z * 0.0174532925f;

			D3DXMatrixRotationYawPitchRoll(&rotatProDelta, yaw, 0, 0);

			D3DXVECTOR4 transformedDelta;
			D3DXVec3Transform(&transformedDelta, &moveDelta, &rotatProDelta);

			m_projectilePosDelta[i].x += transformedDelta.x;
			m_projectilePosDelta[i].y += transformedDelta.y;
			m_projectilePosDelta[i].z += transformedDelta.z;

			D3DXMatrixTranslation(&proWorldMatrixDelta, m_projectilePosDelta[i].x, m_projectilePosDelta[i].y, m_projectilePosDelta[i].z);

			if (!globalSphere->Collides(new BoundingSphere(m_projectilePosDelta[i], 5)))
			{
				m_projectileCountDelta--;
				m_projectilePosDelta.erase(m_projectilePosDelta.begin() + i);
				m_projectileRotDelta.erase(m_projectileRotDelta.begin() + i);
			}

			else if (m_Principal->GetBS()->Collides(new BoundingSphere(m_projectilePosDelta[i], 5)))
			{
				m_projectileCountDelta--;
				m_projectilePosDelta.erase(m_projectilePosDelta.begin() + i);
				m_projectileRotDelta.erase(m_projectileRotDelta.begin() + i);
				m_stadistics->SetMylifeMinus();
				if ((m_stadistics->GetMylife()) <= 0.0f) {
					defeatRender = false;
				}
			}


			/*result = m_DiffuseShader->Render(m_Direct3D->GetDeviceContext(), m_ProjectileDelta->GetIndexCount(), proWorldMatrixDelta, viewMatrix, projectionMatrix,
				m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_ProjectileDelta->GetTexture());*/
			for (auto &group : *m_proyectileQuad->GetGroupList())
			{
				group.Render(m_Direct3D->GetDeviceContext());
				result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), proWorldMatrixDelta, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
				if (!result)
				{
					return false;
				}
			}

			if (!result)
			{
				return false;
			}
		}

		m_Position->SetPositionDelta(posXDelta, posYDelta, posZDelta - 1.0f);
	}

	//Torret Projectile
	static int proTorret = 0;

	if (m_Principal->GetBS()->Collides(BSVisionTorret)) {
		seeVisionTorret = true;
	}
	else {
		seeVisionTorret = false;
	}

	if (enproTorret) {
		if (seeVisionTorret) {
			if (proTorret == 0) {
				m_projectileCountTorret++;
				float angulo = atan2(posXtorret - position.x, posZtorret - position.z)*(180 / D3DX_PI);
				m_projectilePosTorret.push_back(D3DXVECTOR3(posXtorret, posYtorret, posZtorret));
				//m_projectileRotTorret.push_back(D3DXVECTOR3(rotXtorret, rotYtorret, rotZtorret));
				m_projectileRotTorret.push_back(D3DXVECTOR3(0, angulo + 180, 0));
				proTorret = m_ProVelTorret;
			}
			else
			{
				proTorret--;
				if (proTorret <= 0) proTorret = 0;
			}

			//m_ProjectileDelta->Render(m_Direct3D->GetDeviceContext());

			for (int i = 0; i < m_projectileCountTorret; i++)
			{
				D3DXMATRIX rotatProTorret, proWorldMatrixTorret;

				D3DXVECTOR3 moveTorret = D3DXVECTOR3(0, 0, 5);

				pitch = m_projectileRotTorret[i].x * 0.0174532925f;
				yaw = m_projectileRotTorret[i].y * 0.0174532925f;
				roll = m_projectileRotTorret[i].z * 0.0174532925f;

				D3DXMatrixRotationYawPitchRoll(&rotatProTorret, yaw, 0, 0);

				D3DXVECTOR4 transformedTorret;
				D3DXVec3Transform(&transformedTorret, &moveTorret, &rotatProTorret);

				m_projectilePosTorret[i].x += transformedTorret.x;
				m_projectilePosTorret[i].y += transformedTorret.y;
				m_projectilePosTorret[i].z += transformedTorret.z;

				D3DXMatrixTranslation(&proWorldMatrixTorret, m_projectilePosTorret[i].x, m_projectilePosTorret[i].y, m_projectilePosTorret[i].z);

				if (!globalSphere->Collides(new BoundingSphere(m_projectilePosTorret[i], 5)))
				{
					m_projectileCountTorret--;
					m_projectilePosTorret.erase(m_projectilePosTorret.begin() + i);
					m_projectileRotTorret.erase(m_projectileRotTorret.begin() + i);
				}

				else if (m_Principal->GetBS()->Collides(new BoundingSphere(m_projectilePosTorret[i], 5)))
				{
					m_projectileCountTorret--;
					m_projectilePosTorret.erase(m_projectilePosTorret.begin() + i);
					m_projectileRotTorret.erase(m_projectileRotTorret.begin() + i);
					m_stadistics->SetMylifeMinus();
					if ((m_stadistics->GetMylife()) <= 0.0f) {
						defeatRender = false;
					}
				}
				/*result = m_DiffuseShader->Render(m_Direct3D->GetDeviceContext(), m_ProjectileDelta->GetIndexCount(), proWorldMatrixDelta, viewMatrix, projectionMatrix,
				m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_ProjectileDelta->GetTexture());*/
				for (auto &group : *m_proyectileQuad01->GetGroupList())
				{
					group.Render(m_Direct3D->GetDeviceContext());
					result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), proWorldMatrixTorret, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
					if (!result)
					{
						return false;
					}
				}

				if (!result)
				{
					return false;
				}
			}
		}
	}

	//Boss Enemy Projectile
	static int proBossEnemy = 0;
	if (bossRender) {
		if (proBossEnemy == 0) {
			m_projectileCountBoss++;
			float anguloBoss = atan2((posXBoss + AnimXBoss) - position.x, (posZBoss + AnimZBoss) - position.z)*(180 / D3DX_PI);
			m_projectilePosBoss.push_back(D3DXVECTOR3((posXBoss + AnimXBoss), (posYBoss + AnimYBoss), (posZBoss + AnimZBoss)));
			//m_projectileRotTorret.push_back(D3DXVECTOR3(rotXtorret, rotYtorret, rotZtorret));
			m_projectileRotBoss.push_back(D3DXVECTOR3(0, anguloBoss + 180, 0));
			proBossEnemy = m_ProVelBoss;
		}
		else
		{
			proBossEnemy--;
			if (proBossEnemy <= 0) proBossEnemy = 0;
		}

		//m_ProjectileDelta->Render(m_Direct3D->GetDeviceContext());

		for (int i = 0; i < m_projectileCountBoss; i++)
		{
			D3DXMATRIX rotatProBoss, proWorldMatrixBoss;

			D3DXVECTOR3 moveBoss = D3DXVECTOR3(0, 0, 5);

			pitch = m_projectileRotBoss[i].x * 0.0174532925f;
			yaw = m_projectileRotBoss[i].y * 0.0174532925f;
			roll = m_projectileRotBoss[i].z * 0.0174532925f;

			D3DXMatrixRotationYawPitchRoll(&rotatProBoss, yaw, 0, 0);

			D3DXVECTOR4 transformedTorret;
			D3DXVec3Transform(&transformedTorret, &moveBoss, &rotatProBoss);

			m_projectilePosBoss[i].x += transformedTorret.x;
			m_projectilePosBoss[i].y += transformedTorret.y;
			m_projectilePosBoss[i].z += transformedTorret.z;

			D3DXMatrixTranslation(&proWorldMatrixBoss, m_projectilePosBoss[i].x, m_projectilePosBoss[i].y, m_projectilePosBoss[i].z);

			if (!globalSphere->Collides(new BoundingSphere(m_projectilePosBoss[i], 5)))
			{
				m_projectileCountBoss--;
				m_projectilePosBoss.erase(m_projectilePosBoss.begin() + i);
				m_projectileRotBoss.erase(m_projectileRotBoss.begin() + i);
			}

			else if (m_Principal->GetBS()->Collides(new BoundingSphere(m_projectilePosBoss[i], 5)))
			{
				m_projectileCountBoss--;
				m_projectilePosBoss.erase(m_projectilePosBoss.begin() + i);
				m_projectileRotBoss.erase(m_projectileRotBoss.begin() + i);
				m_stadistics->SetMylifeMinus();
				if ((m_stadistics->GetMylife()) <= 0.0f) {
					defeatRender = false;
				}
			}
			/*result = m_DiffuseShader->Render(m_Direct3D->GetDeviceContext(), m_ProjectileDelta->GetIndexCount(), proWorldMatrixDelta, viewMatrix, projectionMatrix,
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_ProjectileDelta->GetTexture());*/
			for (auto &group : *m_proyectileLaser->GetGroupList())
			{
				group.Render(m_Direct3D->GetDeviceContext());
				result = m_GroupShader->Render(m_Direct3D->GetDeviceContext(), proWorldMatrixBoss, viewMatrix, projectionMatrix, m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), D3DXVECTOR4(1, 1, 1, 1), m_Light->GetDirection(), m_Camera->GetPosition(), group);
				if (!result)
				{
					return false;
				}
			}

			if (!result)
			{
				return false;
			}
		}
	}

	/////////

	if (position.z >= 1250.0f) {
		enemyBlockStep = true;
	}
	/*if (m_Principal->Collides(m_building01) || m_Principal->Collides(m_building01_1) || m_Principal->Collides(m_building02) ||
		m_Principal->Collides(m_building03) || m_Principal->Collides(m_building04) || m_Principal->Collides(m_building04_1) ||
		m_Principal->Collides(m_building05) || m_Principal->Collides(m_building06) || m_Principal->Collides(m_building06A) ||
		m_Principal->Collides(m_building06_1) || m_Principal->Collides(m_building06_2)) {
		enemyBlockStep = true;
	}
	else {
		enemyBlockStep = false;
	}*/
	m_Position->MoveForward(!enemyBlockStep);

	//Power ups Effects Collider
	if (m_Principal->Collides(m_pups)) {
		m_stadistics->SetMylifePlus();
		renderPUps01 = false;
	}
	if (m_Principal->Collides(m_pups02)) {
		m_stadistics->SetMylifePlus();
		renderPUps02 = false;
	}
	if (m_Principal->Collides(m_pups03)) {
		m_stadistics->SetMylifePlus();
		renderPUps03 = false;
	}
	if (m_Principal->Collides(m_pups04)) {
		m_ProVel -= 5;
		renderPUps04 = false;
	}
	if (m_Principal->Collides(m_pups05)) {
		m_ProVel -= 5;
		renderPUps05 = false;
	}
	if (m_Principal->Collides(m_pups06)) {
		m_ProVel -= 5;
		renderPUps06 = false;
	}

	static float waterMov = 0;
	waterMov += .1;
	float A = sin(waterMov) * 20;
	D3DXMATRIX watertrans, waterscale, waterWorldMatrix;
	D3DXMatrixTranslation(&watertrans, 0, -1, -330 + A);
	D3DXMatrixScaling(&waterscale, 1, 1, 1);
	waterWorldMatrix = waterscale * watertrans;
	// Render the terrain buffers.
	m_Water->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), waterWorldMatrix, viewMatrix, projectionMatrix,
		m_Water->GetTextures(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(0.9, 0.3, 0.3, 1), 10);

	if (!result)
	{
		return false;
	}

	D3DXMATRIX water01trans, water01scale, water01WorldMatrix;
	D3DXMatrixTranslation(&water01trans, 0, -2, 970 + A);
	D3DXMatrixScaling(&water01scale, 1, 1, 1);
	water01WorldMatrix = water01scale * water01trans;
	// Render the terrain buffers.
	m_Water->Render(m_Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), water01WorldMatrix, viewMatrix, projectionMatrix,
		m_Water->GetTextures(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), D3DXVECTOR4(0.9, 0.3, 0.3, 1), 10);

	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	if (bossRenderPanel) {
		///////////////////////////Panel/////////////////////////////////////////////
		//Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 200, 200);
		if (!result)
		{
			return false;
		}
		//Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, m_baseStaticMatrix, orthoMatrix, m_Bitmap->GetTexture());
		if (!result)
		{
			return false;
		}
		//Turn the Z buffer back on now that all 2D rendering has completed.
		//////////////////////////End panel///////////////////////////
	}

	if (!defeatRender) {
		///////////////////////////Panel/////////////////////////////////////////////
		//Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		enemyBlockStep = true;
		result = m_Bitmap01->Render(m_Direct3D->GetDeviceContext(), 200, 200);
		if (!result)
		{
			return false;
		}
		//Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap01->GetIndexCount(), worldMatrix, m_baseStaticMatrix, orthoMatrix, m_Bitmap01->GetTexture());
		if (!result)
		{
			return false;
		}
		//Turn the Z buffer back on now that all 2D rendering has completed.
		//////////////////////////End panel///////////////////////////
	}

	///////////////////////////Panel Life/////////////////////////////////////////////
	//Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	D3DXMATRIX Lifetrans, Lifescale, LifeWorldMatrix;
	m_Direct3D->GetWorldMatrix(LifeWorldMatrix);
	//D3DXMatrixTranslation(&b01trans, 650, 30, 380);
	D3DXMatrixScaling(&Lifescale, 1, 1, 1);
	D3DXMatrixScaling(&Lifescale, m_stadistics->GetStatusLifeBarRender(m_stadistics->GetMylife()), 1, 1);
	LifeWorldMatrix = Lifescale * LifeWorldMatrix;

	result = m_Bitmap02->Render(m_Direct3D->GetDeviceContext(), 10, 30);
	if (!result)
	{
		return false;
	}
	//Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap02->GetIndexCount(), LifeWorldMatrix, m_baseStaticMatrix, orthoMatrix, m_Bitmap02->GetTexture());
	if (!result)
	{
		return false;
	}
	//Turn the Z buffer back on now that all 2D rendering has completed.
	//////////////////////////End panel///////////////////////////

	///////////////////////////Panel framework/////////////////////////////////////////////
	//Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.

	result = m_Bitmap03->Render(m_Direct3D->GetDeviceContext(), 10, 30);
	if (!result)
	{
		return false;
	}
	//Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap03->GetIndexCount(), worldMatrix, m_baseStaticMatrix, orthoMatrix, m_Bitmap03->GetTexture());
	if (!result)
	{
		return false;
	}
	//Turn the Z buffer back on now that all 2D rendering has completed.
	//////////////////////////End panel///////////////////////////

	if (resetGame) {
		renderPUps01 = true;
		renderPUps02 = true;
		renderPUps03 = true;
		renderPUps04 = true;
		renderPUps05 = true;
		renderPUps06 = true;
		enproDelta = true;
		enproTorret = true;
		seeVisionTorret = false;
		bossRender = false; 
		bossRenderPanel = false; 
		ctrlVisionTorret = false;
		defeatRender = true;
		resetGame = false;
		enemyBlockStep = false;
		m_Position->ResetStructurePosition();
		m_stadistics->resetStructureStadistics();
		m_Position->SetPositionDelta(500.0f, 5.0f, 1000.0f);
		m_Position->SetRotationDelta(0.0f, 180.0f, 0.0f);

		m_Position->SetPositionEnemyBoss(590.0f, -30.0f, 1350.0f);
		m_Position->SetRotationEnemyBoss(0.0f, 0.0f, 0.0f);
		m_ProVel = 20;
		m_ProVelDelta = 20;
		m_ProVelTorret = 30;
		m_ProVelBoss = 40;

	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
