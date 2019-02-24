/*
 * Serve_e.cc
 *
 *  Created on: 2019年2月24日
 *      Author: Jason
 */
# include "serve_e.h"

namespace Learning_omnet_aloha{


Define_Module(Server);

Server::Server()
{
    endRxEvent= nullptr;
}

Server::~Server()
{
    cancelAndDelete (endRxEvent);
}

Server::initialized()
{
    channelStateSignal =registerSignal("channelState");

    endRxEvent=new  cMessage("end-reception");
    channelDelay =false;
    emit(channelStateSignal,IDLE);

    gate("in")->setDeliverOnReceptionStart(true);

    currentCollisionNumFrames=0;
    receiveCounter=0;
    WATCH(currentCollisionNumFrames);

    receiveBeginSignal =registerSignal("receiveBegin");
    receiveSignal= registerSignal("receive");

    collisionSignal= registerSignal("collision");
    collisionLengthSignal =registerSiganl("collisionLength");

    emit(receiveSignal,0L);
    emit(receiveBeginSignal , 0L);

    getDisplayString().setTagArg("p",0,par("x").doubleValue());
    getDisplayString().setTagArg("p",1,par("y").doubleValue());

}

void Sever::handleMessage(cMessage *msg)
{
    if(msg==endRxEvent)
    {
        EV<<"reception finished\n";

        channelBusy =false;

        emit(channelSateSignal,IDLE);

        simtime_t dt=simTime()-recvStartTime;//simtime() get the simulation time ;


        if(currentCollisonNumFrames==0)
        {
            cTimestampedValue tmp(recvStartTime,1l);
            emit(receiveSignal,&tmp);
            emit(receiveSignal,0);

        }
        else
        {
           cTimestampedValue tmp(recvStartTime,currentCollisionNumFrames);
           emit(collisionSignal,&tmp);
           emit(collisionLengthSignal,dt);
        }

        currentCollisionNumFrames=0;
        receiveCounter=0;
        emit(receiveBeginSiganl,receiveCounter);

     }
    else
    {
        cPacket *pkt=check_and_cast<cPacket *> (msg);

        ASSERT(pkt->isReceptionStart());
        simtime_t endReceptionTime=simTime()+pkt->getDuration();

        emit(receiveBeginSignal,++receiveCounter);
        if(!channelBusy)
        {
            EV<<"started receiving\n";
            recvStartTime= simTime();
            channelBusy =true;
            emit(channelStateSignal,TRANSMISSION);
            scheduleAt(endReceptionTime,endRxEvent);

        }
        else
        {
            EV<<"another frame arrived while receiving--collision!\n";
            emit(channelStateSignal,COLLISION);

            if(currentCollisionNumFrames==0)
                currentCollsionNumFrames=2;
            else
                currentCollsionNumFrames++;

            if(endReceptionTime > endRxEvent->getArrivalTime())
            {
                canceEvent(endRxEvent);
                scheduleAt(endReceptionTime,endRxEvent);
            }

            if(hasGUI())
            {
                char buf[32];
                sprintf(buf,"Collision! (%1d frams)",currentCollisionNumFrames);
                bubble(buf);
                getParentModule()->getCanvas()->holdSimulationFor(par("animationHoldTimerOnCollsion"));//what's mean?holdsumulationFor is hold
                //如前所述，保持时间间隔是只发生动画，但模拟时间不前进，且没有处理任何事件的时间间隔。保持间隔是为了动画化的动作，采取零模拟时间。
            }
        }
        channelBusy =true;
        delete pkt;
    }

}


void Sever::refreshDisplay() const
{
    if(!channelBusy)
    {
        getDisplayString().setTagArg("i2",0,"status/off");
        getDistplayString().setTagArg("t",0,"");
    }
    else if(currentCollisionNumFrames==0)
    {
        getDisplayString().setTagArg("i2",0,"status/yellow");
        getDisplayString().setTagArg("t",0,"RECEIVE");
        getDisplayString().setTagArg("t",2,"#808000");

    }
    else
    {
        getDisplayString().setTagArg("i2",0,"status/red");
        getDisplayString().setTagArg("t",0,"COLLISION");
        getDisplayString().setTagArg("t",2,"#800000");
    }
}

void Server::finish()
{
    EV<<"duration"<<simTime()<<endl;
    recordScalar("duration",simTime());

}
};












