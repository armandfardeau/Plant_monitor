//assignation Lcd + Bibliotheque
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/*int switchState = 0;
int prevSwitchState = 0;
int reply;*/

//assignation des capteurs
const int hygroCapteur = A0;
const int LuxCapteur = A4;
const int plantLuxCapteur = A1;
const int tempCapteur = A2;
const int floodCapteur = A3;

//assignation des actionneurs
const unsigned char switchOn = 9 ;
const unsigned char switchOff = 8;
const unsigned char relais1 = 10;
//const int ledGreen = ;
//const int ledRed = ;
//const int ledYellow = ;

//assignation des variables de stockages des valeurs lues par les capteurs
unsigned char hygro;
unsigned char lux;
unsigned char plantLux;
float temp;
unsigned char flood;

//définition des valeurs BESOIN de la plante
float baselineTemp = 20.0; // en deg
unsigned char baselinePlantLux = 80; //en %
unsigned char baselineHygro = 70; //en %
unsigned char tolerance = 10;

// variable d'environnement
bool relaisState;
unsigned char baselineLux = baselinePlantLux - tolerance;
long temps =0;
//variable de déclenchement
unsigned char alertFlood = 50;
bool switchPos;


void setup ()
{
  //assignation des PINs
  pinMode(hygroCapteur, INPUT);
  pinMode(plantLuxCapteur, INPUT);
  pinMode(LuxCapteur, INPUT);
  pinMode(tempCapteur, INPUT);

  pinMode(switchOn, INPUT);
  pinMode(switchOff, INPUT);

  pinMode(relais1, OUTPUT);
  //pinMode(ledGreen, OUTPUT);
  //pinMode(ledRed, OUTPUT);
  //pinMode(ledYellow, OUTPUT);

  // Démarrage de la liaison série
  Serial.begin (9600);
  // initialisation du Lcd
  lcd.begin(16, 2);


}

void loop ()
{

  // Lecture des valeurs des capteurs hygro et lux et bornage sur un interval 
  hygro = map(analogRead (hygroCapteur), 0, 1023, 100, 0);
  lux = map(analogRead (LuxCapteur), 0, 1023, 0, 100);
  plantLux = map(analogRead (plantLuxCapteur), 0, 1023, 0, 100);
  flood = map(analogRead(floodCapteur), 0, 1023, 0, 100);

  //convertir la lecture du can Temp en tension puis en degrees
  int sensorVal = analogRead(tempCapteur);
  float voltage = (sensorVal / 1024.0) * 5.0;
  temp = (int)((voltage - 0.5) * 100);



  /*settings des alertes*/

  if (flood > alertFlood) {
    //printFlood();
    lcd.print("Alerte");
    lcd.setCursor(0, 1);
    lcd.print("Flood!");
    delay(150);
    lcd.clear();
  }

  else if (temp < baselineTemp) {
    lcd.print("Alerte");
    lcd.setCursor(0, 1);
    lcd.print("Trop froid!");
    delay(150);
    lcd.clear();
  }

  else if (hygro > baselineHygro + tolerance) {
    lcd.print("Alerte");
    lcd.setCursor(0, 1);
    lcd.print("Trop d'eau!");
    delay(150);
    lcd.clear();
  }

  else if (hygro < baselineHygro - tolerance) {
    lcd.print("Alerte");
    lcd.setCursor(0, 1);
    lcd.print("j'ai soif");
    delay(150);
    lcd.clear();
  }

  else if (plantLux > baselinePlantLux + tolerance) {
    lcd.print("Alerte");
    lcd.setCursor(0, 1);
    lcd.print("Trop de lumiere!");
    delay(150);
    lcd.clear();
  }

  else if (plantLux < baselinePlantLux - tolerance) {
    lcd.print("Alerte");
    lcd.setCursor(0, 1);
    lcd.print("Allume moi!");
    delay(150);
    lcd.clear();
  }

  else {
    //allume l'ecran
    lcd.display();
    delay(10);

//affichage par défaut si ok
    lcd.print("Je vais");
    lcd.setCursor(0,1);   
    lcd.print("bien !");
    delay(3000);
    scrollLcdLeft(); 


                                          /* Affichage de la valeurs bornées des capteurs*/


    //affichage de l'hygrometrie sur le lcd et port serial

    SerialPrint();

    //PrintToLcd();

    //affichage du temps de lumière artificielle
      if (temps !=0){
        lcd.print("Temps de lumiere artificielle : ");
        lcd.setCursor(0,1);
        lcd.print(temps);
        lcd.print(" minutes");
        delay(1000);
        scrollLcdLeft();
        

       }
    }

                    /*commande du relais*/
  
  
  //lecture de la position Switch
  if (digitalRead(switchOn) == 1) {
    switchPos = true;
  }
  else if (digitalRead(switchOff) == 1) {
    switchPos = false;
  }
  
//commande du relais
  if (lux >= baselineLux || switchPos != true )
  {
    digitalWrite(relais1, LOW);
    relaisState = false;
    temps = 0;
  }  
  else {
    digitalWrite(relais1, HIGH);
    relaisState = true;
    temps = millis()/1000/60;
  }


}
/*Fonctions*/


// groupement des lcd print pour plus de lisibilité
void PrintToLcd() {
  lcd.print("Hygrometrie  : ");
  lcdPrint(hygro);
  lcd.print(" %");

  lcd.print("flood  : ");
  lcdPrint(flood);
  lcd.print(" %");

  lcd.print("lux  : ");
  lcdPrint(lux);
  lcd.print(" %");

  lcd.print("plantLux  : ");
  lcdPrint(plantLux);
  lcd.print(" %");

  lcd.print("temperature ");
  lcdPrint(temp);
  lcd.print(" C");

  lcd.print("SwitchPos : ");
  lcdPrint(switchPos);

  //eteindre l'écran
  lcd.noDisplay();
 

}
 //fonction lcd print
void lcdPrint(int sensorName) {
  lcd.setCursor(0, 1);
  lcd.print(sensorName);
  delay(1500);


  scrollLcdLeft();

}
//fonction scrollLcdRight
void scrollLcdRight(){for (int positionCounter = 13; positionCounter > 13; positionCounter--) {
    // scroll one position left:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }
  
  lcd.clear();
  }

//fonction scrollLcdLeft
void scrollLcdLeft(){for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }
  
  lcd.clear();
  }
////////////////////////////////////////////////////////////////////////
///////////////////////////////DEBUG////////////////////////////////////
////////////////////////////////////////////////////////////////////////


// groupement des Serial Print en fonction pour plus de lisibilité 
void PrintFlood() {
  //Serial.println("FLOOD ALERT");
  //Serial.println("");
}

void SerialPrint() {
  Serial.print("Hygrometrie : ");
  Serial.print(hygro);
  Serial.println("%");
  Serial.print("flood : ");
  Serial.print(flood);
  Serial.println("%");
  Serial.print("Lux : ");
  Serial.print(lux);
  Serial.println("%");
  Serial.print("plantLux : ");
  Serial.print(plantLux);
  Serial.println("%");
  Serial.print("temperature : ");
  Serial.print(temp);
  Serial.println("C");
  Serial.print("SwitchPos : ");
  Serial.println(switchPos);
  Serial.print("RelaisState : ");
  Serial.println(relaisState);
  Serial.println("");
  Serial.println("temps : ");
  Serial.print(temps);
  Serial.println("");
  
}


