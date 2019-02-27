/*
 * BurstyApp.cc
 *
 *  Created on: 2019Äê2ÔÂ26ÈÕ
 *      Author: Jason
 */
# define FSM_DEBUG
# include <omnetpp.h>
# include <packet_m.h>
using namespace omnetpp;

class BurstyApp:public sSimpleModule
{
private:
    int myAddress;
    std::vector<int> destAddresses;
    cPar *sleepTime;
    cPar *burstTime;
    cPar *sendIATime;
    cPar *packetLengthBytes;

    cFSM fsm;
    enum{
        INIT=0,
        SLEEP=FSM_Steady(1),
        ACTIVE=FSM_Steady(2),
        SEND=FSM_Transient(1)
    };

    int pkCounter;
    cMessage *startStopBurst;
    cMessage *sendMessage;
    int numSent;
    int numReceived;

    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;
public:
    BurstyApp();
    virtual ~BurstyApp();
protected:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDeisplay()const override ;

    //new methods;
    virtual void processTimer(cMessage *msg);
    virtual void processPacket(Pakcet *pk);
    virtual void gegneratePacket();

};

Define_Module(BurstyApp);

BurstyApp::BurstyApp()
{
    startStopBurst =sendMessage =nullptr;
}


BurstyApp::~BurstyApp()
{
    cancelAndDelete (startStopBurst);
    cancelAndDelete(sendMessage);
}

void BurstyApp::initialize()
{
    numSent=numReceived=0;
    WATCH(numSent);
    WATCH(numReceived);
    fsm.setName("fsm");

    destAddresses=cStringTokenizer(par("destAddresses").stdstringValue().c_str()).asIntVector();

    myAddress=par("address");
    sleepTime=&par("sleepTime");
    burstTime=&par("burstTime");
    sendIATime=&par("sendIaTime");
    packetLengthBytes=&par("packetLength");

    endToEndDelaySignal=registerSignal("endToEndDelay");
    hopCountSignal= registerSignal("hopCount");
    sourceAddressSignal =registerSignal("sourceAddress");


    pkCounter=0;
    WATCH(pkCounter);
    startStopBurst =new cMessage("startStopBurst");
    sendMessage=new cMessage("sendMessage");

    scheduleAt(0,startStopBurst);
}

void BurstyApp::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
        processTimer(msg);
    else
        processPacket(check_and_cast<Packet *>(msg));
}

void BurstyApp::processTimer(cMessage *msg)
{
    simtime_t d;
    FSM_Switch(fsm)
    {
        case FSM_Exit(INIT):
        {
            FSM_Goto(fsm,SLEEP);
            break;
        }
        case FSM_Enter(SLEEP):
        {
            d=sleepTime->doubleValue();
            scheduleAt(simTime()+d,startStopBurst);
            EV<<"sleeping for "<<d<<"s\n";
            bubble("burst ended,sleeping");
            getDisplayString().setTagArg("i",1,"");
            break;
        }
        case FSM_Exit(SLEEP):
    {
            d=burstTime->doubleValue();
            scheduleAt(simTime()+d,startStopBurst);

            EV<<"starting burst of duration "<<d<<"s\n";
            getDisplayString().setTagArg("i",1,"yellow");

            if(msg!=startStopBurst)
            {
                throw cRuntimeError("invalid event in state ACTIVE");
             }
            FSM_Goto(fsm,ACTIVE);
            break;
    }
        case FSM_Enter(ACTIVE):
    {
            d=sendTime->doubleValue();
            EV<<"next sendign in "<<d<<"s\n";
            scheduleAT(simeTime()+d,sendMessage);
            break;
    }
        case FSM_Exit(ACTIVE):
    {
            if(msg==sendMessage)
            {
                FSM_Goto(fsm,SLEEP);
            }
            else if(msg == startStopBurst)
            {
                cancelEvent(sendMessage);
                FSM_Goto(fsm,SLEEP);
            }
            else
            {
                throw cRuntimeEror("invalid event in state ACTIVE");
            }
            break;
    }
        case FSM_Exit(SEND):
    {
            generatePacket();

            FSM_Goto(fsm,ACTIVE);
            break;
    }
    }
}

void BurstyApp::processPacket(cPacket *pk)
{
    EV<<"received packet"<<pk->getName()<<" after "<<pk->getHopCount()<<" hops "<<endl;
    emit(endToEndDelaySignal,simTime()-pk->getCreationTime());
    emit(hopCountSignal,pk->getHopCount());
    emit(sourceAddressSignal,pk->getSrcAddr());
    numReceived++;
    delete pk;
}


void BurstyApp::generatePacket()
{
    int destAddress= destAddresses[intuniform(0,destAddresses.size()-1)];

    char pkname[40];
    sprintf(pkname,"pk-%d-to-%d",myAddresss,destAddress,pkCounter++);
    EV<<"generating packet"<<pkname<<endl;

    Packet *pkt= new Packet(pkname);
    pk->getByteLength(packetLengthBytes->intValue());
    pk->getSrcAddr(myAddress);
    pk->setDestAddr(destAddress);
    send(pk,"out");
}

void BurstyApp::refreshDisplay() const
{
    char txt[64];
    sprintf(txt,"sent: %d received :%d,numSent,numReceived");
    getDisplayString().setTagArg("t",0,txt);
}














































































