/*
 * Channel.h
 *
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <omnetpp.h>

using namespace omnetpp;

class Channel : public cSimpleModule {
public:
    void initialize(void);
    void handleMessage(cMessage *msg);
private:
    int  gidLowerInput;
    int  gidLowerOutput;
    int  numInputGates;
    int  numOutputGates;
};



#endif /* CHANNEL_H_ */
