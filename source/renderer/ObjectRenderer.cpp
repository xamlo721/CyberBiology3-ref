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
            cell->lock();
            drawOne(cell, worker);
            cell->unlock();
        }
    }

}

void ObjectRenderer::drawOne(Cell* object, IObjectRenderer* worker) {
    if (object->isBot()) {

        if (object->getObjectPointer()->isAlive) {
            //Draw function switch, based on selected render type
            worker->draw(object->getObjectPointer());
        }

    }

}


