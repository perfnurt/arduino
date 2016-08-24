# Introduktion till Arduino - Hemlig knackning

Här är en beskrivning av hur man kan bygga ett system som känner av när man knackat på ett visst sätt och bara då låser upp en dörr (eller öppnar en låda eller vad man nu vill ska hända).

Beskrivningen riktar sig till nybörjare på så sätt att den börjar med grundläggande funktionalitet som steg-för-steg byggs på, lite i taget.

Det är dock bra med viss förkunskap så som

  - hur man kopplar på ström, 
  - hur man laddar ner program till Arduinon
  - vad funktionerna ```setup()``` och ```loop()``` gör.
  - lite om motstånd (resistorer) och deras färgkodning

Det finns gott om information om det på nätet - eller fråga någon i din närhet!  

För detta projekt behövs

  - En Arduino
  - En LED
  - Motstånd
    - 1 st 220Ω
    - 1 st 1MΩ
  - Div kopplingssladdar
  - Piezomikrofon
  - (listan kommer färdigställas när sista punkten är avklarad och beskrivningen är komplett)

Lista över alla steg:

  1. [Tryck på en knapp - säg till Arduinon att tända lampan.](#section_1)
  1. [Knacka - tänd/släck lampan.](#section_2)
  1. [Knacka - tänd/släck lampan, mindre fladder.](#section_3)
  1. [Knacka 2 gånger - tänd lampan lika länge](#section_4)
  1. [Knacka flera gånger - tänd lampan när man slutat](#section_5)
  1. [Spela upp knackningar med lampan.](#section_6)
  1. [Spela upp och spara knackningar - men bara om en knapp trycks ner](#section_7)
  1. [Jämför knackningar med sparad inspelning. Tänd lampan om de stämmer](#section_8)
  1. [Koppla på motor...](#section_9)

## <a name="section_1"></a>1. Tryck på en knapp - säg till Arduinon att tända lampan.
Introduktion till Arduino.

När knappen trycks ner skickas en signal till Arduinon på pinne 2.

Programmet i Arduinon känner av när pinne 2 får en signal och skickar då ut en signal till pinne 10.

![Kopplingsbild](knapp_lampa_bb.png)
*Knappen är kopplad till pinne 2 och till GND.*<br>
*Lampan är kopplad till pinne 10 och till GND via ett 220Ω motstånd.*


Programmet:
```arduino
const int pinne_lampa = 10;
const int pinne_knapp = 2;

void setup()
{
    pinMode(pinne_knapp, INPUT_PULLUP);
    pinMode(pinne_lampa, OUTPUT);    
}

void loop()
{
    if (digitalRead(pinne_knapp) == LOW) // Knappen är nere
        digitalWrite(pinne_lampa, HIGH); // Lampan lyser
    else
        digitalWrite(pinne_lampa, LOW);  // Lampan är släckt
}
```

## <a name="section_2"></a>2. Knacka - tänd/släck lampan.
Istället för en knapp som i steg 1 så används en mikrofon.

![Kopplingsbild](knack_lampa_bb.png)
*Mikrofonen är kopplad till A0 och GND, parallellt med ett 1MΩ motstånd.*<br>
*Lampan är kopplad till pinne 10 och till GND via ett 220Ω motstånd.*

Utifrån hur starkt ljudet är får man ett signalvärde på mellan 0 och 1023, där 0 alltså betyder att den inte känner av något alls och 1023 är det starkaste.

För att inte varenda litet ljud ska störa kan man sätta ett gränsvärde och inte bry sig om ljud som är svagare än det. I programmet anger konstanten ```limit``` gränsvärdet.

```arduino

const int limit = // Välj ett lagom gränsvärde - prova dig fram, 0, 1, 2, ...

const int pinne_lampa = 10;
const int pinne_mikrofon=0;
bool lampan_ar_tand=false;

void setup()
{
    pinMode(2, INPUT);
    pinMode(10, OUTPUT);    
}

void loop()
{
    int ljud_signal = analogRead(pinne_mikrofon);

    if(ljud_signal > limit)
    {
        // Byt från släckt till tänd eller tvärtom
        lampan_ar_tand = !lampan_ar_tand;

        if(lampan_ar_tand)
            digitalWrite(pinne_lampa, HIGH);
        else
            digitalWrite(pinne_lampa, LOW);
    }

}
```

## <a name="section_3"></a>3. Knacka - tänd/släck lampan, mindre fladder.

När man kör programmet ovan kommer man märka att lampan kan fladdrar lite när man knackar på mikrofonen. 

Det gör den eftersom det blir inte bara en enda ljudsignal som tas från mikrofonen, en knackning ger upphov till ett helt gäng av signaler.

Så hur man man få alla signaler att tillhöra ett och samma knack?

Man kan t.ex. kolla ifall signalen är första signalen efter att det varit tyst och bara då ändra lampan.

```arduino

const int limit = 80;

const int pinne_lampa = 10;
const int pinne_mikrofon=0;
bool lampan_ar_tand=false;

void setup()
{
    pinMode(2, INPUT);
    pinMode(10, OUTPUT);    
}

bool det_var_ljud_precis = false;

void loop()
{
    int ljud_signal = analogRead(pinne_mikrofon);
   
    if(ljud_signal > limit)
    {
      // Vi har tagit emot en tillräckligt hög ljudsignal

      if(!det_var_ljud_precis)
      {
          // Det var första signalen i detta knack

          // Byt från släckt till tänd eller tvärtom
          lampan_ar_tand = !lampan_ar_tand;

          // Nu är det inte tyst längre, knack pågår!
          det_var_ljud_precis = true;
      }

    }
    else
    {
      // Inget ljud, då är det alltså tyst (igen)
      if (det_var_ljud_precis)
      {
        // Knacket slutade precis.
        det_var_ljud_precis = false;
      }
    }

    if(lampan_ar_tand)
      digitalWrite(pinne_lampa, HIGH);
    else
      digitalWrite(pinne_lampa, LOW);
}
```
Nu fladdrar inte lampan när man knackar på mikrofonen.

## <a name="section_4"></a>4. Knacka 2 gånger - tänd lampan lika länge

Nästa steg i det hela är att lista ut hur man kan mäta tid, detta är användbart sedan när vi ska känna av om man knackat rätt.

I detta program mäter man tiden mellan två knackningar och efter den andra knackningen låter man sedan lampan vara tänd lika
länge som tiden mellan knackningarna.

För att mäta tid på detta sätt finns funktionen ```millis()```. Den ger antalet *millisekunder* som gått sedan Arduinon startats (det går 1000 millisekunder på en sekund).

Genom att anropa ```millis()``` två gånger och ta det andra värdet minus det första får man alltså svaret på hur många millisekunder som gått mellan de två anropen.

Det kan man använda när man ska räkna ut tiden mellan knackningar.

```arduino

const int limit = 80;

const int pinne_lampa = 10;
const int pinne_mikrofon=0;
bool lampan_ar_tand=false;

void setup()
{
    pinMode(2, INPUT);
    pinMode(10, OUTPUT);    
}

bool det_var_ljud_precis = false;

// ge_mig_knack_tid() 
// Om ett knack precis avslutats returneras tiden, i annat fall returneras 0.
long int ge_mig_knack_tid()
{
    int ljud_signal = analogRead(pinne_mikrofon);
  
    if(ljud_signal > limit)
    {
      det_var_ljud_precis = true;
    }
    else
    {
      if(det_var_ljud_precis)
      {
        // Knacket slutade precis.
        det_var_ljud_precis = false;

        // Returnera tidpunkten
        return millis();
      }
    }

    // Inget knack har precis slutat - då returneras 0
    return 0;
}

long int knack1 = 0;
long int dags_att_slacka_lampan = 0;

void loop()
{
    long int tid = ge_mig_knack_tid();

    if (tid > 0)
    {
      // Vi fick en knacktid. Är det första eller andra knackningen?
      if (knack1 == 0)
      {
         // Vi har ingen första knacktid - då var det första knackningen
         knack1 = tid;
      }
      else
      {
        // Vi hade redan första knacktid, då är det andra knackningen
        long int knack2 = tid;

        long int tid_mellan_knack = knack2 - knack1;

        // Dags att tända lampan
        digitalWrite(pinne_lampa, HIGH);


        // När ska den släckas?
        dags_att_slacka_lampan = knack2 + tid_mellan_knack;

      }
    }

    if (dags_att_slacka_lampan > millis())
    {
      digitalWrite(pinne_lampa, LOW);
      dags_att_slacka_lampan = 0;

      // Genom att sätta knack1 till 0 gör vi oss beredda på att lyssna efter 2 nya knackningar
      knack1 = 0;
    }
}
```

## <a name="section_5"></a>5. Knacka flera gånger - tänd lampan när man slutat

Hur vet man när det knackats färdigt? 

Det är när det varit tyst tillräckligt länge.

Vad är tillräckligt länge? 

Bra fråga, vad tycker du?

```arduino

// Hur många millisekunder är lagom att vänta innan man bestämmer sig för att tända lampan
// Prova olika värden som kan passa. Tänk på att det går 1000 på en sekund.
const long int tid_avslutad_knackning = ?; 

const long int tid_lampan_ska_lysa = ?; // Hur länge ska lampan lysa? 

const int limit = 80;

const int pinne_lampa = 10;
const int pinne_mikrofon=0;
bool lampan_ar_tand=false;

void setup()
{
    pinMode(2, INPUT);
    pinMode(10, OUTPUT);    
}

bool det_var_ljud_precis = false;

// ge_mig_knack_tid() 
// Om ett knack precis avslutats returneras tiden, i annat fall returneras 0.
long int ge_mig_knack_tid()
{
    int ljud_signal = analogRead(pinne_mikrofon);
  
    if(ljud_signal > limit)
    {
      det_var_ljud_precis = true;
    }
    else
    {
      if(det_var_ljud_precis)
      {
        // Knacket slutade precis.
        det_var_ljud_precis = false;

        // Returnera tidpunkten
        return millis();
      }
    }

    // Inget knack har precis slutat - då returneras 0
    return 0;
}

long int tid_senaste_knack = 0;
long int dags_att_slacka_lampan = 0;

void loop()
{
    long int tid = ge_mig_knack_tid();

    if (tid > 0)
    {
      // Vi fick en knacktid. 
      tid_senaste_knack = tid;

    }

    long int tid_nu = millis();

    if (tid_senaste_knack > 0 && tid_nu > tid_senaste_knack + tid_avslutad_knackning){
      // Nu har vi väntat tillräckligt länge
      // Dags att tända lampan
      digitalWrite(pinne_lampa, HIGH);

      tid_senaste_knack = 0;

      dags_att_slacka_lampan = tid_nu + tid_lampan_ska_lysa;
    }

    if (dags_att_slacka_lampan > tid_nu)
    {
      digitalWrite(pinne_lampa, LOW);
      dags_att_slacka_lampan = 0;

    }
}
```

## <a name="section_6"></a>6. Spela upp knackningar med lampan.
Efter varje knack spara undan hur lång tid det för till knacket före.

När den sista knackningen knackats - gå igenom sparade knacktider och låt lampan lysa så länge som varje knackning varat, glöm inte släcka lampan en liten stund mellan varven.

## <a name="section_7"></a>7. Spela upp och spara knackningar - men bara om en knapp trycks ner

## <a name="section_8"></a>8. Jämför knackningar med sparad inspelning. Tänd lampan om de stämmer

Hur lika är tiderna?

Hur fel får det vara men ändå räknas som rätt?

## <a name="section_9"></a>9. Koppla på motor...
