#include <TinyGPS.h>  
#include <Wire.h>
#include <MechaQMC5883.h>

const byte L_L = 13;
const byte R_L = 12;
const byte L_R = 11;
const byte R_R = 10;

MechaQMC5883 QMC;
TinyGPS GPS;

#define mo1 5
#define mo2 4
#define mo3 6
#define mo4 7
#define TOTAL_COORDINATE 5   // number of waypoints

float GPS_HEADING=0;
int COMPASS_HEADING;

int HEADING_DIFF=0;
int COORDINATE=1;

float BOT_LATITUDE = 0, BOT_LONGITUDE = 0;
float BOT_LATITUDE_TEMP, BOT_LONGITUDE_TEMP;
float DEST_LATITUDE_1, DEST_LONGITUDE_1;
float DIF_LATITUDE, DIF_LONGITUDE, DISTANCE_CALC, DISTANCE_CALC_TEMP;
float TOTAL_DISTANCE;

float DEST_LATITUDE_2=28.34140014;   //waypoint 1    //28.324603,-82.267419 home testing
float DEST_LONGITUDE_2=-82.27369689;
float DEST_LATITUDE_3=28.3415508;        //waypoint 2
float DEST_LONGITUDE_3=-82.2736816;
float DEST_LATITUDE_4=28.3415794;       //waypoint 3
float DEST_LONGITUDE_4=-82.2741317;
float DEST_LATITUDE_5=28.3414192;     // waypoint   4
float DEST_LONGITUDE_5=-82.2741470;
float DEST_LATITUDE_6 = 28.3413600;
float DEST_LONGITUDE_6 =-82.2739028;

void gps_run(TinyGPS &GPS);
bool gps_availability();
void print_decimal(double f, int digits = 2);

void setup()
{
      Wire.begin();
      Serial.begin(115200);
      Serial1.begin(9600);
      
      pinMode(mo1, OUTPUT);
      pinMode(mo2, OUTPUT);
      pinMode(mo3, OUTPUT);
      pinMode(mo4, OUTPUT);
      
      QMC.init();


      //Motor
      pinMode(L_L, OUTPUT);
      pinMode(R_L, OUTPUT);
      pinMode(R_R, OUTPUT);
      pinMode(L_R, OUTPUT);

      //pwm = 20;
}

void loop()
{ //-------------------------------------------- this is all gps stuff. This is just the example code given from tiny gps library.
//----------------------------------------------I deleted the part of the code for the time and date. I did not want it.


      //while(Serial.available() == 0);
      //char val = Serial.read();
      //Serial.println(val);
     

      
      bool newdata = false;
      unsigned long start = millis();
      while (millis() - start < 250)            // get new data every 1/4 of a second
      {
          if (gps_availability())
              newdata = true;
      }
      
      if (newdata)
      {
          //Serial.println("GPS and Compass connected");
          //Serial.println("-------------");
          gps_run(GPS);
          //Serial.println("-------------");
          //Serial.println();
      }

      /*
      else if(!newdata)
      {
          Serial.println("Waiting for GPS connection...");
          while (!gps_availability())
              delay(250);
      }
      */
}

void print_decimal(double number, int digits)
{
      // Handle negative numbers
      if (number < 0.0)
      {
         Serial.print('-');
         number = -number;
      }
    
      // Round correctly so that print(1.999, 2) prints as "2.00"
      double rounding = 0.5;
      for (uint8_t i=0; i<digits; ++i)
        rounding /= 10.0;
      
      number += rounding;
    
      // Extract the integer part of the number and print it
      unsigned long int_part = (unsigned long)number;
      double remainder = number - (double)int_part;
      Serial.print(int_part);
    
      // Print the decimal point, but only if there are digits beyond
      if (digits > 0) Serial.print("."); 
      // Extract digits from the remainder one at a time
      while (digits-- > 0)
      {
          remainder *= 10.0;
          int toPrint = int(remainder);
          Serial.print(toPrint);
          remainder -= toPrint; 
      } 
}

void gps_run(TinyGPS &GPS)
{
      long lat, lon;
    
      unsigned long age, date, time, chars;
    
      unsigned short sentences, failed;
    
      
      gps_availability(); // If we don't feed the gps during this long routine, we may drop characters and get checksum errors
    
      GPS.f_get_position(&BOT_LATITUDE, &BOT_LONGITUDE, &age);
      Serial.print("Latitude: "); print_decimal(BOT_LATITUDE, 7); Serial.print(", "); Serial.print("longitude: "); print_decimal(BOT_LONGITUDE, 7);   // print our current posistion
      //Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");
    
      gps_availability();
      Serial.println();
      Serial.print("Altitude(cm): "); Serial.print(GPS.altitude()); Serial.print(" Speed(10^-2 knots): "); Serial.println(GPS.speed());
      Serial.print("Altitude(float): "); print_decimal(GPS.f_altitude());  Serial.println();
      Serial.print("Speed(knots): "); print_decimal(GPS.f_speed_knots()); Serial.print(" (mph): ");  print_decimal(GPS.f_speed_mph());
      Serial.print(" (mps): "); print_decimal(GPS.f_speed_mps()); Serial.print(" (kmph): "); print_decimal(GPS.f_speed_kmph()); Serial.println();
      float speedy=GPS.f_speed_knots();
      gps_availability();
    
      //GPS.stats(&chars, &sentences, &failed);
      //Serial.print("Stats: characters: "); Serial.print(chars); Serial.print(" sentences: "); Serial.print(sentences); Serial.print(" failed checksum: "); Serial.println(failed);
      distance();
      Serial.println("----------------------------------------------");
}
  
bool gps_availability()
{
      while (Serial1.available())
      {
          if (GPS.encode(Serial1.read()))
              return true;
      }
      return false;
}
//------------------------------below is the part of the code where everything is calculated
void distance()
{
      if(COORDINATE==1)
      {
          DEST_LATITUDE_1 = DEST_LATITUDE_2;      // setting x2lat and x2lon equal to our first waypoint
          DEST_LONGITUDE_1 = DEST_LONGITUDE_2;   
      }
      if(COORDINATE==2)
      {
          DEST_LATITUDE_1 = DEST_LATITUDE_3;
          DEST_LONGITUDE_1 = DEST_LONGITUDE_3;
      }
      if(COORDINATE==3)
      {
          DEST_LATITUDE_1 = DEST_LATITUDE_4;
          DEST_LONGITUDE_1 = DEST_LONGITUDE_4;
      }
      if(COORDINATE==4)
      {
          DEST_LATITUDE_1 = DEST_LATITUDE_5;
          DEST_LONGITUDE_1 = DEST_LONGITUDE_5;
      }
      if(COORDINATE==5)
      {
          DEST_LATITUDE_1 = DEST_LATITUDE_6;
          DEST_LONGITUDE_1 = DEST_LONGITUDE_6;
      }
 
      BOT_LATITUDE_TEMP = BOT_LATITUDE;
      BOT_LONGITUDE_TEMP = BOT_LONGITUDE;
      
      //23.8226280, longitude: 90.4276580

      DEST_LATITUDE_1 = 23.823047;   // Destination Latitude
      DEST_LONGITUDE_1 = 90.427688;    // Destination  Longitude

      //23.82277395, 90.42730991
    
      DIF_LATITUDE=radians(DEST_LATITUDE_1-BOT_LATITUDE_TEMP);  
      BOT_LATITUDE_TEMP=radians(BOT_LATITUDE_TEMP);
      DEST_LATITUDE_1=radians(DEST_LATITUDE_1);
      DIF_LONGITUDE=radians((DEST_LONGITUDE_1)-(BOT_LONGITUDE_TEMP));
      
      DISTANCE_CALC = (sin(DIF_LATITUDE/2.0)*sin(DIF_LATITUDE/2.0));
      DISTANCE_CALC_TEMP = cos(BOT_LATITUDE_TEMP);
      DISTANCE_CALC_TEMP *= cos(DEST_LATITUDE_1);
      DISTANCE_CALC_TEMP *= sin(DIF_LONGITUDE/2.0);                                       
      DISTANCE_CALC_TEMP *= sin(DIF_LONGITUDE/2.0);
      DISTANCE_CALC += DISTANCE_CALC_TEMP;
      DISTANCE_CALC = (2*atan2(sqrt(DISTANCE_CALC),sqrt(1.0-DISTANCE_CALC)));
      DISTANCE_CALC *= 6371000.0;                     //Converting to meters

      Serial.println("");
      




//-----------------------------------------heading formula below. Calculates heading to the waypoint from the current locaiton
      BOT_LONGITUDE_TEMP = radians(BOT_LONGITUDE_TEMP);  //also must be done in radians
      DEST_LONGITUDE_1 = radians(DEST_LONGITUDE_1);

      GPS_HEADING = atan2(sin(DEST_LONGITUDE_1-BOT_LONGITUDE_TEMP)*cos(DEST_LATITUDE_1),cos(BOT_LATITUDE_TEMP)*sin(DEST_LATITUDE_1)-sin(BOT_LATITUDE_TEMP)*cos(DEST_LATITUDE_1)*cos(DEST_LONGITUDE_1-BOT_LONGITUDE_TEMP)),2*3.1415926535;
      GPS_HEADING = GPS_HEADING*180/3.1415926535;  // convert from radians to degrees
      int head =GPS_HEADING; 
      if(head < 0)
      {
          GPS_HEADING+=360;   //if the heading is negative then add 360 to make it positive
      }
      
      Serial.println("GPS HEADING FOR FIRST COORDINATE:");
      Serial.println(GPS_HEADING);
      Serial.println("");
      //-------------------------------------------------------------


      //compass
      int turn=0;
      int x, y, z;
      int azimuth;
      //float azimuth; //is supporting float too
      QMC.read(&x, &y, &z,&azimuth);
      //azimuth = QMC.azimuth(&y,&x);//you can get custom azimuth
      COMPASS_HEADING = azimuth;
      Serial.println("ROVER HEADING WITH COMPASS");
      Serial.println(COMPASS_HEADING);
      Serial.println("");




    //Compare
      HEADING_DIFF=COMPASS_HEADING-GPS_HEADING;   //getting the difference of our current heading to our needed heading
      float ABSOLUTE= abs(HEADING_DIFF);    //we will use this for our delay when turning
      if(ABSOLUTE>180){
        ABSOLUTE-=180;        //we only want 0 - 180
      }

      Serial.println("Difference of our current heading to our needed heading");
      Serial.println(ABSOLUTE);
      Serial.println("");



      Serial.println("Goal Distance For First Coordinate: ");
      if((DISTANCE_CALC-10) < 0 )
            Serial.println(0);   
      else           Serial.println(DISTANCE_CALC-10);   
  
      //return DISTANCE_CALC;
      Serial.println("");


      
      if(DISTANCE_CALC < 10 )
      {

          Serial.println("The rover has reached its First Coordinate");
          Serial.println("");

          //done();

        /*
          if(COORDINATE==TOTAL_COORDINATE)
          {
              done();
          }
          COORDINATE+=1;

          */
      }

      else {
          //forward();
      }


      //-------------------------------------- below tells us which way we need to turn
      if(HEADING_DIFF>=-180){
        if(HEADING_DIFF<=0){
          turn=8;    //set turn =8 which means "right"         
        }
      }
      if(HEADING_DIFF<-180){
        turn=5;      //set turn = 5 which means "left"
      }
      if(HEADING_DIFF>=0){
        if(HEADING_DIFF<180){
          turn=5;   //set turn = 5 which means "left"
        }
      }
      if(HEADING_DIFF>=180){     //set turn =8 which means "right"
        turn=8;
      }
      
      
      //-----------------------------------------------------------------------------------------------------------------------------------
      //-----------------------------------------------------------------------------------------------------------------------------------
      //-------------------------------------------------------------real important turning stuff. DO NOT TOUCH!!!
      
      //MOTOR STUFF
      if(COMPASS_HEADING == GPS_HEADING)
      {
          turn=3;   //then set turn = 3 meaning go "straight"
      }
      
      if(turn==3)
      {
          Serial.println("straight");
          digitalWrite(mo1, LOW);               //go straight 
          digitalWrite(mo2, HIGH);
          digitalWrite(mo3, LOW);
          digitalWrite(mo4, LOW);
      }
      //-------------------------------------------------------------------------------------turn right
      if(turn==8)
      {
          rightturn();
      }
      //------------------------------------------------------------------------------------------turn left
      
      if(turn==5)
      {
          leftturn();
      }

 //-------------------------------------------------------------------------
}

void  forward()
{
      analogWrite(L_R, 32);
      analogWrite(R_R, 0);
      analogWrite(R_L, 32);
      analogWrite(L_L, 0);

      //delay(1000);  

      Serial.println("Motor Running");
}

void done()
{
      analogWrite(L_L,0);
      analogWrite(R_L,0);
      analogWrite(L_R,0);
      analogWrite(R_R,0);
      delay(1000);  

      Serial.println("Motor Stops. . . . .");

      done();
}


//----------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//------------------------------------------------------right turning
void rightturn()
{
      if(COMPASS_HEADING+2>GPS_HEADING)
      {
          if(COMPASS_HEADING-2<GPS_HEADING)
          {
              digitalWrite(mo3, LOW);
              digitalWrite(mo4, LOW);
              return;
          }
      }
      
      HEADING_DIFF=COMPASS_HEADING-GPS_HEADING;  
      if(HEADING_DIFF<-180)
      {
          return;
      }
      
      if(HEADING_DIFF >= 0)
      {
          if(HEADING_DIFF<180)
          {
              return;
          }
      }

      digitalWrite(mo1, LOW);
      digitalWrite(mo2, HIGH);   
      digitalWrite(mo3, HIGH);
      digitalWrite(mo4, LOW); 

      //compass
      int x, y, z;
      int azimuth;
      //float azimuth; //is supporting float too
      QMC.read(&x, &y, &z,&azimuth);
      //azimuth = QMC.azimuth(&y,&x);//you can get custom azimuth
      COMPASS_HEADING = azimuth;
      Serial.println("current heaDING:");
      Serial.println(COMPASS_HEADING);      // this is the heading of the compass
      
      rightturn();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------
//----------------------------------------------left turning
void leftturn(){
  
      if(COMPASS_HEADING+2>GPS_HEADING)
      {
          if(COMPASS_HEADING-2<GPS_HEADING)
          {
              digitalWrite(mo3, LOW);
              digitalWrite(mo4, LOW);
              return;
          }
      }
      HEADING_DIFF=COMPASS_HEADING-GPS_HEADING;  
      
      if(HEADING_DIFF>=-180)
      {
          if(HEADING_DIFF<=0)
          {
             return;         
          }
      }

      if(HEADING_DIFF>=180)
      {     
          return;
      }

      digitalWrite(mo1, LOW);
      digitalWrite(mo2, HIGH);
      digitalWrite(mo3, LOW);
      digitalWrite(mo4, HIGH); 
      
      
      //compass
      int x, y, z;
      int azimuth;
      //float azimuth; //is supporting float too
      QMC.read(&x, &y, &z,&azimuth);
      //azimuth = QMC.azimuth(&y,&x);//you can get custom azimuth
      COMPASS_HEADING = azimuth;
      Serial.println("current heaDING:");
      Serial.println(COMPASS_HEADING);      // this is the heading of the compass
      
      leftturn();
}
