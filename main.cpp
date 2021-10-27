//#define DEBUG
#define BIGCHUNGUSMODE
#define NOTERM

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <map>
//#include <windows.h>
#include <stdlib.h>
#include <chrono>
#include <future>
#include <X11/Xlib.h>
#include <unistd.h>
#include "const.h"
#include "renderer/objects/Vector2.h"
#include "renderer/objects/Vector3.h"
#include "renderer/raymarch.h"

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008

GC create_gc(Display *display, Window win, int reverse_video)
{
	GC gc;						 /* handle of newly created GC.  */
	unsigned long valuemask = 0; /* which values in 'values' to  */
								 /* check when creating the GC.  */
	XGCValues values;			 /* initial values for the GC.   */
	unsigned int line_width = 2; /* line width for the GC.       */
	int line_style = LineSolid;	 /* style for lines drawing and  */
	int cap_style = CapButt;	 /* style of the line's edje and */
	int join_style = JoinBevel;	 /*  joined lines.		*/
	int screen_num = DefaultScreen(display);

	gc = XCreateGC(display, win, valuemask, &values);

	/* allocate foreground and background colors for this GC. */
	if (reverse_video)
	{
		XSetForeground(display, gc, WhitePixel(display, screen_num));
		XSetBackground(display, gc, BlackPixel(display, screen_num));
	}
	else
	{
		XSetForeground(display, gc, BlackPixel(display, screen_num));
		XSetBackground(display, gc, WhitePixel(display, screen_num));
	}

	/* define the style of lines that will be drawn using this GC. */
	XSetLineAttributes(display, gc,
					   line_width, line_style, cap_style, join_style);

	/* define the fill style for the GC. to be 'solid filling'. */
	XSetFillStyle(display, gc, FillSolid);

	return gc;
}

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

void generateFrame2(std::string* frames, int h, int w)
{
	// GC anus = create_gc(d, wind, 0);
	//Pixmap frames = XCreatePixmap(d, wind, h, w, 8);
	//std::string *amogus = new std::string[h * w];
	for (int x = 0; x < h; x++) // we render every second pixel strip to compensate for windows being EXTREMELY idiotic and having line spacing
	{
		for (int y = 0; y < w; y++)
		{
			vectors::Vector3 coler = mainImage(vectors::Vector2d(y + 1, x + 1));
#ifdef DEBUG
			std::cout << coler.x;
#endif
			//COLORREF color = RGB(coler.x * 255, coler.y * 255, coler.z * 255);
#ifndef DEBUG
			std::string colerstr = "RGBi:" + std::to_string(coler.x) + "/" + std::to_string(coler.y) + "/" + std::to_string(coler.z);
			frames[x * w + y] = colerstr;

#else
			std::cout << std::endl;
			std::cout << std::endl;
#endif
		}
	}
}

std::map<std::string, std::vector<XPoint>> generateColourLayers(std::string *colourData, int h, int w)
{
	std::map<std::string, std::vector<XPoint>> hatred = std::map<std::string, std::vector<XPoint>>();
	for (int x = 0; x < h; x++)
	{
		for (int y = 0; y < w; y++)
		{
			if (hatred.find(colourData[x * w + y]) == hatred.end())
			{
				hatred.insert(std::pair<std::string, std::vector<XPoint>>(colourData[x * w + y], std::vector<XPoint>()));
				hatred[colourData[x * w + y]].push_back({short(x), short(y)});
			}
			else
			{
				hatred[colourData[x * w + y]].push_back({short(x), short(y)});
			}
		}
	}

	// std::map<std::string, XPoint *> pain = std::map<std::string, XPoint *>();
	// for (std::map<std::string, std::vector<XPoint>>::iterator it = hatred.begin(); it != hatred.end(); ++it)
	// {
	// 	XPoint hell[it->second.size()];
	// 	std::copy(it->second.begin(), it->second.end(), hell);
	// 	pain.insert(std::pair<std::string, XPoint *>(it->first, hell));
	// }
	return hatred;
}

/*
	XColor colero;
			XAllocNamedColor(d, cm, amogus[x * w + y].c_str(), &colero, &colero);
			XSetForeground(d, anus, colero.pixel);
			XDrawPoint(d, wind, anus, x, y);
*/

int renderFrame(Display *d, Window wind, GC anus, Colormap cm, int h, int w, std::map<std::string, std::vector<XPoint>> colourLayers)
{
	for (std::map<std::string, std::vector<XPoint>>::iterator it = colourLayers.begin(); it != colourLayers.end(); ++it)
	{
		XColor colero;
		XAllocNamedColor(d, cm, it->first.c_str(), &colero, &colero);
		XSetForeground(d, anus, colero.pixel);
		XDrawPoints(d, wind, anus, it->second.data(), it->second.size(), CoordModeOrigin);
	}
	return 0;
}

int windowRender()
{
	// Open a display.
	Display *d = XOpenDisplay(0);
	std::cout << "Hello world" << std::endl;

	if (d)
	{
		// Create the window
		Window wind = XCreateWindow(d, DefaultRootWindow(d), 0, 0, RESOLUTION.x,
									RESOLUTION.y, 0, CopyFromParent, CopyFromParent,
									CopyFromParent, 0, 0);

		// Show the window
		XMapWindow(d, wind);
		XFlush(d);

		GC gc = create_gc(d, wind, 0);

		/* try to load the given font. */
		XFontStruct *font_info = XLoadQueryFont(d, "*");

		/* assign the given font to our GC. */
		XSetFont(d, gc, font_info->fid);

		Colormap cm = XDefaultColormap(d, DefaultScreen(d));

		int fps = 1;
		auto start = std::chrono::system_clock::now();

		int w, h;
		w = RESOLUTION.x;
		h = RESOLUTION.y;

		std::string* frames = new std::string[h*w];

		// Sleep long enough to see the window.
		while (true)
		{
			//generateFrame2(d, w, gc, cm, RESOLUTION.x, RESOLUTION.y);
			start = std::chrono::system_clock::now();
			frame(fps);
			std::future<void> fut = std::async(generateFrame2, frames, h, w);
			fut.wait();
			
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end - start;
			fps = (int)floor(1 / elapsed_seconds.count());
			renderFrame(d, wind, gc, cm, h, w, generateColourLayers(frames, h, w));
			XColor colero;
			XAllocNamedColor(d, cm, "white", &colero, &colero);
			XSetForeground(d, gc, colero.pixel);
			std::string fpsmes = "FPS: " + std::to_string(fps);
			char *ch = &fpsmes[0];
			XDrawString(d, wind, gc, 0, 0, ch, strlen(ch));
		}
	}
	return 0;
}

int main()
{
#ifdef NOTERM
	return windowRender();
#endif

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