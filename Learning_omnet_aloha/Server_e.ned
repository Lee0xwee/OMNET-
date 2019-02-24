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

simple Server
{
    parameters:
        @display("i=device/antennatower_l");
        
       @signal [receiveBegin](type = "long");
       @signal[receive](type="long");
       @signal[collision](type="long");
       @signal[collisionLength](type="simetime_t");
       @signal[channnelState](type="long");
       
       double x@unit(m);
       double y @unit(m);
       
       double animationHoldTimeOnCollision @unit(s)=default(0s);//动画在碰撞是保持时间
       
       @statistic[serverChannelState](source="channelState";title ="Channel state";eunm="IDEL=0,TRANSMISION=1,COLLISION=2";record =vector);
       @statistic[receiveBegin](source="receiveBegin";record=vector?;interpolationmode=sample -hold;title="receive begin");
       @statistic[channelUtilization](source="timeavg(receive)";record=last;interpolationmode=liner;title="channel utilization");
       @statistic[collisionMultiplicity](source=collision;record=vector?,histogram;title="collision multiplicity");
       @statistic[collisonLength](record=vector?,histogram,mean,sum,max;title="received frames");
       @statistic[receivedFrames](source="sum(receive)";record=last;title="received frames");
       @statistic[collidedFrames](source="sum(collision)";reocrd=last;title="collided frames");
       
       gates:
           input in @directIn;
       
       
}

















