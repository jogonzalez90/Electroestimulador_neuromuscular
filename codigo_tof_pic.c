/*
Funcion de pulsadores y asignacion de pines
Estimulo manual (TOF) por interrupcion en pin RB0
STOP detiene estimulacion RB1
Estimulacion sonora RB2
*/

#include <codigo_tof_pic.h>
#include <lcd.c> //Librería lcd.c.
#use I2C(MASTER, SDA=PIN_B0, SCL=PIN_B1, SLOW)
#use standard_io(D)
#include <MPU6050.c>//Libreria acelerometro

int conteoint=0;//Variable a contener el numero de estimulos reslizados
int1 ACTIVM=0;//Bandera estado activo TOF
int1 BUZZERM=0;//Bandera estado activo estimulacion sonora
int estado=0;//Variable a implentar switch
int16 conteo=0;//Variable encargada de contar el numero de veces desbordamiento timer 2
int16 perio=0;//Varible para modo automatico
int estadointer=0;
int estadoiner2=0;
int estadointera=0;
int estadoinera2=0;

//Variables lectura acelerometro
float Ax=0;
float Az=0;

//float iniAx = 0;//valor inicial Ax
float Az1 = 0;//valor inicial Az
float Az2 = 0;//segunda medida de aceleracion
float Az3 = 0;//tercera medida de aceleracion
float Az4=0;//valor final de aceleracion
float TOFR=0;//relacion tren de cuatro

int contelec=0;//variable conteo lecturas

 //DEFINE DE PINES PIC
   #DEFINE TOF PIN_B6//Estimimulo manual
   #DEFINE BUZZER PIN_B7//Estimulacion sonora
   #DEFINE BU_SA PIN_A1//Salida buzzer
   #DEFINE TOF_SA PIN_A5//Salida TOF
   #DEFINE ACTIV PIN_A2//Activar desactivar instrumento
   #DEFINE CALIB PIN_A3//Calibrar instrumento
   #DEFINE PILOTO PIN_B3

   VOID FIN1_TOF(VOID)//Funcion principal estimulo manual
   {
      ENABLE_INTERRUPTS(INT_TIMER2);//HABILITAMOS INTERRUPCION TIMER2 
         lcd_putc("\f" ) ; //Borra pantalla.
         lcd_putc("\f" ) ; //Borra pantalla.
         lcd_gotoxy(5,1); //Sitúa el cursor en 5 columna, 1 fila.
         lcd_putc("TOF-PIC"); //Escribe TOF_PIC.
         lcd_gotoxy(3,2); //Sitúa el cursor en 5 columna, 1 fila.
         lcd_putc("Estimulando..."); //Escribe Estimulando
         estado=2;//Varible para determinar estumimulo en operacion
      conteo=0;//Variable encargada de contar el numero de veces desbordamiento timer 2
      SET_TIMER2(0);
   }

   VOID FIN2_TOF(VOID)//FUNCION secundaria estimulo manual
   {
      conteo++;
      If(conteo==3)//If(conteo==1)Valor manejado If(conteo==1)
      {
         OUTPUT_LOW(TOF_SA);//Pin estimulo tof
         ENABLE_INTERRUPTS(INT_TIMER3);//HABILITAMOS INTERRUPCION TIMER2 
         SET_TIMER3(40536);
      }
      If(conteo==1667)//Valor para obtener 0.5 S valor manejado  
      {
         conteoint++;
         conteo=0;
      If((conteoint==4) && (BUZZERM==1))//If para activar buzzer
      {
         OUTPUT_HIGH(BU_SA);//Activamos buzzer
      }
      If(conteoint==5)//Generamos 4 pulsos
      {
         conteoint=0;
         OUTPUT_LOW(BU_SA);//Desactivamos buzzer
         
        // TOFR=(Az4/Az1)*100;//calcula relacion tren de cuatro
         DISABLE_INTERRUPTS(INT_TIMER2);//deshabilitamos timer de 300microS
         /*
          lcd_putc("\f" );//Instruccion para borrar pantalla.
         lcd_gotoxy(1,1); //Sitúa el cursor en 1 columna, 1 fila.
         lcd_putc(Az1); //Escribe TOF_PIC.
         lcd_gotoxy(1,1);
         printf(lcd_putc, "Az1:%0.2f", Az1);   // Imprime Ax
         lcd_gotoxy(1,2);
         printf(lcd_putc, "Az2:%0.2f", Az2);   // Imprime Ax
         delay_ms(3000);//Retardo antirrebote
         lcd_putc("\f" );//Instruccion para borrar pantalla.
         lcd_gotoxy(1,1);
         printf(lcd_putc, "Az3:%0.2f", Az3);   // Imprime Ax
         lcd_gotoxy(1,2);
         printf(lcd_putc, "Az4:%0.2f", Az4);   // Imprime Ax
          delay_ms(3000);//Retardo antirrebote
         lcd_putc("\f" );//Instruccion para borrar pantalla.
         */
            estado=1;//Estado TOF preaparado
            lcd_putc("\f" );//Instruccion para borrar pantalla.
      }
      else
      {
         contelec++;//variable para contabilizar numero de lecturas
         OUTPUT_HIGH(TOF_SA);//Activamos salida TOF durante tres ciclos
      }
      }
   }

   VOID LECTURA (VOID)//Funcion lectura acelerometro
   {
      OUTPUT_TOGGLE(PILOTO);
      /*
      IF(contelec==1)
      {
         Az = MPU6050_get_Az();              // Lee valor acelerometro en eje x y guarda en Az
         Az1= Az; //valor inicial de aceleracion
      }
      IF(contelec==2)
      {
         Az = MPU6050_get_Az();              // Lee valor acelerometro en eje x y guarda en Az
         Az2= Az; //valor inicial de aceleracion
      }
       IF(contelec==3)
      {
         Az = MPU6050_get_Az();              // Lee valor acelerometro en eje x y guarda en Az
         Az3= Az; //valor inicial de aceleracion
      }
      IF(contelec==4)
      {
         contelec=0;//reiniciamos variable para un nuevo estimulo
         Az = MPU6050_get_Az();              // Lee valor acelerometro en eje x y guarda en Az
         Az4= Az; //valor inicial de aceleracion
      }
      */
   }

#INT_TIMER2//DIRECTIVA INTERRUPCION POR TIMER2 PARA 300 µS
   VOID TIMER2_ISR(VOID)
   {
      FIN2_TOF();//FUNCION secundaria estimulo manual
   }   

#INT_TIMER3//DIRECTIVA INTERRUPCION POR TIMER3 PARA 2mS
   VOID TIMER3_ISR(VOID)
   {
      SET_TIMER3(40536);
      //DISABLE_INTERRUPTS(INT_TIMER3);//DESHABILITAMOS INTERRUPCION TIMER3 para lectura de dato
      LECTURA();//FUNCION secundaria estimulo manual
   }

void main(void) //Función principal main.
{
   ENABLE_INTERRUPTS(GLOBAL);//HABILITAMOS INTERRUPCIONES
   //INTERNET ECUACION TIMER2 http://microccspic.blogspot.com/2012/08/interrupcion-timer-2.html
   setup_timer_2 ( T2_DIV_BY_16, 5, 16);//Desbordamiento a 300 µS
   setup_timer_3 ( T3_INTERNAL | T3_DIV_BY_8 );//Timer con valor cargado en 40536 se deborda en 40mS adquisicion de datos
    
   SET_TRIS_A(0B001000);//configura puerto
   SET_TRIS_B(0B00000111);
   OUTPUT_LOW(BU_SA);//Salida buzzer
   OUTPUT_LOW(TOF_SA);//Pin estimulo tof
   OUTPUT_LOW(PILOTO);
   
   lcd_init(); //Inicializa LCD.
   //MPU6050_init();// Inicializa MPU6050

    //Texto saludo
    lcd_gotoxy(5,1); //Sitúa el cursor en 5 columna, 1 fila.
    lcd_putc("Hello..."); //Escribe TOF_PIC.
    lcd_gotoxy(5,2);
    lcd_putc("TOF-PIC");
    delay_ms(2500);//Retardo antirrebote
    lcd_putc("\f" ) ; //Borra pantalla.
    
   
   While(TRUE)
   {
      /*
      IF(INPUT(CALIB)==1)
      {
       Ax = MPU6050_get_Ax();              // Lee valor acelerometro en eje x y guarda en Ax
       Az = MPU6050_get_Az();              // Lee valor acelerometro en eje x y guarda en Az
        lcd_putc("\f" );//Instruccion para borrar pantalla.
         lcd_gotoxy(2,1); //Sitúa el cursor en 2 columna, 1 fila.
         lcd_putc("Calibrando..."); //Escribe calibrando.
        lcd_gotoxy(1,2);
        printf(lcd_putc, "Ax:%0.2f", Ax);   // Imprime Ax
        lcd_gotoxy(10,2);
        printf(lcd_putc, "Az:%0.2f", Az);   // Imprime Az
        delay_ms(2000);//Retardo vusualizacion valor acelerometro
        lcd_putc("\f" );//Instruccion para borrar pantalla.
      }
      */
      estadointer=INPUT(TOF);
      IF((estadointer==1) && (ACTIVM==1) && (estadoiner2==0))//Generar estimulo manual
      {
         //delay_ms(500);//Retardo antirrebote
         //DISABLE_INTERRUPTS(INT_TIMER3);//deshabilitamos timer de 300microS
         FIN1_TOF();//Funcion principal estimulo manual
      }
      estadoiner2=estadointer;
      
       estadointera=(INPUT(ACTIV));
      IF((INPUT(ACTIV)==1) && (estadoinera2==0))//If para determinar estado preparado del TOF
      {
         ACTIVM++;//Variable cambia de estado toggle
         //delay_ms(500);//Retardo antirrebote
         If(ACTIVM==1)
         {
            estado=1;//Tof modo RUN
            lcd_putc("\f" );//Instruccion para borrar pantalla.
         }
         else
         {  
            DISABLE_INTERRUPTS(INT_TIMER1);//DESHABILITAMOS INTERRUPCION TIMER1 PARA TOF AUTOMATICO
            lcd_putc("\f" );//Instruccion para borrar pantalla.
            estado=0;//Tof modo STOP
            lcd_putc("\f" );//Instruccion para borrar pantalla.
         }
      }
      estadoinera2=estadointera;   
         
     IF((INPUT(BUZZER)==1))//If para determinar estimulacion sonora
     {
         BUZZERM++;//Variable cambia de estado toggle
         delay_ms(500);//Retardo antirrebote
     }
    
   SWITCH(estado)
   {
   case 0:
         lcd_gotoxy(5,1); //Sitúa el cursor en 5 columna, 1 fila.
         lcd_putc("TOF-PIC"); //Escribe TOF_PIC.
         lcd_gotoxy(6,2); //Sitúa el cursor en 5 columna, 1 fila.
         lcd_putc("STOP"); //Escribe STOP
      break;
   
      case 1:
         lcd_gotoxy(5,1); //Sitúa el cursor en 5 columna, 1 fila.
         lcd_putc("TOF-PIC"); //Escribe TOF-Pic.
         lcd_gotoxy(7,2); //Sitúa el cursor en 5 columna, 1 fila.
         lcd_putc("RUN"); //Escribe preparado
     break;
   
   }  
 }
      
} 

//https://www.electronicwings.com/pic/mpu6050-gyroscope-accelerometer-temperature-interface-with-pic18f4550
//https://www.electronicwings.com/pic/mpu6050-gyroscope-accelerometer-temperature-interface-with-pic18f4550
//Video manejo mpu6050 https://www.youtube.com/watch?v=Jf3gmhsFD-4

//MPU6050_init(); //Inicializa el sensor MPU6050
//MPU6050_get_Ax();//Obtiene los datos del eje X del acelerómetro
//MPU6050_get_Ay();//Obtiene los datos del eje Y del acelerómetro
//MPU6050_get_Az();//Obtiene los datos del eje Z del acelerómetro


