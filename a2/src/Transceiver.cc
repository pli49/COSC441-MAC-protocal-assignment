/*
 * Transceiver.cc
 *
 */


#include "Transceiver.h"
#include "TransmissionIndication_m.h"
#include "TransmissionConfirmation_m.h"
#include "CSResponse_m.h"
#include <cmath>

Define_Module(Transceiver);

using namespace omnetpp;

double toDB(double nat);
double fromDB(double db);


// ===================================================================================
// ===================================================================================
//
// Initialization and configuration
//
// ===================================================================================
// ===================================================================================


void Transceiver::initialize()
{
    dbg_enter("initialize");

    // read config values and check their validity
    txPowerDBm        = par("txPowerDBm");
    bitRate           = par("bitRate");
    csThreshDBm       = par("csThreshDBm");
    noisePowerDBm     = par("noisePowerDBm");
    pathLossExponent  = par("pathLossExponent");
    turnaroundTime    = par("turnaroundTime");
    csTime            = par("csTime");
    ownXPosition      = par("ownXPosition");
    ownYPosition      = par("ownYPosition");
    ownAddress        = par("ownAddress");

    assert(bitRate > 0);
    assert(pathLossExponent > 0);
    assert(turnaroundTime > 0);
    assert(csTime >= 0);

    // gate id's
    gidFromMac  =  findGate("fromMac");
    gidToMac    =  findGate("toMac");
    gidFromChan =  findGate("fromChan");
    gidToChan   =  findGate("toChan");

    // allocate self messages
    csComplete           = new cMessage("Transceiver::csComplete");
    txComplete           = new cMessage("Transceiver::txComplete");
    turnoverToTxComplete = new cMessage("Transceiver::turnoverToTxComplete");
    turnoverToRxComplete = new cMessage("Transceiver::turnoverToRxComplete");

    dbg_leave("initialize");
}


// ===================================================================================
// ===================================================================================
//
// Event handlers
//
// ===================================================================================
// ===================================================================================



void Transceiver::handleMessage(cMessage* msg)
{
    dbg_string("----------------------------------------------");
    dbg_enter("handleMessage");

    if ((msg->arrivedOn(gidFromChan)) && dynamic_cast<SignalStart*>(msg))
    {
        handleSignalStart((SignalStart*) msg);
        dbg_leave("handleMessage");
        return;
    }

    if ((msg->arrivedOn(gidFromChan)) && dynamic_cast<SignalStop*>(msg))
    {
        handleSignalStop((SignalStop*) msg);
        dbg_leave("handleMessage");
        return;
    }

    if ((msg->arrivedOn(gidFromMac)) && dynamic_cast<CSRequest*>(msg))
    {
        handleCSRequest((CSRequest*) msg);
        dbg_leave("handleMessage");
        return;
    }

    if ((msg->arrivedOn(gidFromMac)) && dynamic_cast<TransmissionRequest*>(msg))
    {
        handleTransmissionRequest((TransmissionRequest*) msg);
        dbg_leave("handleMessage");
        return;
    }

    if (msg == csComplete)
    {
        handleCsComplete();
        dbg_leave("handleMessage");
        return;
    }

    if (msg == txComplete)
    {
        handleTxComplete();
        dbg_leave("handleMessage");
        return;
    }

    if (msg == turnoverToTxComplete)
    {
        handleTurnoverToTxComplete();
        dbg_leave("handleMessage");
        return;
    }

    if (msg == turnoverToRxComplete)
    {
        handleTurnoverToRxComplete();
        dbg_leave("handleMessage");
        return;
    }

    error("Transceiver::handleMessage: unexpected message");
}

// ------------------------------------------------------------

void Transceiver::handleCSRequest (CSRequest* csreq)
{
    dbg_enter("handleCsRequest");

    if (state != Receive)
    {
        error("Transceiver::handleCSRequest: got CSRequest in state other than Receive");
    }

    state = CarrierSense;

    scheduleAt(simTime() + csTime, csComplete);

    delete csreq;

    dbg_leave("handleCsRequest");
}

// ------------------------------------------------------------

void Transceiver::handleCsComplete()
{
    dbg_enter("handleCsComplete");

    if (state != CarrierSense)
    {
        error("Transceiver::handleCsComplete: got csComplete in state other than CarrierSense");
    }

    double channelPower = 0;
    for (auto it = currentTransmissions.begin(); it != currentTransmissions.end(); it++)
    {
        double rxPowerDBm = receivedPowerDBm(it->second);
        channelPower += fromDB(rxPowerDBm);
    }

    dbg_prefix();
    EV << "handleCsComplete: channelPower[DB]" << (toDB(channelPower))
       << ", currentTransmissions.size() = " << currentTransmissions.size()
       << ", csThreshDBm = " << csThreshDBm
       << ", verdict = " << (toDB(channelPower) < csThreshDBm ? "IDLE" : "BUSY")
       << endl;

    CSResponse* csresp = new CSResponse;
    csresp->setBusyChannel(toDB(channelPower) < csThreshDBm ? false : true);
    send(csresp, gidToMac);
    state = Receive;

    dbg_leave("handleCsComplete");
}

// ------------------------------------------------------------

void Transceiver::handleTransmissionRequest(TransmissionRequest* txreq)
{
    dbg_enter("handleTransmissionRequest");

    if (state != Receive)
    {
        error("Transceiver::handleTransmissionRequest: got TransmissionRequest in state other than Receive");
    }

    if (!(dynamic_cast<MacPacket*>(txreq->getEncapsulatedPacket())))
    {
        error("Transceiver::handleTransmissionRequest: encapsulated message is not a MacMessage");
    }

    if (currentMacPacket)
    {
        error("Transceiver::handleTransmissionRequest: currentMacMessage != null");
    }

    currentMacPacket = (MacPacket*) txreq->decapsulate();
    state = TurnoverToTx;
    scheduleAt(simTime() + turnaroundTime, turnoverToTxComplete);

    delete txreq;

    dbg_leave("handleTransmissionRequest");
}

// ------------------------------------------------------------

void Transceiver::handleTurnoverToTxComplete()
{
    dbg_enter("handleTurnoverToTxComplete");

    if (state != TurnoverToTx)
    {
        error("Transceiver::handleTurnoverToTxComplete: we are in a state other than TurnoverToTx");
    }

    state = Transmit;

    SignalStart* sigstart = new SignalStart;
    sigstart->setTransmitPowerDBm(txPowerDBm);
    sigstart->setPositionX(ownXPosition);
    sigstart->setPositionY(ownYPosition);
    sigstart->setTxAddress(ownAddress);
    sigstart->setCollidedFlag(false);

    int     msglen   = currentMacPacket->getByteLength();
    double  duration = msglen * 8 / bitRate;
    sigstart->encapsulate(currentMacPacket);
    currentMacPacket = nullptr;
    send(sigstart, gidToChan);
    scheduleAt(simTime()+duration, txComplete);

    dbg_leave("handleTurnoverToTxComplete");
}

// ------------------------------------------------------------

void Transceiver::handleTxComplete()
{
    dbg_enter("handleTxComplete");

    if (state != Transmit)
    {
        error("Transceiver::handleTxComplete: we are in a state other than Transmit");
    }
    state = TurnoverToRx;
    SignalStop* sigstop = new SignalStop;
    sigstop->setTxAddress(ownAddress);
    send(sigstop, gidToChan);
    scheduleAt(simTime()+turnaroundTime, turnoverToRxComplete);

    dbg_leave("handleTxComplete");
}

// ------------------------------------------------------------


void Transceiver::handleTurnoverToRxComplete()
{
    dbg_enter("handleTurnoverToRxComplete");

    if (state != TurnoverToRx)
    {
        error("Transceiver::handleTurnoverToRxComplete: we are in a state other than TurnoverToRx");
    }
    state = Receive;
    TransmissionConfirmation* txconf = new TransmissionConfirmation;
    send(txconf, gidToMac);

    dbg_leave("handleTurnoverToRxComplete");
}


// ------------------------------------------------------------

void Transceiver::handleSignalStart(SignalStart* sigstart)
{
    dbg_enter("handleSignalStart");

    assert(sigstart);
    int txAddr = sigstart->getTxAddress();
    if (currentTransmissions.find(txAddr) != currentTransmissions.end())
    {
        error("Transceiver::handleSignalStart: there is already a SignalStart for this transmitter");
    }

    dbg_prefix();
    EV << "handleSignalStart: incoming transmission from " << sigstart->getTxAddress()
       << ", transmit power is " << sigstart->getTransmitPowerDBm()
       << "dBm, received power is " << receivedPowerDBm(sigstart)
       << " dBm (" << (receivedPowerDBm(sigstart) < csThreshDBm ? "Below" : "Above") << " CS threshold)"
       << endl;

    currentTransmissions[txAddr] = sigstart;
    if(currentTransmissions.size()>1)
    {
        dbg_string("handleSignalStart: collision with ongoing transmissions detected!");
        for(auto it = currentTransmissions.begin(); it!=currentTransmissions.end();it++)
        {
            (it->second)->setCollidedFlag(true);
        }
    }

    dbg_leave("handleSignalStart");
}


// ------------------------------------------------------------

void Transceiver::handleSignalStop(SignalStop* sigstop)
{
    dbg_enter("handleSignalStop");

    assert(sigstop);
    int txAddr = sigstop->getTxAddress();
    delete sigstop;

    auto elemIt = currentTransmissions.find(txAddr);
    if (elemIt == currentTransmissions.end())
    {
        error("Transceiver::handleSignalStop: could not find corresponding SignalStart");
    }
    SignalStart* sigstart = currentTransmissions[txAddr];
    currentTransmissions.erase(elemIt);

    // check for collision
    if (sigstart->getCollidedFlag())
    {
        dbg_string("handleSignalStop: packet is collided, dropping it");
        delete sigstart;

        dbg_leave("handleSignalStop");
        return;
    }

    // if we are in turnover state or during actual transmission we just drop it
    if ((state == Transmit) || (state == TurnoverToTx) || (state == TurnoverToRx))
    {
        dbg_string("handleSignalStop: packet received while transmitting or during turnover, dropping it");
        delete sigstart;

        dbg_leave("handleSignalStop");
        return;
    }

    // check type of encapsulated packet
    if (!(dynamic_cast<MacPacket*>(sigstart->getEncapsulatedPacket())))
    {
        error("Transceiver::handleSignalStop: encapsulated message is not a MacMessage");
    }

    MacPacket* macmsg = (MacPacket*) sigstart->decapsulate();

    double rxPowerDBm = receivedPowerDBm(sigstart);
    double ehat       = rxPowerDBm - (noisePowerDBm + toDB(bitRate));
    double qarg       = sqrt(fromDB(ehat));
    double ber        = 0.5*erfc(qarg);
    int    pktlen     = macmsg->getByteLength();
    double per        = 1 - pow(1-ber, pktlen);
    double u          = uniform(0,1);

    EV << "Transceiver::handleSignalStop: rxPowerDBm = " << rxPowerDBm
       << " , noisePowerDBm = " << noisePowerDBm
       << " , toDB(bitrate) = " << toDB(bitRate)
       << " , ehat = " << ehat
       << " , ber = " << ber
       << " , per = " << per
       << " , u = " << u
       << endl;

    delete sigstart;

    if (u<per)
    {
        dbg_string("handleSignalStop: packet is erroneous, dropping it");
        delete macmsg;

        dbg_leave("handleSignalStop");
        return;
    }

    if (macmsg->getReceiverAddress() != ownAddress)
    {
        dbg_string("handleSignalStop: packet is correct, but not for me, dropping it");
        delete macmsg;

        dbg_leave("handleSignalStop");
        return;
    }

    if (macmsg->getTransmitterAddress() == ownAddress)
    {
        dbg_string("handleSignalStop: packet is correct, but is my own, dropping it");
        delete macmsg;

        dbg_leave("handleSignalStop");
        return;
    }

    TransmissionIndication* txInd = new TransmissionIndication;
    txInd->encapsulate(macmsg);
    send(txInd,gidToMac);

    dbg_leave("handleSignalStop");
}



// ===================================================================================
// ===================================================================================
//
// Various helpers
//
// ===================================================================================
// ===================================================================================


double toDB(double nat)
{
    return 10 * log10(nat);
}

// ------------------------------------------------------------

double fromDB(double db)
{
    return pow(10,db/10);
}

// ------------------------------------------------------------


double Transceiver::receivedPowerDBm(SignalStart* sigstart)
{
    // dbg_enter("receivedPowerDBm");

    assert(sigstart);
    double distance   = sqrt(pow(ownXPosition - sigstart->getPositionX(),2)+pow(ownYPosition - sigstart->getPositionY(),2));
    double pathloss   = distance <= 1 ? 1 : pow(distance,pathLossExponent);
    double pathlossDB = toDB(pathloss);

    // EV << "Transceiver::receivedPowerDBm: distance = " << distance
    //    << ", pathloss = " << pathloss
    //    << ", pathlossDB = " << pathlossDB
    //    << ", pathLossExponent = " << pathLossExponent
    //    << ", txPowerDBm = " << sigstart->getTransmitPowerDBm()
    //    << endl;

    // dbg_leave("receivedPowerDBm");
    return (sigstart->getTransmitPowerDBm()) - pathlossDB;
}


// ===================================================================================
// ===================================================================================
//
// Debug helpers
//
// ===================================================================================
// ===================================================================================

void Transceiver::dbg_prefix()
{
    EV << "t = " << simTime()
       << " - Transceiver-" << ownAddress
       << "[" << transStateStrings[state]
       << "," << currentMacPacket
       << "," << currentTransmissions.size()
       << "]: ";
}

// ------------------------------------------------------------


void Transceiver::dbg_enter (std::string methname)
{
    dbg_prefix();
    EV << "entering " << methname
       << endl;
}

// ------------------------------------------------------------

void Transceiver::dbg_leave (std::string methname)
{
    dbg_prefix();
    EV << "leaving " << methname
       << endl;
}

// ------------------------------------------------------------

void Transceiver::dbg_string(std::string str)
{
    dbg_prefix();
    EV << str
       << endl;
}

// ===================================================================================
// ===================================================================================
//
// Shutdown
//
// ===================================================================================
// ===================================================================================


Transceiver::~Transceiver()
{
    cancelAndDelete(csComplete);
    cancelAndDelete(txComplete);
    cancelAndDelete(turnoverToTxComplete);
    cancelAndDelete(turnoverToRxComplete);

    for (auto it = currentTransmissions.begin(); it != currentTransmissions.end(); it++)
    {
        auto elem = it->second;
        delete elem;
        currentTransmissions.erase(it);
    }
}

