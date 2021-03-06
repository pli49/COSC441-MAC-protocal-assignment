//
// Generated file, do not edit! Created by nedtool 5.6 from SignalStart.msg.
//

#ifndef __SIGNALSTART_M_H
#define __SIGNALSTART_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>SignalStart.msg:1</tt> by nedtool.
 * <pre>
 * packet SignalStart
 * {
 *     double transmitPowerDBm;
 *     double positionX;
 *     double positionY;
 *     int txAddress;
 *     bool collidedFlag = false;
 * }
 * </pre>
 */
class SignalStart : public ::omnetpp::cPacket
{
  protected:
    double transmitPowerDBm;
    double positionX;
    double positionY;
    int txAddress;
    bool collidedFlag;

  private:
    void copy(const SignalStart& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SignalStart&);

  public:
    SignalStart(const char *name=nullptr, short kind=0);
    SignalStart(const SignalStart& other);
    virtual ~SignalStart();
    SignalStart& operator=(const SignalStart& other);
    virtual SignalStart *dup() const override {return new SignalStart(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual double getTransmitPowerDBm() const;
    virtual void setTransmitPowerDBm(double transmitPowerDBm);
    virtual double getPositionX() const;
    virtual void setPositionX(double positionX);
    virtual double getPositionY() const;
    virtual void setPositionY(double positionY);
    virtual int getTxAddress() const;
    virtual void setTxAddress(int txAddress);
    virtual bool getCollidedFlag() const;
    virtual void setCollidedFlag(bool collidedFlag);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const SignalStart& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, SignalStart& obj) {obj.parsimUnpack(b);}


#endif // ifndef __SIGNALSTART_M_H

