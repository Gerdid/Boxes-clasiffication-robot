//Libraries
#include <Servo.h>
#include <Wire.h>
#include <EEPROM.h>
//Libraries


//Servo declaration
Servo joint1;
Servo joint2;
Servo joint3;
Servo joint4;
Servo joint5;
//Servo declaration

//Robot modes declaration
boolean teach = true;
boolean demo = false;
boolean remote = false;
//Robot modes declaration

//Start position declaration
int posServo1 = 10;
int posServo2 = 115;
int posServo3 = 180;
int posServo4 = 64;
int posServo5 = 50;
//Star position declaration

int incomingByte; //a variable to read incoming serial data into
const int ledPin = 10; // the pin that the LED is attached to

int increment = 1;

int CNY70 = A0; //pin declaration of CNY70 sensor
int val;

//EEPROM memory directions
int addrBlanco=0;
int addrNegro=1;
//EEPROM memory directions

boolean I2C;  //Boolean type I2C indicator


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Serial communication at 9600 baud

  // Servo pinout declaration
  joint1.attach(10);
  joint2.attach(5);
  joint3.attach(6);
  joint4.attach(9);
  joint5.attach(11);
  //  Servo pinout declaration


  //I2C setup
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  //I2C setup

  //Sensor CNY70 as input
  pinMode(CNY70, INPUT);

}

//Method called every time an I2C message is received
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  char x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer

      if (x == 'H')
        I2C=true;
}
//Method called every time an I2C message is received

void loop() {
  // put your main code here, to run repeatedly:

  //If I2C message received execute moveMaterial()
  if(I2C==true){
    moveMaterial();
  }
  
  //Mode control cases
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    switch (incomingByte) {
      case 't':
        teach = true;
        demo = false;
        remote = false;
        break;

      case 'd':
        teach = false;
        demo = true;
        remote = false;
        break;

      case 'm':
        teach = false;
        demo = false;
        remote = true;
        break;

    //Remote control commands
      case 'h':
        increment = 10;
        break;

      case 'f':
        if (increment <= 10)
          increment = increment + 1;
        break;

      case 's':
        if (increment >= 0)
          increment = increment - 1;
        break;

      case 'q':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo1 = posServo1 - increment;
          createFrame(1, posServo1, increment);
        }
        break;

      case 'Q':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo1 = posServo1 + increment;
          createFrame(1, posServo1, increment);

        }
        break;

      case 'w':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo2 = posServo2 - increment;
          createFrame(2, posServo2, increment);

        }
        break;

      case 'W':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo2 = posServo2 + increment;
          createFrame(2, posServo2, increment);


        }
        break;
      case 'e':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo3 = posServo3 - increment;
          createFrame(3, posServo3, increment);

        }
        break;

      case 'E':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo3 = posServo3 + increment;
          createFrame(3, posServo3, increment);

        }
        break;

      case 'r':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo4 = posServo4 - increment;
          createFrame(4, posServo4, increment);

        }
        break;

      case 'R':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo4 = posServo4 + increment;
          createFrame(4, posServo4, increment);

        }
        break;

      case 'y':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo5 = posServo5 - increment;
          createFrame(5, posServo5, increment);

        }
        break;

      case 'Y':
        if (teach == true) {
          digitalWrite(ledPin, HIGH);
          posServo5 = posServo5 + increment;
          createFrame(5, posServo5, increment);

        }
        break;

      case 'L':
        if (teach == true) {
          digitalWrite(ledPin, LOW);

        }
        break;
    }

  }

  
  //Write the updated position to the corresponding servo
  joint1.write(posServo1);
  joint2.write(posServo2);
  joint3.write(posServo3);
  joint4.write(posServo4);
  joint5.write(posServo5);

  //if button demo of the controller is pressed
  //execute method demos()
  if (demo == true) {
    demos();
  }
}

//Method to create and transmit via serial the packet indicating
//the servo number and the servo position
void createFrame(int identifier, int posServo, int increment) {
  Serial.print(identifier);
  Serial.print(",");
  Serial.print(posServo);
  Serial.print(",");
  Serial.println(increment);
}


//Method to move the material to the corresponding container
void moveMaterial() {
  //First move to the initial position
  posicionInicial();
  //Pick up the material
  recogerMaterial();
  //wait .5 seconds
  delay(500);
  //Read and store the value of the CNY70 sensor
  val =leerCNY70();
  delay(500);

  //Compare the lecture with the calibrated black
  if(val>=6 and val<=35){
    int numCajasNegras=EEPROM.read(addrNegro);
    numCajasNegras=numCajasNegras+1;
    //Save the incremented value numCajasNegras to 
    //the direction addrNegro
    EEPROM.write(addrNegro,numCajasNegras);
    Serial.print("Cajas Negras: ");
    Serial.print(numCajasNegras);
    //move the box to the container
    materialNegro(); 
  }

  //Compare the lecture with the calibrated white
  if(val>=36 and val <=800){
    
    int numCajasBlancas=EEPROM.read(addrBlanco);
    numCajasBlancas=numCajasBlancas+1;
    //Save the incremented value numCajasBlancas to
    //the direction addrBlanco
    EEPROM.write(addrBlanco,numCajasBlancas);
    Serial.print("Cajas Blancas: ");
    Serial.print(numCajasBlancas);
    //move the box to the container
    materialBlanco();
    
  }
  //move to the initial position
  posicionInicial(); 
  //Finish sequence setting the value of I2C to false
  I2C=false;
}

//demos() same as moveMaterial but not storing boxes on 
//the EEPROM
void demos() {
  posicionInicial();
  recogerMaterial();
  delay(500);
  val =leerCNY70();
  delay(500);
  if(val>=6 and val<=35){
    materialNegro();
  }
  if(val>=35 and val <=800){
    materialBlanco();
  }
  //posicionInicial(); 
  
}

void posicionInicial() {
  //Declarations of:
  //The delay between angle increment (del)
  //The delay between servo movement (delBetweenServo)
  //Angles desired (angle1, angle2...)
  //The actual angles
  int del = 30;
  int delBetweenServo = 800;
  int angle1 = 10;
  int angle2 = 120;//115
  int angle3 = 180;
  int angle4 = 64;
  int angle5 = 50;
  int actualAngle1 = joint1.read();
  int actualAngle2 = joint2.read();
  int actualAngle3 = joint3.read();
  int actualAngle4 = joint4.read();
  int actualAngle5 = joint5.read();

  //----------First Come First Served (FCFS)---------//
  //
 // Selection function: the process that has been waiting
 //the longest in the ready queue.
 //Decision mode: non pre-emptive (cooperative).
 //-a process run until it block itself
 //Each ready process joints the ready queue.
 //When the current process ceases to execute,
 //the process in the ready queue the longest
 //is selected
            
            //{arrivalTime,servoNumber} 
  int scheduling[5][2]={{1,1},
                        {2,2},
                        {3,3},
                        {4,4},
                        {5,5}
  };
  //The arrivalTime value is symobolic, the servo
  //number refers to the movement the servo has to do 
  //in order to achieve the desired position 

  //Call the FCFS method and send the array to schedule 
  FCFS(scheduling,5);

for(int i=0;i<5;i++){
  if(scheduling[i][1]==1)
    moveServo(joint1, actualAngle1, angle1, del, delBetweenServo);
  if(scheduling[i][1]==2)
      moveServo(joint2, actualAngle2, angle2, del, delBetweenServo);
  if(scheduling[i][1]==3)
      moveServo(joint3, actualAngle3, angle3, del, delBetweenServo);
  if(scheduling[i][1]==4)
      moveServo(joint4, actualAngle4, angle4, del, delBetweenServo);
  if(scheduling[i][1]==5)
      moveServo(joint5, actualAngle5, angle5, del, delBetweenServo);
                            
}
}


void preLiberacion() {
  int del = 15;
  int delBetweenServo = 200;
  int angle1 = 10;
  int angle2 = 115;
  int angle3 = 180;
  int angle4 = 161;
  int angle5 = 0;
  int actualAngle1 = joint1.read();
  int actualAngle2 = joint2.read();
  int actualAngle3 = joint3.read();
  int actualAngle4 = joint4.read();
  int actualAngle5 = joint5.read();
                        //{arrivalTime,servoNumber}
  int scheduling[5][2]={{1,2},
                        {2,1},
                        {3,3},
                        {4,4},
                        {5,5}
  };
  FCFS(scheduling,5);

for(int i=0;i<5;i++){
  if(scheduling[i][1]==1)
    moveServo(joint1, actualAngle1, angle1, del, delBetweenServo);
  if(scheduling[i][1]==2)
      moveServo(joint2, actualAngle2, angle2, del, delBetweenServo);
  if(scheduling[i][1]==3)
      moveServo(joint3, actualAngle3, angle3, del, delBetweenServo);
  if(scheduling[i][1]==4)
      moveServo(joint4, actualAngle4, angle4, del, delBetweenServo);
  if(scheduling[i][1]==5)
      moveServo(joint5, actualAngle5, angle5, del, delBetweenServo);
                            
}
}

//Method to pick up the material
void recogerMaterial(){
  int del = 60;
  int delBetweenServo = 200;
  int angle1 = 10;
  int angle2 = 90;
  int angle3 = 165;//150
  int angle4 = 64;
  int angle5 = 0;
  int actualAngle1 = joint1.read();
  int actualAngle2 = joint2.read();
  int actualAngle3 = joint3.read();
  int actualAngle4 = joint4.read();
  int actualAngle5 = joint5.read();  
                        //{arrivalTime,servoNumber}
  int scheduling[5][2]={{1,1},
                        {2,3},
                        {3,2},
                        {4,4},
                        {5,5}
  };

  FCFS(scheduling,5);
  
for(int i=0;i<5;i++){
  if(scheduling[i][1]==1)
    moveServo(joint1, actualAngle1, angle1, del, delBetweenServo);
  if(scheduling[i][1]==2)
      moveServo(joint2, actualAngle2, angle2, del, delBetweenServo);
  if(scheduling[i][1]==3)
      moveServo(joint3, actualAngle3, angle3, del, delBetweenServo);
  if(scheduling[i][1]==4)
      moveServo(joint4, actualAngle4, angle4, del, delBetweenServo);
  if(scheduling[i][1]==5)
      moveServo(joint5, actualAngle5, angle5, del, delBetweenServo);
                            
}
}


//Move the material to the container
void materialNegro(){
  int del = 15;
  int delBetweenServo = 500;
  int angle1 = 102;
  int angle2 = 135;//125
  int angle3 = 160;
  int angle4 = 161;
  int angle5 = 0;
  int actualAngle1 = joint1.read();
  int actualAngle2 = joint2.read();
  int actualAngle3 = joint3.read();
  int actualAngle4 = joint4.read();
  int actualAngle5 = joint5.read();  
                        //{arrivalTime,servoNumber}
  int scheduling[5][2]={{1,2},
                        {2,4},
                        {3,5},
                        {4,3},
                        {5,1}
  };

  FCFS(scheduling,5);
  
for(int i=0;i<5;i++){
  if(scheduling[i][1]==1)
    moveServo(joint1, actualAngle1, angle1, del, delBetweenServo);
  if(scheduling[i][1]==2)
      moveServo(joint2, actualAngle2, angle2, del, delBetweenServo);
  if(scheduling[i][1]==3)
      moveServo(joint3, actualAngle3, angle3, del, delBetweenServo);
  if(scheduling[i][1]==4)
      moveServo(joint4, actualAngle4, angle4, del, delBetweenServo);
  if(scheduling[i][1]==5)
      moveServo(joint5, actualAngle5, angle5, del, delBetweenServo);
                            
}
  openGripper();
}

void materialBlanco(){
  int del = 15;
  int delBetweenServo = 500;
  int angle1 = 140;
  int angle2 = 125;
  int angle3 = 140;
  int angle4 = 161;
  int angle5 = 0;
  int actualAngle1 = joint1.read();
  int actualAngle2 = joint2.read();
  int actualAngle3 = joint3.read();
  int actualAngle4 = joint4.read();
  int actualAngle5 = joint5.read();  
                        //{arrivalTime,servoNumber}
  int scheduling[5][2]={{1,4},
                        {2,3},
                        {3,5},
                        {4,2},
                        {5,1}
  };

  FCFS(scheduling,5);
  
for(int i=0;i<5;i++){
  if(scheduling[i][1]==1)
    moveServo(joint1, actualAngle1, angle1, del, delBetweenServo);
  if(scheduling[i][1]==2)
      moveServo(joint2, actualAngle2, angle2, del, delBetweenServo);
  if(scheduling[i][1]==3)
      moveServo(joint3, actualAngle3, angle3, del, delBetweenServo);
  if(scheduling[i][1]==4)
      moveServo(joint4, actualAngle4, angle4, del, delBetweenServo);
  if(scheduling[i][1]==5)
      moveServo(joint5, actualAngle5, angle5, del, delBetweenServo);
                            
}
  openGripper();
  
}

void openGripper(){
  int del = 15;
  int delBetweenServo = 250;

  int angle2 = 115;
  int angle3 = 180;
  int angle4 = 64;
  int angle5 = 50;
  
  int actualAngle2 = joint2.read();
  int actualAngle3 = joint3.read();
  int actualAngle4 = joint4.read();
  int actualAngle5 = joint5.read();
    
                        //{arrivalTime,servoNumber}
  int scheduling[4][2]={{1,2},
                        {2,3},
                        {3,4},
                        {4,5}
  };

  FCFS(scheduling,4);
  
for(int i=0;i<4;i++){
  /*
  if(scheduling[i][1]==1)
    moveServo(joint1, actualAngle1, angle1, del, delBetweenServo);
    */
  if(scheduling[i][1]==2)
      moveServo(joint2, actualAngle2, angle2, del, delBetweenServo);
  if(scheduling[i][1]==3)
      moveServo(joint3, actualAngle3, angle3, del, delBetweenServo);
      
  if(scheduling[i][1]==4)
      moveServo(joint4, actualAngle4, angle4, del, delBetweenServo);
      
  if(scheduling[i][1]==5)
      moveServo(joint5, actualAngle5, angle5, del, delBetweenServo);
                            
}

}


//Method to move the servo to the desired angle
void moveServo(Servo servo, int actualAngle, int angle, int del, int delBetweenServo) {
  if (actualAngle < angle) {
    for (int pos = actualAngle; pos <= angle; pos += 1) {
      servo.write(pos);
      delay(del);
    }
  }
  if (actualAngle > angle) {
    for (int pos = actualAngle; pos >= angle; pos -= 1) {
      servo.write(pos);
      delay(del);
    }
  }
  delay(delBetweenServo);
}

//Reads the analog value of the sensor
int leerCNY70(){

  int val=analogRead(CNY70);
  return val;
}


//First Come First Served Method
//Drawback
//-A short process may have to wait a very long time before it
//can execute.
//-A process that does not perform any I/O will monopolize the processor

void FCFS(int a [][2], int size){
  for(int i=0; i<(size-1);i++){
    for(int o=0;o<(size-(i+1));o++){
      if(a[o][0]>a[o+1][0]){
        int t=a[o][0];
        int j=a[o][1];
        a[o][0]=a[o+1][0];
        a[o][1]=a[o+1][1];
        a[o+1][0]=t;
        a[o+1][1]=j;
      }
    }
  }
}



