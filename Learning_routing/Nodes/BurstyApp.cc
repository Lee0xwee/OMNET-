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
    sleepTime=&par("sleeptTme");
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


















































































