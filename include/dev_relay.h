struct relay : Service::Switch
{
    int relayPin;
    SpanCharacteristic *powerState;

    relay(int relayPin) : Service::Switch()
    {
        powerState = new Characteristic::On();
        this->relayPin = relayPin;
        pinMode(relayPin, OUTPUT);
        digitalWrite(relayPin, HIGH);
    } // end constructor

    boolean update()
    {
        digitalWrite(relayPin, !(powerState->getNewVal()));
        return true;
    } // end update
};