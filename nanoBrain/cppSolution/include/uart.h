#ifndef UART_H_
#define UART_H_
#define     BAUDRATE       B115200

class Uart
{
private:
    int handle;
    unsigned char buffer;
public:
    Uart(/* args */);
    ~Uart();
    void send(char *msg, int numBytes);
    char recevie();
    
};


char packByte(int rod, int value);
#endif