#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <string>

class XWindow
{
    private:
        Display *display;
        Window window;
        GC gc;
        char *pixelData;
        XImage *image;
        unsigned int windowWidth;
        unsigned int windowHeight;

    public:
        XWindow(unsigned int, unsigned int, char *, char *, char *);
        ~XWindow(void);
        
        bool isPendingEvent(void) const;
        void getNextEvent(XEvent&);

        void selectInput(long);

        void drawPoint(int, int, char, char, char);
        void drawRectangle(unsigned int, unsigned int, unsigned int, unsigned int, char, char, char);
        
        void flushImage(void);
        void saveImage(std::string);
        
        int getWidth(void) const;
        int getHeight(void) const;

};
#endif
