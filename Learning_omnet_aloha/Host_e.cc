/*
 * Host_e.cc
 *
 *  Created on: 2019Äê2ÔÂ24ÈÕ
 *      Author: Jason
 */
# include <algorithm>
# include "Host_e.h"
# include <iostream>
using namespace std;

namespace aloha
{
    Define_Module(Host);

    Host::Host()
    {
        endTxEvent = nullptr;

    }

    Host::~Host()
    {
        delete lastPacket;
        cancelAndEvent("endTxEvent");
    }

    void Host::initialize()
    {
        stateSignal =registerSignal("state");
        server = getModuleByPath("server");
        if(!sever)
        {
            throw cRuntimeError("server not found");
        }

        txRate= par("txRate");
        iaTime= &par("iaTime");
        pkLenBits =&par("pkLenBits");

        dlotTime=par("slotTime");
        isSlotted =slotTime>0;
        WATCH(slotTime);
        WATCH(isSlotted);

        endTxEvent =new cMessage("send/endTx");
        state=IDLE;
        emit(stateSignal, state);
        pkCounter =0;
        WATCH((int&)state);
        WATCH(pkCounter);

        x=par("x").doubleValue();
        y=par("y").doubleValue();

        double serverX=server->par("x").doubleValue();
        double servery =server->par("y").doubleValue();

        idleAnimationSpeed= par("idleAnimationSpeed");
        transmissEdgeAnimationSpeed =par("transmissEdgeAnimationSpeed");
        midtransmissionAnimationSpeed= par("midtransmissionAnimationSpeed");

        double dist=sqrt((x-serverX)*(x-serverX)+(y-servery)*(y-servery));
        radioDelay=dist/propagationSpeed;

        scheduleAt(getNextTransmissionTime(),endTxEvent);
    }

    void Host::handleMessage(cMessage *msg)
    {
        ASSERT(msg==endTxEVent);
        getParentModule()->geCanvas()->serAnimationSpeed(transmissionEdgeAnimationSpeed, this);
        if(state==IDLE)
        {
            char pkname[40];
            sprintf(pkname,"pk-%d-#%d",getId(),pkCounter++);
            EV<<"generating packet"<<pkname<<endl;
            state=TRANSMIT;
            emit(stateSignal,state);

            cPacket *pk=new cPacket(pkname);
            pk->setBitLength(pkLenBits->intValue());
            simtime_t duration =pk->getBitLength()/txRate;
            sendDirect(pk,radioDelay,duration,server->gate("in"));

            scheduleAt(simTime()+duration,endTxEvent);

            if(transmissionRing !=nullptr)
            {
                delete lastPacket;
                lastPacket=pk->dup();
            }
        }
        else if(state=TRANSMIT)
        {
            state =IDLE;
            emit(stateSignal,state);
            scheduleAt(getNextTransmissionTime(),endTxEvent);
        }
        else
        {
            throw cRuntimeError("invalid state");
        }
    }

    simtime_t Host::getNextTransmissionTime()
    {
        simtime_t t=simTime()+iaTime->doubleValue();
        if(!isSlotted)
        {
            return t;
        }
        else
        {
            return slotTime * ceil(t/slottime);
        }
    }

    void Host::refreshDisplay() const
    {
        cCanvas *canvas= getParentModule()->getCanvas();
        const int numCircles= 20;
        const double circuleLineWidth=10;

        if(!transmissionRing)
        {
            auto color =cFigure::GOOD_DARK_COLORS[getId() % cFigure::NUM_GOOD_DARK_COLORS];
            transmissionRing =new cRingFigure(("Host"+to_string(getIndex())+"Ring").c_str());
            transmissionRing ->setOutlined(falsed);
            transmissionRing->setFillColor(color);
            transmissionRing->setFilled(true);
            transmissionRing->setVisible(false);
            transmissionRing-> setZIndex(-1);
            canvas->addFigure(transmissionRing);

            for(int i=0;i<numCircles;i++)
            {
                auto circle =new cOvalFigure(("Host"+to_string(getIndex())+"Circle"+to_string(i)).c_str());
                circle->setFilled(false);
                circle->setLineColor(color);
                circle->setLineOpacity(0.75);
                circle->setLineWidth(circuleLineWidth);
                circle->setZoomLineWidth(true);
                circle->setVisible(false);
                circle->setZIndex(-0.5);
                transmissionCircles.push_back(circle);
                canvas->addFigure(circle);
            }

        }
        if(lastPacket)
        {
            if(transmissionRing->getAssociatedObject()!=lastPcaket)
            {
                transmissionRing->setAssociatedObject(lastPacket);
                for(auto c:transmissionCircles)
                    c->setAssociateObject(lastPacket);
            }

            simtime_t now= simTime();
            simtime_t frontTravelTime=now -lastPacaket->getSendingTime();
            simtime_t backTravelTime= now-(lastPacket->getSendingTime()+lastPcaket->getDuration());

            double frontRadius =min(ringMaxRadius,frontravelTime.dbl()*propagationSpeed);
            double backRadius=backTraveTime()*propagationSpeed;
            double curcleRadiusIncrement =circleMaxRadius/numCircles;
        }
    }
};




























