packet SignalStart {
    double      transmitPowerDBm;
    double      positionX;
    double      positionY;
    int         txAddress;
    bool        collidedFlag = false;
}