#include "uart.h"

#include "util.h"

#include <string.h>

static uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;
static volatile uint8_t rx_size = 0;
static uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;
static volatile uint8_t tx_size = 0;

ISR(USART_RX_vect) {
    // Test if parity error occured
    if (bit_set(UCSR0A, UPE0)) {
        // Read data without using it
        UDR0;
        return;
    }

    // Test if RX buffer is full. If buffer is full received byte will be lost
    if (rx_size == RX_BUFFER_SIZE) {
        return;
    }

    rx_buffer[rx_tail] = UDR0;
    rx_tail = (uint8_t)(rx_tail + 1) % RX_BUFFER_SIZE;
    rx_size++;
    return;
}

ISR(USART_UDRE_vect) {
    // Test if TX buffer is empty
    if (tx_size == 0) {
        clear_bit(UCSR0B, UDRIE0);
        return;
    };

    // Send next byte from TX buffer
    UDR0 = tx_buffer[tx_head];
    tx_head = (uint8_t)(tx_head + 1) % TX_BUFFER_SIZE;
    tx_size--;
    return;
}

void init_uart() {
    cli();
    // Set UART serial baudrate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    set_bit_high(UCSR0A, U2X0);
#else
    clear_bit(UCSR0A, U2X0);
#endif
    // Set UART serial format to 8N1
    set_bit_high(UCSR0C, UCSZ00);
    set_bit_high(UCSR0C, UCSZ01);
    // Enable UART RX interrupt
    set_bit_high(UCSR0B, RXCIE0);
    //set_bit_high(UCSR0B, UDRIE0);
    // Enable UART RX and TX
    set_bit_high(UCSR0B, RXEN0);
    set_bit_high(UCSR0B, TXEN0);

    sei(); // Enable interrupts
}

void serial_write_byte(uint8_t data) {
    //while (!bit_set(UCSR0A, UDRE0)) {}
    if (tx_size == TX_BUFFER_SIZE) {
        return;
    }
    tx_buffer[tx_tail] = data;
    tx_tail = (uint8_t)(tx_tail + 1) % TX_BUFFER_SIZE;
    tx_size++;
    set_bit_high(UCSR0B, UDRIE0);
}

void serial_write_string(char* str) {
    while (*str) {
        serial_write_byte(*str++);
    }
    serial_write_byte('\0');
}

void serial_write_string_blocking(char* str) {
    while (*str) {
        if (tx_size < TX_BUFFER_SIZE) {
            serial_write_byte(*str++);
        }
    }
    serial_write_byte('\0');
}

bool serial_data_avialable() {
    return rx_size > 0;
}

uint8_t serial_read_byte() {
    if (rx_size == 0) {
        return 0;
    }
    uint8_t recv_data = rx_buffer[rx_head];
    rx_head = (uint8_t)(rx_head + 1) % RX_BUFFER_SIZE;
    rx_size--;
    return recv_data;
}

void serial_read_string(char* str, uint8_t buffer_size) {
    uint16_t read_count = 0;
    while (rx_buffer[rx_head]) {
        str[read_count] = serial_read_byte();
        read_count++;
    }
    if (read_count >= buffer_size) {
        str[buffer_size - 1] = serial_read_byte();
    } else {
        str[read_count] = serial_read_byte();
    }
}

void serial_read_string_blocking(char* str, uint8_t buffer_size) {
    uint16_t read_count = 0;
    while (rx_buffer[rx_head]) {
        if(rx_size > 0) {
            str[read_count] = serial_read_byte();
            read_count++;
        }
    }
    if (read_count >= buffer_size) {
        str[buffer_size - 1] = serial_read_byte();
    } else {
        str[read_count] = serial_read_byte();
    }
}
