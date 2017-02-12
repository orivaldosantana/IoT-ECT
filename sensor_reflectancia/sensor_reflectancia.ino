#define S_LUZ D8
#define LED D15


void setup() {
  pinMode(S_LUZ,INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
 if ( digitalRead(S_LUZ) == 0 )
    digitalWrite(LED, HIGH);
 else
    digitalWrite(LED, LOW);

}
