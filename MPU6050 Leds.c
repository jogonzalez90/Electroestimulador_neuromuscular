#include <16f877a.h>
#fuses HS,NOWDT,NOPROTECT,NOPUT,NOLVP,NOBROWNOUT
#use delay(clock=20M)
#use I2C(MASTER, SDA=PIN_C4, SCL=PIN_C3, SLOW)
#use standard_io(B)
#use standard_io(D)

#define led_1 PIN_B6                      // Pines de los leds
#define led_2 PIN_B7
#define led_3 PIN_B4
#define led_4 PIN_B5

#define LCD_DB4  PIN_D4                   // Pines de la pantalla LCD
#define LCD_DB5  PIN_D5
#define LCD_DB6  PIN_D6
#define LCD_DB7  PIN_D7
#define LCD_RS   PIN_D2
#define LCD_E    PIN_D3
#include <LCD_20X4.c>                     // Libreria para el manejo de la LCD 20x4
#include <MPU6050.c>                      // Libreria para el manejo del modulo MPU6050
float Ax, Ay, Az, Gx, Gy, Gz, t;          // Variables para almacenar los datos del acelerometro y giroscopio

void main()
{
   lcd_init();                            // Inicializa la pantalla LCD
   MPU6050_init();                        // Inicializa MPU6050
   
   while(true)
   {
      Ax = MPU6050_get_Ax();              // Acelerometro eje x
      Ay = MPU6050_get_Ay();              // Acelerometro eje y
      Az = MPU6050_get_Az();              // Acelerometro eje z
      
      Gx = MPU6050_get_Gx();              // Giroscopio eje x
      Gy = MPU6050_get_Gy();              // Giroscopio eje y
      Gz = MPU6050_get_Gz();              // Giroscopio eje z
      
      t = MPU6050_get_Temp();             // Temperatura
      
      if(Ay < -0.40)
      {
         output_high(led_1);
      }
      else if(Ay > 0.40)
      {
         output_high(led_2);
      }
      else if(Ax > 0.48)
      {
         output_high(led_3);
      }
      else if(Ax < -0.40)
      {
         output_high(led_4);
      }
      else
      {
         output_low(led_1);
         output_low(led_2);
         output_low(led_3);
         output_low(led_4);
      }
      
      lcd_clear();
      lcd_gotoxy(1,1);
      printf(lcd_putc, "Ax:%0.2f", Ax);   // Imprime Ax
      lcd_gotoxy(1,2);
      printf(lcd_putc, "Ay:%0.2f", Ay);   // Imprime Ay
      lcd_gotoxy(1,3);
      printf(lcd_putc, "Az:%0.2f", Az);   // Imprime Az
      lcd_gotoxy(1,4);
      printf(lcd_putc, "T:%f", t);        // Imprime temperatura
      
      lcd_gotoxy(11,1);
      printf(lcd_putc, "Gx:%0.2f", Gx);   // Imprime Gx
      lcd_gotoxy(11,2);
      printf(lcd_putc, "Gy:%0.2f", Gy);   // Imprime Gy
      lcd_gotoxy(11,3);
      printf(lcd_putc, "Gz:%0.2f", Gz);   // Imprime Gz
      delay_ms(200);
   }
}
