//#define DEBUG
//#define BIGCHUNGUSMODE

#include <iostream>
#include <string>
#include <sstream>
//#include <windows.h>
#include <stdlib.h>
#include <chrono>
#include <future>
#include <unistd.h>
#include "const.h"
#include "renderer/objects/Vector2.h"
#include "renderer/objects/Vector3.h"
#include "renderer/raymarch.h"

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008

void ClearScreen()
{
	std::cout << std::string(100, '\n');
}

std::string generateFrame(int h, int w)
{
	std::string frames = "";
	for (int x = 0; x < h; x += 2) // we render every second pixel strip to compensate for windows being EXTREMELY idiotic and having line spacing
	{
		for (int y = 0; y < w; y++)
		{
			vectors::Vector3 coler = mainImage(vectors::Vector2d(y + 1, x + 1));
#ifdef DEBUG
			std::cout << coler.x;
#endif
			//COLORREF color = RGB(coler.x * 255, coler.y * 255, coler.z * 255);
#ifndef DEBUG
			frames += "\x1b[38;2;" + std::to_string((int)floor(coler.x * 255)) + ";" + std::to_string((int)floor(coler.y * 255)) + ";" + std::to_string((int)floor(coler.z * 255)) + "m" + "#";
#else
			std::cout << std::endl;
			std::cout << std::endl;
#endif
		}
		frames += "\x1b[0m\n";
	}
	return frames;
}

int main()
{
	int w, h;
	w = RESOLUTION.x;
	h = RESOLUTION.y;
	int fps = 1;
	auto start = std::chrono::system_clock::now();

	//if (system("CLS"))
	//	system("clear");
	std::cout.sync_with_stdio(false);
	std::cin.tie(NULL);

	ClearScreen();

	sleep(0.1f);
	while (true)
	{
		//ClearScreen();
		start = std::chrono::system_clock::now();
		frame(fps);
		std::future<std::string> fut = std::async(generateFrame, h, w);
#ifdef DEBUG
		break;
#endif
		fut.wait();
		std::string f = fut.get();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		fps = (int)floor(1 / elapsed_seconds.count());
		//SetConsoleCursorPosition(hStdout, bufferInfo.dwCursorPosition);
		//puts(f.c_str()); // this is the quickest way of printing stuff I think
		std::cout << "\033[2J\033[1;1H";
		std::cout << f;
		std::cout << "FPS: " + std::to_string(fps) << "      " //pass some empty space just to make sure no numbers stick around after the FPS
				  << std::endl;
	}
	return 0;
}