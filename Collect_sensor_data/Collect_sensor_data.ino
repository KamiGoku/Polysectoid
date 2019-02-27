
int sensorPin1 = A0;                              // One of the sensors
int sensorPin2 = A1;                              // The other sensor in the same segment   

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //To make the " Serial << output " syntax possible
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int s_val = analogRead(sensorPin1);
  Serial << s_val << '\n';
  delay(1);
}
