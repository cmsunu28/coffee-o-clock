// coffee o clock
// this coffee grinder grinds your coffee super slowly and silently over 24 hours
// limiting you to one cup a day

int mosfet=D6;
int coffeeHour=7; // in 24 hour format
int hour;

int grindTime=60*1000*5;  // in ms

int grindState=-1;
//0: don't grind
//1: get ready to grind
//2: grinding
//3: finished grinding, returning to regular state
//-1: boot state

int timeGrindOn;
int timeAtGrindStart;
int timeBetweenGrinds;
int timeAtGrindOff;

void setup() {
  pinMode(mosfet,OUTPUT);
  Particle.function("grind", grindCoffee);
  Particle.function("toggle", coffeeToggle);

  // get current hour
  hour=Time.hour();
  Serial.print(hour);
  setGrindOff();
}

void setGrindOn() {
    timeGrindOn=60000;
    timeAtGrindStart=millis();
    coffeeToggle("");
    grindState=2;
}

void grinding() {
    if (millis()>timeAtGrindStart+timeGrindOn) {
        setGrindOff();
        grindState=3;
    }
}

void setGrindOff() {
    int m=Time.minute();
    timeBetweenGrinds=(60-m)*60*1000;
    timeAtGrindOff=millis();
    if (grindState==3) {coffeeToggle("");}
    grindState=0;
}

void noGrind() {
    if (millis()>timeAtGrindOff+timeBetweenGrinds) {
        setGrindOn();
        grindState=1;
    }
}

void loop(){

    if (grindState==0) {
        noGrind();
    }

    else if (grindState==2) {
        grinding();
    }

}

int grindCoffee(String command) {
  coffeeToggle("");
  delay(grindTime);
  coffeeToggle("");
  return 0;
}

int coffeeToggle(String command) {
  digitalWrite(mosfet,LOW);
  delay(500);
  digitalWrite(mosfet,HIGH);
  delay(500);
  digitalWrite(mosfet,LOW);
  return 0;
}
