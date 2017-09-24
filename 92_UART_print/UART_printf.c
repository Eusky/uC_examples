#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define strWelcome	"Welcome"
#define strWelcome1	"Microcontroller"

void putch(unsigned char data) {
	//�����غ� �� ������ ���
	while ((UCSR0A & (1 << UDRE0)) == 0)
		;
	// while(!(UCSR0A & 0x20)) ;

	UDR0 = data;
}

int printfStr(char data, FILE * stream) {
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = data;
	return 0;
}

unsigned char getch() {
	unsigned char data;

	//�����͸� ���� ������ ���
	while ((UCSR0A & (1 << RXC0)) == 0)
		;

	//while (bit_is_set(UCSR0A,RXC0);
	//while (!(UCSR0A & 0x80);

	// Receiver register�� �������� �ϴ� �����͸� ����
	data = UDR0;

	return data;
}

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

void uart_init(unsigned long iBaudrate) {

	// UCSRnA �������͸� �ʱ�ȭ��Ų��.
	// 0��° ��Ʈ, �� MPCMn �� 0���� ��Ʈ (USARTn�� ��Ƽ ���μ��� ��Ÿ��� ����)
	UCSR0A = 0x00;

	// UCSRnB �������͸� �̿��Ͽ� �۽� �� ���� ��뼳���� �Ѵ�.
	// Rx, Tx enable
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// 3��°, 4��° ��Ʈ ��Ʈ ��, TXENn (USARTn����� �۽ź� ���� enable) RXENn (USARTn����� ���ź� ���� enable)
	//  2�� ��Ʈ UCSZ02 = 0���� ��Ʈ

	// UCRnC �������͸� �̿��Ͽ� ���(����/�񵿱�), �и�Ƽ���, ������Ʈ,
	// ���� ������ ��Ʈ���� �����Ѵ�.
	// �񵿱� ���, No Parity bit, 1 Stop bit, 8bits
	UCSR0C |= (1 << UCSZ01);
	UCSR0C |= (1 << UCSZ00);
/*
	// UBRRnH(L) �������͸� �̿��� �ۼ��� ������Ʈ ����
	UBRR0H = 0x00;
	switch (iBaudrate) {
	case 9600:
		UBRR0L = 95; // 14.7456 MHz -> 9600 bps
		break;
	case 19200:
		UBRR0L = 47; // 14.7456 MHz -> 19200 bps
		break;
	case 115200:
		UBRR0L = 7;  // 14.7456 MHz -> 115200 bps
		break;
	default:
		UBRR0L = 95;
	}
	*/
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
}

static FILE PrnDevice = FDEV_SETUP_STREAM(printfStr, NULL, _FDEV_SETUP_WRITE);

int main() {
	unsigned char text[] =
			"\r\nWelcome! Serial communication world!!\r\n Good Luck\r\n";
	unsigned char echo[] = "HKNU >> ";
	unsigned char i = 0;

	DDRB = 0xff;
	DDRC = 0x00;

	uart_init(9600UL);

	while (text[i] != '\0') {
		putch(text[i++]);
		_delay_ms(40);
	}
	i = 0;
	while (echo[i] != '\0')
		putch(echo[i++]);

	stdout = &PrnDevice;
	i = 0;
	while (1) {
		printf("i:%3d  %.3f\r\n", i++, i + 1.1);
		sprintf(text, "i:%3d  %.3f", i, i + 1.1);
		_delay_ms(1000);
		if (i > 10)
			i = 0;
	};

	return 0;
}

/*
 #include <avr/io.h>

 #define USART_BAUDRATE 9600
 #define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

 int main(void) {
 char recieved_byte;

 UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
 UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
 UBRR0H = (BAUD_PRESCALE >> 8);
 UBRR0L = BAUD_PRESCALE;

 for (;;) {
 // wait until a byte is ready to read
 while (( UCSR0A & (1 << RXC0)) == 0) {
 }

 // grab the byte from the serial port
 recieved_byte = UDR0;

 // wait until the port is ready to be written to
 while (( UCSR0A & (1 << UDRE0)) == 0) {
 }

 // write the byte to the serial port
 UDR0 = recieved_byte;
 }
 return 0; // never reached
}
*/
