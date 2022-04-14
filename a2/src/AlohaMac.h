#ifndef ALOHAMAC_H_
#define ALOHAMAC_H_

#include <omnetpp.h>
#include "CsmaMac.h"

using namespace omnetpp;

class AlohaMac : public CsmaMac {
private:
    void carrierSenseAttempt(void);
    cMessage* alohaChanFree;
};


#endif /* ALOHAMAC_H_ */
