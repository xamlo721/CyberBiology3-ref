#include "ObjectRenderer.h"

#include "../entity/EnumObjectType.h"
#include "../SDL.h"
#include "../renderer/WorldRenderer.h"
#include "../world/World.h"

#include <mutex>
#include <shared_mutex>
std::shared_mutex renderMutex;

void ObjectRenderer::drawObjects(std::vector<Cell*> entityes, IObjectRenderer * worker) {


    //for (Cell* tmpObj : entityes) {
    //    drawOne(tmpObj, worker);
    //}

    auto lck = std::shared_lock{ renderMutex };

    for (int i = 0; i < FieldCellsWidth; i++) {
        for (int j = 0; j < FieldCellsHeight; j++) {
            Cell* cell = World::INSTANCE()->world.worldMap[i][j];
            drawOne(cell, worker);
        }
    }

}

void ObjectRenderer::drawOne(Cell* object, IObjectRenderer* worker) {
    if (object->objectType == EnumObjectType::Bot) {

        if (object->object->isAlive) {
            //Draw function switch, based on selected render type
            worker->draw(object->object);
        }

    }

}


