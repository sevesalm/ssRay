#include "RenderThread.h"

static unsigned int threadCount = 0;

RenderThread::RenderThread(World *world, Camera *camera, RectangleInfo *renderArea) : world(world), camera(camera), film(camera->getFilm()), renderArea(renderArea), threadIndex(threadCount++), rNG(5489UL + threadIndex)
{
}

RenderThread::~RenderThread()
{
    threadCount--;
    return;
}

void RenderThread::execute(void* arg)
{
    unsigned int minSamples = 0;
    if(renderArea->catchUp)
        minSamples = film->getMinSamples(renderArea);

    for(unsigned int y=renderArea->ystart + threadIndex; y <= renderArea->yend ; y += threadCount)
    {
        for(unsigned int x=renderArea->xstart; x <= renderArea->xend ; x++)
        {
        // If we are rendering full screen in catchUp-mode, render those pixels which have had the least passes. 
            if(!renderArea->catchUp || film->getSamples(x,y) == minSamples)
            {
                Ray initialRay = camera->createInitialRay(x,y, rNG);
                Vector finalColor = world->trace(initialRay, rNG);
                film->writeRadiance(x,y, finalColor);
            }
        }
    }


    return;
}
