struct light : Service::LightBulb
{
    int lightPin;
    SpanCharacteristic *powerState;

    light(int lightPin) : Service::LightBulb()
    {
        powerState = new Characteristic::On();
        this->lightPin = lightPin;
        pinMode(lightPin, OUTPUT);
    } // end contructor

    boolean update()
    {
        digitalWrite(lightPin, powerState->getNewVal());
        return true;
    } // end update
};