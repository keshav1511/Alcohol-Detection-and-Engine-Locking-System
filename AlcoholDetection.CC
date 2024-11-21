#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

SoftwareSerial gpsSerial(11, 12); 
SoftwareSerial gsmSerial(3, 2); 
TinyGPSPlus gps; 

void setup() 
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(1, INPUT);
  pinMode(A0, INPUT);

  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);

  Serial.println("System Initialized.");
}

void loop() 
{
  int alcoholAnalogValue = analogRead(A0);
  int alcoholDigitalValue = digitalRead(1);

  if (alcoholAnalogValue > 500 || alcoholDigitalValue == HIGH) {
    Serial.println("Alcohol Detected!");

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);

    String locationMessage = getLocation();
    sendSMS(locationMessage);

    while (1);
  }

  while (gpsSerial.available() > 0) 
{
    gps.encode(gpsSerial.read());
  }
}

String getLocation()		// Function to get GPS Location
{
  String location = "";
  if (gps.location.isValid()) 
{
    location = "Latitude: " + String(gps.location.lat(), 6) + "\nLongitude: " + String(gps.location.lng(), 6);
  } 
else 
{
    location = "Unable to fetch location!";
  }
  Serial.println("Location: " + location);
  return location;
}

void sendSMS(String location) 	//Function to send SMS
{
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+918971040800\"");
  delay(1000);
  gsmSerial.print("Alcohol Detected!!\n");
  gsmSerial.print("Engine Locked for safety Purpose!\n");
  gsmSerial.print("Location:\n");
  gsmSerial.print(location);
  delay(1000);
  gsmSerial.write(26);
  delay(5000);
  Serial.println("SMS Sent.");
}
