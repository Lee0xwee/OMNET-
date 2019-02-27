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

simple L2Queue
{
    parameters:
        int frameCapacity=default(0);
        bool useCutThroughSwithcing=default(false);
        @display("i=block/queue;q=queue");
        @signal[qlen](type="long");
        @signal[busy](type="bool");
        @signal[queueingTime](type="sime_t");
        @signal[drop](type="long");
        @signal[txBytes](type="long");
        @signal[rxBytes](type="long");
        @stastic[qlen](title="queue length";record=vector?,timeavg,max;interpolationmode=sample-hold);
        @stastic[busy](title="server bust state";record=vector?,timeavg;interpolationmode=sample-hold);
        @stastic[queueingTime](title="queueing time at dequeue";unit=s;interpolationmode=none);
        @stastic[drop](title="dropped packet byte length";unit=bytes;;record=vector?,count,sum,histogram;interpolationmode=none);
        @stastic[txBytes](title="transmitting packet byte length";unit=bytes;record=vector?,count,sum,histogram;interpolationmode=none);
        @stastic[rxBytes](title="received packet byte length";unit=bytes;record=vector?,count ,sum,histogram;interploationgmode=none);
        gates:
            input in;
            output out;
            inout line;
}
