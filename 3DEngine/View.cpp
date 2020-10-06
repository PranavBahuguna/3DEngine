#include "View.h"

View::View() { }

// Constructor - orthographic
View::View(float left, float right, float bottom, float top, float zNear, float zFar)
    : Projection(left, right, bottom, top, zNear, zFar) {}

// Constructor - persepective
View::View(float fovy, float aspectRatio, float zNear, float zFar)
    : Projection(fovy, aspectRatio, zNear, zFar) {}