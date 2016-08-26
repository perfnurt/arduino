// Konstanter
const int gransvarde = 5;
const int pinne_lampa = 10;
const int pinne_mikrofon = 0; // Pinne A0 på kortet.


// Variabler
unsigned long dags_att_slacka_lampan = 0;
unsigned long knacktid1 = 0; // Håller reda på den första knackingens tid
unsigned long knacklangd = 100; 

unsigned long next_debug_print = 0;
// Funktioner
void setup()
{
    pinMode(pinne_lampa, OUTPUT);    
}

void loop()
{
    const unsigned long tid_nu = millis();

    if(next_debug_print == 0) next_debug_print = tid_nu + 500;
      
    if (dags_att_slacka_lampan == 0)
    {
      if (analogRead(pinne_mikrofon) > gransvarde) // Låter det tillräckligt?
      {
          if (knacktid1 == 0)
          {
            // Vi har ingen tid för första knackingen sedan tidigare
            // alltså är detta första knackningen.
  
            knacktid1 = tid_nu;
            dags_att_slacka_lampan = tid_nu + knacklangd;
            digitalWrite(pinne_lampa, HIGH);  // Tänd lampan liten stund för att visa att Arduninon hört knacket
          }
          else
          if (tid_nu > knacktid1 + knacklangd) // Testa att ta bort denna rad. Vad händer? Varför?
          {
            // Vi hade redan sparat tiden för första knackningen, då
            // är detta andra knackningen.
            const unsigned long knacktid2 = tid_nu;
  
            const unsigned long tid_mellan_knackningrna = knacktid2 - knacktid1;
  
            dags_att_slacka_lampan = tid_nu + tid_mellan_knackningrna;
            digitalWrite(pinne_lampa, HIGH);  // Tänd lampan
            knacktid1 = 0;
          }
      }
    }

    if (tid_nu > next_debug_print){
      next_debug_print = 0;
    }
    if (dags_att_slacka_lampan>0 && tid_nu > dags_att_slacka_lampan)
    {
        digitalWrite(pinne_lampa, LOW);  // Släck lampan
        dags_att_slacka_lampan = 0;
    }
}

