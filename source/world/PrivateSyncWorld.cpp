#include "PrivateSyncWorld.h"

PrivateSyncWorld::PrivateSyncWorld() {

}




bool PrivateSyncWorld::removeObjectUnsafetly(int globalXCoord, int globalYCoord) {

    if (worldMap[globalXCoord][globalYCoord].objectType == EnumObjectType::Empty) {
        return false;
    }

    if (worldMap[globalXCoord][globalYCoord].objectType == EnumObjectType::Bot) {
        Object* tmpO = worldMap[globalXCoord][globalYCoord].object;


        entityes.remove(tmpO);
        tempEntityes.remove(tmpO);

        delete tmpO;

        worldMap[globalXCoord][globalYCoord].objectType = EnumObjectType::Empty;
        worldMap[globalXCoord][globalYCoord].object = NULL;

        return true;
    }

}


bool PrivateSyncWorld::addObjectUnsafetly(Object* obj) {


    //Если там занято, о чём речь?
    if (worldMap[obj->x][obj->y].objectType != EnumObjectType::Empty) {
        return false;
    }

    //Пометить, что теперь там бот
    worldMap[obj->x][obj->y].objectType = EnumObjectType::Bot;
    worldMap[obj->x][obj->y].object = obj;

    tempEntityes.push_back(obj);


}
