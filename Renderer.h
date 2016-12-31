#ifndef RENDERER_H
#define RENDERER_H
#include <vector>
#include "Random.h"
#include "RectangleInfo.h"
#include "World.h"
#include "Camera.h"
#include "RenderThread.h"
class RenderThread;
class UIThread;
class Renderer
{
    private:
        std::vector<RenderThread*> renderThreads;
        World *world;
        Camera *camera;
        RectangleInfo renderArea, newArea;
        UIThread *uiThread;
        bool openUI;

    public:
        Renderer(World *, Camera *, unsigned int);
        ~Renderer(void);

        void start(void);
};
#endif

