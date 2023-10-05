#include "MoveAction.h"

void MoveAction::onActivate(Bot* object, CellCluster* cluster) {

    if (object->TakeEnergy(MoveCost)) {
        object->isAlive = false;
        return;
    }
    Point clusterLookAtCoords = validateBotLookAtCoordinates(object->lookAt);
    Point clusterBootCoords = Point(0, 0);

    this->moveObject(object, clusterBootCoords, clusterLookAtCoords, cluster);


}
bool MoveAction::moveObject(Object* obj, Point fromPoint, Point toPoint, CellCluster* cluster) {

    if (cluster->area[toPoint.x][toPoint.y]->objectType != EnumObjectType::Empty) {
        return false;
    }

    //��������, ��� ������ ��� ��� ����
    cluster->area[fromPoint.x][fromPoint.y]->objectType = EnumObjectType::Empty;
    cluster->area[fromPoint.x][fromPoint.y]->object = NULL;

    //��������, ��� ������ ��� ���
    cluster->area[toPoint.x][toPoint.y]->objectType = EnumObjectType::Bot;
    cluster->area[toPoint.x][toPoint.y]->object = obj;

    obj->x += toPoint.x;
    obj->y += toPoint.y;

    return true;
}