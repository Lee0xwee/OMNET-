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

//
// TODO auto-generated module
//
import Nodes.Node;
import ned.DatarateChannel;
network NET5
{
     types:
        channel C extends DatarateChannel
        {
            delay  =uniform(0.01ms,1s);
            datarate =uniform(1kbps,100kbps);
        }
     submodules:
            rte[5]:Node{
                address=index;
            }
     connections:
                rte[1].port++ <-->C<--> rte[0].port++;
                rte[0].port++ <-->C<-->rte[2].port++;
                rte[4].port++ <-->C <--> rte[3].port++;
                rte[3].port++ <-->C <-->rte[1].port++;
              
                rte[3].port++ <-->C <-->rte[2].port++;
                rte[2].port++ <-->C <-->rte[4].port++;
                rte[1].port++ <-->C <-->rte[2].port++;
}
