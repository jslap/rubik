#include "Cube.h"

class CubeGLDrawer
{
    public:
        CubeGLDrawer(const Cube& curState, float cubeSize);
        virtual ~CubeGLDrawer();

        void draw();
        void draw(RubikColor rotatingSide, bool clockwise, float curStep);

    private:
        Cube m_State; 
        float m_CubeSize;

};