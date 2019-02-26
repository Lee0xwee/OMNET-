/*
 * Routing.cc
 *
 *  Created on: 2019Äê2ÔÂ26ÈÕ
 *      Author: Jason
 */

# include <map>
# include <omnetpp.h>
# include <packet_m.h>
# include <iostream>

using namespace std;
using namespace omnetpp;

class Routing :public cSimpleModule
{
private:
    int myAddress;

    typedef std::map<int ,int > RoutingTable;
    RoutingTable rtable;

    simsignal_t dropSignal;
    simsignal_t outputIfSignal;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override ;
};

Define_Module(Routing)

void Routing::initialize()
{
    myAdress=getParentModule()->par("address");

    dropSignal=registerSignal("drop");
    outputIfSignal=registerSignal("outputIf");

    cTopology *topo =new cTopology("topo");

    std::vector<std::string> nedTypes;
    nedTypes.push_back(getParentModule()->getNedTypeName());
    topo->extractByNedTypeName(nedTypes);
    EV<<"cTopology found"<<topo->getNumNodes()<<" nodes \n";

    cTopology::Node *thisNode=topo->getNodeFor(getParentModule());

    for(int i=0;i<topo->getNumNodes();i++)
    {
        if(topo->getNode(i)==thisNode)
        {
            continue;
        }
        topo->calculateUnweightedSingleShortestPathsTo(thisNode);
        if(thisNode->getNumPaths()==0)
        {
            continue;
        }

        cGate *parentMouduleGate =thisNode->getPath(0)->getLocalNode();
        int gateIndex=parentModuleGate->getIndex();
        int adress =topo->getNode(i)->getModule()->par("address");
        rtable[address]=gateIndex;
        EV<<" towards address"<<address<<" gateIndex is "<<gateIndex<<endl;
    }
    delete topo;

}

void Routing::handleMessage(cMessage *msg)
{
        Packet *pk= check_and_cast<Packet *>( msg);
        int destAddr=pk->getDestAddr();

        if(destAddr==myAddress)
        {
            EV<<"local delivery of packet "<<pk->getName()<<endl;
            send(pk,"localOut");
            emit(outputIfSignal,-1);
            return ;
        }

        RoutingTable::iterator it =rtable.find(destAddr);
        if(it==rtable.end())
        {
            EV<<"address "<<destAddr <<" unreachable , discarding packet "<< pk->getName()<<endl;
            emit(dropSignal,(long)pk->getByteLength());
            delete pk;
            return ;
        }

        int outGateIndex=(*it).second;
        EV<<"forwarding packet"<<pk->getName()<<" on gate index"<<outGateIndex<<endl;
        pk->setHopCount(pk->getHopCount()+1);
        emit(outputIfSignal,outGateIndex);
        send(pk,"out",outGateIndex);
}

