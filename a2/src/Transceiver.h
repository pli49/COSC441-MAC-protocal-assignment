/*
 * Transceiver.h
 *
 */

#ifndef TRANSCEIVER_H_
#define TRANSCEIVER_H_

#include <omnetpp.h>
#include "SignalStart_m.h"
#include "SignalStop_m.h"
#include "TransmissionRequest_m.h"
#include "CSRequest_m.h"
#include "MacPacket_m.h"
#include <map>

using namespace omnetpp;


enum TransceiverState {
    Receive       = 0,
    CarrierSense  = 1,
    Transmit      = 2,
    TurnoverToTx  = 3,
    TurnoverToRx  = 4
};

const char * const transStateStrings[] = {
        "Receive",
        "CarrierSense",
        "Transmit",
        "TurnoverToTx",
        "TurnoverToRx"
};


class Transceiver : public cSimpleModule {
public:
    void initialize();
    void handleMessage(cMessage* msg);
    ~Transceiver();

private:

    // internal state
    TransceiverState             state = Receive;
    std::map<int,SignalStart*>   currentTransmissions;
    MacPacket*                   currentMacPacket = nullptr;

    // config values
    double    txPowerDBm;
    double    bitRate;
    double    csThreshDBm;
    double    noisePowerDBm;
    double    pathLossExponent;
    double    turnaroundTime;
    double    csTime;
    double    ownXPosition;
    double    ownYPosition;
    int       ownAddress;

    // gate id's
    int gidFromMac;
    int gidFromChan;
    int gidToMac;
    int gidToChan;

    // self-messages
    cMessage*    csComplete;
    cMessage*    txComplete;
    cMessage*    turnoverToTxComplete;
    cMessage*    turnoverToRxComplete;

private:

    // main event handlers
    void handleCSRequest (CSRequest* csreq);
    void handleTransmissionRequest (TransmissionRequest* txreq);
    void handleSignalStart(SignalStart* sigstart);
    void handleSignalStop(SignalStop* sigstop);
    void handleCsComplete();
    void handleTxComplete();
    void handleTurnoverToTxComplete();
    void handleTurnoverToRxComplete();

    // various helpers
    double receivedPowerDBm(SignalStart* msg);

    // debug helpers
    void dbg_prefix();
    void dbg_enter(std::string methname);
    void dbg_leave(std::string methname);
    void dbg_string(std::string str);
};



#endif /* TRANSCEIVER_H_ */
