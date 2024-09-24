#include<SoftwareSerial.h>
#include<TinyGPS++.h>

TinyGPSPlus gps;

int temp=0,i;
SoftwareSerial sgps(4,3);
SoftwareSerial sgsm(6,7);
String stringVal = "";

void setup(){
  sgsm.begin(9600);
  sgps.begin(9600);
  temp=0;
}

void serialEvent()
{
  while(sgsm.available()>0)
  { 
    if(sgsm.find("Track Bag"))
    {
      temp=1;
      break;
    }
	
	else if(sgsm.find("Complaint"))
    {
      temp=2;
      break;
    }
    else
    {
    temp=0;
    }
  }
}

void init_sms()
{
  sgsm.println("AT+CMGF=1");
  delay(400);
  sgsm.println("AT+CMGS=\"+ABXXXXXXXXX\""); // mobile number of the user  
  delay(400);
}

void init_sms_complaint()
{
  sgsm.println("AT+CMGF=1");
  delay(400);
  sgsm.println("AT+CMGS=\"+ABXXXXXXXXX\"");   // mobile number of the person who should be alerted
  delay(400);
}

void send_data(String message)
{
  sgsm.print(message);
 delay(200);
}

void send_sms()
{
  sgsm.write(26);
}

void tracking(){
	init_sms();
	send_data("Your bag's location:");
	sgsm.println(" "); 
    sgsm.print("Latitude: ");
    sgsm.print(gps.location.lat(), 6);
    sgsm.print("\n Longitude: ");
    sgsm.println(gps.location.lng(), 6);
	 sgsm.print("https://www.google.com/maps/@");
   sgsm.print(gps.location.lat(), 6);
   sgsm.print(',');
   sgsm.print(gps.location.lng(), 6);
   sgsm.print(",14z");
   send_sms();
}

void complaint(){
	init_sms_complaint();
	send_data("Sir, I lost my bag could you please look at it");
	 sgsm.println(" "); 
    sgsm.print("Latitude: ");
    sgsm.print(gps.location.lat(), 6);
    sgsm.print("\n Longitude: ");
    sgsm.println(gps.location.lng(), 6);
	 sgsm.print("https://www.google.com/maps/@");
    sgsm.print(gps.location.lat(), 6);
    sgsm.print(',');
    sgsm.print(gps.location.lng(), 6);
    sgsm.print(",14z");
    send_sms();
}

void loop(){
	serialEvent();
	if(temp==1){
		while(sgps.available()>0){
			gps.encode(sgps.read());
			if(gps.location.isUpdated()){
				tracking();
			}
		}
	}
	if(temp==2){
		while(sgps.available()>0){
			gps.encode(sgps.read());
			if(gps.location.isUpdated()){
				complaint();
			}
		}
	}
}