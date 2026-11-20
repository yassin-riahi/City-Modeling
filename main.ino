#include <Grove_I2C_Motor_Driver.h>
#include <SharpIR.h>

#define I2C_ADDRESS 0x0f

// ---- Réglages ----
const int SEUIL_ARRET  = 20; // cm : arrêt (doit rester > 10 cm)
const int SEUIL_REPART = 25; // cm : repart
const int VITESSE      = 50; // de -100 à 100

// Capteur Sharp GP2Y0A21YK0F sur le port A0 du Base Shield
SharpIR sharp(SharpIR::GP2Y0A21YK0F, A0);

bool arrete = false;          // état actuel du robot
unsigned long tDetection = 0; // instant de la détection (ms)

// Lit la distance deux fois pour écarter une mesure parasite
int lireDistance() {
  int d1 = sharp.getDistance();
  int d2 = sharp.getDistance();
  return max(d1, d2); // on garde la mesure la plus grande :  2 mesures proches pour s'arrêter
}

// Fait avancer les 2 moteurs simultanement à la même vitesse
void avancer() {
  Motor.speed(MOTOR1, VITESSE);
  Motor.speed(MOTOR2, VITESSE);
}

// Stoppe les 2 moteurs simultanement
void stopper() {
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
}

void setup() {
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  delay(500);
  avancer(); // la voiture avance
}

// boucle de detection d'obstacle
void loop() {
  int d = lireDistance(); // lit la distance entre la voiture et un obstacle

  if (!arrete && d < SEUIL_ARRET) { // si la voiture n'est pas à l'arrêt et que la distance avec un obstacle est inférieur au seuil d'arrêt
    stopper(); // voiture s'arrête
    arrete = true;
  }

  if(arrete && d > SEUIL_REPART) { // si la voiture est à l'arrêt et que la distance avec un obstacle est supérieur au seuil de repart
    avancer(); // voiture repart
    arrete = false;
  }
}