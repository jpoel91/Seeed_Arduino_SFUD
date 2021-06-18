#include <sfud.h>

#define SFUD_DEMO_TEST_BUFFER_SIZE                     1024
#define SFUD_DEMO_CHIP_CAPACITY                    1024
static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE];
static int sfud_demo(uint32_t addr, size_t size, uint8_t *data);
	
#define SERIAL Serial

void setup()
{
    char ret;
    SERIAL.begin(115200);
    while(!SERIAL) {};
    while(!(sfud_init() == SFUD_SUCCESS));
    #ifdef SFUD_USING_QSPI
    sfud_qspi_fast_read_enable(sfud_get_device(SFUD_W25Q32_DEVICE_INDEX), 4);
    #endif 
    for(int i = 0; i < SFUD_DEMO_CHIP_CAPACITY; i+=1024){
      ret = sfud_demo(i, sizeof(sfud_demo_test_buf), sfud_demo_test_buf);
      if (ret == -1) break;
    }
      
}

void loop()
{   
      
}
/**
 * SFUD demo for the first flash device test.
 *
 * @param addr flash start address
 * @param size test flash size
 * @param size test flash data buffer
 */
static int sfud_demo(uint32_t addr, size_t size, uint8_t *data) {
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    size_t i;
    /* prepare write data */
    for (i = 0; i < size; i++) {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(flash, addr, size);
    if (result == SFUD_SUCCESS) {
        SERIAL.println("Erase the flash data finish");
    } else {
        SERIAL.println("Erase flash data failed");
        return -1;
    }
    /* write test */
    result = sfud_write(flash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        SERIAL.println("Write the flash data finish");
    } else {
        SERIAL.println("Write the flash data failed");
        return -1;
    }
    /* read test */
    size_t BaseTime = micros();
    memset(data, 0, sizeof data);
    result = sfud_read(flash, addr, size, data);
    size_t CostTime = micros() - BaseTime;
    if (result == SFUD_SUCCESS) {
        SERIAL.println("Read the flash data success.");
        SERIAL.println("Offset\t00\t01\t02\t03\t04\t05\t06\t07\t08\t09\t0A\t0B\t0C\t0D\t0E\t0F");
        for (i = 0; i < size; i++) {
            if (i % 16 == 0) {
                SERIAL.print("0x");
                SERIAL.print(addr + i,HEX);
                SERIAL.print("\t");
            }
            SERIAL.print(data[i],HEX);
            SERIAL.print("\t");
            if (((i + 1) % 16 == 0) || i == size - 1) {
                SERIAL.println("");
            }
        }
        SERIAL.println(" ");
    } else {
        SERIAL.println("Read the flash data failed.");
    }
    /* data check */
    for (i = 0; i < size; i++) {
        if (data[i] != i % 256) {
            SERIAL.println("Read and check write data has an error.");
			break;
        }
    }
    if (i == size) {
        SERIAL.println("The flash test is success.\r\n");
        SERIAL.print("read costTime: ");
        SERIAL.print(CostTime);
        SERIAL.println(" us");
    }
}