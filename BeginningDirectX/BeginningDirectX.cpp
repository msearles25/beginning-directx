#include <Windows.h>
#include <windowsx.h>

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
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	// Register the window class
	RegisterClassEx(&wc);

	// Create the window and use the result as a handle
	hWnd = CreateWindowEx(
		NULL,
		L"WindowClass1",
		L"First Windowed Program",	// Title of the window
		WS_OVERLAPPEDWINDOW,		// window style
		300, 300, 500, 400,			// x, y, width, height
		NULL, NULL,					// no parent Window and not using menus
		hInstance,					// application handle
		NULL);						// used with multiple windows

	// display the window to the screen
	ShowWindow(hWnd, nCmdShow);

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
	}

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