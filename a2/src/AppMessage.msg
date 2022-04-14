packet AppMessage {
    simtime_t    timestamp;
    int          senderAddress;
    int          receiverAddress;
    int          sequenceNumber;
}