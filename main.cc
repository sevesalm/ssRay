#include "Camera.h"
#include "Renderer.h"
#include "World.h"
#include "Random.h"
#include "utils.h"
#include "Material.h"
#include "Transformation.h"
#include <iostream>
//#define CORNELL
//#define CLEAN
//#define GLOSSY
//#define BOXTEST
#define CAUSTICS
//#define TRIANGLE
//#define ROOM
void displayProgramInfo(void)
{
    std::cout<<"ssRay 0.15 - Copyright (C) 2009 Severi Salminen"<<std::endl<<std::endl;
    return;
}

void parseParameters()
{
    unsigned int threads;


    return;
}
template <class T> T sqr(const T& x) { return x*x; } 
int main()
{
    parseParameters();
    displayProgramInfo();  
    Random rNG(2647UL);
    World world;
#ifdef CAUSTICS
    Material white;
    Material glass;
    glass.setRefractiveCoef(0.95);
    glass.setSpecularCoef(0.05);
    glass.setRefractiveIndex(1.5);
    Material grid;
    grid.setPigmentGrid(Vector(0.4), Vector(1));
    grid.setPigmentScale(1);
    Material lamp;
    lamp.setEmitting(true);
    lamp.setPigmentSolid(Vector(7));
    Material gold;
    gold.setPigmentSolid(Vector(1, 1, 0.6));
    gold.setSpecularCoef(0.9);

    world.addPlane(Point(0,0,0), Vector(0,1,0), &grid);
    world.addSphere(Point(-5,3.0,-1.5), 0.4, &lamp);
    world.addSphere(Point(-1,0.41,1.5), 0.4, &glass);
    world.addSphere(Point(-1,0.41,-1.5), 0.4, &glass);
    Transformation trans;
    trans.RotateY(M_PI/4);
    trans.RotateZ(-M_PI/6);
    trans.RotateX(M_PI/6);
    trans.Translate(Vector(-1.5, 0.8, 0));
    world.addBox(Point(-0.5), Point(0.5), &glass, &trans);
    Transformation trans1;
    trans1.Scale(Vector(2));
    trans1.Translate(Vector(1,-0.5,0));
    
    world.addMesh("obj.obj", &gold, &trans1);
    Camera camera(Point(-0.4,5.5,-2.5), Point(0,0.5,0), 65, 800, 600);
#endif
#ifdef CLEAN
    Material white;
    Material grid;
    grid.setPigmentGrid(Vector(0.1), Vector(1));
    grid.setPigmentScale(10);
    Material lamp;
    lamp.setEmitting(true);
    lamp.setPigmentSolid(Vector(5));

    world.addPlane(Point(0,0,0), Vector(0,1,0), &grid);
    world.addSphere(Point(5,1,0), 0.5, &lamp);
    Camera camera(Point(2,1.5,-3), Point(0,0.5,0.5), 65, 512, 512);
#endif
#ifdef BOXTEST
    Material white;
    white.setPigmentSolid(Vector(0.95));
    Material grey;
    grey.setPigmentSolid(Vector(0.75));
    Material red;
    red.setPigmentSolid(Vector(0.9, 0.3, 0.2));
    Material fence;
    fence = white;

    Material green;
    green.setPigmentSolid(Vector(0.2,0.5, 0.1));
    Material brown;
    brown.setPigmentSolid(Vector(0.5, 0.4, 0.1));


    Material grid;
//    grid.setPigmentSolid(Vector(0.9, 0.85, 0.7));
    grid.setPigmentGrid(Vector(0.3), Vector(0.8, 0.75, 0.6));
    grid.setPigmentScale(2);
    Material lamp;
    lamp.setEmitting(true);
    lamp.setPigmentSolid(Vector(20));

    world.addPlane(Point(0,0,0), Vector(0,1,0), &grid);
    world.addSphere(Point(80,120,60), 7, &lamp);
//    world.skyDome.setSolidColor(Vector(1.3));
    Camera camera(Point(19.1,2.2,-12.1), Point(13,-10,0),110, 800,600);//, 0.013, 0.9 );

    #define SIZE 100
    #define TOTALSIZE 100.0
    #define GAPFRAC 0.55
    #define CAPELEMENT (10.0/SIZE)
    #define MAX_HEIGHT 3
    #define MIN_HEIGHT 1

    for(int i = 0; i<SIZE;i++)
    {
        for(int j = 0; j<SIZE;j++)
        {
            myFloat distance = sqrt(sqr(i-SIZE/2)+sqr(j-SIZE/2));
            if(distance >= SIZE/2.0)
                continue;
            //if(rNG.randomNumberClosed()-(1-distance/(SIZE/2)) > 0.2 )
            //    continue;
            
            Vector color1 = Vector(0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed());
            myFloat height = MIN_HEIGHT+(MAX_HEIGHT-MIN_HEIGHT)*(0.5-0.5*cos( 0.5*2*M_PI/(SIZE/2)*sqrt(sqr(i-SIZE/2)+sqr(j-SIZE/2))))*(1-0.8*(rNG.randomNumberClosed()));
            if(rNG.randomNumberClosed()<0.1)
                height*=rNG.randomNumberClosed()*1.75;
            myFloat width = (1.0-GAPFRAC)*TOTALSIZE/SIZE;
            myFloat gap = GAPFRAC*TOTALSIZE/SIZE;
            myFloat offset = -(width+gap)*(SIZE-1)/2.0;
            Vector translation = Vector(offset+(i+0.5*GAPFRAC*rNG.randomNumberClosed())*(width+gap), 0, offset+(j+0.5*GAPFRAC*rNG.randomNumberClosed())*(width+gap));

            world.addBox(Point(-width/2.2, 0, -width/2.2)+translation, Point(width/2.2,height,width/2.2)+translation, &white);
            world.addBox(Point(-width/1.85, 0, -width/1.85)+translation, Point(width/1.85,0+CAPELEMENT,width/1.85)+translation, &grey);
            world.addBox(Point(-width/1.85, height/2, -width/1.85)+translation, Point(width/1.85,height/2+CAPELEMENT,width/1.85)+translation, &grey);
            world.addBox(Point(-width/1.85, height, -width/1.85)+translation, Point(width/1.85,height+CAPELEMENT,width/1.85)+translation, &grey);
//            world.addBox(Point(-width/2.3, height+CAPELEMENT, -width/2.3)+translation, Point(width/2.3,height+2*CAPELEMENT,width/2.3)+translation, &grey);
            world.addBox(Point(-width/4, height+CAPELEMENT, -width/4)+translation, Point(width/4,height+2.4*CAPELEMENT,width/4)+translation, &white);
            world.addBox(Point(-width/2.5, height+2.6*CAPELEMENT, -width/2.5)+translation, Point(width/2.5,height+2.8*CAPELEMENT,width/2.5)+translation, &grey);
            world.addBox(Point(-width/5, height+2.8*CAPELEMENT, -width/5)+translation, Point(width/5,height+3*CAPELEMENT,width/5)+translation, &grey);
            world.addBox(Point(-width/40, height+3*CAPELEMENT, -width/40)+translation, Point(width/40,height+3.5*CAPELEMENT,width/40)+translation, &grey);
            world.addBox(Point(-width/200, height+3.5*CAPELEMENT, -width/200)+translation, Point(width/200,height+5.5*CAPELEMENT,width/200)+translation, &white);
            world.addSphere(Point(0,height+5.5*CAPELEMENT,0)+translation, width/40, &red); 
            world.addSphere(Point(0,height+5*CAPELEMENT,0)+translation, width/80, &red); 
            world.addSphere(Point(0,height+4.5*CAPELEMENT,0)+translation, width/80, &red); 
            world.addBox(Point(-width/1.77, 0, width/1.77)+translation, Point(-width/2.30,height+1.5*CAPELEMENT,width/2.30)+translation, &grey);
            world.addBox(Point(-width/1.77, 0, -width/2.3)+translation, Point(-width/2.30,height+1.5*CAPELEMENT,-width/1.77)+translation, &grey);
            world.addBox(Point(width/2.30, 0, width/1.77)+translation, Point(width/1.77,height+1.5*CAPELEMENT,width/2.30)+translation, &grey);
            world.addBox(Point(width/2.30, 0, -width/2.30)+translation, Point(width/1.77,height+1.5*CAPELEMENT,-width/1.77)+translation, &grey);
            world.addBox(Point(-width/2.3, height+1.4*CAPELEMENT, -width/2)+ translation, Point(width/2.3, height+1.5*CAPELEMENT, -width/2.02)+translation, &fence);
            world.addBox(Point(-width/2.3, height+1.4*CAPELEMENT, width/2.02)+ translation, Point(width/2.3, height+1.5*CAPELEMENT, width/2)+translation, &fence);
            world.addBox(Point(-width/2.0, height+1.4*CAPELEMENT, -width/2.3)+translation, Point(-width/2.02, height+1.5*CAPELEMENT, width/2.3)+translation, &fence);
            world.addBox(Point(width/2.02, height+1.4*CAPELEMENT, -width/2.3)+translation, Point(width/2, height+1.5*CAPELEMENT, width/2.3)+translation, &fence);
            world.addBox(Point(-width/2.3, height+1.2*CAPELEMENT, -width/2)+ translation, Point(width/2.3, height+1.3*CAPELEMENT, -width/2.02)+translation, &fence);
            world.addBox(Point(-width/2.3, height+1.2*CAPELEMENT, width/2.02)+ translation, Point(width/2.3, height+1.3*CAPELEMENT, width/2)+translation, &fence);
            world.addBox(Point(-width/2.0, height+1.2*CAPELEMENT, -width/2.3)+translation, Point(-width/2.02, height+1.3*CAPELEMENT, width/2.3)+translation, &fence);
            world.addBox(Point(width/2.02, height+1.2*CAPELEMENT, -width/2.3)+translation, Point(width/2, height+1.3*CAPELEMENT, width/2.3)+translation, &fence);
            
            world.addBox(Point(-width/(3*2.3), height+CAPELEMENT, width/2.02)+ translation, Point(-width/(3.4*2.3), height+1.5*CAPELEMENT, width/2)+translation, &fence);
            world.addBox(Point(width/(3.4*2.3), height+CAPELEMENT, width/2.02)+ translation, Point(width/(3*2.3), height+1.5*CAPELEMENT, width/2)+translation, &fence);
            world.addBox(Point(-width/(3*2.3), height+CAPELEMENT, -width/2)+ translation, Point(-width/(3.4*2.3), height+1.5*CAPELEMENT, -width/2.02)+translation, &fence);
            world.addBox(Point(width/(3.4*2.3), height+CAPELEMENT, -width/2)+ translation, Point(width/(3*2.3), height+1.5*CAPELEMENT, -width/2.02)+translation, &fence);
            
            world.addBox(Point(-width/(2.0), height+CAPELEMENT, -width/(3*2.3))+ translation, Point(-width/(2.02), height+1.5*CAPELEMENT, -width/(3.4*2.3))+translation, &fence);
            world.addBox(Point(-width/(2.0), height+CAPELEMENT, width/(3.4*2.3))+ translation, Point(-width/(2.02), height+1.5*CAPELEMENT, width/(3*2.3))+translation, &fence);
            world.addBox(Point(width/(2.02), height+CAPELEMENT, -width/(3*2.3))+ translation, Point(width/(2.0), height+1.5*CAPELEMENT, -width/(3.4*2.3))+translation, &fence);
            world.addBox(Point(width/(2.02), height+CAPELEMENT, width/(3.4*2.3))+ translation, Point(width/(2.0), height+1.5*CAPELEMENT, width/(3*2.3))+translation, &fence);

            int Z = round(rNG.randomNumberClosed())*2-1;
            int X = round(rNG.randomNumberClosed())*2-1;
            world.addSphere(Point(X*width/2.7, height+CAPELEMENT*1.15, Z*width/2.7)+translation, CAPELEMENT/6, &brown);
            world.addSphere(Point(X*width/2.7, height+CAPELEMENT*1.4, Z*width/2.7)+translation, CAPELEMENT/4, &green);

        }
    }
#endif
#ifdef CORNELL
    Material white;
    Material red;
    Material green;
    Material lamp;
    Material black;

    white.setPigmentSolid(Vector(0.76,0.75,0.5));
    red.setPigmentSolid(Vector(0.63,0.06,0.04));
    green.setPigmentSolid(Vector(0.15,0.48,0.09));
    black.setPigmentSolid(Vector(0));
    
    lamp.setEmitting(true);
    lamp.setPigmentSolid((80*Vector(1,0.8,0.43)).power(1.0/GAMMA));

    //Camera camera(Point(275,275,-800), Point(275,275,0), 38.5,  100,100);
    Camera camera(Point(275,275,-800), Point(275,275,0), 38.5,  1024/2,1024/2);
    
    world.addPlane(Point(0,0,0), Vector(0,1,0), &white);
    world.addPlane(Point(0,550,0), Vector(0,-1,0), &white);
    world.addPlane(Point(0,0,0), Vector(1,0,0), &red);
    world.addPlane(Point(550,0,0), Vector(-1,0,0), &green);
    world.addPlane(Point(0,0,550), Vector(0,0,-1), &white);
    world.addPlane(Point(0,0,-20), Vector(0,0,1), &black);

    Transformation trans1;
    trans1.RotateY(2*M_PI*(17.0/360.0));
    trans1.Translate(Vector(260,0,114));
    world.addBox(Point(0,0,0), Point(165,165,165), &white, &trans1);
   
    Transformation trans2;
    trans2.RotateY( 2*M_PI*(-17.0/360.0));
    trans2.Translate(Vector(127,0,247));
    world.addBox(Point(0,0,0), Point(165,330,165), &white, &trans2);
   
    world.addBox(Point(215,549,227), Point(345,550,332), &lamp);

#endif
#ifdef ROOM 
//    Camera camera(Point(-2.1,1.6,-3.9), Point(0.6,0.8,0.75), 83, 600,400);
    Camera camera(Point(-1.5,1.6,-3.0), Point(0.0,1.2,-0.5), 105, 800,600);

    Material window;
    window.setPigmentSolid(2*(Vector(1)));
    window.setEmitting(true);

    Material white;
    white.setPigmentSolid(Vector(0.9));

    Material mirror;
    mirror.setSpecularCoef(0.95);
    mirror.setGlossiness(100);

    Material dark;
    dark.setPigmentSolid(Vector(0.1));

    Material glass;
    glass.setRefractiveCoef(0.95);
    glass.setSpecularCoef(0.05);
    glass.setRefractiveIndex(1.5);

    Material yellow;
    yellow.setPigmentSolid(0.9*Vector(0.8,1,0.2));
    Material yellow2;
    yellow2.setPigmentSolid(0.9*Vector(1,0.8,0.2));
    Material red;
    red.setPigmentSolid(0.9*Vector(1,0.4,0.4));
    Material brown;
    brown.setPigmentSolid(0.5*Vector(0.9,0.5,0.1));

    world.addBox(Point(-4,1,-1), Point(-2.99,2.5,1),  &window);
 //   world.addTriangle(Point(-2.99,1,-1), Point(-2.99, 1,1), Point(-2.99, 2.5, 1), &window);
 //   world.addTriangle(Point(-2.99,1, -1), Point(-2.99, 2.5,-1), Point(-2.99, 2.5, 1), &window);

    world.addBox(Point(-3,0.95,-1), Point(-2.8,1,1), &white);
    world.addBox(Point(-3,1.72,-1.01), Point(-2.96,1.78,1.01), &white ); 
    world.addBox(Point(-3,1,-.03), Point(-2.96,2.5,0.03), &white );
    world.addBox(Point(-2.82,2.4,-1.1), Point(-2.8,2.6,1.1), &white );

    world.addBox(Point(-2,1,1.99), Point(-0.5,2.5,3), &window);
    world.addBox(Point(-2,0.95,1.8), Point(-0.5,1,2), &white );
    world.addBox(Point(-2.01,1.72,1.96), Point(-0.49,1.78,2), &white );
    world.addBox(Point(-1.28,1.,1.96), Point(-1.22,2.5,2), &white );
    world.addBox(Point(-2.1,2.4,1.80), Point(-0.4,2.6,1.82), &white );

    world.addBox(Point(0.5,1,1.99), Point(2,2.5,3),  &window);
    world.addBox(Point(0.5,0.95,1.8), Point(2,1,2), &white );
    world.addBox(Point(0.49,1.72,1.96), Point(2.01,1.78,2), &white );
    world.addBox(Point(1.22,1.,1.96), Point(1.28,2.5,2), &white );
    world.addBox(Point(0.4,2.4,1.8), Point(2.1,2.6,1.82), &white );

    world.addBox(Point(-1.0,0,-2.2), Point(3.0, 1.0, -1.5), &white );
    world.addBox(Point(-1.03,0.97,-2.23), Point(3.0, 1.0, -1.47), &white );
    world.addBox(Point(-1.0,2.2,-2.2), Point(3.0, 3.0, -1.5), &white );
    world.addBox(Point(-0.8,1.0,-1.87), Point(-0.76, 2.2, -1.83), &white );


//Shelves
    for(int i = 0; i<5; i++)
    {
        world.addBox(Point(-3,0.98 + i*0.4,1.5), Point(-2.5, 1.0+i*0.4, 2), &white);
    }

    for(int i = 0; i<5; i++)
    {
        world.addBox(Point(-0.35,0.97+i*0.25,1.7), Point(0.35, 1.0+i*0.25, 2), &white);
    }

    world.addBox(Point(2.2,0.8,1.99), Point(2.8,2.2, 2.0), &mirror);
    world.addBox(Point(2.19,0.79,1.991), Point(2.81,2.21, 2.0), &dark);

    world.addBox(Point(2.94,1.0,1.0), Point(3,2.0, 1.4), &yellow);
    world.addBox(Point(2.96,1.0,0.6), Point(3,2.0, 1.0), &yellow);
    world.addBox(Point(2.98,1.0,0.2), Point(3,2.0, 0.6), &red);
    world.addBox(Point(2.92,1.4,0.4), Point(3,1.6, 1.2), &brown);

    world.addSphere(Point(0.4,1.2,-1.85), 0.2,  &mirror);
    world.addSphere(Point(-0.3,1.2,-1.85), 0.2, &glass);

    //Table
    world.addBox(Point(-2,0.75,-0.25), Point(1, 0.7999, 1.5), &white);
    world.addBox(Point(-1.95,0.0,-0.25), Point(-1.9, 0.75, -0.2), &white);
    world.addBox(Point(-1.95,0.0,1.4), Point(-1.9, 0.75, 1.45), &white);
    world.addBox(Point(0.9,0.0,-0.25), Point(0.95, 0.75, -0.2), &white);
    world.addBox(Point(0.9,0.0,1.4), Point(0.95, 0.75, 1.45), &white);
//Chairs    
    world.addBox(Point(-1.7,0.0,-0.6), Point(-1.3, 0.5, -0.1), &white);
    world.addBox(Point(-0.7,0.0,-0.6), Point(-0.3, 0.5, -0.1), &white);
    world.addBox(Point(0.3,0.0,-0.6), Point(0.7, 0.5, -0.1), &white);
    world.addBox(Point(0.85,0.0,0.425), Point(1.35, 0.5, 0.825), &white);


    
    
    world.addPlane(Point(0,3,0), Vector(0,-1,0), &white);
    world.addPlane(Point(-3,0,0), Vector(1,0,0), &white);
    world.addPlane(Point(3,0,0), Vector(-1,0,0), &white);
    world.addPlane(Point(0,0,2), Vector(0,0,-1), &white);
    world.addPlane(Point(0,0,-5), Vector(0,0,1), &white);

    Material floor;
    floor.setPigmentSolid(0.9*Vector(0.9,0.6,0.4));
    world.addPlane(Point(0,0,0), Vector(0,1,0), &floor);
    
#endif
#ifdef DOFTEST
    Camera camera(Point(8,2,-3.8), Point(0,1.0,3), 60, 0.0, 700,450);
    
    world.addPlane(Point(0,-0.0001,0), Vector(0,1,0), 0.9*Vector(1));
    world.addPlane(Point(0,100,0), Vector(0,-1,0), Vector(), 0.2*Vector(1));
//    world.addSphere(Point(15,20,15), 7, Vector(), 12*Vector(1,0.95,0.9));

    world.addLight(new Sphere(Point(15,20,15), 7, Vector(), 12*Vector(1,0.95,0.9), 0,0,0));

    int i;
    for(i=0; i< 3; i++)
    {
        Vector color1 = Vector(0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed());
        Vector color2 = Vector(0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed());
        if(i%2)
        {
            world.addBox(Point(-0.5,0,-1.0+4*i), Point(1,1.5,-0.5+4*i), 0.9*color1);
            world.addBox(Point(-0.5,0,0.5+4*i), Point(1,1.5,1+4*i), 0.9*color1);
            world.addBox(Point(-0.5,1.5,-1+4*i), Point(0.3,3,1+4*i), 0.9*color2);
        }
        else
        {
            world.addBox(Point(-0.5,0,-1+4*i), Point(0.3,1.5,-0.5+4*i), 0.9*color1);
            world.addBox(Point(-0.5,0,0.5+4*i), Point(0.3,1.5,1+4*i), 0.9*color1);
            world.addBox(Point(-0.5,1.5,-1+4*i), Point(1,3,1+4*i), 0.9*color2);
        }
        color1 = Vector(0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed());
        world.addSphere(Point(1,0.4,2+4*i), 0.4, Vector(color1), Vector(), 0.95);
        world.addBox(Point(2.0,0,-1+4*i), Point(3.0,0.2,1+4*i), 0.2*Vector(1));
        world.addSphere(Point(2.5,0.5,4*i),0.3,Vector(1), Vector(), 0.05, 0.95, 1.5);

        color1 = Vector(0.9+0.1*rNG.randomNumberClosed(), 0.9+0.1*rNG.randomNumberClosed(), 0.9+0.1*rNG.randomNumberClosed());
        world.addBox(Point(3.6,0,-1+i*4), Point(4.2,0.4,1+i*4), Vector(color1), Vector(), 0.05, 0.95, 1.5);
    }

    world.addBox(Point(-3,0,-2), Point(-2,5,i*4), Vector(0.3,0.5,0.9), Vector(), 0.95);
#endif
#ifdef BLOCKS
    Camera camera(Point(3,10,-10), Point(-1,-3,0), 85, 1024,768, 0.15, (Point(3,10,-10)-Point(0,5,0)).length());

    world.addPlane(Point(0,-0.0001,0), Vector(0,1,0));
    world.getLastObject().setPigmentCheckers(Vector(0.9), Vector(0.3,0.3,0.2),1);
    //world.addPlane(Point(0,100,0), Vector(0,-1,0), 0.1*Vector(1));
    //world.addSphere(Point(18,24,12), 8, Vector(), 10*Vector(1,0.95,0.9));
    /*world.addLight(new Sphere(8*Vector(1,0.95,0.9), 0,0,0));
    world.getLastObject().Scale(Vector(8));
    world.getLastObject().Translate(Vector(14,18,10));*/
    world.addSphere(Point(14,18,10), 8, 8*Vector(1,0.95,0.9));

    #define SIZE 7
    #define MAX_HEIGHT 5
    #define MIN_HEIGHT 1

    for(int i = 0; i<SIZE;i++)
    {
        for(int j = 0; j<SIZE;j++)
        {
            Vector color1 = Vector(0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed(), 0.5+0.5*rNG.randomNumberClosed());
            myFloat x = 1.0 - (fabs(i-(SIZE-1)/2.0) / ((SIZE-1)/2.0));
            myFloat y = 1.0 - (fabs(j-(SIZE-1)/2.0) / ((SIZE-1)/2.0));
            myFloat temp = min(x,y);
            myFloat height = temp*(MAX_HEIGHT-MIN_HEIGHT)+MIN_HEIGHT+rNG.randomNumberClosed()*0.5;
            myFloat width = 1.2;
            myFloat gap = 0.8;
            myFloat offset = -(width+gap)*(SIZE-1)/2.0;
            world.addBox(Point(-width/2,0,-width/2), Point(width/2,height,width/2));
            world.getLastObject().RotateY(rNG.randomNumberClosed()*M_PI*2);
            myFloat scale = rNG.randomNumberClosed()*0.7+0.3;
            world.getLastObject().Scale(Vector(scale,1,1));
            world.getLastObject().Translate(Vector(offset+i*(width+gap),0,offset+j*(width+gap)));
            world.getLastObject().setPigmentSolid(color1);
            if((i+j)%2)
            {
            world.addSphere(Point(0,height+0.71,0),0.7, Vector(), 0.05,0.95,1.5);
            world.getLastObject().Scale(Vector(0.3,1,1));
            world.getLastObject().RotateY(rNG.randomNumberClosed()*M_PI*2);
            world.getLastObject().Translate(Vector(offset+i*(width+gap),0,offset+j*(width+gap)));
            world.getLastObject().setPigmentSolid(Vector(1));
            }

        }
    }
#endif

#ifdef SKYDOME
    Camera camera(Point(-1,2.5,-9), Point(0,2.0,0), 102, 1024,640);
    //world.addPlane(Point(0,0,0), Vector(0,1,0));world.getLastObject().setPigmentSolid(0.8*Vector(0.8,0.6,0.3));

    world.addBox(Point(-5.1,-10,-5.1), Point(5.1,0,5.1));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(-4.9,-0,-4.9), Point(4.9,0.3,4.9));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(-4.7,0.3,-4.7), Point(4.7,0.6,4.7));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(-4.7,4.0,-4.7), Point(4.7,4.3,4.7));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(-4.4,4.3,-4.4), Point(4.4,4.6,4.4));world.getLastObject().setPigmentSolid(Vector(0.7));

    world.addBox(Point(-4.4,0.6,-4.5), Point(-4.1,4.0,-4.1));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(-4.4,1.6,-4.1), Point(-4.1,4.0,4.1));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(-1.5,0.6,-4.5), Point(-1.2,4.0,-4.2));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(1.2,0.6,-4.5), Point(1.5,4.0,-4.2));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(4.1,0.6,-4.4), Point(4.4,4.0,-4.1));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(4.1,0.6,-4.1), Point(4.4,3.0,4.1));world.getLastObject().setPigmentSolid(Vector(0.7));
    world.addBox(Point(-4.4,0.6,4.1), Point(4.4,4.0,4.4));world.getLastObject().setPigmentSolid(Vector(0.7));
    
    world.addSphere(Point(-3.0,1.3,-3.7),0.7, Vector(), 0.05, 1.0, 0.95,1.5);world.getLastObject().setPigmentSolid(Vector(0.95));
    world.addSphere(Point(3.0,1.3,-3.7),0.7, Vector(), 0.95, 0.75);world.getLastObject().setPigmentSolid(Vector(0.95));
    world.addSphere(Point(0,1.3,-3.7),0.7);world.getLastObject().setPigmentSolid(Vector(0.95));
    
    world.addSkyDome("aa.tga");

#endif

#ifdef GLOSSY
    Camera camera(Point(-1.5,2.3,-7), Point(-0.5,0,-2.0), 75, 512,400);
    //world.skyDome.setImage("aa.tga");
    //world.skyDome.setSolidColor(Vector(0.15,0.0,0.1));
#define A 4.0
    Material glass;
    glass.setRefractiveIndex(1.5);
    glass.setRefractiveCoef(1.0);

    Material gold0;
    gold0.setPigmentSolid(Vector(1,0.9,0.4));
    gold0.setSpecularCoef(0.97);
    gold0.setGlossiness(A*10000);
    Material gold1;
    gold1.setPigmentSolid(Vector(1,0.9,0.4));
    gold1.setSpecularCoef(0.97);
    gold1.setGlossiness(A*1000);
    Material gold2;
    gold2.setPigmentSolid(Vector(1,0.9,0.4));
    gold2.setSpecularCoef(0.97);
    gold2.setGlossiness(A*100);
    Material gold3;
    gold3.setPigmentSolid(Vector(1,0.9,0.4));
    gold3.setSpecularCoef(0.97);
    gold3.setGlossiness(A*10);
    Material silver;
    silver.setPigmentSolid(Vector(0.95,0.95,0.95));
    silver.setSpecularCoef(0.97);
    silver.setGlossiness(A*1000);

    Material white;
    white.setPigmentSolid(Vector(0.95));

    Material sky;
    sky.setPigmentSolid(Vector(0.1));
    sky.setEmitting(true);

    Material ground;
    //ground.setPigmentCheckers(Vector(0.0), Vector(0.95));
    ground.setPigmentGrid(Vector(0.8, 0.8, 0.9), Vector(0.95));
    ground.setPigmentScale(8);

    Material sun;
    sun.setPigmentSolid(Vector(3.5));
    sun.setEmitting(true);
    
    //world.addPlane(Point(0,100,0), Vector(0,-1,0), &sky); 
    
    world.addSphere(Point(-3,0.8,-2), 0.8, &gold0); 
    world.addSphere(Point(-1,0.8,-1), 0.8, &gold1); 
    world.addSphere(Point(1,0.8,-1), 0.8, &gold2); 
    world.addSphere(Point(3,0.8,-2.0), 0.8, &gold3); 
    
    Vector color;
    for(int i = 0; i<12; i++)
    {
        Transformation trans1;
        trans1.RotateY(100000*rNG.randomNumberClosed());
        trans1.Translate(Vector(-3+rNG.randomNumberClosed()*5.4,0,-5+rNG.randomNumberClosed()*3));
        if(i%2)
            world.addBox(Point(-0.15,0,-0.15), Point(0.15,0.3,0.15), &white, &trans1);
        else
            world.addBox(Point(-0.15,0,-0.15), Point(0.15,0.3,0.15), &silver, &trans1);
    }
    
    world.addSphere(Point(-0.5,0.25,-2.5), 0.25, &white); 
    world.addSphere(Point(0.75,0.25,-3.4), 0.25, &white); 
    world.addSphere(Point(-0.2,0.25,-4.3), 0.25, &white); 
    
    world.addSphere(Point(10,10,-10), 4, &sun); 
    world.addPlane(Point(0,-0.0001,0), Vector(0,1,0), &ground);


#endif

#ifdef TRIANGLE
    Camera camera(Point(-1,1.1,-2.2), Point(-0.3,0.4,0), 65, 800,600);
//    Camera camera(Point(-1,1.1,-2.2), Point(0.0,0.45,-1),1, 800,600);

    Material white;

    Material horse;
    horse.setPigmentSolid(0.75*Vector(0.6,0.4,0.1));

    Material glass;
    glass.setPigmentSolid(Vector(0.95,1,0.95));
    glass.setRefractiveIndex(1.5);
    glass.setRefractiveCoef(1.0);

    Material ground;
    ground.setPigmentGrid(Vector(0.8, 0.8, 0.9), Vector(0.95));
    ground.setPigmentScale(8);

    Material silver;
    silver.setPigmentSolid(Vector(0.95));
    silver.setSpecularCoef(1);
    silver.setGlossiness(10000);
    
    Material sun;
    sun.setPigmentSolid(14*Vector(1,1,0.95));
    sun.setEmitting(true);

//    world.skyDome.setSolidColor(Vector(0.3));

    world.addPlane(Point(0,0,0), Vector(0,1,0), &ground);
    world.addSphere(Point(10,10,-12), 1, &sun);
//    world.addSphere(Point(0,1,0), 1, &white);

    
    Transformation trans;
    trans.Translate(Vector(0,-0.05272,0));
    trans.Scale(Vector(8));
    trans.Translate(Vector(0.6,0,0));
    world.addMesh("dragon.obj", &silver, &trans);

    Transformation trans2;
    trans2.RotateY(1.7*M_PI);
    trans2.Translate(Vector(0,-0.05272,0));
    trans2.Scale(Vector(5));
    trans2.Translate(Vector(-0.9,0,-0.4));
    world.addMesh("dragon.obj", &glass, &trans2);
    

    /*trans.RotateX(M_PI/2);
    trans.Scale(Vector(0.008));
    trans.Translate(Vector(0,0.7,0));
    world.addMesh("figureFull.obj", &white, &trans);*/
    
 
    Transformation trans3;   
    trans3.RotateY(-M_PI/2);
    trans3.Translate(Vector(0,0.078,0));
    trans3.Scale(4);
    trans3.Translate(Vector(0.0,0,-0.8));
    world.addMesh("horse.obj", &horse, &trans3, true);
  
//    world.skyDome.setImage("aa.tga");
//    world.addBox(Point(-0.57322,0,-0.592816), Point(0.61499,1.67873,0.744851), &white);
//    world.addMesh("box.obj", &white, &trans);
//    world.addMesh("monkey.obj", &white, &trans);
//    world.addMesh("sSphere.obj", &silver);

#endif
   
    world.initialize(); 
    Renderer renderer(&world, &camera,2 );
    renderer.start();
    return 0;
}
