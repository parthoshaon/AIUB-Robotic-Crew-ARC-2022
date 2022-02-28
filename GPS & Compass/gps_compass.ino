/*
It is compatible with Arduino Mega Board but if you want to use it on Uno, you have to use software serial library.
Change Serial1 to the software serial object according to the TX/RX pin. 
*/

#include <TinyGPS.h>  
#include <Wire.h>
#include <MechaQMC5883.h>

MechaQMC5883 QMC;
TinyGPS GPS;

#define mo1 5
#define mo2 4
#define mo3 6
#define mo4 7
#define TOTAL_COORDINATE 5   					//Number of Coordinate

float GPS_HEADING=0;
int COMPASS_HEADING;

int HEADING_DIFF=0;
int COORDINATE=1;

float BOT_LATITUDE = 0, BOT_LONGITUDE = 0;
float BOT_LATITUDE_TEMP, BOT_LONGITUDE_TEMP;
float DEST_LATITUDE_1, DEST_LONGITUDE_1;
float DIF_LATITUDE, DIF_LONGITUDE, DISTANCE_CALC, DISTANCE_CALC_TEMP;
float TOTAL_DISTANCE;

float DEST_LATITUDE_2=28.34140014;  			//First Coordinate 
float DEST_LONGITUDE_2=-82.27369689;

float DEST_LATITUDE_3=28.3415508;               //Second Coordinate
float DEST_LONGITUDE_3=-82.2736816;

float DEST_LATITUDE_4=28.3415794;       		//Third Coordinate
float DEST_LONGITUDE_4=-82.2741317;

float DEST_LATITUDE_5=28.3414192;     			//Fouth Coordinate
float DEST_LONGITUDE_5=-82.2741470;

float DEST_LATITUDE_6 = 28.3413600;				//Fifth Coordinate
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
}

void loop()
{
      bool newdata = false;
      unsigned long start = millis();
      while (millis() - start < 250)            //Get new data every 250 m. sec
      {
          if (gps_availability())
              newdata = true;
      }
      
      if (newdata)
      {
          Serial.println("GPS Connected");
          Serial.println("-------------");
          gps_run(GPS);
          Serial.println("-------------");
          Serial.println();
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
      //Negative numbers
      if (number < 0.0)
      {
         Serial.print('-');
         number = -number;
      }
    
      // Rounding the number. EX: print(5.999, 2) prints as "5.00"
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
    
      
      gps_availability(); 						//Repeated calls are being made to keep GPS data up to date
    
      GPS.f_get_position(&BOT_LATITUDE, &BOT_LONGITUDE, &age);
	  //GPS position by degree
      Serial.print("Lat/Long(float): "); print_decimal(BOT_LATITUDE, 7); Serial.print(", "); print_decimal(BOT_LONGITUDE, 7);
      Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");
    
      gps_availability();
      Serial.print("Alt(cm): "); Serial.print(GPS.altitude()); Serial.print(" Course(10^-2 deg): "); Serial.print(GPS.course()); Serial.print(" Speed(10^-2 knots): "); Serial.println(GPS.speed());
      Serial.print("Alt(float): "); print_decimal(GPS.f_altitude()); Serial.print(" Course(float): "); print_decimal(GPS.f_course()); Serial.println();
      Serial.print("Speed(knots): "); print_decimal(GPS.f_speed_knots()); Serial.print(" (mph): ");  print_decimal(GPS.f_speed_mph());
      Serial.print(" (mps): "); print_decimal(GPS.f_speed_mps()); Serial.print(" (kmph): "); print_decimal(GPS.f_speed_kmph()); Serial.println();
      float speedy=GPS.f_speed_knots();
      gps_availability();
    
      GPS.stats(&chars, &sentences, &failed);
      Serial.print("Stats: characters: "); Serial.print(chars); Serial.print(" sentences: "); Serial.print(sentences); Serial.print(" failed checksum: "); Serial.println(failed);
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



//------------------------------------------------CALCULATION PART--------------------------------------------------------------
void distance()
{
	  //------------------------------------------Check The next GOAL Coordinate-----------------------------------------
      if(COORDINATE==1)
      {
          DEST_LATITUDE_1 = DEST_LATITUDE_2;      
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
      
      DEST_LATITUDE_1 = 23.8362306;  	// Destination Latitude
      DEST_LONGITUDE_1 = 90.4212758;  	// Destination  Longitude

      //23.82277395, 90.42730991
    

	  //--------------------------------DISTANCE between ROVER location and GOAL-------------------------------------
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
    
      
      Serial.println("distance");
      Serial.println(DISTANCE_CALC);                  
      //return DISTANCE_CALC;

	  //-----------------------------Check if the ROVER reached to GOAL Cordinate, if yes then its target the next GOAL--------------------------------
      if(DISTANCE_CALC < 4 )
      {
          if(COORDINATE==TOTAL_COORDINATE)
          {
              //done();
          }
          COORDINATE+=1;
      }




	  //---------------Fixing the heading from the current position to the destination position--------------------------------
      BOT_LONGITUDE_TEMP = radians(BOT_LONGITUDE_TEMP);  //also must be done in radians
      DEST_LONGITUDE_1 = radians(DEST_LONGITUDE_1);

      GPS_HEADING = atan2(sin(DEST_LONGITUDE_1-BOT_LONGITUDE_TEMP)*cos(DEST_LATITUDE_1),cos(BOT_LATITUDE_TEMP)*sin(DEST_LATITUDE_1)-sin(BOT_LATITUDE_TEMP)*cos(DEST_LATITUDE_1)*cos(DEST_LONGITUDE_1-BOT_LONGITUDE_TEMP)),2*3.1415926535;
      GPS_HEADING = GPS_HEADING*180/3.1415926535;  // convert from radians to degrees
      int head =GPS_HEADING; 
      if(head < 0)
      {
          GPS_HEADING+=360;   //if the heading is negative then add 360 to make it positive
      }
      
      Serial.println("GPS_HEADING:");
      Serial.println(GPS_HEADING);
      //-------------------------------------------------------------


      //-------------------------------------------ROVER heading using COMPASS------------------------------------------------
      int turn=0;
      int x, y, z;
      int azimuth;
      //float azimuth; //is supporting float too
      QMC.read(&x, &y, &z,&azimuth);
      //azimuth = QMC.azimuth(&y,&x);//you can get custom azimuth
      COMPASS_HEADING = azimuth;
      Serial.println("current heaDING:");
      Serial.println(COMPASS_HEADING);




      //----------------------------------------Getting the difference of our current heading to our needed heading--------------------------
      HEADING_DIFF=COMPASS_HEADING-GPS_HEADING;   
      float ABSOLUTE= abs(HEADING_DIFF);    //we will use this for our delay when turning
      if(ABSOLUTE>180){
        ABSOLUTE-=180;        			//Because of only need of  0 - 180
      }
      Serial.println(ABSOLUTE);



	  //------------------------------------------------------------------------MOTOR STUFF-------------------------------------------------------------------
      //--------------------------------------------------Tells us which way ROVER need to turn---------------------------------------------------

      if(HEADING_DIFF>=-180)
	  {
        if(HEADING_DIFF<=0)
		{
          turn=8;    						//8 defines as to go RIGHT         
        }
      }

      if(HEADING_DIFF<-180)
	  {
        turn=5;      						//5 defines as to go LEFT
      }

      if(HEADING_DIFF>=0)
	  {
        if(HEADING_DIFF<180)
		{
          turn=5;   							//5 defines as to go LEFT
        }
      }
      
	  if(HEADING_DIFF>=180)
	  {     
        turn=8;								//8 defines as to go RIGHT
      }
      
      -
      if(COMPASS_HEADING == GPS_HEADING)
      {
          turn=3;   							//3 defines as to go STRAIGHT
      }
      


	  //---------------------------------------------------------TURNING THE MOTORS---------------------------------------------------------
      

	  //---------------------------------------------------------NO TURN, GO STRAIGHT--------------------------------------------------------
	  if(turn==3)
      {
          Serial.println("straight");
          digitalWrite(mo1, LOW);                
          digitalWrite(mo2, HIGH);						
          digitalWrite(mo3, LOW);
          digitalWrite(mo4, LOW);
      }
      

	  //-------------------------------------Rght Turn----------------------------------
      if(turn==8)
      {
          rightturn();
      }
      
	  //-------------------------------------Left Turn------------------------------------
      if(turn==5)
      {
          leftturn();
      }
}

  

//--------------------------------------------------Keep Motors TURN OFF recursievly---------------------------------------------
void done()
{
      digitalWrite(mo1, LOW);   
      digitalWrite(mo2, LOW);
      digitalWrite(mo3, LOW);
      digitalWrite(mo4, LOW);
      done();
}


//------------------------------------------------------------FUNCTION DEFINE for RIGHT TURN-------------------------------------------------------------------
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

      //--------------------------------------------------compass-------------------------------------------------------------------
      int x, y, z;
      int azimuth;
      //float azimuth; //is supporting float too
      QMC.read(&x, &y, &z,&azimuth);
      //azimuth = QMC.azimuth(&y,&x);//you can get custom azimuth
      COMPASS_HEADING = azimuth;
      Serial.println("current heaDING:");
      Serial.println(COMPASS_HEADING);      // this is the heading of the compass
      
      rightturn(); 							//Call again untill the heading get fixed 
}


//------------------------------------------------------------FUNCTION DEFINE for LEFT TURN-------------------------------------------------------------------
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
      
      
      //-----------------------------------Compass-------------------------------------
      int x, y, z;
      int azimuth;
      //float azimuth; //is supporting float too
      QMC.read(&x, &y, &z,&azimuth);
      //azimuth = QMC.azimuth(&y,&x);//you can get custom azimuth
      COMPASS_HEADING = azimuth;
      Serial.println("current heaDING:");
      Serial.println(COMPASS_HEADING);      // this is the heading of the compass
      
      leftturn();								//Call again untill the heading get fixed 
}
