#include <sfud.h>

#define SFUD_DEMO_TEST_BUFFER_SIZE                     1024
static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE] = {0};
static void sfud_demo(uint32_t addr, size_t size, uint8_t *data);
	
#define SERIAL Serial

void setup()
{
    SERIAL.begin(115200);
    while(!SERIAL) {};
    while(!(sfud_init() == SFUD_SUCCESS));
    #ifdef SFUD_USING_QSPI
    sfud_qspi_fast_read_enable(sfud_get_device(SFUD_W25Q32_DEVICE_INDEX), 4);
    #endif
    const sfud_flash *flash = sfud_get_device_table() + 0;
    uint32_t addr = 0;
    size_t size = sizeof(sfud_demo_test_buf);
    uint8_t result = sfud_read(flash, addr, size, sfud_demo_test_buf);
    if (result == SFUD_SUCCESS) {
        SERIAL.println("Read the flash data success.");
        SERIAL.println("Offset\t00\t01\t02\t03\t04\t05\t06\t07\t08\t09\t0A\t0B\t0C\t0D\t0E\t0F");
        for (int i = 0; i < size; i++) {
            if (i % 16 == 0) {
                SERIAL.print("0x");
                SERIAL.print(addr + i,HEX);
                SERIAL.print("\t");
            }
            SERIAL.print(sfud_demo_test_buf[i],HEX);
            SERIAL.print("\t");
            if (((i + 1) % 16 == 0) || i == size - 1) {
                SERIAL.println("");
            }
        }
        SERIAL.println(" ");
    } else {
        SERIAL.println("Read the flash data failed.");
    }
}

void loop()
{     
}