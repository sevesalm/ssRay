#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <iostream>
class Thread
{
    private:
        pthread_t thread;
        void * userData;

    protected:
        void run(void)
        {
            execute( userData );
            return;
        }

        virtual void execute(void* arg) =0;
        
        static void * threadAddress(void * pthis)
        {
            Thread *pt = (Thread *) pthis;
            pt->run();
            return NULL;
        }

    public:
        Thread(void) {}
        virtual ~Thread(void) {}
        
        void start(void * arg = NULL) 
        {
            userData = arg;
            pthread_create(&thread, NULL, &(Thread::threadAddress), (void*) this);
            return;
        }
        
        void join(void)
        {
            pthread_join(thread, NULL);
            return;
        }

};

#endif
