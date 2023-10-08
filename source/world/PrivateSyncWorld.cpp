#include "PrivateSyncWorld.h"

PrivateSyncWorld::PrivateSyncWorld() {


    for (int i = 0; i < FieldCellsWidth; i++) {
        for (int j = 0; j < FieldCellsHeight; j++) {
            worldMap[i][j] = new Cell();
        }
    }
}




bool PrivateSyncWorld::removeObjectUnsafetly(int globalXCoord, int globalYCoord) {

    if (getCellPointer(globalXCoord, globalYCoord)->isEmpty()) {
        return false;
    }

    if (getCellPointer(globalXCoord, globalYCoord)->isBot()) {
        Object* tmpO = worldMap[globalXCoord][globalYCoord]->getObjectPointer();

        delete tmpO;

        setEmpty(globalXCoord, globalYCoord);

        return true;
    }

}


bool PrivateSyncWorld::addObjectUnsafetly(Object* obj) {

    //Если там занято, о чём речь?
    if (!getCellPointer(obj->x, obj->y)->isEmpty()) {
        return false;
    }

    setObject(obj->x, obj->y, obj);



}
