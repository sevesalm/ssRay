#ifndef WORLD_H
#define WORLD_H
#include "Object.h"
#include "SkyDome.h"
#include <vector>

class Transformation;
class KDTree;
class Ray;
struct IntersectionInfo;
class World
{
    private:
        std::vector<Object*> objectVector;
        std::vector<Object*> lightVector;
        AABBox bounds;
        KDTree *kDTree;
        
        Vector createRandomRay(const Vector&,  Random&) const;
        Vector createSpecularRay(const Vector&, const Vector&) const; 
        Vector createGlossyRay(const Vector&, const Vector&, Random&, myFloat) const; 
        Vector createRefractiveRay(Vector, const Vector&, myFloat, myFloat) const;

    public:
        World();
        ~World();

        void initialize();
        SkyDome skyDome;
        void addSphere(Point, myFloat, Material *, Transformation * = NULL);
        void addBox(Point, Point, Material *, Transformation * = NULL);
        void addPlane(Point, Vector, Material *, Transformation * = NULL);
        void addTriangle(Point, Point, Point, Material *, Transformation * = NULL);
        void addMesh(std::string, Material *, Transformation * = NULL, bool = false);
        
        unsigned int objectCount() const;
        unsigned int lightCount() const;

        Vector trace(Ray, Random&) const;
};
#endif
