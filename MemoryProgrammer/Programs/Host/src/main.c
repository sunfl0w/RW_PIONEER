#include <ctype.h>
#include <unistd.h>
#include <libserialport.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RECEIVE_LINE_MIN_BUF_SIZE 32

int min(int a, int b) {
    return a < b ? a : b;
}

int read_bin_file(const char* file_path, size_t* file_size, char** data) {
    FILE* file = fopen(file_path, "rb");
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);
    *data = (char*)malloc(*file_size);
    fread(data, 1, *file_size, file);
    fclose(file);

    if (data && file_size > 0) {
        return 0;
    }
    return -1;
}

int serial_check(enum sp_return result) {
    char* error_message;
    switch (result) {
    case SP_ERR_ARG:
        fprintf(stderr, "Serial Port Error. Invalid argument\n");
        return SP_ERR_ARG;
    case SP_ERR_FAIL:
        error_message = sp_last_error_message();
        fprintf(stderr, "Serial Port Error. %s\n", error_message);
        sp_free_error_message(error_message);
        return SP_ERR_FAIL;
    case SP_ERR_SUPP:
        fprintf(stderr, "Serial Port Error. Operation not supported\n");
        return SP_ERR_SUPP;
    case SP_ERR_MEM:
        fprintf(stderr, "Serial Port Error. Memory allocation failed\n");
        return SP_ERR_MEM;
    case SP_OK:
        return SP_OK;
    default:
        return 0;
    }
}

int serial_send_data(struct sp_port* port, const char* data, int data_size) {
    if (serial_check(sp_blocking_write(port, data, data_size, 1000)) != 0) {
        return -1;
    }
    if (serial_check(sp_drain(port)) != 0) {
        return -1;
    }
    return 0;
}

int serial_send_byte(struct sp_port* port, char byte) {
    if (serial_check(sp_blocking_write(port, &byte, 1, 1000)) != 0) {
        return -1;
    }
    if (serial_check(sp_drain(port)) != 0) {
        return -1;
    }
    return 0;
}

int serial_receive_string(struct sp_port* port, char** data, size_t* data_size) {
    *data = (char*)malloc(RECEIVE_LINE_MIN_BUF_SIZE * sizeof(char));
    *data_size = 0;
    size_t buffer_size = RECEIVE_LINE_MIN_BUF_SIZE;
    while (1) {
        int serial_input_buf_size = sp_input_waiting(port);
        if (serial_input_buf_size > 0) {
            char next_byte;
            if (serial_check(sp_blocking_read(port, &next_byte, 1, 1000)) != 0) {
                return -1;
            }

            if (buffer_size - *data_size < 1) {
                buffer_size *= 2;
                *data = (char*)realloc(*data, buffer_size);
            }
            
            memcpy(*data + *data_size, &next_byte, 1);
            (*data_size)++;

            if ((*data)[*data_size - 1] == '\0') {
                *data = (char*)realloc(*data, *data_size);
                return 0;
            }
        }
        // Sleep for 1us
        nanosleep((struct timespec[]){{0, 1000L}}, NULL);
    }
}

int operation_str_valid(const char* str) {
    if (strcmp(str, "Program") == 0) {
        return 1;
    } else if (strcmp(str, "Erase") == 0) {
        return 1;
    } else if (strcmp(str, "MemoryDump") == 0) {
        return 1;
    } else {
        return 0;
    }
}

int device_str_valid(const char* str) {
    if (strcmp(str, "AT28C64") == 0) {
        return 1;
    } else if (strcmp(str, "AT28C128") == 0) {
        return 1;
    } else if (strcmp(str, "AT28C256") == 0) {
        return 1;
    } else if (strcmp(str, "SST39SF010A") == 0) {
        return 1;
    } else if (strcmp(str, "SST39SF020A") == 0) {
        return 1;
    } else if (strcmp(str, "SST39SF040") == 0) {
        return 1;
    } else {
        return 0;
    }
}

int scan_serial_ports() {
    struct sp_port** ports;

    int port_list_result = serial_check(sp_list_ports(&ports));
    if (port_list_result != SP_OK) {
        fprintf(stderr, "Finding ports failed!\n");
        return -1;
    }

    printf("Listing available ports:\n\n");

    for (int i = 0; ports[i] != NULL; i++) {
        printf("Port %i\n", i);
        printf("Name: %s\n", sp_get_port_name(ports[i]));
        printf("Description: %s\n", sp_get_port_description(ports[i]));

        enum sp_transport transport = sp_get_port_transport(ports[i]);

        if (transport == SP_TRANSPORT_NATIVE) {
            printf("Type: Native\n");
        } else if (transport == SP_TRANSPORT_USB) {
            printf("Type: USB\n");
            printf("Manufacturer: %s\n", sp_get_port_usb_manufacturer(ports[i]));
            printf("Product: %s\n", sp_get_port_usb_product(ports[i]));
            printf("Serial: %s\n", sp_get_port_usb_serial(ports[i]));

            int vID;
            int pID;
            sp_get_port_usb_vid_pid(ports[i], &vID, &pID);
            printf("VID: %i PID: %i\n", vID, pID);

            int bus;
            int address;
            sp_get_port_usb_bus_address(ports[i], &bus, &address);
            printf("BUS: %i Address: %i\n", bus, address);
        } else if (transport == SP_TRANSPORT_BLUETOOTH) {
            printf("Type: Bluetooth\n");
            printf("MAC: %s\n", sp_get_port_bluetooth_address(ports[i]));
        }
        printf("\n");
    }
    sp_free_port_list(ports);
    return 0;
}

int main(int argc, char* argv[]) {
    printf("Starting RW_FLASH\n");

    char* binary_file_path = NULL;
    char* serial_port_name = NULL;
    char* operation = NULL;
    char* target = NULL;
    int scan_enabled = 0;
    int operation_length = 0;
    int arg;
    while ((arg = getopt(argc, argv, "d:hl:o:p:st:")) != -1) {
        switch (arg) {
        case 'd':
            binary_file_path = optarg;
            break;
        case 'h':
            printf("Help for RW_FLASH\n");
            printf("RW_FLASH is used for programming EEPROM and FLASH ICs. It is meant to be used with the accompanying RW_FLASH_MICRO program to be used with Arduino capable micro constrollers\n");
            printf("Currently supported EEPROMS: AT28C64, AT28C128, AT28C256\n");
            printf("Currently supported FLASH-Devices: SST39SF010A, SST39SF020A, SST39SF040\n");
            printf("Currently supported operations: Program, Erase, MemoryDump\n");
            printf("When programming SST39SF*-Devices the Erase operation needs to be performed before the Program operation as the FLASH memory needs to be empty to be rewritten\n");
            printf("-d => Path to binary data file which contents should be written to memory device. Only used when executing the Program operation\n");
            printf("-h => Displays this help page and application returns\n");
            printf("-l => Optinal size of binary data in bytes to be written to memory device or to be dumped in a MemoryDump operation. If not specified the whole file's content will be written to the device when "
                   "programming. Only used when using the Program and MemoryDump operations.\n");
            printf("-o => Operation to be executed\n");
            printf("-p => Serial port to use for communicating with the micro controller that does the actual programming\n");
            printf("-s => When specified a list of available serial ports will be printed before the application returns\n");
            printf("-t => Programming target. This is one of the supported EEPROMS or FLASH-Devices\n");
            return 0;
        case 'l':
            operation_length = strtol(optarg, NULL, 10);
            break;
        case 'o':
            operation = optarg;
            break;
        case 'p':
            serial_port_name = optarg;
            break;
        case 's':
            scan_enabled = 1;
            break;
        case 't':
            target = optarg;
            break;
        case '?':
            if (isprint(optopt)) {
                fprintf(stderr, "Option -%c unknown\n", optopt);
            } else {
                fprintf(stderr, "Option character -\\x%x unknown\n", optopt);
            }
            return -1;
        default:
            return -1;
        }
    }

    if (scan_enabled) {
        printf("Scanning serial ports\n");
        return scan_serial_ports();
    }

    if (operation == NULL) {
        fprintf(stderr, "Operation not specified. Aborting\n");
        return -1;
    }

    if (serial_port_name == NULL) {
        fprintf(stderr, "Serial port not specified. Aborting\n");
        return -1;
    }

    if (target == NULL) {
        fprintf(stderr, "Target not specified. Aborting\n");
        return -1;
    }

    size_t binary_data_size = 0;
    char* binary_data = NULL;
    if (strcmp(operation, "Program") == 0) {
        if (binary_file_path == NULL) {
            fprintf(stderr, "Path to binary data file not specified. Aborting\n");
            return -1;
        }
        if (read_bin_file(binary_file_path, &binary_data_size, &binary_data) != 0) {
            fprintf(stderr, "Error while reading binary data file. Aborting\n");
            return -1;
        }
    } else if (strcmp(operation, "Erase") != 0 && strcmp(operation, "MemoryDump") != 0) {
        fprintf(stderr, "Specified operation %s unknown. Aborting\n", operation);
        return -1;
    }

    if(operation_length <= 0) {
        operation_length = binary_data_size;
    }

    struct sp_port** ports;
    int port_list_result = serial_check(sp_list_ports(&ports));
    if (port_list_result != SP_OK) {
        fprintf(stderr, "Finding ports failed!\n");
        return -1;
    }

    int selected_port_index = -1;
    for (int i = 0; ports[i] != NULL; i++) {
        if (strcmp(serial_port_name, sp_get_port_name(ports[i])) == 0) {
            selected_port_index = i;
        }
    }

    if (selected_port_index == -1) {
        fprintf(stderr, "Sepcified serial port %s invalid. Aborting\n", serial_port_name);
        sp_free_port_list(ports);
        return -1;
    }

    struct sp_port* port;
    sp_copy_port(ports[selected_port_index], &port);
    sp_free_port_list(ports);

    serial_check(sp_open(port, SP_MODE_READ_WRITE));
    serial_check(sp_set_baudrate(port, 9600));
    serial_check(sp_set_bits(port, 8));
    serial_check(sp_set_parity(port, SP_PARITY_NONE));
    serial_check(sp_set_stopbits(port, 1));
    serial_check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));
    serial_check(sp_flush(port, SP_BUF_INPUT));

    printf("Serial port opened\n");

    int operation_completed = 0;
    int current_address = 0;
    while (!operation_completed) {
        char* line = NULL;
        size_t line_size = 0;
        serial_receive_string(port, &line, &line_size);

        printf("Received: %s\n", line);

        if (strcmp(line, "Send operation") == 0) {
            printf("Sending operation %s\n", operation);
            serial_send_data(port, operation, strlen(operation) + 1);
        } else if (strcmp(line, "Send target") == 0) {
            printf("Sending target %s\n", target);
            serial_send_data(port, target, strlen(target) + 1);
        } else if(strcmp(line, "Send operation range") == 0) {
            int str_length = snprintf(NULL, 0, "%i", operation_length);
            char operation_length_str[str_length + 1];
            snprintf(operation_length_str, str_length + 2, "%i", operation_length);
            operation_length_str[str_length] = '\0';
            printf("Sending operation range %s\n", operation_length_str);
            serial_send_data(port, operation_length_str, str_length + 1);
        } else if (strcmp(line, "Send next byte") == 0 && current_address < binary_data_size) {
            printf("Sending next byte %x\n", binary_data[current_address]);
            serial_send_byte(port, binary_data[current_address]);
            current_address++;
        } else if (strcmp(line, "Operation completed") == 0) {
            operation_completed = 1;
        }
        // Sleep for 1us
        nanosleep((struct timespec[]){{0, 1000L}}, NULL);
    }

    printf("Closing port\n");

    serial_check(sp_close(port));
    sp_free_port(port);
    return 0;
}
