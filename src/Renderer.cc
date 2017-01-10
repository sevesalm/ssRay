#include "Renderer.h"
#include "UIThread.h"
#include "Intersection.h"
#include "Ray.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

Renderer::Renderer(World *world, Camera *camera, unsigned int threads) : world(world), camera(camera), openUI(true)
{
    newArea.xstart = 0;
    newArea.ystart = 0;
    newArea.xend = camera->getFilm()->getWidth()-1;
    newArea.yend = camera->getFilm()->getHeight()-1;
    newArea.fullScreen = true;
    newArea.catchUp = false;

    if(openUI)
    {
        try
        {
            uiThread = new UIThread(camera, &newArea);
        }
        catch(int i)
        {
            std::cout<<"Renderer::Renderer(): UIThread threw an exception: "<<i<<'\n';
            openUI = false;
        }
    }
    for(unsigned int i = 0; i < threads; i++)
        renderThreads.push_back(new RenderThread(world, camera, &renderArea));
    return;
}

Renderer::~Renderer(void)
{
    for(unsigned int i =0; i<renderThreads.size(); i++)
        delete renderThreads[i];

    if(openUI)
        delete uiThread;
    
    return;
}

void Renderer::start(void)
{
    unsigned int threads = renderThreads.size();
#ifdef PASSES
    unsigned int counter = 0;
#endif
    double renderStartTime, lastSaveTime;

    std::cout<<"Rendering World ["<<camera->getFilm()->getWidth()<<"x"<<camera->getFilm()->getHeight()<<"]\n\n";
    
    renderStartTime=lastSaveTime = getTime();

    if(openUI)
        uiThread->start();

    std::cout << std::setw(16) << "Time" << std::setw(9) << "Threads" <<  std::setw(6) << "Mode" << std::setw(7) << "Min" << std::setw(7) << "Max"<< std::endl;

#ifdef PASSES
    while(counter++ < PASSES )
#else
    while(true)
#endif
    {
        renderArea = newArea;
        //std::cout<<"Renderer: renderArea: "<<renderArea.xstart<<"\n";

        //Start the rendering threads
        for(unsigned int i = 0; i < threads; i++)
            (*renderThreads[i]).start();

        //Wait for the threads to finish
        for(unsigned int i = 0; i < threads; i++)
            (*renderThreads[i]).join();

        std::string mode;
        if(renderArea.fullScreen)
            mode += "F";
        else 
            mode += "S";

        if(renderArea.catchUp)
            mode += "C";
        else
            mode += "N";
        
        std::cout << std::setw(16) << timeToString(getTime() - renderStartTime) << std::setw(9) << threads << std::setw(6) << mode << std::setw(7) << camera->getFilm()->getMinSamples(&renderArea) << std::setw(7) << camera->getFilm()->getMaxSamples(&renderArea) << std::setprecision(5) << '\r' << std::flush;

        if(openUI)
            uiThread->newFilm(renderArea);
     }

    if(openUI)
        uiThread->stop();

    std::cout << "\n\n" << "Render complete. Elapsed time: " << timeToString(getTime() - renderStartTime) << '\n';

    return;
}
