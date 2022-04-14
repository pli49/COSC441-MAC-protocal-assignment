package cosc441_mac;

simple Transceiver 
{
    parameters:
        double     txPowerDBm;
        double     bitRate;
        double     csThreshDBm;
        double     noisePowerDBm;
        double     pathLossExponent;
        double     turnaroundTime @unit(s);
        double     csTime @unit(s);
 
        double     ownXPosition @unit(m);
        double     ownYPosition @unit(m);
		int        ownAddress;

    gates:
        input    fromMac;
        output   toMac;
        input    fromChan;
        output   toChan;
}