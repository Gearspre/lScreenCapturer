#include "drawpoints.h"

drawPointSingleton* drawPointSingleton::m_instance = nullptr;

drawPointSingleton::drawPointSingleton()
{
    drawInit();
}