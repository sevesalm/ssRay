#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "Thread.h"
#include "Camera.h"
#include "Random.h"
#include "RectangleInfo.h"
#include "World.h"

class RenderThread : public Thread
{
    private:
        World *world;
        Camera *camera;
        Film *film;
        RectangleInfo *renderArea;
        unsigned int threadIndex;
        Random rNG;

    public:
        RenderThread(World *, Camera *, RectangleInfo *);
        ~RenderThread();
        void execute(void *);
};

#endif
