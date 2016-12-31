#ifndef MESH_H
#define MESH_H

class Mesh : public Object
{
    private:
        std::vector<Triangle*> triangles;
    public:
        Mesh();
        ~Mesh();
};
#endif
