#include "PrivateSyncWorld.h"

PrivateSyncWorld::PrivateSyncWorld() {


    for (int i = 0; i < FieldCellsWidth; i++) {
        for (int j = 0; j < FieldCellsHeight; j++) {
            worldMap[i][j] = new Cell();
        }
    }
}




bool PrivateSyncWorld::removeObjectUnsafetly(int globalXCoord, int globalYCoord) {

    if (isEmpty(globalXCoord, globalYCoord)) {
        return false;
    }

    if (isBot(globalXCoord, globalYCoord)) {
        Object* tmpO = worldMap[globalXCoord][globalYCoord]->object;

        delete tmpO;

        setEmpty(globalXCoord, globalYCoord);

        return true;
    }

}


bool PrivateSyncWorld::addObjectUnsafetly(Object* obj) {

    //Если там занято, о чём речь?
    if (!isEmpty(obj->x, obj->y)) {
        return false;
    }

    setObject(obj->x, obj->y, obj);



}
