package cosc441_mac;

module NetworkInterface
{
    parameters:
        int ownAddress;
        double ownXPosition @unit(m);
        double ownYPosition @unit(m);
        string macType;

        mac.ownAddress = ownAddress;
        transceiver.ownAddress = ownAddress;
        transceiver.ownXPosition = ownXPosition;
        transceiver.ownYPosition = ownYPosition;

    gates:
        output toChan;
        input fromChan;
        output toHigher;
        input fromHigher;

    submodules:
        mac: <macType> like InterfaceMac;
        transceiver: Transceiver;
    connections:
        mac.toHigher --> toHigher;
        mac.fromHigher <-- fromHigher;
        mac.toTransceiver --> transceiver.fromMac;
        mac.fromTransceiver <-- transceiver.toMac;
        transceiver.toChan --> toChan;
        transceiver.fromChan <-- fromChan;
}
