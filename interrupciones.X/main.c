/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F18875
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "main.h"


#define Display_TRIS       TRISD            // Este será el Puerto que se conectará a los display de 7 segmentos ánodo común.
#define Display_LAT        LATD             
#define Multiplexor_TRIS   TRISB            // Este será el Puerto que se conectará a los pines que controlen la multiplexación. 
#define Multiplexor_LAT    LATB
#define Display_1 (1<<3)                    // 0b00001000
#define Display_2 (1<<2)                    // 0b00000100
#define Display_3 (1<<1)                    // 0b00000010
#define Display_4 (1<<0)                    // 0b00000001

#define A   (1<<0)                          //0b00000001
#define B   (1<<1)                          //0b00000010
#define C   (1<<2)                          //0b00000100
#define D   (1<<3)                          //0b00001000
#define E   (1<<4)                          //0b00010000
#define F   (1<<5)                          //0b00100000
#define G   (1<<6)                          //0b01000000
#define DP  (1<<7)                          //0b10000000

/*=====================================================================*/
int16_t Units        = 0;                   // Variable de 16 bits unidades.
int16_t Tens         = 0;                   // Variable de 16 bits decenas.
int16_t Hundred      = 0;                   // Variable de 16 bits centenas.
int16_t Thousands    = 0;                   // Variable de 16 bits unidades de millar.

/*=====================================================================*/
void Set_Port (int8_t value);               // Función para poner a 1 los bits del Puerto.
void Clear_Port (int8_t value);             // Función para poner a 0 los bits del Puerto.
void Decode_Display_AC (int8_t value);      // Función para decodificar los digitos en su valor de 7 segmentos ánado común.
void Show_Digits_Display (int16_t value);   // Función para mostrar números en los display de 7 segmentos ánodo común.

/*
                         Main application
 */

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    Display_TRIS=0x00;        // Configuramos el Puerto D como salida para los display de 7 Segmentos Anodo Comun.
    Multiplexor_TRIS=0b00010000;    // Configuramos el Puerto B como salida para los pines de multiplexación para los 4 digitos del display.
    
    while (1)
    {
          printf("%i\n",mostrar);
          Show_Digits_Display(mostrar);

        //__delay_ms(20);
        // Add your application code
    }
}

/*=====================================================================*/
void Set_Port (int8_t value)            // Función para poner a 1 los bits del Puerto.
{
    Display_LAT|=value;                 // Configuramos con 1s los bits del Puerto.
}

/*=====================================================================*/
void Clear_Port (int8_t value)          // Función para poner a 0 los bits del Puerto.
{
    Display_LAT&=~(value);              // Configuramos con 0s los bits del Puerto.
}

/*=====================================================================*/
void Decode_Display_AC (int8_t value)   // Función para decodificar los digitos en su valor de 7 segmentos ánado común.
{
    Set_Port(A|B|C|D|E|F|G|DP);         // Ponemos a 1 todos los bits del puerto.
    switch(value)
    {
        case 0: Clear_Port(A|B|C|D|E|F);   break;  // Digito 0 
        case 1: Clear_Port(B|C);           break;  // Digito 1
        case 2: Clear_Port(A|B|E|D|G);     break;  // Digito 2
        case 3: Clear_Port(A|B|C|D|G);     break;  // Digito 3
        case 4: Clear_Port(B|C|F|G);       break;  // Digito 4
        case 5: Clear_Port(A|C|D|F|G);     break;  // Digito 5
        case 6: Clear_Port(A|C|D|E|F|G);   break;  // Digito 6
        case 7: Clear_Port(A|B|C);         break;  // Digito 7
        case 8: Clear_Port(A|B|C|D|E|F|G); break;  // Digito 8
        case 9: Clear_Port(A|B|C|F|G);     break;  // Digito 9   
    }
}

/*=====================================================================*/
void Show_Digits_Display (int16_t value) // Función para mostrar números en los display de 7 segmentos ánodo común.
{
       Thousands      = value/1000;
       value = value%1000;
       Hundred        = value/100;
       value = value%100;
       Tens           = value/10;
       Units          = value%10;
        
       Decode_Display_AC(Units);
       Multiplexor_LAT=Display_1;
       __delay_ms(2);
       Decode_Display_AC(Tens);
       Multiplexor_LAT=Display_2;
       __delay_ms(2);
       Decode_Display_AC(Hundred);
       Multiplexor_LAT=Display_3;
       __delay_ms(2);
       Decode_Display_AC(Thousands);
       Multiplexor_LAT=Display_4;
       __delay_ms(2);
}

/**
 End of File
*/