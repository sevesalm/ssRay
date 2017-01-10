#include "UIThread.h"
#include "Camera.h"
#include "Film.h"
#include "utils.h"
//#include <iostream>

UIThread::UIThread(Camera *camera, RectangleInfo *newArea) try : UIWindow(camera->getFilm()->getWidth(), camera->getFilm()->getHeight(), "Camera", "ssRay", "Renderer"), camera(camera), film(camera->getFilm()), newArea(newArea), oldRenderArea(*newArea), selectedArea(*newArea), width(film->getWidth()), height(film->getHeight()), newData(false), uiRunning(true)
{
    // Select the events we want to get a notification of.
    UIWindow.selectInput(ExposureMask|ButtonPressMask|ButtonReleaseMask|Button1MotionMask);
    imageBuffer = new char[3*film->getHeight()*film->getWidth()];
    for(unsigned int i=0; i<3*film->getWidth()*film->getHeight();i++)
        imageBuffer[i] = 0;

    return;
}
// Catch the exceptions the constructor generated. Mostly if it was unable to create XWindow object.
catch(int i)
{
}

UIThread::~UIThread()
{
    delete[] imageBuffer;
}

/*
 * UIThread is the thread that takes user input and handles all UI related events like drawing the image etc.
 */

void UIThread::execute(void * arg)
{
    double previousUpdate = getTime();
    bool letsDraw = false;
    myFloat updateInterval = 1;
    RectangleInfo oldSelectedArea;
    
    while(uiRunning)
    {
        oldSelectedArea = selectedArea;
        if(selectedArea.fullScreen)
            usleep(CLOCKS_PER_SEC/10);
        else
            usleep(CLOCKS_PER_SEC/30);

        processEvents();
        
        if(newData && (((getTime() - previousUpdate) > updateInterval) || (oldRenderArea != *newArea)) )
        {
            RectangleInfo oldArea = oldRenderArea;
            newData = false;
            letsDraw = true;
            for(unsigned int y=oldArea.ystart; y <= oldArea.yend; y++)
                for(unsigned int x=oldArea.xstart; x <= oldArea.xend; x++)
                {
                    Vector radiance = film->readRadiance(x,y);
                    imageBuffer[3*(y*width+x)] = film->pixelValue(radiance.z);
                    imageBuffer[3*(y*width+x)+1] = film->pixelValue(radiance.y);
                    imageBuffer[3*(y*width+x)+2] = film->pixelValue(radiance.x);
                }
            film->save("test.tga", imageBuffer);
            previousUpdate = getTime();
            
            //If the user changed the render area, let's reset the update interval, otherwise increase it by 20%.
            if(oldRenderArea == *newArea)
            {
                updateInterval *= 1.1;
                if(updateInterval > 30)
                    updateInterval = 30;
            }
            else
                updateInterval = 1;
        }

        if(letsDraw || (selectedArea != oldSelectedArea))
        {
            drawBuffer();
            if(!selectedArea.fullScreen)
                UIWindow.drawRectangle(selectedArea.xstart, selectedArea.ystart, selectedArea.xend, selectedArea.yend, 0x40, 0xff, 0x40);
            UIWindow.flushImage();
            letsDraw = false;
        }
    }
    return;
}

void UIThread::stop()
{
    uiRunning = false;
    join();
    return;
}

void UIThread::processEvents(void)
{
    XEvent event;
    static int xstart, xend, ystart, yend;
    
    while(UIWindow.isPendingEvent())
    {
        UIWindow.getNextEvent(event);
        switch(event.type)
        {
            case Expose:
                UIWindow.flushImage();
                break;
            
            case ButtonPress:
                if(event.xbutton.button == Button1)
                {
                    selectedArea.fullScreen = false;
                    xstart = event.xbutton.x;
                    ystart = event.xbutton.y;
                    
                    xstart = std::max(0, xstart);
                    xstart = std::min((int) width-1, xstart);
                    ystart = std::max(0, ystart);
                    ystart = std::min((int) height-1, ystart);
                    
                    selectedArea.xstart = selectedArea.xend = xstart;
                    selectedArea.ystart = selectedArea.yend = ystart;
                }
                else if(event.xbutton.button == Button3 && !selectedArea.fullScreen)
                {
                    selectedArea.fullScreen = true;
                    selectedArea.xstart = 0;
                    selectedArea.ystart = 0;
                    selectedArea.xend = width-1;
                    selectedArea.yend = height-1;
                    *newArea = selectedArea;
                }
                else if(event.xbutton.button == Button2)
                {
                    if(selectedArea.catchUp)
                        selectedArea.catchUp = false;
                    else
                        selectedArea.catchUp = true;
                    *newArea = selectedArea;
                }
                break;

            case ButtonRelease:
                if(event.xbutton.button == Button1)
                {
                    *newArea = selectedArea;
                }
                break;

            case MotionNotify:
                xend = event.xbutton.x;
                yend = event.xbutton.y;
                xend = std::max(0, xend);
                xend = std::min((int) width-1, xend);
                yend = std::max(0, yend);
                yend = std::min((int) height-1, yend);

                selectedArea.xstart = std::min(xstart, xend);
                selectedArea.ystart = std::min(ystart, yend);
                selectedArea.xend = std::max(xstart, xend);
                selectedArea.yend = std::max(ystart, yend);
                break;

             default:
                std::cout<<"UIThread::processEvents(): unrecognized event: "<<event.type<<'\n';
                break;
        }
    }
    return;
}

/*
 * Renderer uses this method to inform that the renderArea has been completely rendered so
 * the UIThread can update the window.
 */

void UIThread::newFilm(const RectangleInfo& renderArea)
{
    oldRenderArea = renderArea;
    newData = true;
    //std::cout<<"newFilm: renderArea: "<<oldRenderArea.xstart<<"\n";
    return;
}

/*
 * Reads variance values from backup film, converts them to pixel values (+ gamma correction) and draws them
 * to window buffer.
 */

void UIThread::drawBuffer(void)
{
    for(unsigned int y=0; y < height; y++)
        for(unsigned int x=0; x < width; x++)
            UIWindow.drawPoint(x,y, imageBuffer[3*(y*width+x)+2], imageBuffer[3*(y*width+x)+1], imageBuffer[3*(y*width+x)]);

    return;
}
