/*
 * L2Queue.cc
 *
 *  Created on: 2019Äê2ÔÂ26ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;
class L2Queue:public cSimpleModule
{
private:
    long frameCapacity;
    cQueue queue;
    cMessage *endTransmissionEvent;
    bool isBusy;

    simsignal_t qlenSignal;
    simsignal_t busySignal;
    simsiganl_t queueingTimeSignal;
    simsignal_t dropSignal;
    simsignal_t txBytesSignal;
    simsignal_t rxBytesSignal;
public:
    L2Queue();
    ~L2Queue();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override ;
    virtual void startTrasmiting(cMessage *msg);

};

Define_Module(L2Queue);
L2Queue::L2Queue()
{
    endTransmissionEvent=nullptr;
}

L2Queue::~L2Queue()
{
    cancelAndDelete(endTransmissionEvent);
    }

void L2Queue::initialize()
{
    queue.setName("queue");
    endTransmissionEvent=new cMessage("endTxEvent");

    if(par("useCutThroughSwitching"))
    {
        gate("line$i")->setDeliverOnReceptionStart(true);

        frameCapacity=par("frameCapacity");

        qlenSignal=registerSignal("qlen");
        busySignal=registerSignal("busy");
        queueingTimeSignal=registerSignal("queueingTime");
        dropSignal=registerSignal("drop");
        txBytesSignal=registerSignal("txBytes");
        rxBytesSignal=registerSignal("rxBytes");

        emit(qlenSignal,queue.getLength());
        emit(busySignal,false);
        isBusy=false;

    }

}

void L2Queue::startTransmitting(cMessage *msg)
{
    EV<<"Starting transmission  of "<<msg<<endl;
    isBusy=true;

    int64_t numBytes=check_and_cast<cPacket *>(msg)->getByteLength();
    send(msg,"line$o");
    emit(txBytesSignal,(long)numBytes);

    simtime_t endTransmission=gate("line$o")->getTransmiisonChannel()->getTransmissionFinishTime();
    scheduleAt(endTransmission,endTransmissionEvent);

}

void L2Queue::handleMessage(cMessage *msg)
{
    if(msg==endTransmissionEvent)
    {
        EV<<"Transmission finished.\n";
        isBusy=false;
        if(queue.isEmpty())
        {
            emit(busySignal,false);
        }
        else
        {
            msg=(cMessage *) queue.pop();
            emit(queueingTimeSignal,simTime()-msg->getTimestamp());
            emit(qlenSignal,queue.getLength());
            startTransmitting(msg);
        }

    }
    else if(msg->arrivedOn("line$i"))
    {
        emit(rxBytesSignal,(long)check_and_cast<cPacket *>(msg)->getByteLength());
        send(msg,"out");
    }
    else
    {
        if(endTransmissionEvent->isScheduled())
        {
            if(frameCapacity&&queue.getLength()>=frameCapacity)
            {
                EV<<"Received"<<msg<<"but transmitter and queue full:discarding\n";
                emit(dropSignal,(long)check_and_cast<cPacket *>(msg)->getByteLength());
                delete msg;

            }
            else
            {
                EV<<"Received "<<msg<<" but transmitter busy : queueing up \n";
                msg->setTimestamp();
                queue.insert(msg);
                emit(qlenSignal,queue.getLength());
            }
        }
        else
        {
            EV<<"Received "<<msg<<endl;
            emit(queueingTimeSignal,SIMTIME_ZERO);
            startTransmitting(msg);
            emit(busySignal,true);
        }

    }

}

void  L2Queue::refreshDisplay()const
{
    getDisplayString().setTagArg("t",0,isBusy?"transmission":"idle");
    getDisplayString().setTagArg("i",1,isBusy?(queue.getLength()>=3?"red":"yellow"):"");

}
































