
/*******************************************
Smart Shopping Cart - Tracking and Obstacle Avoidance
******************************************/
//MOTOR 1 - Right Motor
#define INA1 29 //4     
#define INB1 25 //8
#define PWM_MOTOR_1 11  // pin 5, enable pin for right motor

//MOTOR 2 - Left Motor
 
#define INA2 31 //7
#define INB2 27 //9;
#define PWM_MOTOR_2 10 // pin 6,enable pin of the left motor

//initializing velocity of motors based on distance
int velocityRight;    //variables that hold speed output for motor
int velocityLeft;

int check;

//final velocity of motors after considering direction
int MOTOR1;
int MOTOR2;

int maximum = 255; 
int minimum = 150;
int maxDiff=24; //Determines how sharp the turns are. The higher the less sharp the turns
int fillerSpeed=0;


int minDist =230;
int maxDist =500;

int jerkCount=0;
int jerk=0;

#define sensor0 A0 // Sharp IR GP2Y0A41SK0F
  float volts0;
  float frontDistance;
  int forwardSpace=30;


#define sensor1 A1 // Sharp IR GP2Y0A41SK0F
float rightDistance;
float volts1;


#define sensor2 A2 // Sharp IR GP2Y0A41SK0F 
float leftDistance;
float volts2;

int sideSpace=30;

int chill = 100;

/*Receivers*/
const int RX_trigPin1 = 5; //ultrasonic trigger 2 on Cart
const int RX_pingPin1 = 3; //ultrasonic receiver 2 (echo) on Cart

const int RX_trigPin2 = 6; //ultrasonic trigger 3 on Cart
const int RX_pingPin2 = 2; //ultrasonic receiver 3 (echo) on Cart

const int ledPin = 13;   //indication status led

const int distanceTimeout = 275;  

long difference = 0;
boolean direction = 0;
boolean finished = false;
boolean oneFire = false;
boolean twoFire= false;
long setPoint1 = 0;
long setPoint2 = 0;
boolean debug = true;
long distance = 0;

long counter;
long differenceSum, differenceAvg;
long distanceSum, distanceAvg;

int beeper=33;
int number=3;

int correct;

void setup()
{
  pinMode(INA1,OUTPUT);
  pinMode(INA2,OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  
  pinMode(INA2,OUTPUT);
  pinMode(INB2,OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

//MOTORS SET-UP TO BE ALWAYS IN FORWARD MODE
  digitalWrite(INA1,LOW);                 //these inputs determine foward movement or reverse. This is for foward
  digitalWrite(INB1,HIGH);
  digitalWrite(INA2,LOW);
  digitalWrite(INB2,HIGH);

  
  pinMode(13, 1);                       //This is just pin 13 on the Arduino
  pinMode(ledPin, OUTPUT);              //Connected to pin 4, is an indicator

 // Set-up of the 2 serial ports for xbee and the debugging/testing monitor serial port
  Serial.begin(9600); //Serial port for monitor
  Serial1.begin(9600); //Serial port for xbee

  pinMode(13, OUTPUT); 

  pinMode(beeper,OUTPUT); //BEEPER is an output

  delay(3000); //delay for initialization of automated billing
  
  beep();

}

void loop()
{
  // establish variables for duration of the ping, 
        long counter;
        long differenceSum, differenceAvg;
        long distanceSum, distanceAvg;
        
        differenceSum = 0;
        differenceAvg = 0;
        counter = 0;
        
        distanceSum = 0;
        distanceAvg = 0;
          
        for (int i = 0; i < 4; i++){  //An average of 4 ping cycles are used to determine distance and direction of tracker
          long tempInt = doPingDiff();
 
        
        if (abs(tempInt) < 100){
            differenceSum = differenceSum + tempInt;
            counter++;  
          }
          distanceSum = distanceSum + distance;
            
        delay (20);
        } 
        
       if (counter != 0){
          differenceAvg = differenceSum/counter;
          distanceAvg = distanceSum/counter;
        }
       
        else{
          differenceSum = 0;
          distanceAvg = 0;
        } 
   
        
        Serial.print ("Avg Distance = ");
        Serial.println (distanceAvg);      
        Serial.print ("Distance Difference = ");
        Serial.println (differenceAvg);


    checkObstacles();
    
    if (frontDistance<forwardSpace)               //that is there is an obstacles within range
    {
        for (int beepCount=0; beepCount<1; beepCount++)
        {
        digitalWrite (beeper, HIGH);
        delay(40);  
        digitalWrite (beeper, LOW);
        delay(600); 
        }
    }

    if ((rightDistance<sideSpace)  || (leftDistance<sideSpace))              //that is there is an obstacles within range
    {
        for (int beepCount=0; beepCount<1; beepCount++)
        {
        digitalWrite (beeper, HIGH);
        delay(40);  
        digitalWrite (beeper, LOW);
        delay(300); 
        }
    }
    
    
    if (distanceAvg>230 && frontDistance>forwardSpace){               //that is there is significant enough distance from tracker and no obstacles within range  
        Serial.print("Foward: ");   
        //correct=15;
        if(jerkCount==0){
            MOTOR1=jerk;   //scale foward velocity between max & min motor operation speed
            MOTOR2=jerk;
            updateMotors();
            Serial.println ("jerk");
            jerkCount=1;
 //         delay(1000);
            }

     /*     velocityRight=map(distanceAvg,minDist,maxDist,minimum,maximum);   //scale foward velocity between max & min motor operation speed
            velocityLeft=map(distanceAvg,minDist,maxDist,minimum,maximum);    //scale foward velocity between max & min motor operation speed*/
   
            velocityRight=220; //map(distanceAvg,minDist,maxDist,minimum,maximum);
            velocityLeft=220; //map(distanceAvg,minDist,maxDist,minimum,maximum);
            
            //DIRECTION CONTROL -----if (-ve) left, if (+ve) right)

            differenceAvg=differenceAvg/3;  //Scaling the difference average for sharper turns to be achieved

                      if (differenceAvg > 0){                   //that is tracker is towards the left
                          MOTOR2 = map(differenceAvg,0,maxDiff/3,velocityLeft,0);    //(Speed of left wheel lowered proportionally btwn current its original speed to 0, corresponding to the difference)
                          MOTOR1=velocityRight;                                      //Speed maintained thus will turn to left
                          Serial.print(" Left turn: ");  
                          check=2;                                                   //counter, just in case the tracker ping becomes only visible to the left receiver, and tracker was initially towards the left
      
                          }
                      else if (differenceAvg < 0){                  //that is move to left
                          
                          MOTOR1 = map(differenceAvg,0,-maxDiff/3,velocityRight,0);  // (Speed of right wheel lowered proportionally btwn current speed to 0, corresponding to the difference)
                          MOTOR2=velocityLeft;                                       //Speed maintained thus will turn to right
                          Serial.print(" Right turn: ");
                          check=1;                                                   //counter, just in case the tracker ping becomes only visible to the right receiver, and tracker was initially towards the right
 
                          }
                       else if (differenceAvg == 0 && check == 1){                  //if ping can only be received by right receiver, an was initially towards the right
                          MOTOR2 = velocityLeft;
                          MOTOR1 = 0;                                               //(Speed of left wheel lowered to 0), for sharp right turn
                          Serial.print(" Can't see left");  
       
                          }
                          
                      else if (differenceAvg == 0 && check == 2){                  //if ping can only be received by right receiver, an was initially towards the right
                          MOTOR1 = velocityRight;
                          MOTOR2 = 0;                                               //(Speed of left wheel lowered to 0), for sharp left turn
                          Serial.print(" Can't see right");  
       
                          }                         
                      //If the right sensor is the only one that receives or  the left sensor  is the only one that receives, then the average speed will be low, the cart will turn either right or left respectively, slowly
                       else{                                       //that is no acceleration
                            MOTOR1=velocityRight;
                            MOTOR2=velocityLeft;                                  //set wheel velocity to 0
                       }

                
                      if (rightDistance <  sideSpace){                   //that is obstacle towards the right
                          MOTOR2 = MOTOR2/4;    //(Speed of left wheel lowered, cart turns left)
                          Serial.print(" Left turn: ");        
                          }
                      if (leftDistance <  sideSpace){                   //that is obstacle towards the left
                          MOTOR1 = MOTOR1/4;                            //(Speed of right wheel lowered, cart turns left)
                          Serial.print(" Right turn: ");        
                          }
    }


      else{
            MOTOR1=0;
            MOTOR2=0;
            jerkCount=0;
            //correct=0;
            Serial.println ("End jerk");       
      }

      updateMotors();       
              
      Serial.print("Left Wheel - ");                        //prints out the speed of the right and left wheel
      Serial.print(MOTOR2);
      Serial.print(" rpm         Right Wheel - ");
      Serial.print(MOTOR1);
      Serial.println(" rpm");
      Serial.println ("----------------------------------------------");

      delay (chill);      //Delay before the next 4 ping cycles
     }



//Initiating a single ultrasonic ping cycle
long doPingDiff(){
  
  // The PING is triggered by a HIGH pulse of 2 or more microseconds.
  // Giving a short LOW pulse beforehand to ensure a clean HIGH pulse:
  // The echo pins are set HIGH as well, at the same time the trigger is activated to initiate the counting cycle
  long timeout = 0;
  long duration = 0;
  boolean started = false;
  twoFire=0;
  oneFire=0;
  
  difference = 0;
  
  digitalWrite(13, HIGH);
  pingCommand();  //Command to initiate ultrasonic transmitter via radio


  pinMode(RX_trigPin1, OUTPUT);
  pinMode(RX_trigPin2, OUTPUT);
   
  digitalWrite(RX_trigPin1, LOW);
  digitalWrite(RX_trigPin2, LOW);
  delayMicroseconds(2);
  
  digitalWrite(RX_trigPin1, HIGH);
  digitalWrite(RX_trigPin2, HIGH);
  delayMicroseconds(15);
  
  digitalWrite(RX_trigPin1, LOW);
  digitalWrite(RX_trigPin2, LOW);


  pinMode(RX_pingPin1, INPUT); 
  pinMode(RX_pingPin2, INPUT); 
  delayMicroseconds(600);
  
  oneFire = false;
  twoFire = false;
  finished = false;
  setPoint1 = 0;
  setPoint2 = 0;
  difference = 0;
  
  while ((oneFire == 0)|| (twoFire == 0)){
    if (((PINE & 0x10) == 0) && !oneFire){
      oneFire = true;
      setPoint1 = timeout;
    }
    if (((PINE & 0x20) == 0) && !twoFire){
      twoFire = true;
      setPoint2 = timeout;
    }
    
    delayMicroseconds(1);
    timeout++;
    if (timeout > 19000)
      break;
  }

  difference = (setPoint1 - setPoint2);
  distance = (setPoint1 + setPoint2)/10;
    
  digitalWrite(13, LOW);
        
  return difference;
}
    
void pingCommand() {
  
  setRemoteState(0x4);    //0x5 is a high, PNP INVERSE
  delayMicroseconds (10);
  setRemoteState(0x5);    //0x4 is a LOW, PNP inverse
}

//XBee communication set-up
void setRemoteState(char value) {
    Serial1.write(0x7E); //start byte
    Serial1.write((byte)0x0); //high part of length (always zero)
    Serial1.write(0x10); //low part of length (the no. of bytes that follow, not including check sum
    Serial1.write(0x17); //remote AT command
    Serial1.write((byte)0x0); //frame id set to zero for no reply
    //ID of recipient, or use 0xFFFF for broadcast.
    Serial1.write((byte)0x00);
    Serial1.write((byte)0x00); 
    Serial1.write((byte)0x00); 
    Serial1.write((byte)0x00);
    Serial1.write((byte)0x00); 
    Serial1.write((byte)0x00);
    Serial1.write(0xFF); //for broadcast
    Serial1.write(0xFF); //for broadcast

    //16 bit of recipient or 0xFFFE if unknown
    Serial1.write(0xFF);
    Serial1.write(0xFF);

    Serial1.write(0x02);                    //0x02 to apply changes immediately on the remote

    //command name in ASCII character
    Serial1.write('D');
    Serial1.write('3');
    
    //command data in as many bytes as needed
    Serial1.write(value);
    long sum = 0x17 + 0xFF + 0xFF + 0xFF + 0xFF + 0X02 + 'D' + '3' + value;

    Serial1.write(0xFF - (sum & 0xFF) );      //calculates the proper checksum
   }


void checkObstacles() {
  
volts0 = analogRead(sensor0)*0.0048828125;    // value from sensor * (5/1024)
frontDistance = 13*pow(volts0, -1);           // worked out from datasheet graph

  Serial.print("Front Distance = ");
  Serial.print(frontDistance);
  Serial.println(" cm");

volts1 = analogRead(sensor1)*0.0048828125;   // value from sensor * (5/1024)
rightDistance = 13*pow(volts1, -1);          // worked out from datasheet graph

  Serial.print("Right Distance = ");
  Serial.print(rightDistance);
  Serial.println(" cm");

volts2 = analogRead(sensor2)*0.0048828125;  // value from sensor * (5/1024)
leftDistance = 13*pow(volts2, -1);          // worked out from datasheet graph

  Serial.print("Left Distance = ");
  Serial.print(leftDistance);
  Serial.println(" cm");
  }


void updateMotors(){
  analogWrite(PWM_MOTOR_2,MOTOR2);         //write wheel velocity to enable pin
  analogWrite(PWM_MOTOR_1-35,MOTOR1);
}

void beep ()
{
  for (int beepCount=0; beepCount<number; beepCount++)
  {
  digitalWrite (beeper, HIGH);
  delay(100);  
  digitalWrite (beeper, LOW);
  delay(100); 
  }

}


