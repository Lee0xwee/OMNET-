//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package Nodes;

simple BurstApp like IApp
{
    parameters:
        int address;
        string destAddresses;
        volatile double sleepTime @unit(s)=default(30s);
        volatile double burstTime@unit(s)=default(10s);
        volatile double sendIaTime@unit(s)=default(expomemtial(1s));
        volatile int packetLength@unit(byte);
        
        @display("i=block/source");
        @display[endToEndDelay](type="simtime_t");
        @display[hopCount](type="long");
        @display[sourceAddress](type="long");
        @statistic[endToEndDelay](title="end to end dealy of arrrrived packets";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[sourceAddress](title="sources address of arrived packets";interpolationmode=none;record?,mean,max);
        @statistic[hopCount](title="hop count of arrived packets";interpolationmode=none;record=vertor?,mean,max);
        
        gates:
            input in;
            output out;
}