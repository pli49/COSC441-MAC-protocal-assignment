package cosc441_mac;

simple Channel
{
    parameters:
        @signal[signalStartArrivalSig](type=bool);
        @signal[signalStopArrivalSig](type=bool);
        @statistic[signalStartArrivalSig](record=count; title"Channel: number of SignalStart messages");
        @statistic[signalStopArrivalSig](record=count; title"Channel: number of SignalStop messages");
    gates:
        input  fromTransceiver[];
        output toTransceiver[];
}