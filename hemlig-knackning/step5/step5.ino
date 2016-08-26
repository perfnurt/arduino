
// Konstanter
const int gransvarde = 5;
const int pinne_lampa = 10;
const int pinne_mikrofon = 0; // Pinne A0 på kortet.
const unsigned long vante_tid = ?; // Hur länge ska man vänta?
const unsigned long lys_tid = ?; // Så länge lyser lampan

// Variabler
unsigned long dags_att_slacka_lampan = 0;
unsigned long senaste_knacktid = 0; 

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
        senaste_knacktid = tid_nu;
    }

    if (senaste_knacktid > 0)
    {
      if (tid_nu > senaste_knacktid + vante_tid)
      {
        senaste_knacktid = 0;
        dags_att_slacka_lampan = tid_nu + lys_tid;
        digitalWrite(pinne_lampa, HIGH);  // Tänd lampan
      }
    }

    if (dags_att_slacka_lampan>0 && tid_nu > dags_att_slacka_lampan)
    {
        digitalWrite(pinne_lampa, LOW);  // Släck lampan
        dags_att_slacka_lampan = 0;       
    }
}
