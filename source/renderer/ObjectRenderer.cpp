#include "ObjectRenderer.h"

#include "../entity/EnumObjectType.h"
#include "../SDL.h"
#include "../renderer/WorldRenderer.h"

void ObjectRenderer::drawObjects(Object* allCells[FieldCellsWidth][FieldCellsHeight], IObjectRenderer * worker) {

    //Objects
    Object* tmpObj;
    int ix = WorldRenderer::renderX;

    for (uint i = 0; i < FieldRenderCellsWidth; ++i) {

        for (uint iy = 0; iy < FieldCellsHeight; ++iy) {

            if (ix >= FieldCellsWidth) {
                ix -= FieldCellsWidth;
            }

            tmpObj = allCells[ix][iy];

            drawOne(tmpObj, worker);

        }

        ++ix;
    }

}

void ObjectRenderer::drawOne(Object* object, IObjectRenderer* worker) {
    if (object) {
        //Draw function switch, based on selected render type
        worker->draw(object);
    }

}


