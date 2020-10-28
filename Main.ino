#define BAUD_RATE 9600

void setup(void)
{
    Serial.begin(BAUD_RATE);
    Serial.println("Setup done");
}

void loop(void)
{
    Serial.println(millis());
    delay(1000);
}