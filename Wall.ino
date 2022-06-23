#include <OneWire.h>                                 // initialize One Wire library
#include <DallasTemperature.h>                       // initialize Dallas Tempreature library
#include <LiquidCrystal.h>                           // initialize LCD library 
       
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);               // Digital pins to which LCD is connected


// ****************  Defining constants  **************** // 


const int IN = 2;                                    // A2 is where LM35 sensor (in) is connected
const int IN1 = 3;                                   // A3 is where LM35 sensor (in) is connected

const int OUT = 0;                                   // A0 is where LM35 sensor (out) is connected
const int OUT1 = 1;                                  // A1 is where LM35 sensor (out) is connected

const float  K_PU = 0.03 ;                           // K (Thermal Conductivity) of PU [W/m.K]
const float  K_AIR = 0.025 ;                         // K (Thermal Conductivity) of Air [W/m.K]
const float  K_WOOD = 0.105 ;                        // K (Thermal Conductivity) of Wood [W/m.K]

const float  H_AIRi = 30 ;                           // h (Thermal Convection Coefficient) of Air in [W/m^2.K] 
const float  H_AIRo = 60 ;                           // h (Thermal Convection Coefficient) of Air out [W/m^2.K]

const float  Area = 0.038 ;                          // Area of PU = Area of Air = Area of Wood [m^2]


const float  L_PU = 0.03 ;                           // Thickness of PU [m]
const float  L_AIR = 0.02 ;                          // Thickness of AIR [m]
const float  L_WOOD = 0.005 ;                        // Thickness of WOOD [m]



// **************** Function Calculates Thermal Resistance for Conduction **************** // 

float Rconduction (float L, float K, float Area){                                        

    return ( L / ( K * Area ) );
  }





void setup()
{
  lcd.begin(16,2);                                                          // Intiallize LCD interface
  Serial.begin(9600);                                                       // Intiallize Serial Monitor
  }


void loop()
{


  // ****************  SENSOR I/O  **************** // 
  
  int valueIN = analogRead( IN ) ;                                           // Read the value from the sensor
  float millivoltsIN = ( valueIN / 1024.0 ) * 5000 ;                         // Change reading to millivolts 
  float celsiusIN = ( millivoltsIN / 10 ) ;                                  // Change millivolts to degree celsius
  
  int valueOUT = analogRead( OUT ) ;                                         // Read the value from the sensor
  float millivoltsOUT = ( valueOUT / 1024.0 ) * 5000 ;                       // Change reading to millivolts 
  float celsiusOUT = ( millivoltsOUT / 10 ) ;                                // Change millivolts to degree celsius
  
  int valueIN1 = analogRead( IN1 ) ;                                         // Read the value from the sensor
  float millivoltsIN1 = ( valueIN1 / 1024.0 ) * 5000 ;                       // Change reading to millivolts 
  float celsiusIN1 = ( millivoltsIN1 / 10 ) ;                                // Change millivolts to degree celsius

  int valueOUT1 = analogRead( OUT1 ) ;                                       // Read the value from the sensor
  float millivoltsOUT1 = ( valueOUT1 / 1024.0 ) * 5000 ;                     // Change reading to millivolts 
  float celsiusOUT1 = ( millivoltsOUT1 / 10 ) ;                              // Change millivolts to degree celsius
  
  float tempINavg= (celsiusIN +celsiusIN1 )/2 ;                                      // Calculate average tempreature in heated room to enhance accuracy
  float tempOUTavg= (celsiusOUT +celsiusOUT )/2 ;                                   // Calculate average tempreature in insulated room to enhance accuracy
  
   
  // **************** Heat Transfer **************** //

  float RcondPU = Rconduction(L_PU,K_PU,Area) ;                                                        // Conduction Thermal Resistance of PU
  float RcondAir = Rconduction(L_AIR,K_AIR,Area);                                                      // Conduction Thermal Resistance of Air
  float RcondWood = Rconduction(L_WOOD,K_WOOD,Area);                                                   // Conduction Thermal Resistance of Wood
  float RconvIN = 1 / ( H_AIRi * Area ) ;                                                              // Convection Thermal Resistance (IN)
  float RconvOUT = 1 / (H_AIRo * Area ) ;                                                              // Convection Thermal Resistance (OUT)     
  float R_Thermal = RconvIN + ( 4*RcondWood ) + ( 2*RcondPU ) + RcondAir + RconvOUT ;                  // Equivalent Thermal Resistance    
  float tempChange = ( tempINavg - tempOUTavg ) ;                                                      // Tempreature Change    
  float Q = ( tempChange / R_Thermal ) ;                                                               // Calculating heat transfer in watts

  
  
  // ****** Print temp in ****** //
  
  lcd.clear() ;                                                                                      
  
  lcd.setCursor( 0 , 0 ) ;
  lcd.print( "T In=" ) ;
  lcd.print( tempINavg ) ;
  lcd.print( " C" ) ;

  // ****** Print temp out ****** //
  
  lcd.setCursor( 0 , 1 ) ;
  lcd.print( "T Out=" ) ;
  lcd.print( tempOUTavg ) ;
  lcd.print( " C" ) ;
  
delay(2000);
  
  // ****** Print Q ****** //
  
  lcd.clear() ;
  
  lcd.setCursor( 0 , 0 ) ;
  lcd.print( "Q = " ) ;
  lcd.print( Q ) ;
  lcd.print( " Watt" ) ;
  
  delay(2000); 


  // ********* Monitor Readings ********* //

 Serial.print ("Temp OUT 1 = ");
  Serial.print (celsiusOUT );
  Serial.print ("\n");
  Serial.print ("Temp OUT 2 = ");
  Serial.print (celsiusOUT );
  Serial.print ("\n");
  Serial.print ("Temp IN 1 = ");
  Serial.print (celsiusIN);
  Serial.print ("\n");
  Serial.print ("Temp IN 2= ");
  Serial.print (celsiusIN1);
  Serial.print ("\n");
  Serial.print("*******");
  Serial.print ("\n");
  delay (500);
}
