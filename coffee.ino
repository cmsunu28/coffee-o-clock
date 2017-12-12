// coffee o clock
// this coffee grinder grinds your coffee super slowly and silently over 24 hours
// limiting you to one cup a day

// SET YOUR OWN TIME ZONE-- the number is the UTC offset
// (Example: Eastern time is UTC-5, so the number is -5)
int timezone=-8;

// SET THE HOUR YOU WANT THE GRINDER TO GO OFF using 24 hr format
// This is a weekly calendar-- shows the hour scheduled for M,T,W,Th,F,S,S
int coffeeHour[7]={7,7,7,7,7,9,9}; // in 24 hour format

// SET HOW LONG YOU WANT THE GRINDER TO GRIND EACH DAY
// note: it's wayyyyy slower than a regular grinder.
// This is a weekly calendar-- shows the minutes of grinding scheduled for M,T,W,Th,F,S,S
// If you are using google calendar instead, you can set every number list to 0
int grindTimeList[7]={5,5,5,4,4,4,3};  // in minutes


// No need to change these:
int mosfet=D6;
int hour;

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

  // set time zone
  Time.zone(timezone);

  // get current hour
  hour=Time.hour();
  Serial.print(hour);
  setGrindOff();
}

void loop(){
    
    if (grindState==0) {
        noGrind();
    }
    
    else if (grindState==2) {
        grinding();
    }
    
}

int getGrindTime() {
    // get day of week
    int g=grindTimeList[Time.weekday()-1];
    g=g*1000*60;
    return g;
}

int grindCoffee(String command) {
  char inputStr[64];
  command.toCharArray(inputStr,64);
  int grindTime=atoi(inputStr);
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

void setGrindOn() {
    timeGrindOn=getGrindTime();
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
        Particle.publish("coffee-on", NULL, 60, PRIVATE);
        grindState=1;
    }
}