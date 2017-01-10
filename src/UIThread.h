#ifndef UITHREAD_H
#define UITHREAD_H
#include "Thread.h"
#include "XWindow.h"
#include "RectangleInfo.h"
#include "Film.h"
class Camera;
struct RectangleInfo;
class UIThread : public Thread 
{
    private:
        XWindow UIWindow;
        Camera *camera;
        Film *film;
        char *imageBuffer;
        
        RectangleInfo *newArea;
        RectangleInfo oldRenderArea;
        RectangleInfo selectedArea;
        unsigned int width;
        unsigned int height;
        bool newData;
        bool uiRunning;
        void drawBuffer(void);
        void processEvents(void);        

    public:
        UIThread(Camera *, RectangleInfo *);
        ~UIThread();
        void execute(void *);
        void newFilm(const RectangleInfo&);
        void stop();
};
#endif
