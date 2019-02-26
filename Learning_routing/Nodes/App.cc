/*
 * App.cc
 *
 *  Created on: 2019Äê2ÔÂ26ÈÕ
 *      Author: Jason
 */

# include <vector>
# include <iostream>
# include <omnetpp.h>
# include <packet_m.h>

using namespace omnetpp;
using namespace std;
class App :public cSimpleModule
{
private:
    //configure
    int myAddress;
    vector<int> destAddresses;
    cPar *sendIATime;
    cPar *packetLengthBytes;


    cMessage *generatePacket;
    long pkCounter;

    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

public:
    App();
    virtual ~App();
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override ;
};

Define_Module(App);

App::App()
{
    generatePacket =nullptr;
}

App::~App()
{
    cancelAndDelete (generatePacket);
}

void App::initialize()
{
    myAdress =par("address");
    packetLengthBytes=&par("packetLength");
    sendIaTime=&par("sendIaTime");
    pkCounter=0;

    WATCH(pkCounter);
    WATCH(myAddress);

    const char *destAdderssPar=par("destAddresses");

    cStringTokenizer tokenizer(destAdderssesPar);
    const char *token;

    while((token==tokenizer.nextToken())!=nullptr)
    {
        destAddresses.push_back(atoi(token));
    }
    if(destAddresses.size()==0)
    {
        throw cRuntimeError("At least one address must be specified in the destAdress parameter!");
    }

    generatePacket =new cMessage("nextPacket");
    scheduleAt(sendIATime->doubleValue(),generatePacket);

    endToEndDelaySignal =registerSignal("endToEndDelay");
    hopCountSignal =registerSignal("hopCount");
    sourceAddressSignal =registerSignal("sourceAdress");

}

void App::handleMessage(cMessage *msg)
{
    if(msg=generatePacket)
    {
        //sending packet
        int destAddress= destAddress[intuniform(0,destAddress.size()-1)];

        char pkname[40];
        sprintf(pkname,"pk-%d-to-%d-#1d",myAddress,destAddress,pkCounter);
        EV<<"Generate packet "<<pkname<<endl;

        Packet *pk=new Packet(pkname);
        pk->setByteLength(packetLengthBytes->intValue());
        pk->setKind(intuniform(0,7));
        pk->setSrcAddr(destAddress);
        send(pk,"out");

        scheduleAt(simTime()+sendTime->doubleValue(),generatePacket);

        if(hasGUI())
        {
            getParentModule()->bubble("Generating packet.....");
        }


    }
    else
    {
        //handle Packet
        Packet *pk= check_and_cast<Packet *>(msg);
        EV<<"Received packet"<<pk->getName()<<"after"<<pk->getHopCount()<<"hops"<<endl;
        emit(endToEndDelaySignal,simTime()-pk->getCreationTime());
        emit(hopCountSignal,pk->gethopCount());
        emit(sourceAddressSignal,pk->getSrcAddr());
        if(hasGUI())
        {
            getParentMoudle()->bubble("Arrived");
        }
    }

























}

