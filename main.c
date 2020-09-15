/*
*	A simple program testing Windows global hotkey usage.
*	Using the keybind 'shift + space' will print a message in the program console.
*	Largely taken from MSDN topic on RegisterHotkey here:
*	https://msdn.microsoft.com/en-us/library/windows/desktop/ms646309%28v=vs.85%29.aspx
*/

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <Windows.h>

// comment this will show the console
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#define HOTKEY_ID 0x53C8
#define MOD_NOREPEAT 0x4000

static void print_message(const char *text, ...)
{
	va_list args;
	time_t now;
	struct tm *local;

	time(&now);
	local = localtime(&now);

	printf("[%04d-%02d-%02d %02d:%02d:%02d] ",
			local->tm_year + 1900,
			local->tm_mon + 1,
			local->tm_mday,
			local->tm_hour, local->tm_min, local->tm_sec);

	va_start(args, text);
	vprintf(text, args);
	va_end(args);
}

int main(int argc, char *argv[])
{
	if (RegisterHotKey(NULL, HOTKEY_ID, MOD_SHIFT | MOD_NOREPEAT, 0x20))
	{
		print_message("Successfully registered shift + space global hotkey\n");
	}

	MSG winMessage = { 0 };

	int converted = 0;

	// perpetually wait until a message is received
	while (GetMessage(&winMessage, NULL, 0, 0))
	{
		if (winMessage.message == WM_HOTKEY)
		{
			converted++;
			print_message("Received hotkey message: %d\n", converted);

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
		print_message("Successfully unregistered hotkey\n");
	}

	return 0;
}
