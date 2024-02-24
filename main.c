#include <avr/io.h>
#define F_CPU 8000000UL
#define BAUD 2400 // Define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) //Set baud rate value for UBRR

// Initialize UART
void uart_init(void){
    UBRRH = (BAUDRATE>>8); // Shift the register right by 8 bits
    UBRRL = BAUDRATE; // Set baud rate
    UCSRB = (1<<TXEN) | (1<<RXEN) ; // Enable receiver and transmitter
    UCSRC = (1<<URSEL) | (1<<UPM1) | (1<<UCSZ1); // 7 bit data format and even parity
}

void uart_transmit(unsigned char data){ // Function to send data
    while (! ( UCSRA & (1<<UDRE) )); // Wait while register is free
    UDR = data; // Load data in the register
} 

unsigned char uart_receive(void) { // function to receive data
    while(!(UCSRA & (1<<RXC))); // wait while data is being received 
    return UDR; // return 8-bit data
}
int main(void){
    DDRB = OxFF;
    unsigned int count = 0;
    uart_init(); // initialize UART 
    while(1){ // loop forever
        unsigned char data = uart_receive(); // receive a char
        count++;
        char msg[10];
        int temp = count, i=0; // make a copy of count
        while(temp){
            msg[i++] = temp%10 + 48;
            temp = temp/10;
        }
        while(i>0){
            uart_transmit (msg[i-1]);
            i--;
        }

        uart_transmit(' ');
        if(count%10==0){
            uart_transmit(13);
            uart_transmit(10);
        }
    }
}