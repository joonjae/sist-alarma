/*
  1. voy a hacer la practica de blink del LED L (13) como hola mundo

  CH-   FFA25D
  CH    FF629D
  CH+   FFE21D
  <<    FF22DD
  >>    FF02FD
  >||   FFC23D
  -     FFE01F
  +     FFA857
  EQ    FF906F
  0     FF6897
  100+  FF9867
  200+  FFB04F
  1     FF30CF
  2     FF18E7
  3     FF7A85
  4     FF10EF
  5     FF38C7
  6     FF5AA5
  7     FF42BD
  8     FF4AB5
  9     FF52AD

  ON/OFF  B24D
  SOURCE  2AD5
  NOSPECK 6897
  RECORD  32CD
  CH+     A05F
  TIME SHIFT 30CF
  VOL-    50AF
  FULL SCREEN 2FD
  VOL+    7887
  0       48B7
  CH-     40BF
  RECALL  38C7
  1       906F
  2       B847
  3       F807
  4       B04F
  5       9867
  6       D827
  7       8877
  8       A857
  9       E817  
*/

#include <IRremote.h>
#include "funcion_v1.h"

#define PIN_SENSOR1_NO 3  // Sensor infrarojo
#define PIN_SENSOR2_NO 4  // Sensor magnetico1
#define PIN_SENSOR3_NO 5  // Sensor magnetico1
#define LED_L 13
#define ALARMA 2          //
#define ENCIENDE LOW
#define APAGA HIGH
#define MAX_COUNT_INTERVAL 360  // La idea es contar 3*60s/500ms = 360 veces (es decir 3 minutos)

unsigned long PreviousMillis = 0;        // will store last time LED was updated
const long Interval = 500;         // interval at which to blink (milliseconds)
int Count_Interval = 0;            // cuenta cantidad de veces de Interval en este caso 500ms. 
                                   // La idea es contar 3*60s/500ms = 360 veces
int ledState = LOW;             // ledState used to set the LED
bool Flag_alarma = 0;

int Control_Port = 6;         // puerto digital D6
IRrecv irrecv(Control_Port);
decode_results Codigo_control;

unsigned char Password[4] = "1234";
unsigned char Estado_Password = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); // solo para probar el cod de control remoto.
  // initialize digital pin LED_BUILTIN as an output.
  irrecv.enableIRIn();                  // habilito entrada de control remoto
  pinMode(LED_L, OUTPUT);
  pinMode(ALARMA, OUTPUT);
  pinMode(PIN_SENSOR1_NO, INPUT);    // sets the digital pin 2 as input
  digitalWrite(LED_L, APAGA);
  digitalWrite(ALARMA, APAGA);
  delay(1000);                       // wait for a second
}

// the loop function runs over and over again forever
void loop() {
  unsigned int cod_control_remoto;
  if(irrecv.decode(&Codigo_control)){
    cod_control_remoto = (int) 0x00FFFF & Codigo_control.value;
    Serial.print("Cod recibido: ");
    Serial.println(cod_control_remoto,HEX);
    if(f_pasword(cod_control_remoto)){
        
    }
    
    irrecv.resume();
    delay(100); // delay de 100ms
  }
  
  unsigned long currentMillis;
  if(digitalRead(PIN_SENSOR1_NO)||digitalRead(PIN_SENSOR2_NO)||digitalRead(PIN_SENSOR3_NO)){ // si no hay contacto entra a verificar alarma
    delay(10);                    // 10mseg de delay para anti rebote
    if(digitalRead(PIN_SENSOR1_NO)||digitalRead(PIN_SENSOR2_NO)||digitalRead(PIN_SENSOR3_NO)){ // si continua con el alarma enciende
      Flag_alarma = 1;
      Count_Interval=0;
      digitalWrite(LED_L, HIGH);      // enciende alarma
      digitalWrite(ALARMA, ENCIENDE);   // turn the LED on (HIGH is the voltage level)
    }
  }
  else
  {
    delay(10);
  }

  currentMillis = millis();
  // Blinking de LED para verificar que la placa de esta funcionando
  if (currentMillis - PreviousMillis >= Interval) {
    // save the last time you blinked the LED
    PreviousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    Count_Interval++;
    if((Flag_alarma==1)&&(Count_Interval>MAX_COUNT_INTERVAL)){
      digitalWrite(LED_L, LOW);      // apaga led
      digitalWrite(ALARMA, APAGA);   // apaga alarma
      Flag_alarma=0;
      Count_Interval=0;
    }
    if(Flag_alarma==0){
      // set the LED with the ledState of the variable:
      digitalWrite(LED_L, ledState);  // blink de led
    }
  }
}


int f_pasword(unsigned int cod_control_remoto){
    int i=0;
      switch(Estado_Password){
      case 0: if(cod_control_remoto==0xB24D) Estado_Password=1; break;  // si preciona el encendido, continua con el ingreso de password
      case 1: if(cod_control_remoto==0xB847) Estado_Password=2; break;  // espera un 2
      case 2: if(cod_control_remoto==0xE817) Estado_Password=3; break;  // espera un 9
      case 3: if(cod_control_remoto==0xB847) Estado_Password=4; break;  // espera un 2
      case 4: if(cod_control_remoto==0xF807) i=1; break;                   // espera un 3
      default:  Estado_Password=0;
    }
    return i;
  }
