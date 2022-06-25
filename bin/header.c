/* Fernando Camussi
   fcamussi@gmail.com */


#include <stdio.h>
#include <stdlib.h>


#define TYPE 0xFE
#define DIR_MIN 0x0000
#define DIR_MAX 0xFFFF
#define BUFFER 1000

#define LOW(x) (uint8_t)x
#define HI(x) (uint8_t)(x >> 8)


typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;



int main(int argc, char *argv[])
{
    FILE *file1, *file2;
    uint8_t header[7];
    uint8_t buffer[BUFFER];
    size_t read;
    unsigned int begin, end, start;

    if (argc != 5)
    {
        fprintf(stderr, "Modo de uso: %s fichero1 fichero2 comienzo inicio\n"
                "fichero1: binario de MSX sin cabecera (entrada)\n"
                "fichero2: binario de MSX con cabecera (salida)\n"
                "comienzo: direccion de comienzo (0x9C40 por ejemplo)\n"
                "inicio: direccion de inicio (0x9C40 por ejemplo)\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    file1 = fopen(argv[1], "rb");
    if (! file1)
    {
        fprintf(stderr, "Error al abrir el fichero %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    file2 = fopen(argv[2], "wb");
    if (! file2)
    {
        fprintf(stderr, "Error al crear el fichero %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    begin = strtol(argv[3], NULL, 16);
    if (DIR_MIN > begin || begin > DIR_MAX)
    {
        fprintf(stderr, "Direccion de comienzo incorrecta. "
                        "Debe ser mayor igual a 0x%04X y menor igual a 0x%04X\n",
                        DIR_MIN, DIR_MAX);
        exit(EXIT_FAILURE);
    }        

    start = strtol(argv[4], NULL, 16);
    if (DIR_MIN > start || start > DIR_MAX)
    {
        fprintf(stderr, "Direccion de inicio incorrecta. "
                        "Debe ser mayor igual a 0x%04X y menor igual a 0x%04X\n",
                        DIR_MIN, DIR_MAX);
        exit(EXIT_FAILURE);
    }        

    fseek(file1, 0, SEEK_END);
    end = (unsigned int)ftell(file1) + begin + 1;
    fseek(file1, 0, SEEK_SET);

    header[0] = TYPE;
    header[1] = LOW(begin);
    header[2] = HI(begin);
    header[3] = LOW(end);
    header[4] = HI(end);
    header[5] = LOW(start);
    header[6] = HI(start);

    fwrite(header, 1, sizeof(header), file2);

    read = fread(buffer, 1, BUFFER, file1);
    while (read > 0)
    {
        fwrite(buffer, 1, read, file2);
        read = fread(buffer, 1, BUFFER, file1);
    }

    fclose(file1);
    fclose(file2);

    exit(EXIT_SUCCESS);
}

