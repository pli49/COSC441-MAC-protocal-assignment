//
// Generated file, do not edit! Created by nedtool 5.6 from TransmissionIndication.msg.
//

#ifndef __TRANSMISSIONINDICATION_M_H
#define __TRANSMISSIONINDICATION_M_H

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
 * Class generated from <tt>TransmissionIndication.msg:1</tt> by nedtool.
 * <pre>
 * packet TransmissionIndication
 * {
 * }
 * </pre>
 */
class TransmissionIndication : public ::omnetpp::cPacket
{
  protected:

  private:
    void copy(const TransmissionIndication& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const TransmissionIndication&);

  public:
    TransmissionIndication(const char *name=nullptr, short kind=0);
    TransmissionIndication(const TransmissionIndication& other);
    virtual ~TransmissionIndication();
    TransmissionIndication& operator=(const TransmissionIndication& other);
    virtual TransmissionIndication *dup() const override {return new TransmissionIndication(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const TransmissionIndication& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, TransmissionIndication& obj) {obj.parsimUnpack(b);}


#endif // ifndef __TRANSMISSIONINDICATION_M_H

