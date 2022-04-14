// Creates a two-dimensional network with one receiver node at the origin and a number
// of transmitter nodes placed equidistantly on a circle of given radius around the
// receiver node.
// The macType is configurable.

package cosc441_mac;

module CircularNetwork
{
    parameters:
        double radius @unit(m) = default(10m);
        int numTransmitters = default(5);
        string macType = default("AlohaMac");

    submodules:

        receiver: Node {
            parameters:
                ownAddress = 0;
                ownXPosition = 0m;
                ownYPosition = 0m;
                nwinterface.macType = macType;
                appl.interArrivalTime = 99999.9s;
                appl.receiverNodeAddress = 999999;
        }

        transmitters[numTransmitters]: Node {
            parameters:
                ownAddress = 1 + index;
                ownXPosition = m(radius * cos(2*3.1415926536*index/numTransmitters));
                ownYPosition = m(radius * sin(2*3.1415926536*index/numTransmitters));
                nwinterface.macType = macType;
                appl.receiverNodeAddress = 0;
        }

        chan: Channel;
    connections:
        receiver.toChan --> chan.fromTransceiver++;
        receiver.fromChan <-- chan.toTransceiver++;
        for i=0..numTransmitters-1 {
            transmitters[i].toChan --> chan.fromTransceiver++;
            transmitters[i].fromChan <-- chan.toTransceiver++;
        }
}
