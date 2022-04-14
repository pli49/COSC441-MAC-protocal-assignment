package cosc441_mac;

moduleinterface InterfaceMac
{
    parameters:
        // own station address
        int    ownAddress;
        
        // max number of AppMessages the MAC can store in FIFO buffer
        int    bufferSize;
        
        // Overhead for a MAC data packet
        int    macOverheadSizeData @unit(byte);
        
        // Overhead for a MAC acknowledgement
        int    macOverheadSizeAck @unit(byte);    
        
    gates:
        input  fromHigher;
        output toHigher;
        input  fromTransceiver;
        output toTransceiver;
}