// Konstanter
const unsigned long lampans_lystid = 100; // Hur många millisekunder ska lampan lysa?
const int gransvarde = 100;

const int pinne_lampa = 10;
const int pinne_mikrofon = 0; // Pinne A0 på kortet.

// Variabler
unsigned long dags_att_slacka_lampan = 0;

// Funktioner
void setup()
{
    pinMode(pinne_lampa, OUTPUT);    
}

void loop()
{
    const unsigned long tid_nu = millis();

    if (analogRead(pinne_mikrofon) > gransvarde) // Låter det tillräckligt?
    {
        digitalWrite(pinne_lampa, HIGH);  // Tänd lampan
        dags_att_slacka_lampan = tid_nu + lampans_lystid;
    }

    if (dags_att_slacka_lampan>0 && tid_nu > dags_att_slacka_lampan)
    {
        digitalWrite(pinne_lampa, LOW);  // Släck lampan
        dags_att_slacka_lampan = 0;
    }
}
