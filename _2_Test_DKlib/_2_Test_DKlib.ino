#include <DeltaKinematics.h>
#include <AccelStepper.h>

#define POS_SECU 290
#define COEFF 0.1767

#define ES1 A5
#define ES2 7
#define ES3 6
#define EM 13

#define FULLSTEP 4

AccelStepper m1(FULLSTEP, A0, A2, A1, A3);
AccelStepper m2(FULLSTEP, 2,4,8,5);
AccelStepper m3(FULLSTEP, 9, 11, 10, 12);

DeltaKinematics DK(24,170,16,47);

void m(int s1, int s2, int s3)
{
  m1.moveTo(s1);
  m2.moveTo(s2);
  m3.moveTo(s3);
}

void mAll(int s)
{
  m(s,s,s);
}

void autoHome()
{
  
  m1.setMaxSpeed(1000.0);
  m1.setAcceleration(50.0);
  m1.setSpeed(-100);
  //m1.moveTo(2038);
  //m1.moveTo(-m1.currentPosition());
  m1.moveTo(-2000);
  
  m2.setMaxSpeed(1000.0);
  m2.setAcceleration(50.0);
  m2.setSpeed(200);
  //m2.moveTo(2038);
  m2.moveTo(-2000);
  
  m3.setMaxSpeed(1000.0);
  m3.setAcceleration(50.0);
  m3.setSpeed(200);
  //m3.moveTo(2038);
  //m3.moveTo(-m3.currentPosition());
  m3.moveTo(-2000);
  
  while(!digitalRead(ES1))
  {
    m1.run();
    
  }
  m1.stop();
  delay(200);
  m1.setCurrentPosition(0);
  m1.moveTo(POS_SECU);
    while(!digitalRead(ES2))
  {
    m1.run();
    m2.run();
    
  }
  m2.stop();
  m2.setCurrentPosition(0);
  m2.moveTo(POS_SECU);
    while(!digitalRead(ES3))
  {
    m1.run();
    m3.run();
    m2.run();
    
  }
  m3.stop();
  m3.setCurrentPosition(0);
  m3.moveTo(POS_SECU);
  
  int t = millis();
  while (millis()-t < 5000)
  {
    m1.run();
    m3.run();
    m2.run();
  }  
  
  m1.setCurrentPosition(0);
  m2.setCurrentPosition(0);  
  m3.setCurrentPosition(0);
}

void setup() 
{  
  Serial.begin(115200);
  Serial.println("Hello");
  pinMode(EM, OUTPUT);
  pinMode(ES1, INPUT);
  pinMode(ES2, INPUT);
  pinMode(ES3, INPUT);
  
  autoHome();
  
}

int motors()
{
  Serial.print(DK.x);
  Serial.print(",");
  Serial.print(DK.y);
  Serial.print(",");
  Serial.println(DK.z);
  Serial.print(DK.a);
  Serial.print(",");
  Serial.print(DK.b);
  Serial.print(",");
  Serial.println(DK.c);
  Serial.println();
  
  int s1 = DK.a/COEFF;
  int s2 = DK.b/COEFF;
  int s3 = DK.c/COEFF;
  
  if(s1 < -POS_SECU || s2 < -POS_SECU || s3 < -POS_SECU)
  {
    Serial.println("ERROR: Point outside of the  workplace");
    return 0;
    
  }
  
  m1.moveTo(s1);
  m2.moveTo(s2);
  m3.moveTo(s3);
  
  return 1;
}

void loop() 
{
  
  if (Serial.available() )
  {
    //Si un message a été reçu  faire ceci
    long sX = Serial.parseInt();
    Serial.print("X: ");
    Serial.println(sX);  // Lit le message reçu et l'affiche sur le moniteur
    
    while(!Serial.available()){}
    long sY = Serial.parseInt();
    Serial.print("Y: ");
    Serial.println(sY);  // Lit le message reçu et l'affiche sur le moniteur
    
    
    while(!Serial.available()){}
    long sZ = Serial.parseInt();
    Serial.print("Z: ");
    Serial.println(-sZ);  // Lit le message reçu et l'affiche sur le moniteur
    
    DK.inverse(sX,sY,-sZ);
    motors();
    
  }
  else
  {
    // Si rien n'est reçu faire cela
    m1.run();
    m2.run();
    m3.run();
  }
}
