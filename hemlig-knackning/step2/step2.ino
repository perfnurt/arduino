// Konstanter
const int gransvarde = 100;
const int pinne_lampa = 10;
const int pinne_mikrofon = 0; // Pinne A0 på kortet.

// Variabler
bool lampan_lyser = false;

// Funktioner
void setup()
{
    pinMode(pinne_mikrofon, INPUT_PULLUP);
    pinMode(pinne_lampa, OUTPUT);    
}

void loop()
{
    bool lampan_ska_lysa = lampan_lyser;

    if (analogRead(pinne_mikrofon) > gransvarde) // Låter det tillräckligt?
    {
        lampan_ska_lysa = true;
    }
    else
    {
        lampan_ska_lysa = false;
    }

    if (lampan_lyser != lampan_ska_lysa)
    {
        if (lampan_ska_lysa)
        {
            digitalWrite(pinne_lampa, HIGH);  // Tänd lampan
        }
        else
        {
            digitalWrite(pinne_lampa, LOW);  // Släck lampan
        }

        lampan_lyser = lampan_ska_lysa;
    }
}§void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
