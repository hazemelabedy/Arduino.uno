void(* resetFunc) (void) = 0;
#include <SoftwareSerial.h>
#include <Sim800l.h>

 SoftwareSerial gprsSerial(2, 3); // GSM Module Connected Here TX , RX


String textForSMS1;
String textForSMS11;
String textForSMS2;
String textForSMS21;
String textSms, numberSms;
String textSms1, numberSms1;
char incomingByte;
String inputString;
int i = 0;
int hh=0;

//NES-18 = NES-2  , NES-22 = NES-4  , DNES-1X = NES-6
//NES-18
bool data1_Nes2 ;
bool data2_Nes2 ;
bool con1  ;
bool data2 ;
float pi2;
float pd2;
#define Nes2  5 // LED connected to D5 "With 5 Volt MAX"
#define pi22  A0
#define pd22  A1

//NES-22
bool data1_Nes4 ;
bool data2_Nes4 ;
bool con4 ;
bool data4 ;
float pi4;
float pd4;
#define Nes4  6  // LED connected to D6 "With 5 Volt MAX"
#define pi44  A2
#define pd44  A3

// DNes-1X
bool data1_Nes6 ;
bool data2_Nes6 ;
bool con6  ;
bool data6 ;
float pi6;
float pd6;
#define Nes6  7 // LED connected to D7 "With 5 Volt MAX"

// cell numbers to which you want to send the security alert message
String f1001 = "+201221557256";

void setup()
{
      randomSeed(analogRead(0));
  Serial.begin(9600);
  gprsSerial.begin(9600);               // the GPRS baud rate   
 Serial.println(" logging time completed!");
  pinMode(Nes2, INPUT_PULLUP);
  pinMode(Nes4, INPUT_PULLUP);
  pinMode(Nes6, INPUT_PULLUP);
  pinMode(pi22, INPUT); // set pin A0 as Input
  pinMode(pd22, INPUT); // set pin A1 as Input
  pinMode(pi44, INPUT); // set pin A2 as Input
  pinMode(pd44, INPUT); // set pin A3 as Input
  pinMode(13, OUTPUT); // set pin D13 as OUTPUT  for Buzzer
  
} 
void loop()
{    

   int j=0;
  // Check if you're currently connected to SIM800L "will Resrat after 90 Sec"
  while (!gprsSerial.available()) {
    gprsSerial.println("AT");
    delay(1000);
    Serial.println("connecting....");
    digitalWrite (13,HIGH);
    delay(100);
    digitalWrite (13,LOW);
    delay(500);
    j=j+1;
    Serial.println(j);
 if (j > 60 ) 
 {    Serial.println("Restarting Arduino");
  resetFunc();
 }
 else {};

  }
    Serial.println("Connected..");
    digitalWrite (13,LOW);
   // Check if you're currently connected to SIM800L
  while (!gprsSerial.available()) {
    gprsSerial.println("AT");
    delay(1000);
    Serial.println("connecting....");
        digitalWrite (13,HIGH);
    delay(100);
    digitalWrite (13,LOW);
    delay(500);
  }
  digitalWrite (13,LOW);
  //////////////////// Digital Read 1 //////////////////
 data1_Nes2 = digitalRead(Nes2);
 data1_Nes4 = digitalRead(Nes4);
 data1_Nes6 = digitalRead(Nes6);
 
if ( data1_Nes2 == 1 )  con1=0;
else  con1=1;
if ( data1_Nes4 == 1 )  con4=0;
else  con4=1;
if ( data1_Nes6 == 1 )  con6=0;
else  con6=1;
  i= i+1 ;
   Serial.println(i);
   
                         //////////////////// Digital Read //////////////////
 float pi222=0 ;
 float pd222=0 ;
 float pi444=0 ;
 float pd444=0 ;

           pi222 = analogRead(pi22);
           pd222 = analogRead(pd22);
           pi444 = analogRead(pi44);
           pd444 = analogRead(pd44);
// --------------------------------------------------------------------------------------------------------------------
  // Convert the analog reading (which goes from 180 - 1900) to a pressure (0 - 3000) psi :
  
if (pi222 > 180 & pi222 < 900 ) pi2=4.17*(pi222-180.0);
else  pi2 = 0;
if (pd222 > 180 & pd222 < 900 ) pd2=4.17*(pd222-180.0);
else  pd2 = 0;
if (pi444 > 180 & pi444 < 900 ) pi4=4.17*(pi444-180.0);
else  pi4 = 0;
if (pd444 > 180 & pd444 < 900 ) pd4=4.17*(pd444-180.0);
else  pd4 = 0;

       ///////////////// Serial Print //////////
             Serial.print("(NES-18)  ");
              Serial.print("Pi:");
               Serial.print(pi2);
                Serial.print(" psi  , ");
                Serial.print("Pd:");
               Serial.print(pd2);
                Serial.print(" psi  , ");
                Serial.print("on/off: ");
               Serial.print(con1);
                Serial.println();
                  Serial.print("(NES-22)  ");
              Serial.print("Pi:");
               Serial.print(pi4);
                Serial.print(" psi  , ");
                Serial.print("Pd:");
               Serial.print(pd4);
                Serial.print(" psi  , ");
                Serial.print("on/off: ");
               Serial.println(con4);
               Serial.print("(DNES-1X)  ");
                Serial.print("on/off: ");
               Serial.println(con6);
                
               String str ="";
if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  //gprsSerial.println("AT+CPIN?");
  //delay(1000);
 
 
  gprsSerial.println("AT+CGATT=1");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
    gprsSerial.println("AT+CMODE=0");
  delay(1000);
 
  gprsSerial.println("AT+CSTT=\"orangeweb\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(7000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(6000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(4000);
 
  ShowSerialData();
  
    gprsSerial.println("AT+CIPSPRT=1");
  delay(3000);
  ShowSerialData();
   
  //  ---------------------------  TRANSMIT DATA TO THINKSPEAK IOT --------------   data1_Nes6
     str="GET https://api.thingspeak.com/update?api_key=BJLEZ6H27OJYVQOQ&field1=" + String(pi2) +"&field2="+String(pd2)+"&field3="+String(con1)+"&field4="+String(pi4)+"&field5="+String(pd4)+"&field6="+String(con4)+"&field7="+String(data1_Nes6);
      delay(6000);
          Serial.println(str);
   gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(5000);
  ShowSerialData();
  gprsSerial.println(str);//begin send data to remote server
  delay(4000);
  ShowSerialData();
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 

     ////////////////////////////////////// if cipsend ok /////////////
  if (sendATcommand2("AT+CIPSEND", ">", "ERROR", 3000) == 1)

  {/// sending buzzer ////////
    digitalWrite (13,HIGH);
    delay(1000);
    digitalWrite (13,LOW);
    delay(100);
    digitalWrite (13,HIGH);
    delay(1000);
    digitalWrite (13,LOW);
    delay(100);
    digitalWrite (13,HIGH);
    delay(1000);
    digitalWrite (13,LOW);
    
    Serial.print("Error sending");
    hh=hh+1;
    Serial.print("No:");
     Serial.print(hh);
    if (hh>4){
    delay(1000);
    Serial.println("Restarting GSM");
    gprsSerial.println("AT+CFUN=0");
    delay(500);
    gprsSerial.println("AT+CFUN=1");
        delay(40000);
    Serial.println("Restarting ARDUINO");
    sendATcommand2("AT+CIPSHUT", "OK", "ERROR", 10000);
   }
  else
  {
      /////////////////////////// Sending Buzzer ///////////
    digitalWrite (13,HIGH);
    delay(100);
    digitalWrite (13,LOW);
    delay(100);
    digitalWrite (13,HIGH);
    delay(100);
    digitalWrite (13,LOW);
  } }
    else {
      }
     //////////////////////////////////////////////
    /////////////////////////////////////////////////
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
    delay(20000);

 //////////////////// Digital Read 2 //////////////////
 data2_Nes2 = digitalRead(Nes2);
 data2_Nes4 = digitalRead(Nes4);
 data2_Nes6 = digitalRead(Nes6);
  /////////////////////////////////////// NES-18  On/Off //////////////////

  if ( data1_Nes2 == LOW & data2_Nes2 == HIGH ) //
  {
    textForSMS1 =  "NES-18 ESP Stopped";
    //sendSMS(textForSMS1);
    sendsms(textForSMS1, f1001); // you can use a variable of the type String
    Serial.println(textForSMS1);
    Serial.println("message sent.");
    delay (2100);
  }
  else if ( data1_Nes2 == HIGH & data2_Nes2 == LOW ) //
  {
    textForSMS2 =  "NES-18 ESP Switched On ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS2, f1001); // you can use a variable of the type String
    Serial.println(textForSMS2);
    Serial.println("message sent.");
  }
  else {
  }
    /////////////////////////////////////// NES-22  On/Off //////////////////

  if ( data1_Nes4 == LOW & data2_Nes4 == HIGH ) //
  {
    textForSMS1 =  "NES-22 ESP Stopped";
    //sendSMS(textForSMS1);
    sendsms(textForSMS1, f1001); // you can use a variable of the type String
    Serial.println(textForSMS1);
    Serial.println("message sent.");
     delay (2100);
  }
  else if ( data1_Nes4 == HIGH & data2_Nes4 == LOW ) //
  {
    textForSMS2 =  "NES-22 ESP Switched On ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS2, f1001); // you can use a variable of the type String
    Serial.println(textForSMS2);
    Serial.println("message sent.");
    delay (2100);
  }
  else {
  }
    /////////////////////////////////////// DNES-1X  On/Off //////////////////

  if ( data1_Nes6 == LOW & data2_Nes6 == HIGH ) //
  {
    textForSMS1 =  "DNES-1 ESP Stopped";
    //sendSMS(textForSMS1);
    sendsms(textForSMS1, f1001); // you can use a variable of the type String
    Serial.println(textForSMS1);
    Serial.println("message sent.");
     delay (2100);
  }
  else if ( data1_Nes6 == HIGH & data2_Nes6 == LOW ) //
  {
    textForSMS2 =  "DNES-1 ESP Switched On ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS2, f1001); // you can use a variable of the type String
    Serial.println(textForSMS2);
    Serial.println("message sent.");
    delay (2100);
  }
  else {
  }
   ///////////////////// DEFINE SMS SEND ////////////
  gprsSerial.println("AT+CMGF=1\r\n");  //Set SMS Text Mode
  gprsSerial.println("AT+CNMI=1,2,0,0,0");  //procedure, how receiving of new messages from the network
  delay(500);
  //gprsSerial.println("AT+CMGL=\"REC UNREAD\""); // Read unread messages
  //delay(1000); // wait for 5 seconds
  if (gprsSerial.available()) {
  delay(100);

      //Serial buffer
      while (gprsSerial.available()) {
        incomingByte = gprsSerial.read();
        inputString += incomingByte;
      }

      delay(10);

      Serial.println(inputString);
      inputString.toUpperCase(); // uppercase the message received
          Serial.println("SENDING CONDITION");

     ///////////////////////////////////////// NES-18 Condition /////////////////
  // print out the state of the button:
   if (inputString.indexOf("NES-18 COND") > -1)
  {
      // Pump Status
      if (data2_Nes2 == 0){
         textForSMS21 =  "NES-18 ESP is Working Good ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS21, f1001); // you can use a variable of the type String
    Serial.println(textForSMS21);
    Serial.println("message sent.");
          inputString = "";
    delay (2100);
  
  }
      // Pump Status
        else if (data2_Nes2 == 1) {
      textForSMS21 =  "NES-18 ESP is Stopped ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS21, f1001); // you can use a variable of the type String
    Serial.println(textForSMS21);
    Serial.println("message sent.");
          inputString = "";
    delay (2100);
      }
      else{
      }
      }
   ///////////////////////////////////////// NES-22 Condition /////////////////
  // print out the state of the button:
    if (inputString.indexOf("NES-22 COND") > -1)
  {
      // Pump Status
           if (data2_Nes4 == 0){
         textForSMS21 =  "NES-22 ESP is Working Good ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS21, f1001); // you can use a variable of the type String
    Serial.println(textForSMS21);
    Serial.println("message sent.");
          inputString = "";
    delay (2100);
  
  }
      // Pump Status
        else if (data2_Nes4 == 1) {
      textForSMS21 =  "NES-22 ESP is Stopped ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS21, f1001); // you can use a variable of the type String
    Serial.println(textForSMS21);
    Serial.println("message sent.");
          inputString = "";
    delay (2100);
      }
      else{
      }
      }
     ///////////////////////////////////////// DNES-1X Condition /////////////////
  // print out the state of the button:
    if (inputString.indexOf("DNES-1 COND") > -1)
  {
      // Pump Status
            if (data2_Nes6 == 0){
         textForSMS21 =  "DNES-1 ESP is Working Good ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS21, f1001); // you can use a variable of the type String
    Serial.println(textForSMS21);
    Serial.println("message sent.");
          inputString = "";
    delay (2100);
  
  }
      // Pump Status
        else if (data2_Nes6 == 1) {
      textForSMS21 =  "DNES-1 ESP is Stopped ";
    //sendSMS(textForSMS2);
    sendsms(textForSMS21, f1001); // you can use a variable of the type String
    Serial.println(textForSMS21);
    Serial.println("message sent.");
          inputString = "";
    delay (2100);
      }
      else{
      }
     }
  }
      Serial.println("End OF CODE");
//resetFunc();
}


void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000);   
}
void sendsms(String message, String number)
{
  String mnumber = "AT+CMGS=\"+201221557256\"";
  gprsSerial.print("AT+CMGF=1\r\n");
  delay(100);
  gprsSerial.println(mnumber);  // recipient's mobile number, in international format
  delay(100);
  gprsSerial.println(message);                         // message to send
  delay(100);
  gprsSerial.println((char)26);                        // End AT command with a ^Z, ASCII code 26
  delay(100);
  gprsSerial.println();
   delay(100);
  gprsSerial.println("AT+CMGDA=\"DEL ALL\"");
  delay(1000);                                     // give module time to send SMS
  // SIMl800power();
}
int8_t sendATcommand2(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout) {
  //show();

  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, "" , 100);   // Initialize the string

  delay(100);

  while ( gprsSerial.available() > 0) gprsSerial.read();   // Clean the input buffer
  //cell.flush();

  gprsSerial.println(ATcommand);
  Serial.println(ATcommand); // Send the AT command


  x = 0;
  previous = millis();

  // this loop waits for the answer
  do {
    // if there are data in the UART input buffer, reads it and checks for the asnwer
    if (gprsSerial.available() != 0) {
      response[x] = gprsSerial.read();
      x++;
      // check if the desired answer 1  is in the response of the module
      if (strstr(response, expected_answer1) != NULL)
      {
        answer = 1;
      }
      // check if the desired answer 2 is in the response of the module
      else if (strstr(response, expected_answer2) != NULL)
      {
        answer = 2;
      }
    }
  }

  // Waits for the asnwer with time out
  while ((answer == 0) && ((millis() - previous) < timeout));
  //Serial.println(response);

  return answer;
}
