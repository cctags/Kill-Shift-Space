/*
*	A simple program testing Windows global hotkey usage.
*	Using the keybind 'shift + space' will print a message in the program console.
*	Largely taken from MSDN topic on RegisterHotkey here:
*	https://msdn.microsoft.com/en-us/library/windows/desktop/ms646309%28v=vs.85%29.aspx
*/

#include <stdio.h>
#include <Windows.h>

// comment this will show the console
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#define HOTKEY_ID 0x53C8
#define MOD_NOREPEAT 0x4000

int main(int argc, char *argv[])
{
	if (RegisterHotKey(NULL, HOTKEY_ID, MOD_SHIFT | MOD_NOREPEAT, 0x20))
	{
		printf("Successfully registered shift + space global hotkey\n");
	}

	MSG winMessage = { 0 };

	// perpetually wait until a message is received
	while (GetMessage(&winMessage, NULL, 0, 0))
	{
		if (winMessage.message == WM_HOTKEY)
		{
			printf("Received hotkey message\n");

			// "shift space" map to "space".
			// {
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SPACE, 0, 0, 0);
			keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
			// }
		}
	}

	// unregister the hotkey
	if (UnregisterHotKey(NULL, HOTKEY_ID))
	{
		printf("Successfully unregistered hotkey\n");
	}

	return 0;
}
