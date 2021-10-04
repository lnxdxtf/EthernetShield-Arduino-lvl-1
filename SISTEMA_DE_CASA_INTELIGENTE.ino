#include <Ethernet.h>
#include "thermistor.h"
 
int pinNTC = A2;
float temperatura;
THERMISTOR thermistor(pinNTC, 10000, 3950, 10000);


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177 };
     
EthernetServer servidor(80);
  
String readString;
#define luz A0
#define buzzer 8

void setup(){
  pinMode(buzzer,OUTPUT);
  pinMode(luz, INPUT);
  pinMode(6, OUTPUT);
  
  Ethernet.begin(mac, ip);
  servidor.begin();

  digitalWrite(6,HIGH);
  tone(buzzer,1000);
  delay(500);
  digitalWrite(6,LOW);
  noTone(buzzer);
  delay(500);
  digitalWrite(6,HIGH);
  tone(buzzer,1500);
  delay(300);
  digitalWrite(6,LOW);
  noTone(buzzer);
  delay(300);
  digitalWrite(6,HIGH);
  tone(buzzer,1400);
  delay(100);
  digitalWrite(6,LOW);
  noTone(buzzer);
  delay(100);
  digitalWrite(6,HIGH);
}
  
void loop(){
  int luz = analogRead(A0);
  temperatura = thermistor.read();
 
  EthernetClient cliente = servidor.available();
  if (cliente) {
    
    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();
  
        if (readString.length() < 100) {
          readString += c;             
        }
 
        if (c == '\n') {
          cliente.println("HTTP/1.1 200 OK");
          cliente.println("Content-Type: text/html");
          cliente.println("Connection: close");
          cliente.println("Refresh: 2");
          
          cliente.println();
           
          cliente.println("<HTML>");
          cliente.println("<HEAD><TITLE> CASA </TITLE> </HEAD>");
          cliente.println("<BODY>");
          cliente.println("<H1>CASA</H1>");
          cliente.println("<hr />");
          cliente.println("<br />");
          cliente.println("<p>Luz Quarto 1 <a href=\"/?luzon\"><button>LIGA</button></a><a href=\"/?luzoff\"><button>DESLIGA</button></a>");
          cliente.println("<hr />");
          cliente.println("<br />");
          cliente.println("<p>ALARME <a href=\"/?alarmeon\"><button>LIGA</button></a><a href=\"/?alarmeoff\"><button>DESLIGA</button></a>");
          cliente.println("<br /> ATENCAO! SE O ALARME FOR ATIVADO, SO SERA POSSIVEL DESATIVA-LO REINICIANDO O SISTEMA.");
          cliente.println("<hr />");
          cliente.println("<br />");
          cliente.print("<p>Intensidade da luz: ");
          cliente.println(luz);
          if (luz>=500){
            cliente.println("<p> CLARO!");
            cliente.println("<br />");
         }
         if (luz<200){
            cliente.println("<p> ESCURO!");
            cliente.println("<br />");
         }
          cliente.println("<hr />");
          cliente.println("<br />");
          cliente.print("<p>Temperatura do ambiente: ");
          cliente.print(-temperatura);
          cliente.println(" Graus Celsius");
          cliente.println("<hr />");
          cliente.println("<br />");
          cliente.println("</BODY>");
          cliente.println("</HTML>");
         
          delay(1);
          cliente.stop();
          
          if(readString.indexOf("?luzoff")>0 ){
            digitalWrite(6, HIGH);
            cliente.println("Refresh: 1");
          }
          
          if(readString.indexOf("?luzon")>0){
            digitalWrite(6,LOW);
            cliente.println("Refresh: 1");
          }

           
          while(readString.indexOf("?alarmeon")>0){
            
            digitalWrite(6,LOW);
            tone(buzzer,1500);
            delay(500);
            noTone(buzzer);
            digitalWrite(6,HIGH);
            delay(500);
            cliente.println("Refresh: 1");
           
          }
          
          readString="";

          
        }
      }   
    }
  }
}
