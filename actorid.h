#ifndef ACTORID_H
#define ACTORID_H

#include "singleton.h"
#include "actor.h"

#include <map>

class ActorId : QRR::Singleton<ActorId>
{
public:
    ActorId();

    std::map<Actor, int> actorIdMap;



    bool searchId();

};

#endif // ACTORID_H
