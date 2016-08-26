// Konstanter
const int pinne_lampa = 10;
const int pinne_knapp = 2;

// Variabler
bool lampan_lyser = false;

// Funktioner
void setup()
{
    pinMode(pinne_knapp, INPUT_PULLUP);
    pinMode(pinne_lampa, OUTPUT);    
}

void loop()
{
    bool lampan_ska_lysa = lampan_lyser;

    if (digitalRead(pinne_knapp) == LOW) // Är knappen nere?
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
            digitalWrite(pinne_lampa, HIGH);  // Tänd lampan
        else
            digitalWrite(pinne_lampa, LOW);  // Släck lampan

        lampan_lyser = lampan_ska_lysa;
    }
}
