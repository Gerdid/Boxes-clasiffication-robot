import processing.serial.*; 

Serial myPort;    // The serial port

int[] inString= {0, 0, 0};  // Input string from serial port
int Servo1=0;  
int Servo2=0;
int Servo3=0;
int Servo4=0;
int Servo5=0;
int increment;

String mode;

boolean teachState=true;
boolean demoState=false;
boolean remoteState=false;

////---------------------------------------------------------------------------------------------------------------------////
class Button{        //Clase para la creacion de botón
  int xpos, ypos, wid, hei;  //Parámetros para la creación de una caja
  String label;              //Texto dentro del botón
  boolean over=false;        
  boolean down=false;  
  boolean clicked = false;
  
  Button(int tx, int ty,int tw, int th,String tlabel){
    xpos = tx;
    ypos = ty;
    wid = tw;
    hei = th;
    label = tlabel;
  }
  
 
  void update(){
    
//it is important that this comes first
    if(down&&over&&!mousePressed){
      clicked=true;
    }else{
      clicked=false;
    }
    smooth();
    fill(0);
    stroke(255);
    rect(xpos, ypos, wid, hei, 10);//draws the rectangle, the last param is the round corners
    fill(0);
    textSize(10); 
    text(label, xpos+wid/2-(textWidth(label)/2), ypos+hei/2+(textAscent()/2)); 
    //all of this just centers the text in the box
    if(mouseX>xpos&&mouseY>ypos&&mouseX<xpos+wid&&mouseY<ypos+hei){    
       over=true; 
    }else{ 
      over=false; 
    }     
    if(!over){
      fill(0);
    }else{
      fill(100);
    }
   stroke(0);
   rect(xpos, ypos, wid, hei, 10);
   
   
if(mouseX>xpos && mouseY>ypos && mouseX<xpos+wid && mouseY<ypos+hei){
      over=true;
      if(mousePressed){
        down=true;
      }else{
        down=false;
      }
    }else{
      over=false;
    }
if(!over){
      fill(73,66,58); //Color del botón desde el inicio
    }else{
      if(!down){
        fill(100);
      }else{
        fill(0);
      }
    }
    stroke(0);
    rect(xpos, ypos, wid, hei, 10);//draws the rectangle, the last param is the round corners
    if(down){
      fill(255);
    }else{    
      fill(0);
    }
    fill(222,184,103);  //Color de texto de inicio
    textSize(10); 
    text(label, xpos+wid/2-(textWidth(label)/2), ypos+hei/2+(textAscent()/2)); 
    //all of this just centers the text in the box    
    
  } 
}
////---------------------------------------------------------------------------------------------------------------------////

////---------------------------------------------------------------------------------------------------------------------////
class CircularButton{        //Clase para botones circulares
  int xpos, ypos, wid, hei;
  String label;
  boolean over=false;
  boolean down=false;
  boolean clicked = false;
  
  CircularButton(int tx, int ty,int tw, int th,String tlabel){
    xpos = tx;
    ypos = ty;
    wid = tw;
    hei = th;
    label = tlabel;
  }
  
 
  void update(){
    
//it is important that this comes first
    if(down&&over&&!mousePressed){
      clicked=true;
    }else{
      clicked=false;
    }
    smooth();
    fill(0);
    stroke(255);
    ellipse(xpos, ypos, wid, hei);//draws the rectangle, the last param is the round corners
    fill(0);
    textSize(10); 
    text(label, xpos+wid/2-(textWidth(label)/2), ypos+hei/2+(textAscent()/2)); 
    //all of this just centers the text in the box
    if(mouseX>xpos&&mouseY>ypos&&mouseX<xpos+wid&&mouseY<ypos+hei){    
       over=true; 
    }else{ 
      over=false; 
    }     
    if(!over){
      fill(0);
    }else{
      fill(100);
    }
   stroke(0);
   ellipse(xpos, ypos, wid, hei);
   
   
if(mouseX>xpos && mouseY>ypos && mouseX<xpos+wid && mouseY<ypos+hei){
      over=true;
      if(mousePressed){
        down=true;
      }else{
        down=false;
      }
    }else{
      over=false;
    }
if(!over){
      fill(236,28,36); //Color del botón desde el inicio
    }else{
      if(!down){
        fill(100);
      }else{
        fill(0);
      }
    }
    stroke(0);
    ellipse(xpos, ypos, wid, hei);//draws the rectangle, the last param is the round corners
    if(down){
      fill(255);
    }else{    
      fill(0);
    }
    fill(222,184,103);  //Color de texto de inicio
    textSize(10); 
    text(label, xpos+wid/2-(textWidth(label)/2), ypos+hei/2+(textAscent()/2)); 
    //all of this just centers the text in the box    
    
  } 
}

////---------------------------------------------------------------------------------------------------------------------////

Button b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,demo,remote,teach,servo1d,servo1i,servo2d,servo2i,servo3d,servo3i,servo4d,servo4i,
         servo5d,servo5i,servo6d,servo6i,highspeed,fast,slow,n1,n2,n3,n4,n5,n6,n7,n8,n9,n0,nx,ny,enter,info,robot,delete;
         //Botones rectanculares en controlador virtual

CircularButton emergency;//Botón circular de emergencia

void setup(){
  background(251,250,248);      //Declaración de fondo de pantalla

  myPort = new Serial(this, Serial.list()[0], 9600); //nuevo puerto serial
  myPort.bufferUntil(ENTER);
  size(650,1000);            //Tamaño del controlador
  //fill(73,66,58);
  demo = new Button(40,20,30,30,"DEMO");  
  remote=new Button(100,20,30,30,"RMT");
  teach=new Button(160,20,30,30,"TEACH");
  emergency=new CircularButton(600,50,70,70," ");
  
  b0=new Button(180,440,40,40," ");
  b1=new Button(240,440,40,40," ");
  b2=new Button(300,440,40,40," ");
  b3=new Button(360,440,40,40," ");
  b4=new Button(420,440,40,40," ");
  
  b5=new Button(180,500,40,40," ");
  b6=new Button(240,500,40,40," ");
  b7=new Button(300,500,40,40," ");
  b8=new Button(360,500,40,40," ");
  b9=new Button(420,500,40,40," ");
  
  servo1d=new Button(160,600,50,30,"SERVO1(-)");
  servo1i=new Button(230,600,50,30,"SERVO1(+)");
  servo2d=new Button(160,640,50,30,"SERVO2(-)");
  servo2i=new Button(230,640,50,30,"SERVO2(+)");
  servo3d=new Button(160,680,50,30,"SERVO3(-)");
  servo3i=new Button(230,680,50,30,"SERVO3(+)");  
  servo4d=new Button(360,600,50,30,"SERVO4(-)");
  servo4i=new Button(430,600,50,30,"SERVO4(+)");     
  servo5d=new Button(360,640,50,30,"SERVO5(-)");
  servo5i=new Button(430,640,50,30,"SERVO5(+)");          //Creación de botones
  servo6i=new Button(360,680,50,30,"SERVO6(-)");
  servo6d=new Button(430,680,50,30,"SERVO6(+)");
  highspeed=new Button(305,630,30,50,"HIGH\n SPEED");
  fast=new Button(305,690,30,30,"FAST");  
  slow=new Button(305,730,30,30,"SLOW");
  
  //b7=new Button(300,500,40,40," ");
  //b8=new Button(360,500,40,40," ");
  //b9=new Button(420,500,40,40," ");  
  
  n7=new Button(260,780,30,30,"7");
  n8=new Button(305,780,30,30,"8");                    
  n9=new Button(350,780,30,30,"9");  
  n4=new Button(260,820,30,30,"4");
  n5=new Button(305,820,30,30,"5");
  n6=new Button(350,820,30,30,"6");    
  n1=new Button(260,860,30,30,"1");
  n2=new Button(305,860,30,30,"2");
  n3=new Button(350,860,30,30,"3");   
  n0=new Button(260,900,30,30,"0");
  nx=new Button(305,900,30,30,".");
  ny=new Button(350,900,30,30,"-");     
 
  enter=new Button(390,900,60,30,"ENTER");  
  //b2 = new Button(150,60,50,50,"servo2");
  
  info=new Button(215,860,30,30,"INFO");
  robot=new Button(215,900,30,30,"ROBOT");
  
  delete=new Button(395,860,30,30,"DEL");
  
  
  fill(149,130,98);  
  rect(130,430,390,510);  //Rectángulo del fondo de botones
  
  
  fill(36,224,86);
  rect(295,600,50,20,20);  //Rectángulo verde
  
  fill(255,224,15);
  ellipse(600,50,90,90);  //Círculo amarillo
  //fill(236,28,36);
  //ellipse(600,50,70,70);  
  fill(0);
  rect(110,120,430,300);  //Rectángulo para pantalla
}











void draw(){
  background(251,250,248);
  
  fill(149,130,98);  
  rect(130,430,390,510);  //Rectángulo del fondo de botones
  
  
  fill(36,224,86);
  rect(295,600,50,20,20);  //Rectángulo verde
  
  fill(255,224,15);
  ellipse(600,50,90,90);  //Círculo amarillo
  //fill(236,28,36);
  //ellipse(600,50,70,70);  
  fill(0);
  rect(110,120,430,300);  //Rectángulo para pantalla
  
  fill(255);
  textSize(14);
  text("Servo 1:", 120, 200);
  text("Servo 2:", 120, 240);
  text("Servo 3:", 120, 280);
  text("Servo 4:", 120, 320);
  text("Servo 5:", 120, 360);
  
 
  b0.update();
  b1.update();
  b2.update();
  b3.update();
  b4.update();

  b5.update();
  b6.update();
  b7.update();
  b8.update();
  b9.update();  
  
  demo.update();
  remote.update();
  teach.update();
  emergency.update();
  
  servo1d.update();
  servo1i.update();
  servo2d.update();
  servo2i.update();
  servo3d.update();
  servo3i.update();
  servo4d.update();
  servo4i.update();
  servo5d.update();
  servo5i.update();
  servo6d.update();
  servo6i.update();
  
  highspeed.update();
  fast.update();
  slow.update();
  
  n7.update();
  n8.update();
  n9.update();
  n4.update();
  n5.update();
  n6.update();
  n1.update();
  n2.update();
  n3.update();
  n0.update();
  nx.update();
  ny.update();
  
  enter.update();
  info.update();
  robot.update();
  delete.update();
  //b.update();
  //b2.update();
  //if(b.clicked){
   // myPort.write('H');
    //delay(500);
   // myPort.write('L');

  //}
  //if(b2.clicked){
  //  myPort.write('2');
  //  //delay(500);
  //  myPort.write('L');    

  //}
  //if (inString[0]==1) {
   // Servo1=inString[1];
  //}
  //if (inString[0]==2) {
  //  Servo2=inString[1];
  //}
  
  //textSize(14);
  //text(Servo1, 60, 30);
  //text(Servo2, 60, 50);
  
  if(demo.clicked){     //Click en botón DEMO
    demoState=true;
    remoteState=false;
    teachState=false;
   myPort.write('d');
    delay(500);
    myPort.write('L');
  }  
  if(remote.clicked){     //Click en botón REMOTE
      demoState=false;
    remoteState=true;
    teachState=false;
   myPort.write('m');
    delay(500);
    myPort.write('L');
  }   
  if(teach.clicked){     //Click en botón TEACH
      demoState=false;
    remoteState=false;
    teachState=true;
   myPort.write('t');
    delay(500);
    myPort.write('L');
  }   
  if(servo1d.clicked){     //Click en botón SERVO1(-)
   myPort.write('q');
    delay(500);
    myPort.write('L');
  }  
  if(servo1i.clicked){    //Click en botón SERVO1(+)
   myPort.write('Q');
    delay(500);
    myPort.write('L');
  }
  if(servo2d.clicked){     //Click en botón SERVO2(-)
   myPort.write('w');
    delay(500);
    myPort.write('L');
  }  
  if(servo2i.clicked){    //Click en botón SERVO2(+)
   myPort.write('W');
    delay(500);
    myPort.write('L');
  }
  if(servo3d.clicked){     //Click en botón SERVO3(-)
   myPort.write('e');
    delay(500);
    myPort.write('L');
  }  
  if(servo3i.clicked){    //Click en botón SERVO3(+)
   myPort.write('E');
    delay(500);
    myPort.write('L');
  } 
  if(servo4d.clicked){     //Click en botón SERVO4(-)
   myPort.write('r');
    delay(500);
    myPort.write('L');
  }  
  if(servo4i.clicked){    //Click en botón SERVO3(+)
   myPort.write('R');
    delay(500);
    myPort.write('L');
  }
  if(servo5d.clicked){     //Click en botón SERVO5(-)
   myPort.write('y');
    delay(500);
    myPort.write('L');
  }  
  if(servo5i.clicked){    //Click en botón SERVO5(+)
   myPort.write('Y');
    delay(500);
    myPort.write('L');
  }
  if(highspeed.clicked){
    myPort.write('h');
    delay(500);
    myPort.write('L');
  }
  if(fast.clicked){
    myPort.write('f');
    delay(500);
    myPort.write('L');
  }
  if(slow.clicked){
    myPort.write('s');
    delay(500);
    myPort.write('L');
  }
  //textSize(14);
  //text(Servo1, 60, 30);
  //text(Servo2, 60, 50);
 
  fill(255);
  textSize(18);
  text("MODE: ",120,140);
  
  if(teachState==true){
    mode="TEACH";
 //    fill(0);
 // textSize(16);
 // text("DEMO",180,140);
 // text("REMOTE",180,140);
 // fill(255);
 text(mode, 180, 140);   
    
  }
  if(demoState==true){
    mode="DEMO";
 //    fill(0);
 // textSize(16);
  //text("TEACH",180,140);
  //text("REMOTE",180,140);
  //fill(255);
 text(mode, 180, 140);   
  }  
  if(remoteState==true){
    mode="REMOTE";
  //       fill(0);
  //textSize(16);
  //text("TEACH",180,140);
  //text("DEMO",180,140);
  //fill(255);
 text(mode, 180, 140);   
  
}
  if (inString[0]==1) {
     Servo1=inString[1];
  }
  if (inString[0]==2) {
    Servo2=inString[1];
  }
  if (inString[0]==3) {
     Servo3=inString[1];
  }
  if (inString[0]==4) {
    Servo4=inString[1];
  }
  if (inString[0]==5) {
     Servo5=inString[1];
  }

   //fill(0);
  textSize(14);

  fill(255);
  text(Servo1, 180, 200); 
  text(Servo2, 180, 240);
  text(Servo3,180,280);
   text(Servo4,180,320);
  text(Servo5,180,360);
  
 increment=inString[2];
  
 text(increment,120,170); 
 textSize(12);
 text("degrees/click",140,170);
  
  

}

void serialEvent(Serial p) { 
  inString =int(trim(split( p.readString(), ',')));
  redraw=true;
}
