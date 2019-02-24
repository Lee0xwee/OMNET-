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

simple Host
{	
    parameters:
        @signal[state](type="long");
        @statistic[radioState](source="state";title="Radio state";enum="IDLE=0;TRANSMIT=1";record=vector);
        
        double txRate@unit(bps);
        volatile int pkLenBits @unit(b);
        volatile double iaTime @unit(s); 
        
        double slotTime @unit(s);
        double x@unit(m);
        double y@unit(m);
        double idleAnimationSpeed;
        double transmissionEdgeAnimationSpeed;
        double midTransmissionAnimationSpeed;
        bool controlAnimationSpeed = default(true);
        @display("i=device/cc");
}




























