#include "CubeGLDrawer.h"

#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>

#include <Eigen/OpenGLSupport>
using namespace Eigen;

namespace 
{
    // simple cube data
    GLint cube_num_vertices = 8;

    GLfloat cube_vertices [8][3] = {
    {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };

    GLfloat cube_vertex_colors [8][3] = {
    {1.0, 1.0, 1.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
    {1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0} };

    GLint num_faces = 6;

    short cube_faces [6][4] = {
    {3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

    static void drawFace(GLfloat fSize, int faceInd)
    {
        glBegin (GL_QUADS);
        for (int i = 0; i < 4; i++)
        {
            glVertex3f(cube_vertices[cube_faces[faceInd][i]][0] * fSize, cube_vertices[cube_faces[faceInd][i]][1] * fSize, cube_vertices[cube_faces[faceInd][i]][2] * fSize);
        }
        glEnd ();
    }

    static void drawCubFace(GLfloat fSize, int faceInd, RubikColor col)
    {
        // glColor3f (0.0, 0.0, 0.0);
        // drawFace(fSize, faceInd);

        //glPushMatrix() ;
        //glTranslatef(, , );

        switch (col)
        {
            case white :    glColor3f (1.0, 1.0, 1.0); break;
            case yellow :   glColor3f (1.0, 1.0, 0.0); break;
            case blue :     glColor3f (0.0, 0.0, 1.0); break;
            case red :      glColor3f (1.0, 0.0, 0.0); break;
            case orange :   glColor3f (1.0, 0.5, 0.0); break;
            case green :    glColor3f (0.0, 1.0, 0.0); break;
        }

        drawFace(fSize*0.92, faceInd);

    }    
}

CubeGLDrawer::CubeGLDrawer(const Cube& curState, float cubeSize):
    m_State(curState),
    m_CubeSize(cubeSize)
{
}

CubeGLDrawer::~CubeGLDrawer()
{

}

void CubeGLDrawer::draw()
{
    draw(noColor, true, 0.0f);
}

void CubeGLDrawer::draw(RubikColor rotatingSide, bool clockwise, float curStep)
{
    Matrix4f rotMatRotating = Matrix4f::Identity();
    float rotatingAngle = 0.0f;
    if (rotatingSide != noColor)
    {
        rotatingAngle = curStep* (clockwise ? -3.1416/2.0 : 3.1416/2.0);
        AngleAxisf ax(rotatingAngle, getVectorFromColor(rotatingSide).cast<float>());
        rotMatRotating.block<3,3>(0,0) = ax.matrix();
    }

    const Cube::EdgeList& edges = m_State.getEdges();
    for (Cube::EdgeList::const_iterator it = edges.begin(); it != edges.end() && true; ++it)
    {
        glPushMatrix();

        // check if we are rotating this side.
        if (std::find(it->getPosition().begin(), it->getPosition().end(), rotatingSide) != it->getPosition().end())
            glMultMatrix(rotMatRotating);

        Vector3f xAxis(1,0,0);
        Vector3f yAxis(0,1,0);
        Vector3f firstColPos = getVectorFromColor(it->getPosition()[0]).cast<float>();
        Vector3f secondColPos = getVectorFromColor(it->getPosition()[1]).cast<float>();
        Vector3f cubePos = firstColPos + secondColPos;

        glTranslate(cubePos*m_CubeSize);


        // generate rotation so first color is x and second color is y.
        Matrix3f rotMat;
        rotMat.col(0) = firstColPos;
        rotMat.col(1) = secondColPos;
        rotMat.col(2) = secondColPos.cross(firstColPos);

        //printf("trans(%f, %f, %f) %s\n", cubePos.x(), cubePos.y(), cubePos.z(), colorName(it->getColor()[1]).c_str());

        Matrix4f rotMat4 = Matrix4f::Identity();
        rotMat4.block<3,3>(0,0) = rotMat;
        glMultMatrix(rotMat4);

        drawCubFace(m_CubeSize/2.0, 2, it->getColor()[0]);
        drawCubFace(m_CubeSize/2.0, 3, it->getColor()[1]);

        glPopMatrix();
    }


    const Cube::CornerList& corners = m_State.getCorners();
    for (Cube::CornerList::const_iterator it = corners.begin(); it != corners.end()  && true; ++it)
    {
        glPushMatrix();

        // check if we are rotating this side.
        if (std::find(it->getPosition().begin(), it->getPosition().end(), rotatingSide) != it->getPosition().end())
            glMultMatrix(rotMatRotating);

        Vector3f xAxis(1,0,0);
        Vector3f yAxis(0,1,0);
        Vector3f zAxis(0,0,1);
        Vector3f firstColPos = getVectorFromColor(it->getPosition()[0]).cast<float>();
        Vector3f secondColPos = getVectorFromColor(it->getPosition()[1]).cast<float>();
        Vector3f thirdColPos = getVectorFromColor(it->getPosition()[2]).cast<float>();
        Vector3f cubePos = firstColPos + secondColPos + thirdColPos;

        glTranslate(cubePos*m_CubeSize);


        // generate rotation so first color is x and second color is y.
        Matrix3f rotMat;
        rotMat.col(0) = firstColPos;
        rotMat.col(1) = secondColPos;
        rotMat.col(2) = thirdColPos;

        //printf("trans(%f, %f, %f) %s\n", cubePos.x(), cubePos.y(), cubePos.z(), colorName(it->getColor()[1]).c_str());

        Matrix4f rotMat4 = Matrix4f::Identity();
        rotMat4.block<3,3>(0,0) = rotMat;
        glMultMatrix(rotMat4);

        drawCubFace(m_CubeSize/2.0, 2, it->getColor()[0]);
        drawCubFace(m_CubeSize/2.0, 3, it->getColor()[1]);
        drawCubFace(m_CubeSize/2.0, 0, it->getColor()[2]);

        glPopMatrix();
    }

    for (auto rotCol : RubikBase::RubikColors)
    {
        glPushMatrix();

        // check if we are rotating this side.
        if (rotCol == rotatingSide)
            glMultMatrix(rotMatRotating);


        Vector3f xAxis(1,0,0);
        Vector3f firstColPos = getVectorFromColor(rotCol).cast<float>();
        Vector3f cubePos = firstColPos;

        glTranslate(cubePos*m_CubeSize);

        // generate rotation so first color is x and second color is y.
        Matrix3f rotMat;
        rotMat.col(0) = firstColPos;
        rotMat.col(1) = Vector3f(firstColPos.y(), firstColPos.z(), firstColPos.x());
        rotMat.col(2) = rotMat.col(1).cross(firstColPos);

        //printf("trans(%f, %f, %f) %s\n", cubePos.x(), cubePos.y(), cubePos.z(), colorName(it->getColor()[1]).c_str());

        Matrix4f rotMat4 = Matrix4f::Identity();
        rotMat4.block<3,3>(0,0) = rotMat;
        glMultMatrix(rotMat4);

        drawCubFace(m_CubeSize/2.0, 2, rotCol);

        glPopMatrix();

    }
}

