#include "MoveAction.h"

void MoveAction::onActivate(Bot* object, CellCluster* cluster) {

    if (object->TakeEnergy(MoveCost)) {
        object->isAlive = false;
        return;
    }
    Point clusterBootCoords = Point(0, 0);

    this->moveObject(object, clusterBootCoords, object->lookAt, cluster);


}
bool MoveAction::moveObject(Object* obj, Point fromPoint, Point toPoint, CellCluster* cluster) {

    if (!cluster->cell(toPoint.x, toPoint.y)->isEmpty()) {
        return false;
    }

    //Пометить, что теперь там нет бота
    cluster->cell(fromPoint.x, fromPoint.y)->setEmpty();

    //Пометить, что теперь там бот
    cluster->cell(toPoint.x, toPoint.y)->setObject(obj);

    obj->x = (obj->x + toPoint.x) % (FieldCellsWidth);
    obj->y = (obj->y + toPoint.y) % (FieldCellsHeight);

    return true;
}