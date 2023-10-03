#include "ObjectRenderer.h"

#include "../entity/EnumObjectType.h"
#include "../SDL.h"
#include "../renderer/WorldRenderer.h"
#include "../world/World.h"

void ObjectRenderer::drawObjects(std::vector<Object*> entityes, IObjectRenderer * worker) {


    for (Object* tmpObj : entityes) {
        drawOne(tmpObj, worker);
    }

}

void ObjectRenderer::drawOne(Object* object, IObjectRenderer* worker) {
    if (object->isAlive) {
        //Draw function switch, based on selected render type
        worker->draw(object);   
    }

}


