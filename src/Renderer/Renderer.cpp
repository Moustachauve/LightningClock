#include "Solid.h"

namespace Renderer {
    Renderer::Renderer(Adafruit_NeoMatrix* pMatrix)
    {
        matrix = pMatrix;
    }

    Renderer::~Renderer()
    {
    }
}