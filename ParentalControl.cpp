#include <iostream>
#include <time.h>
#include <Windows.h>
#include <shlobj.h>

struct CustomTime
{
	int day = 0;
	int hour = 0;
};

CustomTime GetCustomCurrentTime()
{
	struct tm aTime;
	time_t now = time(0);
	localtime_s(&aTime, &now);
	CustomTime t;
	t.day = aTime.tm_wday;
	t.hour = aTime.tm_hour;
	return t;
}

enum Days
{
	Sunday,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
};
bool ShutDownPc()
{
	CustomTime time = GetCustomCurrentTime();

	int day = (Days)time.day;
		
	if (day < 5)
	{
		if (time.hour >= 20 || time.hour < 6)
			return true;
	}
	else
	{
		if (time.hour >= 21 || time.hour < 6)
			return true;
	}
	return false;
}

std::string GetStartupPath()
{
	char path[MAX_PATH];

	if (SHGetFolderPathA(NULL, CSIDL_STARTUP, NULL, 0, path) == S_OK)
		return path;

	return "";
}

void WriteToStartup()
{
	char charArray[MAX_PATH];
	HMODULE hmodule = GetModuleHandle(NULL);
	GetModuleFileNameA(hmodule, charArray, MAX_PATH);
	if (!CopyFileA(charArray,( GetStartupPath() + "\\ParentalControl.exe").c_str(), TRUE))
		MessageBoxA(NULL, "Copy Failed!!", "Copy Failed!!", MB_OK);
	else
		ShellExecuteA(NULL, NULL, GetStartupPath().c_str(), NULL, NULL, SW_SHOWNORMAL);
}
bool NeedsCopy()
{
	char charArray[MAX_PATH];
	HMODULE hmodule = GetModuleHandle(NULL);
	GetModuleFileNameA(hmodule, charArray, MAX_PATH);
	if (strstr(charArray,"Startup") != 0)
		return false;
	return true;
}
int CALLBACK WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
	if (NeedsCopy())
	{
		WriteToStartup();
		return 0;
	}

	std::string t = GetStartupPath();
	int counter = 15;
	while (counter > 0)
	{
		if (GetAsyncKeyState(VK_F12) && GetAsyncKeyState(VK_F1))
		{
			Beep(500, 500);
			return 0;
		}
		Sleep(1000);
		counter--;
	}
	
	while (true)
	{
		if (ShutDownPc())
			system("shutdown -s -f -t 00");

		Sleep(1000);
	}

	return 0;
}