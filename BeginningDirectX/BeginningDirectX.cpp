#include <Windows.h>
#include <windowsx.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

// defining the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 640

// FVF codes (Flexible Vertex Format)
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// global declarations
LPDIRECT3D9 d3d;			// the pointer to our Direct3d interface
LPDIRECT3DDEVICE9 d3ddev;	// The pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL; // the pointer to the vertex buffer

// function prototypes
void initD3d(HWND hWnd);// sets up and intializes Direct3D
void render_frame();	// renders a single frame
void clean3D();			// closes Direct3Dand releases the memory
void initGraphics();	// 3d declarations

// VERTEX struct
struct CUSTOMVERTEX
{
	float x, y, z, rhw;	// from the D3DFVF_XYZRHW
	DWORD color;		// from the D3DFVF_DIFFUSE
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

	// create a device class using the information above 
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
}

void initGraphics()
{
	// create three vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{320.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255) },
		{520.0f, 4000.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
		{120.0f, 40.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0) },
	};

	// create the vertex and store the pointer into v_buffer, which is created globably
	d3ddev->CreateVertexBuffer(
		3 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid; // the void poiner

	v_buffer->Lock(0, 0, (void**)&pVoid, 0); // lock the vertex buffer
	memcpy(pVoid, vertices, sizeof(vertices)); // copy the vertices into the locked buffer
	v_buffer->Unlock();

}

void render_frame()
{
	// Clear the window to a a deep blue color
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

	d3ddev->BeginScene();	// Begins the 3D scene

	// do 3d rendering on the back buffer

	d3ddev->EndScene();		// ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);	// Displays the created frame
}

void clean3D()
{
	d3ddev->Release();	// close and release the 3D devices
	d3d->Release();		// close and release Direct3D
}