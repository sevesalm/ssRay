#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Vector.h"
#include "Point.h"
#include "Matrix.h"

class Transformation
{
    private:
        Matrix4x4 transformation;
        Matrix4x4 invTransformation;
    public:
        Transformation() {}

        Transformation(const Matrix4x4& matrix)
        {
            transformation = matrix;
        }

        // Constructor to create a identical Transformation from a pointer to another Transformation.
        // Can be NULL, too.
        Transformation(const Transformation *trans)
        {
            if(trans)
                *this = *trans;
        }

        void Transform(const Matrix4x4& matrix)
        {
            transformation = matrix*transformation;
        }

        void Transform(const Transformation *trans)
        {
            transformation = trans->transformation*transformation;
        }

        void Translate(const Vector& translation)
        {
            Matrix4x4 matrix;
            matrix.matrix[0][3] = translation.x;
            matrix.matrix[1][3] = translation.y;
            matrix.matrix[2][3] = translation.z;
            transformation = matrix*transformation;
        }

        void RotateX(myFloat angle)
        {
            Matrix4x4 matrix;
            matrix.matrix[1][1] = cos(angle);
            matrix.matrix[1][2] = -sin(angle);
            matrix.matrix[2][1] = sin(angle);
            matrix.matrix[2][2] = cos(angle);
            transformation = matrix*transformation;
        }

        void RotateY(myFloat angle)
        {
            Matrix4x4 matrix;
            matrix.matrix[0][0] = cos(angle);
            matrix.matrix[0][2] = sin(angle);
            matrix.matrix[2][0] = -sin(angle);
            matrix.matrix[2][2] = cos(angle);
            transformation = matrix*transformation;
        }

        void RotateZ(myFloat angle)
        {
            Matrix4x4 matrix;
            matrix.matrix[0][0] = cos(angle);
            matrix.matrix[0][1] = -sin(angle);
            matrix.matrix[1][0] = sin(angle);
            matrix.matrix[1][1] = cos(angle);
            transformation = matrix*transformation;
        }

        void Scale(const Vector& scale)
        {
            Matrix4x4 matrix;
            matrix.matrix[0][0] = scale.x;
            matrix.matrix[1][1] = scale.y;
            matrix.matrix[2][2] = scale.z;
            transformation = matrix*transformation;
        }

        // Inverse transformation is  calculated.
        void createInverse()
        {
            invTransformation = transformation.inverse();
            //transform.output();
            //invTransform.output();
        }
        
        Vector objectNormalToWorld(const Vector& normal) const
        {
            return Vector(invTransformation.matrix[0][0]*normal.x + invTransformation.matrix[1][0]*normal.y + invTransformation.matrix[2][0]*normal.z,
                          invTransformation.matrix[0][1]*normal.x + invTransformation.matrix[1][1]*normal.y + invTransformation.matrix[2][1]*normal.z,
                          invTransformation.matrix[0][2]*normal.x + invTransformation.matrix[1][2]*normal.y + invTransformation.matrix[2][2]*normal.z).unit();
        }


        Point worldToObject(const Point& point) const
        {
           return invTransformation*point;
        }

        Point objectToWorld(const Point& point) const
        {
           return transformation*point;
        }
        
        Vector worldToObject(const Vector& vector) const
        {
            return invTransformation*vector;
        }
        
        Vector objectToWorld(const Vector& vector) const
        {
            return transformation*vector;
        }
};

#endif
