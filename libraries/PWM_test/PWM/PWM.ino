#define pulse_period 10
#define activeperiod 5
#define delayperiod 5

int sensorPin1 = 2;                              // One of the sensors
int sensorPin2 = 3;                              // The other sensor in the same segment   

int ledState1 = LOW;             // ledState used to set the LED

const int duty_cycle = 9;
const int active_cycle = activeperiod*1000/pulse_period; //time active
int activecount = active_cycle;

bool left = true;
bool active = true;

const int delay_cycle = delayperiod*1000/pulse_period; //time delay
int delaycount = delay_cycle;

// constants won't change:  
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //To make the " Serial << output " syntax possible
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorPin1,OUTPUT);
  pinMode(sensorPin2,OUTPUT);
}

void loop() {

    if(active){
      if(activecount){
        activecount--;
      }
      else{
        active = false;
        delaycount = delay_cycle;
      }
    }
    else{
      if(delaycount){
        delaycount--;
      }
      else{
        active = true;
        activecount = active_cycle;
        if(left){
          left=false;
        }
        else{
          left=true;
        }
      }
    }
    
    // if the LED is off turn it on and vice-versa:
    /*if (ledState1 == LOW) {
      ledState1 = HIGH;
    } else {
      ledState1 = LOW;
    }*/

    // set the LED with the ledState of the variable:
    if(active){
      if(left){
        digitalWrite(sensorPin1, HIGH);
        delay(duty_cycle);
        digitalWrite(sensorPin1, LOW);
        delay(pulse_period-duty_cycle);
      }
      else{
        digitalWrite(sensorPin2, HIGH);
        delay(duty_cycle);
        digitalWrite(sensorPin2, LOW);
        delay(pulse_period-duty_cycle);
      }
    }
    else{
      delay(pulse_period);
    }
    
}
