/*
 * Serve_e.h
 *
 *  Created on: 2019Äê2ÔÂ24ÈÕ
 *      Author: Jason
 */

#ifndef SERVE_E_H_
#define SERVE_E_H_

# include <omnetpp.h>
using namespace omnetpp;
namespace Learning_omnet_aloha{


class Server: public cSimpleModule
{
private:
    bool channelBusy;
    cMessage *endRxEvent;

    long currentCollisionNumFrames;
    long receiveCounter;
    simtime_t recvStartTime;
    enum {IDLE=0,TRANSMISSION=1,COLLISON=2};

    simsignal_t receiveBeginSignal;
    simsignal_t receiveSignal;
    simsignal_t collisionLengthSignal;
    simsignal_t collisionSignal;

public:
    Server();
    virtual ~Server();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override ;
    virtual void refreshDisplay() const override;
};

};

#endif /* SERVE_E_H_ */
