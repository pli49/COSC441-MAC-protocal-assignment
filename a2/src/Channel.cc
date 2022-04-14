/*
 * Channel.cc
 *
 */


#include "Channel.h"
#include "SignalStart_m.h"
#include "SignalStop_m.h"

Define_Module(Channel);

using namespace omnetpp;

simsignal_t signalStartArrivalSig = cComponent::registerSignal("signalStartArrivalSig");
simsignal_t signalStopArrivalSig  = cComponent::registerSignal("signalStopArrivalSig");




void Channel::initialize(void)
{
    numInputGates  = gateSize("fromTransceiver");
    numOutputGates = gateSize("toTransceiver");

    if (numInputGates != numOutputGates)
    {
        error("Channel::initialize: input and output gate vectors have different sizes");
    }

    gidLowerInput  = gateBaseId("fromTransceiver");
    gidLowerOutput = gateBaseId("toTransceiver");
}


void Channel::handleMessage(cMessage* msg)
{
    int arrivalGate = msg->getArrivalGateId();

    if (!((gidLowerInput <= arrivalGate) && (arrivalGate < gidLowerInput + numInputGates)))
    {
        error("Channel::handleMessage: got message on illegal arrival gate");
    }

    if (dynamic_cast<SignalStart*>(msg) || (dynamic_cast<SignalStop*> (msg)))
    {
        for (int i=gidLowerOutput; i < gidLowerOutput + numOutputGates; i++)
        {
            send(msg->dup(),i);
        }

        delete msg;
    }
    else
    {
        error("Channel::handleMessage:  unexpected message type");
    }
}



