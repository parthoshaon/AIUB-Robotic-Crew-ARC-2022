//Created by Khaliduzzaman Mredul && Mahbubul Hasan Akash//

/*L_L -> Here L_ is the side of the car L is the direction of wheel rotation L=Left, R=Right*/

const byte L_L = 13;
const byte R_L = 12;
const byte L_R = 11;
const byte R_R = 10;

const byte h1[] = {50, 34};
const byte h2[] = {42, 26};
const byte h3[] = {46, 30};
const byte h4[] = {38, 22};
const byte h5[] = {48, 32};
const byte h6[] = {40, 24};

byte pwm;

void setup() {
  Serial.begin(9600);
  pinMode(L_L, OUTPUT);
  pinMode(R_L, OUTPUT);
  pinMode(R_R, OUTPUT);
  pinMode(L_R, OUTPUT);


  pinMode(h1[0], OUTPUT);
  pinMode(h1[1], OUTPUT);

  pinMode(h2[0], OUTPUT);
  pinMode(h2[1], OUTPUT);

  pinMode(h3[0], OUTPUT);
  pinMode(h3[1], OUTPUT);

  pinMode(h4[0], OUTPUT);
  pinMode(h4[1], OUTPUT);

  pinMode(h5[0], OUTPUT);
  pinMode(h5[1], OUTPUT);

  pinMode(h6[0], OUTPUT);
  pinMode(h6[1], OUTPUT);

  pwm = 70;

}

void loop() {
  while (Serial.available() == 0);


  digitalWrite(h1[1], LOW);
  digitalWrite(h1[2], LOW);
  digitalWrite(h2[1], LOW);
  digitalWrite(h2[2], LOW);
  digitalWrite(h3[1], LOW);
  digitalWrite(h3[2], LOW);
  digitalWrite(h4[1], LOW);
  digitalWrite(h4[2], LOW);
  digitalWrite(h5[1], LOW);
  digitalWrite(h5[2], LOW);
  digitalWrite(h6[1], LOW);
  digitalWrite(h6[2], LOW);


  char val = Serial.read();
  Serial.println(val);
  switch (val) {

    //Wheel instructions

    case '0':
      pwm = 25;
      break;
    case '1':
      pwm = 65;
      break;
    case '2':
      pwm = 100;
      break;
    case '3':
      pwm = 150;
      break;
    case '4':
      pwm = 200;
      break;
    case '5':
      pwm = 255;
      break;


    case 'w': //drive

    
      analogWrite(L_L, 0);
      analogWrite(R_L, pwm); //forward
      analogWrite(L_R, pwm);
      analogWrite(R_R, 0);
      

      break;
      
    case 's':
      analogWrite(L_L, pwm);
      analogWrite(R_L, 0);  //backward
      analogWrite(L_R, 0);
      analogWrite(R_R, pwm);
      
      break;
      
    case 'a':
    
      analogWrite(L_L, pwm);
      analogWrite(R_L, 0);
      analogWrite(L_R, pwm); // left
      analogWrite(R_R, 0);
      break;
      
    case 'd':
      analogWrite(L_L, 0);
      analogWrite(R_L, pwm); // right
      analogWrite(L_R, 0);
      analogWrite(R_R, pwm);
      break;
      
    case 'b':
      analogWrite(L_L, 0);
      analogWrite(R_L, 0);
      analogWrite(L_R, 0);
      analogWrite(R_R, 0);
      break;
      
    case 'z':
      analogWrite(L_L, 0 );
      analogWrite(R_L, 0);
      analogWrite(L_R, pwm);
      analogWrite(R_R, pwm);
      delay(5000); //forword
      analogWrite(L_L, 0 );
      analogWrite(R_L, pwm);
      analogWrite(L_R, pwm);
      analogWrite(R_R, 0);
      delay(4000);  //right
      break;
    case 'x':
      pwm = 65;
      analogWrite(L_L, 0 );
      analogWrite(R_L, 0);
      analogWrite(L_R, 0);
      analogWrite(R_R, 0);
      delay(1000);  //halt

      analogWrite(L_L, pwm);
      analogWrite(R_L, 0);
      analogWrite(L_R, 0);
      analogWrite(R_R, pwm);
      delay(2000);  //right

      analogWrite(L_L, pwm);
      analogWrite(R_L, 0);
      analogWrite(L_R, pwm);
      analogWrite(R_R, 0);
      delay(2000);  //forward

      analogWrite(L_L, pwm );
      analogWrite(R_L, 0);
      analogWrite(L_R, 0);
      analogWrite(R_R, pwm);
      delay(2000); //left

      analogWrite(L_R, pwm);
      analogWrite(R_R, pwm);
      analogWrite(R_L, 0);
      analogWrite(L_L, 0);
      delay(3000); //forward

      analogWrite(L_L, pwm );
      analogWrite(R_L, 0);
      analogWrite(L_R, 0);
      analogWrite(R_R, pwm);
      delay(2000); //left

      analogWrite(L_R, pwm);
      analogWrite(R_R, pwm);
      analogWrite(R_L, 0);
      analogWrite(L_L, 0);
      delay(2000);  //forward

      analogWrite(L_L, 0 );
      analogWrite(R_L, pwm);
      analogWrite(L_R, pwm);
      analogWrite(R_R, 0);
      delay(2000);  //right

      analogWrite(L_R, pwm);
      analogWrite(R_R, pwm);
      analogWrite(R_L, 0);
      analogWrite(L_L, 0);
      delay(5000);  //forward

      analogWrite(L_R, 0);
      analogWrite(R_R, 0);
      analogWrite(R_L, 0);
      analogWrite(L_L, 0);
      break;

    //ARM instructions

    case'r':
      digitalWrite(h6[0], HIGH); // base motor left
      digitalWrite(h6[1], LOW);
      break;
    case'f':
      digitalWrite(h6[0], LOW);
      digitalWrite(h6[1], HIGH); // base motor right
      break;
    case't':
      digitalWrite(h3[0], LOW);
      digitalWrite(h3[1], HIGH);  // 1st accu up
      break;
    case'g':
      digitalWrite(h3[0], HIGH);
      digitalWrite(h3[1], LOW); // 1st accu down
      break;

    case'y':
      digitalWrite(h5[0], LOW); // 2nd accu up
      digitalWrite(h5[1], HIGH);
      break;
    case'h':
      digitalWrite(h5[0], HIGH); // 2nd accu down
      digitalWrite(h5[1], LOW);
      break;
    case'u':
      digitalWrite(h2[0], LOW); // claw_up / 3rd accu up
      digitalWrite(h2[1], HIGH);
      break;
    case'j':
      digitalWrite(h2[0], HIGH); // claw_down/3rd accu down
      digitalWrite(h2[1], LOW);
      break;
    case'i':
      digitalWrite(h4[0], HIGH); // claw close
      digitalWrite(h4[1], LOW);
      break;
    case'k':
      digitalWrite(h4[0], LOW); // claw open
      digitalWrite(h4[1], HIGH);
      break;
    case 'o':
      digitalWrite(h1[0], HIGH); // claw_right_rotate
      digitalWrite(h1[1], LOW);
      break;
    case 'l':
      digitalWrite(h1[0], LOW);  // claw_left_rotate
      digitalWrite(h1[1], HIGH);
      break;
    case'v':   // halt hand motors
      for (byte i = 0 ; i < 2 ;  ++i)
      {
        digitalWrite(h1[i], LOW);
        digitalWrite(h2[i], LOW);
        digitalWrite(h3[i], LOW);
        digitalWrite(h4[i], LOW);
        digitalWrite(h5[i], LOW);
        digitalWrite(h6[i], LOW);
      }
      break;
  }
}
