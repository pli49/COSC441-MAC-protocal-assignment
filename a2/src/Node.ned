package cosc441_mac;

module Node
{
    parameters:
        int ownAddress = default(-1);
        double ownXPosition @unit(m) = default(0m);
        double ownYPosition @unit(m) = default(0m);

        appl.ownAddress = ownAddress;
        nwinterface.ownAddress = ownAddress;
        nwinterface.ownXPosition = ownXPosition;
        nwinterface.ownYPosition = ownYPosition;

    gates:
        output toChan;
        input fromChan;

    submodules:
        appl: Application;
        nwinterface: NetworkInterface;
    connections:
        appl.toLower --> nwinterface.fromHigher;
        appl.fromLower <-- nwinterface.toHigher;
        nwinterface.toChan --> toChan;
        nwinterface.fromChan <-- fromChan;
}
