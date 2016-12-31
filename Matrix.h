#ifndef MATRIX4x4_H
#define MATRIX4x4_H
#include "utils.h"
#include <iostream>
#include <algorithm>
static const myFloat initialMatrix[4][4] = {{1.0,0,0,0},{0,1.0,0,0},{0,0,1.0,0},{0,0,0,1.0}};

class Matrix4x4
{
    private:
    public:
        myFloat matrix[4][4];
        Matrix4x4(void)
        {
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                    matrix[i][j] = initialMatrix[i][j];
            return;
        }

        Matrix4x4(const Matrix4x4& _matrix)
        {
            *this = _matrix;
            return;
        }

        Matrix4x4(const myFloat data[][4])
        {
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                    matrix[i][j] = data[i][j];

            return;
        }

        Matrix4x4& operator=(const Matrix4x4& _matrix)
        {
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                    matrix[i][j] = _matrix.matrix[i][j];
            return *this;
        }

        Matrix4x4 operator*(const Matrix4x4& _matrix) const
        {
            myFloat data[4][4];

            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                {
                    myFloat value = 0.0;
                    for(int n=0;n<4;n++)
                        value += matrix[i][n] * _matrix.matrix[n][j];

                    data[i][j] = value; 
                }

            return Matrix4x4(data);
        }

        Point operator*(const Point& point) const
        {

            myFloat result[3];
            for(unsigned int i=0; i<3;i++)
                result[i] = matrix[i][0]*point.x + matrix[i][1]*point.y + matrix[i][2]*point.z + matrix[i][3];

            return Point(result[0],result[1],result[2]);
        }

        Vector operator*(const Vector& vector) const
        {

            myFloat result[3];
            for(unsigned int i=0; i<3;i++)
                result[i] = matrix[i][0]*vector.x + matrix[i][1]*vector.y + matrix[i][2]*vector.z ;

            return Vector(result[0],result[1],result[2]);
        }

        Matrix4x4 inverse(void) const
        {
            Matrix4x4 original = *this;
            Matrix4x4 inverse;
            for(unsigned int i=0; i<4;i++)
            {
                // Find the row with largest absolute value and make it the pivot.
                myFloat maxPivot = fabs(original.matrix[i][i]);
                unsigned int maxRow = i;
                for(unsigned int a=i;a<4;a++)
                    if(fabs(original.matrix[a][i]) > maxPivot)
                    {
                        maxPivot = fabs(original.matrix[a][i]);
                        maxRow = a;
                    }

                if(maxPivot == 0.0)
                {
                    std::cout<<"Matrix::inverse(): No pivot point found!\n";
                    abort();
                }

                original.swapRows(maxRow,i);
                inverse.swapRows(maxRow,i);

                //Next make pivot value 1 and use it to zero other rows.
                myFloat scale = 1.0/original.matrix[i][i];
                original.rowMultiply(i, scale);
                inverse.rowMultiply(i, scale);
                for(unsigned int j=0; j<4;j++)
                {
                    if(j==i)
                        continue;
                    myFloat scale = original.matrix[j][i];
                    original.rowSubtract(j, scale, original.matrix[i]);
                    inverse.rowSubtract(j, scale, inverse.matrix[i]);
                }

            }
            return inverse;
        }

        void rowMultiply(unsigned int row, myFloat scale)
        {
            for(unsigned int i=0;i<4;i++)
                matrix[row][i]*=scale;
            return;
        }

        void rowSubtract(unsigned int row, myFloat scale, myFloat rowData[4])
        {
            for(unsigned int i=0;i<4;i++)
                matrix[row][i]-=scale*rowData[i];

            return;
        }

        void swapRows(unsigned int rowA, unsigned int rowB)
        {
            for(unsigned int i=0; i<4;i++)
                std::swap(matrix[rowA][i], matrix[rowB][i]);

            return;
        }

        void output(void) const
        {
            for(int i=0;i<4;i++)
            {
                for(int j=0;j<4;j++)
                    std::cout<<matrix[i][j]<<' ';
                std::cout<<'\n';
            }
        }

};

#endif
