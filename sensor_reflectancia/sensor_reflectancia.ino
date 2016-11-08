#define S_LUZ 8
#define LED 13


void setup() {
  // put your setup code here, to run once:
  pinMode(S_LUZ,INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 if ( digitalRead(S_LUZ) == 0 )
    digitalWrite(LED, HIGH);
 else
    digitalWrite(LED, LOW);
 
}
