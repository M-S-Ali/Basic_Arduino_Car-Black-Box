#include <SoftwareSerial.h>

#include <SPI.h>
#include <SD.h>

SoftwareSerial mySerial(9, 6);

File myFile;

#define sms_pin A3
int sms_flag=0;

#define a1 7
#define b1 4
#define a2 3
#define b2 2 
#define en1 8
#define en2 5
char a;

#define achl_snsr A0
#define buz_achl A4

#define co_snsr A1

#define door_snsr A2

void setup()
{
  mySerial.begin(9600); 
  Serial.begin(9600);
  pinMode(sms_pin,INPUT);

  pinMode(a1,OUTPUT);
  pinMode(b1,OUTPUT);
  pinMode(a2,OUTPUT);
  pinMode(a2,OUTPUT);
  pinMode(en1,OUTPUT);
  pinMode(en2,OUTPUT);
  analogWrite(en1,100);
  analogWrite(en2,100);

  pinMode(achl_snsr,INPUT);
  pinMode(buz_achl,OUTPUT);

  pinMode(co_snsr,INPUT);

  pinMode(door_snsr,INPUT);
  
  while (!Serial) {}
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) 
  {
    Serial.println("initialization failed!");
    while (1);
  }
  
  Serial.println("initialization done.");
  
}

void loop()
{
  if (analogRead(achl_snsr) < 700 && analogRead(door_snsr) > 900)
  {
    Serial.println("Vehicle Turned ON");
    gsm();
    bluetooth();
    Serial.print("CO2 level = ");
    Serial.println(analogRead(co_snsr));
  }
  else
  {
    Serial.println(analogRead(achl_snsr));
    Serial.println("Vehicle Turned Down");
    digitalWrite(buz_achl,HIGH);
    delay(2000);
    digitalWrite(buz_achl,LOW);
    if (analogRead(achl_snsr) > 700)
    {
      Serial.println("Alchohol Detected");
    }
    if (analogRead(door_snsr) < 900)
    {
      Serial.println("Door Not Locked");
    }
  }
}


//*****************************************************************


void gsm()
{
    if(analogRead(sms_pin)>1000 && sms_flag==0)
  {
    Serial.println("Sending SMS.....");
    mySerial.println("AT+CMGF=1");
    delay(1000);
    mySerial.println("AT+CMGS=\"<phone number here>\"\r");
    delay(1000);
    mySerial.println("Alert!!!!");
    delay(100);
    mySerial.println((char)26);
    sms_flag=1;
    delay(3000);
    Serial.println("SMS Send Successful");

    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile)
    {
      Serial.println("Writing to CarBlackbox.txt...");
      myFile.println("!!!!!!ALERT!!!!!");
      myFile.print("CO2 level : ");
      myFile.println(analogRead(co_snsr));
      myFile.print("Alcohol level : ");
      myFile.println(analogRead(achl_snsr));
      myFile.close();
      Serial.println("done writing to car blackbox");
    }
    else
    {
      Serial.println("error opening Carblackbox.txt");
    }
 }
  if(analogRead(sms_pin)<1000 && sms_flag==1)
  {
    sms_flag=0;
    Serial.println("SMS System Reset");
  }
}


//*****************************************************************


void bluetooth()
{
   if (Serial.available())
   {
      a= Serial.read();
      Serial.println(a);
      if (a=='F')
      {
        digitalWrite(a1,HIGH);
        digitalWrite(b1,LOW);
        digitalWrite(a2,HIGH);
        digitalWrite(b2,LOW);
      }
      else if (a=='B')
      {
        digitalWrite(a1,LOW);
        digitalWrite(b1,HIGH);
        digitalWrite(a2,LOW);
        digitalWrite(b2,HIGH);
      }
      else if (a=='R')
      {
        digitalWrite(a1,HIGH);
        digitalWrite(b1,LOW);
        digitalWrite(a2,LOW);
        digitalWrite(b2,HIGH);;
      }
      else if (a=='L')
      {
        digitalWrite(a1,LOW);
        digitalWrite(b1,HIGH);
        digitalWrite(a2,HIGH);
        digitalWrite(b2,LOW);
      }
      else if (a=='U')
      {
        digitalWrite(a1,LOW);
        digitalWrite(b1,LOW);
        digitalWrite(a2,LOW);
        digitalWrite(b2,LOW);
      }
   }
}
