#include "ESP32APA102Driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_adc_cal.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "time.h"
#include "sys/time.h"
#include "esp_pthread.h"
#include "push_buttons.h"
#include "touch_button.h"

#define GPIO_INPUT_IO_0     25
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

#define SPP_TAG "SPP_ACCEPTOR_DEMO"
#define SPP_SERVER_NAME "SPP_SERVER"
#define EXCAMPLE_DEVICE_NAME "ESP_SPP_ACCEPTOR"
#define SPP_SHOW_DATA 0
#define SPP_SHOW_SPEED 1
#define SPP_SHOW_MODE SPP_SHOW_DATA    /*Choose show mode: show data or speed*/

static const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;

static struct timeval time_new, time_old;
static long data_num = 0;

static const esp_spp_sec_t sec_mask = ESP_SPP_SEC_AUTHENTICATE;
static const esp_spp_role_t role_slave = ESP_SPP_ROLE_SLAVE;

unsigned char tmpColour[8][3]={{0,0,0},{255,0,0},{0,255,0},{255,255,0},{0,255,255},{255,0,255},{255,255,255}};
struct apa102LEDStrip leds;

uint32_t hand;


static void print_speed(void)
{
    float time_old_s = time_old.tv_sec + time_old.tv_usec / 1000000.0;
    float time_new_s = time_new.tv_sec + time_new.tv_usec / 1000000.0;
    float time_interval = time_new_s - time_old_s;
    float speed = data_num * 8 / time_interval / 1000.0;
    ESP_LOGI(SPP_TAG, "speed(%fs ~ %fs): %f kbit/s" , time_old_s, time_new_s, speed);
    data_num = 0;
    time_old.tv_sec = time_new.tv_sec;
    time_old.tv_usec = time_new.tv_usec;
}

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) //, char *ch = "response undefined\n")
{
    char buf[1024];
    char spp_data[256];
    hand=param->write.handle;
    switch (event) {
    case ESP_SPP_INIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
        esp_bt_dev_set_device_name(EXCAMPLE_DEVICE_NAME);
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        esp_spp_start_srv(sec_mask,role_slave, 0, SPP_SERVER_NAME);
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT");
        break;
    case ESP_SPP_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT");
        break;
    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT");
        break;
    case ESP_SPP_START_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_START_EVT");
        break;
    case ESP_SPP_CL_INIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CL_INIT_EVT");
        break;
    case ESP_SPP_DATA_IND_EVT:
#if (SPP_SHOW_MODE == SPP_SHOW_DATA)
        ESP_LOGI(SPP_TAG, "ESP_SPP_DATA_IND_EVT len=%d handle=%d",
                 param->data_ind.len, param->data_ind.handle);
        if (param->data_ind.len < 1023) {
            snprintf(buf, (size_t)param->data_ind.len, (char *)param->data_ind.data);
            printf("%s\n", buf);
            size_t length = strlen(buf);
            size_t i = 0; 
            for (; i < length; i++) {
                //int x = atoi(buf[i]);
                printf("%c\n", buf[i]);
                int x = buf[i] - '0';    
                setPixel(&leds, i, tmpColour[x],255);
            }
            renderLEDs();
            sprintf(spp_data, "Receined characters: %d\n", getcont());
            esp_spp_write(param->write.handle, strlen(spp_data), (uint8_t *)spp_data);
        }
        else {
            esp_log_buffer_hex("",param->data_ind.data,param->data_ind.len);
        }
#else
        gettimeofday(&time_new, NULL);
        data_num += param->data_ind.len;
        if (time_new.tv_sec - time_old.tv_sec >= 3) {
            print_speed();
        }
#endif
        break;
    case ESP_SPP_CONG_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CONG_EVT");
        break;
    case ESP_SPP_WRITE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_WRITE_EVT");
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT");
        gettimeofday(&time_old, NULL);
        break;
    default:
        break;
    }
}

void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_BT_GAP_AUTH_CMPL_EVT:{
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(SPP_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            esp_log_buffer_hex(SPP_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
        } else {
            ESP_LOGE(SPP_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }
    case ESP_BT_GAP_PIN_REQ_EVT:{
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
        if (param->pin_req.min_16_digit) {
            ESP_LOGI(SPP_TAG, "Input pin code: 0000 0000 0000 0000");
            esp_bt_pin_code_t pin_code = {0};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
        } else {
            ESP_LOGI(SPP_TAG, "Input pin code: 1234");
            esp_bt_pin_code_t pin_code;
            pin_code[0] = '1';
            pin_code[1] = '2';
            pin_code[2] = '3';
            pin_code[3] = '4';
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
        }
        break;
    }

#if (CONFIG_BT_SSP_ENABLED == true)
    case ESP_BT_GAP_CFM_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", param->cfm_req.num_val);
        esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
        break;
    case ESP_BT_GAP_KEY_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;
#endif

    default: {
        ESP_LOGI(SPP_TAG, "event: %d", event);
        break;
    }
    }
    return;
}

static xQueueHandle gpio_evt_queue = NULL;

/*void send_by_BT(char **ch){
    esp_spp_cb(ESP_SPP_DATA_IND_EVT, param, ch);
}*/

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

unsigned char colourList[9*3]={maxValuePerColour,0,0, maxValuePerColour,maxValuePerColour,0, 0,maxValuePerColour,0, 0,maxValuePerColour,maxValuePerColour, 0,0,maxValuePerColour, maxValuePerColour,0,maxValuePerColour, maxValuePerColour,maxValuePerColour,maxValuePerColour, maxValuePerColour,0,0, 0,0,0};


struct colourObject dynColObject;

//SPI Vars
spi_device_handle_t spi;
spi_transaction_t spiTransObject;
esp_err_t reto;
spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;
xQueueHandle demo_queue;
int cont;
int activated;
int donecont=1;
int doneactivated = 1;

void tx_task3(void *arg) {

	while (1) {
		cont=getcont();s
        if(cont==1 && donecont==1){
            //printf("up");
            donecont=0;
            char spp_data[256];
            sprintf(spp_data, "pause");//param->data_ind.len);
            esp_spp_write(hand, strlen(spp_data), (uint8_t *)spp_data);
            vTaskDelay(300 / portTICK_RATE_MS);
        }
        else
        {
            donecont=1;
        }
        
        vTaskDelay(10 / portTICK_RATE_MS);
	}

}

void tx_task4(void *arg) {

	while (1) {
		activated=getactivated();
        if(activated==1 && doneactivated==1){
            //printf("up");
            doneactivated=0;
            char spp_data[256];
            sprintf(spp_data, "next");//param->data_ind.len);
            esp_spp_write(hand, strlen(spp_data), (uint8_t *)spp_data);
            vTaskDelay(3000 / portTICK_RATE_MS);
        }else
        {
            doneactivated=1;
        }
        
        vTaskDelay(10 / portTICK_RATE_MS);
	}

}


void apa102_main()
{
	esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s gap register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_spp_init(esp_spp_mode)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp init failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

#if (CONFIG_BT_SSP_ENABLED == true)
    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
#endif

    /*
     * Set default parameters for Legacy Pairing
     * Use variable pin, input pin code when pairing
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
    esp_bt_pin_code_t pin_code;
    esp_bt_gap_set_pin(pin_type, 0, pin_code);

	
	int     read_raw;
    esp_err_t r;
    gpio_config_t io_conf;

    gpio_num_t adc_gpio_num;

    vTaskDelay(2 * portTICK_PERIOD_MS);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    //xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    //gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin
    //gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

    int cont = 0;
	
	unsigned short int cIndex=0, cnt=0;
	
    printf("\r\n\r\n\r\nHello Pixels!\n");
	
	//Set up SPI
	printf("Setting up SPI now\t[%d]\r\n", setupSPI());
	
	//set up LED object
	printf("Creating led object...\t");
	initLEDs(&leds, totalPixels, bytesPerPixel,1);
	printf("Frame Length\t%d\r\n", leds._frameLength);

	//set up colours
	initComplexColourObject(&dynColObject, maxValuePerColour, 9, colourList);
		
	//Set up SPI tx/rx storage Object
	memset(&spiTransObject, 0, sizeof(spiTransObject));
	spiTransObject.length = leds._frameLength*8;
	spiTransObject.tx_buffer = leds.LEDs;
	printf("SPI Object Initilized...\r\n");


	printf("Sending SPI data block to clear all pixels....\r\n");
	spi_device_queue_trans(spi, &spiTransObject, portMAX_DELAY);
	printf("Pixels Cleared!\r\n");
	for(cnt=totalPixels-1; cnt>0; cnt--)
		{
			//getPixel(&leds, cnt-1, tmpColour);
			setPixel(&leds, cnt, tmpColour[1],255);
		}
		renderLEDs();

		vTaskDelay(2 * portTICK_PERIOD_MS);
	
	cnt=totalPixels-1;
	int orange=0;
	int down =1;
	int stop=0;
    xTaskCreate(tx_task3, "tx_task3", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 8, NULL);
    xTaskCreate(tx_task4, "tx_task4", CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 8, NULL);
}

void renderLEDs()
{
	spi_device_queue_trans(spi, &spiTransObject, portMAX_DELAY);
}

int setupSPI()
{
	//Set up the Bus Config struct
	buscfg.miso_io_num=-1;
	buscfg.mosi_io_num=PIN_NUM_MOSI;
	buscfg.sclk_io_num=PIN_NUM_CLK;
	buscfg.quadwp_io_num=-1;
	buscfg.quadhd_io_num=-1;
	buscfg.max_transfer_sz=maxSPIFrameInBytes;
	
	//Set up the SPI Device Configuration Struct
	devcfg.clock_speed_hz=maxSPIFrequency;
	devcfg.mode=0;                        
	devcfg.spics_io_num=-1;             
	devcfg.queue_size=1;

	//Initialize the SPI driver
	reto=spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(reto);	
	//Add SPI port to bus
	reto=spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
	ESP_ERROR_CHECK(reto);
	return reto;
}

