/*
 * Application.cc
 *
 */

#include "Application.h"
#include "AppMessage_m.h"

Define_Module(Application);

using namespace omnetpp;


simsignal_t rxReceivedPacketsSig     = cComponent::registerSignal("rxReceivedPacketsSig");
simsignal_t rxUniquePacketsSig       = cComponent::registerSignal("rxUniquePacketsSig");
simsignal_t rxDelaySig               = cComponent::registerSignal("rxDelaySig");
simsignal_t rxGapSig                 = cComponent::registerSignal("rxGapSig");
simsignal_t rxDupSig                 = cComponent::registerSignal("rxDupSig");

simsignal_t txGeneratedPacketsSig    = cComponent::registerSignal("txGeneratedPacketsSig");
simsignal_t txDroppedPacketsSig      = cComponent::registerSignal("txDroppedPacketsSig");
simsignal_t txFailedPacketsSig       = cComponent::registerSignal("txFailedPacketsSig");
simsignal_t txSuccessfulPacketsSig   = cComponent::registerSignal("txSuccessfulPacketsSig");



void Application::initialize()
{
    ownAddress      = par("ownAddress");
    gidToLower      = findGate("toLower");
    gidFromLower    = findGate("fromLower");
    wakeup          = new cMessage ("Application::wakeup");
    sequenceNumber  = 0;
    rcvdCount       = 0;

    scheduleAt(simTime() + par("interArrivalTime").doubleValue(), wakeup);

}


void Application::handleMessage(cMessage* msg)
{
    if (msg == wakeup)
    {
        handleWakeup();
        return;
    }

    if (dynamic_cast<AppMessage*>(msg) && msg->arrivedOn(gidFromLower))
    {
        handleAppMessage((AppMessage*) msg);
        return;

    }

    if ((msg->arrivedOn(gidFromLower)) && dynamic_cast<AppResponse*>(msg))
    {
        handleAppResponse((AppResponse*) msg);
        return;
    }

    error("Application::handleMessage: unexpected message");
}


void Application::handleAppMessage(AppMessage* appmsg)
{
    assert(appmsg);

    int         sender   = appmsg->getSenderAddress();
    int         length   = appmsg->getByteLength();
    int         seqno    = appmsg->getSequenceNumber();
    simtime_t   delay    = simTime() - (appmsg->getTimestamp());
    int         gap      = -1;
    bool        gapFound = false;
    bool        dupFound = false;

    emit(rxReceivedPacketsSig, true);
    rcvdCount++;

    if (lastSeenSeqno.find(sender) != lastSeenSeqno.end())
    {
        gapFound = true;
        gap      = seqno - lastSeenSeqno[sender];

        if (gap == 0)
        {
            dupFound = true;
            emit(rxDupSig, true);
        }
        if (gap < 0)
        {
            error("Application: found gap of 0");
        }
    }
    lastSeenSeqno[sender] = seqno;

    EV << "Application[" << ownAddress << "]: Received packet at time " << simTime()
       << " , rcvdCount = " << rcvdCount
       << " , sender = " << sender
       << " , seqno = " << seqno
       << " , delay = " << delay
       << " , gap = " << gap
       << " , length = " << length
       << (dupFound ? " [DUPLICATE]" : "")
       << endl;

    emit(rxDelaySig, delay);

    if (gapFound && (!dupFound))
    {
        emit(rxGapSig, gap);
    }

    if (!dupFound)
    {
        emit(rxUniquePacketsSig, true);
    }

    delete appmsg;

}


void Application::handleWakeup()
{
    int  pktSize        = par("packetSize");
    AppMessage *appMsg  = new AppMessage;

    appMsg->setTimestamp(simTime());
    appMsg->setSenderAddress(ownAddress);
    appMsg->setReceiverAddress(par("receiverNodeAddress"));
    appMsg->setSequenceNumber(sequenceNumber);
    appMsg->setByteLength(pktSize);

    sequenceNumber++;

    EV << "t = " << simTime()
       << " - Application[" << ownAddress
       << "]::handleMessage: generating packet with length " << pktSize
       << endl;

    scheduleAt(simTime() + par("interArrivalTime").doubleValue(), wakeup);
    send(appMsg,gidToLower);

    emit(txGeneratedPacketsSig, true);
}

void Application::handleAppResponse(AppResponse* appresp)
{
    assert(appresp);

    switch(appresp->getOutcome()) {
    case Success:         emit(txSuccessfulPacketsSig, true); break;
    case BufferDrop:      emit(txDroppedPacketsSig, true); break;
    case ChannelFailure:  emit(txFailedPacketsSig, true); break;
    }

    EV << "Application[" << ownAddress
       << "]::handleMessage: getting an AppResponse, seqno = " << appresp->getSequenceNumber()
       << ", outcome = " << appresp->getOutcome()
       << " and dropping it"
       << endl;
    delete appresp;
}

Application::~Application()
{
    cancelAndDelete(wakeup);
}
