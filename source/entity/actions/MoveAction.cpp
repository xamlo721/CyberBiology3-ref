#include "MoveAction.h"

void MoveAction::onActivate(Bot* object, CellCluster* cluster) {

    if (object->TakeEnergy(MoveCost)) {
        object->isAlive = false;
        return;
    }
    //Place object in a new place
    int tmpY = object->y;

    this->moveObject(object, object->lookAt.x, object->lookAt.y, cluster);


}
bool MoveAction::moveObject(Object* obj, int toX, int toY, CellCluster* cluster) {

    if (cluster->area[toX][toY]->objectType != EnumObjectType::Empty) {
        return false;
    }

    Object* tmpObj = cluster->area[obj->x][obj->y]->object;

    //Пометить, что теперь там нет бота
    cluster->area[obj->x][obj->y]->objectType = EnumObjectType::Empty;
    cluster->area[obj->x][obj->y]->object = NULL;

    //Пометить, что теперь там бот
    cluster->area[toX][toY]->objectType = EnumObjectType::Bot;
    cluster->area[toX][toY]->object = tmpObj;

    tmpObj->x = toX;
    tmpObj->y = toY;

    return true;
}