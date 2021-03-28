#include "Solid.h"

namespace Renderer {

    void Solid::Draw()
    {
        matrix->fillScreen(matrix->Color(0, 100, 100));
    }

    Solid::Solid(Adafruit_NeoMatrix* pMatrix) : Renderer(pMatrix)
    {
    }

    Solid::~Solid()
    {
    }
}