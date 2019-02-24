/*
 * Host_e.h
 *
 *  Created on: 2019Äê2ÔÂ24ÈÕ
 *      Author: Jason
 */

#ifndef HOST_E_H_
#define HOST_E_H_

# include <omnetpp.h>
using namespace omnetpp;
namespace aloha
{
    class Host:public cSimpleModule
    {
    private:
        simtime_t radioDelay;
        double txRate;
        cPar *iaTime;
        cPar *pkLenBits;
        simtime_t slotTime;
        bool isSlotted;

        cModule *sever;
        cMessage *endTxEvent;
        enum {IDLE=0,RAMSMIT=1} state;
        simsignal_t stateSignal;

        int pkCounter;
        double x,y;

        const double propagationSpeed =300000000;

        const double ringMaxRadiux= 2000;
        const double circleMaxRadius =1000;
        double idleAnimationSpeed;
        double transmissionEdgeAnimationSpeed;
        double midtransmitssionAnimationSpeed;


        cPacket *lastPacket =nullptr;
        mutable cRingFigure *transmissionRing =nullptr;
        mutable  std::vector<cOvalFigure *> transmissionCircles;

    public:
        Host();
        ~Host();
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg)override;
        virtual void refreshDisplay()const override ;
        sime_t getNextTransmissionTime();
    };
};



#endif /* HOST_E_H_ */
