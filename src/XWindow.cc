#include "XWindow.h"
#include <X11/Xutil.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

XWindow::XWindow(unsigned int width, unsigned int height, char * windowTitle, char * windowClass, char * windowResource)
{
    XClassHint *classHint;
    display = XOpenDisplay(NULL);
   
    // Throw an exception if we can't connect to X at all.
    if(display == NULL)
    {
        std::cout<<"[XWindow::XWindow()] Unable to open display. X not available.\n";
        throw 1;
    }
    
    unsigned long blackColor = BlackPixel(display, DefaultScreen(display));
    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, width, height, 0, blackColor, blackColor);

    XStoreName(display, window, windowTitle);
    classHint = XAllocClassHint();
    classHint->res_name = windowResource;
    classHint->res_class = windowClass;
    XSetClassHint(display, window, classHint);
    XFree(classHint);

    windowWidth = getWidth();
    windowHeight = getHeight();

    gc = XCreateGC(display, window, 0, NULL);

    //Should use Xmalloc, but didn't work. calloc() also initializes to 0.
    pixelData = (char *) calloc(4*windowWidth*windowHeight, 1);

    Visual *visual = DefaultVisual(display, 0);
    image = XCreateImage(display, visual, 24, ZPixmap, 0, pixelData, windowWidth, windowHeight, 32, 0);

    XMapRaised(display, window);
    XFlush(display);
}

XWindow::~XWindow(void)
{
    XDestroyImage(image);
    XDestroyWindow(display, window);
    XFreeGC(display, gc);
    XCloseDisplay(display);
    
    return;
}

bool XWindow::isPendingEvent(void) const
{
    return XPending(display);
}

void XWindow::getNextEvent(XEvent& event)
{
    XNextEvent(display, &event);
    return;
}

void XWindow::selectInput(long eventMask)
{
    XSelectInput(display, window, eventMask);
    return;
}

int XWindow::getWidth(void) const
{
    XWindowAttributes win_attr;

    XGetWindowAttributes(display, window, &win_attr);
    return win_attr.width;
}

int XWindow::getHeight(void) const
{
    XWindowAttributes win_attr;

    XGetWindowAttributes(display, window, &win_attr);
    return win_attr.height;
}

void XWindow::flushImage(void)
{
    XPutImage(display, window, gc, image, 0, 0, 0, 0, windowWidth, windowHeight);
    return;
}

void XWindow::drawPoint(int x, int y, char red, char green, char blue)
{
    pixelData[4*(y*windowWidth + x)] = blue;
    pixelData[4*(y*windowWidth + x)+1] = green;
    pixelData[4*(y*windowWidth + x)+2] = red;
    pixelData[4*(y*windowWidth + x)+3] = 0;

    return;
}

void XWindow::drawRectangle(unsigned int xstart, unsigned int ystart, unsigned int xend, unsigned int yend, char red, char green, char blue)
{
    for(unsigned int i = xstart; i<= xend; i++)
    {
        drawPoint(i, ystart, red, green, blue);
        drawPoint(i, yend, red, green, blue);
    }

    for(unsigned int i = ystart; i<= yend; i++)
    {
        drawPoint(xstart, i, red, green, blue);
        drawPoint(xend, i, red, green, blue);
    }
    return;
}

void XWindow::saveImage(std::string fileName)
{
    std::ofstream myFile(fileName.c_str(), std::ios::out | std::ios::binary);
    
    if (myFile.is_open())
    {
        //This is the TGA file header - 18bytes. 32bit data, origin at top-left. 
        char header[] = {0, 0, 2, 0, 0, 0, 0, 32, 0, 0, 0, 0, (unsigned char) (windowWidth&0xff), (unsigned char) ((windowWidth&0xff00)>>8), (unsigned char) (windowHeight&0xff), (unsigned char) ((windowHeight&0xff00)>>8), 32, 1<<5};

        myFile.write(header, 18);
        myFile.write(pixelData, windowWidth * windowHeight * 4);
    }
    else std::cout<<"XWindow::saveBuffer(): Unable to open file: " << fileName << '\n';

    return;
}
