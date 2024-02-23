#ifndef UART_H_
#define UART_H_
#define     BAUDRATE       B115200

class Uart
{
private:
    int id;
    unsigned char buffer[20];
public:
    Uart(/* args */);
    ~Uart();
    void send(char *msg, int numBytes);
    
};


char packByte(int rod, int value);
#endif