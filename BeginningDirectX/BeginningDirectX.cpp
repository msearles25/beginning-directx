#include <Windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// defining the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 640

// FVF codes (Flexible Vertex Format)
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE) // for 3d space, we removed the RHW

// global declarations
LPDIRECT3D9 d3d;			// the pointer to our Direct3d interface
LPDIRECT3DDEVICE9 d3ddev;	// The pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL; // the pointer to the vertex buffer
LPDIRECT3DINDEXBUFFER9 i_buffer = NULL; // the pointer to the index buffer

// function prototypes
void initD3d(HWND hWnd);// sets up and intializes Direct3D
void render_frame();	// renders a single frame
void clean3D();			// closes Direct3Dand releases the memory
void initGraphics();	// 3d declarations

// VERTEX struct
struct CUSTOMVERTEX
{
	float x, y, z;	// from the D3DFVF_XYZ (removed RHW)
	DWORD color;	// from the D3DFVF_DIFFUSE
};

// WindProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// The handle to the window, fille by a function
	HWND hWnd;
	// struct that holds information about the window class
	WNDCLASSEX wc;

	// clear the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// Fill in the struct with needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW; leave the background color untouched with this commented out
	wc.lpszClassName = L"WindowClass1";

	// Register the window class
	RegisterClassEx(&wc);

	// Create the window and use the result as a handle
	hWnd = CreateWindowEx(
		NULL,
		L"WindowClass1",
		L"First Windowed Program",	// Title of the window
		WS_OVERLAPPEDWINDOW,		// windowed mode
		0, 0,						// starting x, y should be 0
		SCREEN_WIDTH, SCREEN_HEIGHT,// width, height
		NULL, NULL,					// no parent Window and not using menus
		hInstance,					// application handle
		NULL);						// used with multiple windows

	// display the window to the screen
	ShowWindow(hWnd, nCmdShow);

	initD3d(hWnd);
	// enter the main loop of the program
	// This struct holds Windows event messages
	MSG msg;

	// Wait for the next message in the queue, store the result in msg;
	while(TRUE)
	{
		// Check to see if any messages are waiting in the queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to WindowProc function
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		render_frame();
	}

	clean3D();

	return msg.wParam;
}

// This is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	// handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// this funciton will initialize and prepare Direct3D for use
void initD3d(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);		// Create the Direct3D interface
	D3DPRESENT_PARAMETERS d3dpp;				// Create a struct to hold various device informaton

	ZeroMemory(&d3dpp, sizeof(d3dpp));			// Clear out the struct for use
	d3dpp.Windowed = TRUE;						// windowed mode
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// Discard old frames
	d3dpp.hDeviceWindow = hWnd;					// se the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// set teh back buffer format to 32-bit
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// set the width of the buffer
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// set the height of the buffer
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using the information above 
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	
	initGraphics(); // call the function to initilize the triangle
	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);	// turn off the 3D lighting (no lighting code, so nothing would show up if it was on)
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);	// enable the z-buffer
}

void initGraphics()
{
	// create three vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		// cube
		//{ -3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 255), },
		//{ 3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 0), },
		//{ -3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(255, 0, 0), },
		//{ 3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 255), },
		//{ -3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(0, 0, 255), },
		//{ 3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(255, 0, 0), },
		//{ -3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 0), },
		//{ 3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 255), }

		// pyramid
		// base
		//{ -3.0f, 0.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 0), },
		//{ 3.0f, 0.0f, 3.0f, D3DCOLOR_XRGB(0, 0, 255), },
		//{ -3.0f, 0.0f, -3.0f, D3DCOLOR_XRGB(255, 0, 0), },
		//{ 3.0f, 0.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 255), },

		//// top point
		//{ 0.0f, 7.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), }

		// Ship thing model
		// body
		{ 3.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0) },	
		{ 0.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 255) },
		{ 0.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 0, 0) },	
		{ -3.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 255) },

		// left gun
		{ 3.2f, -1.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 255) },
		{ 3.2f, -1.0f, 11.0f, D3DCOLOR_XRGB(0, 255, 0) },
		{ 2.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0) },

		// right gun
		{ -3.2f, -1.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 255) },
		{ -3.2f, -1.0f, 11.0f, D3DCOLOR_XRGB(0, 255, 0) },
		{ -2.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0) },
	};

	// create the vertex and store the pointer into v_buffer, which is created globably
	d3ddev->CreateVertexBuffer(
		10 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid; // the void poiner

	v_buffer->Lock(0, 0, (void**)&pVoid, 0); // lock the vertex buffer
	memcpy(pVoid, vertices, sizeof(vertices)); // copy the vertices into the locked buffer
	v_buffer->Unlock();

	// create the indices using an int array
	short indices[] =
	{
		// cube
		//0, 1, 2,    // side 1
		//2, 1, 3,
		//4, 0, 6,    // side 2
		//6, 0, 2,
		//7, 5, 6,    // side 3
		//6, 5, 4,
		//3, 1, 7,    // side 4
		//7, 1, 5,
		//4, 5, 0,    // side 5
		//0, 5, 1,
		//3, 7, 2,    // side 6
		//2, 7, 6

		// pyramid
		//0, 2, 1, // base
		//1, 2, 3,
		//0, 1, 4, // sides
		//1, 3, 4,
		//3, 2, 4,
		//2, 0, 4

		// model ship thing
		0, 1, 2,	// body
		2, 1, 3,
		3, 1, 0,
		0, 2, 3,
		4, 5, 6,	// wings
		7, 8, 9
	};

	// create an index buffer
	d3ddev->CreateIndexBuffer(
		18 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer,
		NULL);

	// lock the i_buffer and load the indices into it
	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer->Unlock();
}

void render_frame()
{
	// Clear the window to a a deep blue color
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();	// Begins the 3D scene

	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);

	// SET UP THE PIPELINE

	// View
	D3DXMATRIX matView;			// the view transform matrix

	D3DXVECTOR3 firstVec(0.0f, 10.0f, 25.0f);
	D3DXVECTOR3 secondtVec(0, 0, 0);
	D3DXVECTOR3 thirdVec(0, 1.0f, 0);

	D3DXMatrixLookAtLH(
		&matView,
		&firstVec,		// the camera position
		&secondtVec ,	// the look-at position
		&thirdVec );	// the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView); // set the view transform to matView

	// Projection
	D3DXMATRIX matProjection;	// the projection transorm matrix

	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		D3DXToRadian(45),							// the horizontal field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		1.0f,										// the near view-plane
		100.0f);									// the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);	// set the projection transform

	// world transform
	static float index = 0.0f; index += 0.03f;
	D3DXMATRIX matRotateY;
	D3DXMatrixRotationY(&matRotateY, index); // rotation matrix
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY)); // set world transform

	// copy the vertex buffer to the back buffer

	// select the vertex buffer to display
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(i_buffer);

	// Drawing the cube
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 10, 0, 6);

	////// Drawing flat triangles //////

	//D3DXMATRIX matTranslateA;	// A matrix to store the translation for triangle A
	//D3DXMATRIX matTranslateB;	// A matrix to store the translation for triangle B
	//D3DXMATRIX matRotateY;		// a matrix to store the rotation for each triangle
	//static float index = 0.0f; index += 0.01f;	// an ever-increasing float

	//// build multiple matrices to translate the model and just one to rotate
	//D3DXMatrixTranslation(&matTranslateA, 0.0f, 0.0f, 2.0f);
	//D3DXMatrixTranslation(&matTranslateB, 0.0f, 0.0f, -2.0f);
	//D3DXMatrixRotationY(&matRotateY, index);	// front side

	//// tell Direct3D about each world transform and then draw another triangle
	//D3DXMATRIX triangleA = (matTranslateA * matRotateY);
	//d3ddev->SetTransform(D3DTS_WORLD, &triangleA);
	//d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//D3DXMATRIX triangleB = (matTranslateB * matRotateY);
	//d3ddev->SetTransform(D3DTS_WORLD, &triangleB);
	//d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	d3ddev->EndScene();		// ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);	// Displays the created frame
}

void clean3D()
{
	v_buffer->Release(); // close and release the vertex buffer
	d3ddev->Release();	// close and release the 3D devices
	d3d->Release();		// close and release Direct3D
}