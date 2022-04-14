#include <omnetpp.h>
#include "AlohaMac.h"
#include "CSResponse_m.h"

Define_Module(AlohaMac);

using namespace omnetpp;

void AlohaMac::carrierSenseAttempt()
{
    EV << "Assuming channel to be free for ALOHA" << endl;
    CSResponse *csResp = new CSResponse();
    csResp->setBusyChannel(false);
    scheduleAt(simTime(), csResp);
}
