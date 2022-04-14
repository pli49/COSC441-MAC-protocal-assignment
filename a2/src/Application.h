/*
 * Application.h
 *
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <omnetpp.h>
#include "AppMessage_m.h"
#include "AppResponse_m.h"
#include <map>

using namespace omnetpp;

class Application : public cSimpleModule {

public:
    void initialize();
    void handleMessage(cMessage* msg);
    ~Application();

private:
    int                 gidFromLower;
    int                 gidToLower;
    std::map<int,int>   lastSeenSeqno;
    cMessage*           wakeup;
    int                 ownAddress;
    int                 sequenceNumber = 0;
    int                 rcvdCount = 0;

private:
    void handleAppMessage(AppMessage* appmsg);
    void handleAppResponse(AppResponse* appresp);
    void handleWakeup();
};



#endif /* APPLICATION_H_ */
