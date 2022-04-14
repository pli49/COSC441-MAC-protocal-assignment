package cosc441_mac;

simple Application
{
    parameters:
	    @signal[rxReceivedPacketsSig](type=bool);
	    @signal[rxUniquePacketsSig](type=bool);
        @signal[rxDelaySig](type=simtime_t);
        @signal[rxGapSig](type=long);
        @signal[rxDupSig](type=bool);
        @statistic[rxReceivedPacketsSig](record=count; title="Application-Rx: raw number of received packets");
        @statistic[rxUniquePacketsSig](record=count; title="Application-Rx: number of non-duplicate received packets");
        @statistic[rxDelaySig](record=stats; title="Application-Rx: delay statistics");
        @statistic[rxGapSig](record=stats; title="Application-Rx: gap size statistics");
        @statistic[rxDupSig](record=count; title="Application-Rx: number of duplicate messages");

        @signal[txGeneratedPacketsSig](type=bool);
        @signal[txDroppedPacketsSig](type=bool);
        @signal[txFailedPacketsSig](type=bool);
        @signal[txSuccessfulPacketsSig](type=bool);
        @statistic[txGeneratedPacketsSig](record=count; title="Application-Tx: raw number of generated packets");
        @statistic[txDroppedPacketsSig](record=count; title="Application-Tx: number of dropped packets");
        @statistic[txFailedPacketsSig](record=count; title="Application-Tx: number of failed packets");
        @statistic[txSuccessfulPacketsSig](record=count; title="Application-Tx: number of successful packets");

        volatile double  interArrivalTime @unit(s) = default(exponential(20ms));
        volatile int     packetSize @unit(byte)    = default(500byte);
        volatile int     receiverNodeAddress       = default(0);
                 int     ownAddress                = default(-1);        

    gates:
        input  fromLower;
        output toLower;
}
