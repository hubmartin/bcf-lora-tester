#include <application.h>

// LED instance
bc_led_t led;

bc_button_t button;

// LoRa instance
bc_cmwx1zzabz_t lora;

char deveui[20] = "...";
char devaddr[20] = "...";

void lcd_redraw(char *state)
{
    bc_module_lcd_clear();

    bc_module_lcd_set_font(&bc_font_ubuntu_15);
    bc_module_lcd_draw_string(10, 5, "LoRaWAN Tester", true);
    bc_module_lcd_draw_line(5, 20, 115, 20, true);

    bc_module_lcd_draw_string(1, 24, deveui, true);
    bc_module_lcd_draw_string(5, 36, devaddr, true);

    bc_module_lcd_draw_string(5, 50, state, true);

    bc_module_lcd_update();
}

void lora_callback(bc_cmwx1zzabz_t *self, bc_cmwx1zzabz_event_t event, void *event_param)
{
    if (event == BC_CMWX1ZZABZ_EVENT_READY)
    {

        bc_cmwx1zzabz_get_deveui(&lora, (char*)deveui);
        bc_cmwx1zzabz_get_devaddr(&lora, (char*)devaddr);

        bc_log_debug("deveui: %s", deveui);
        bc_log_debug("devaddr: %s", devaddr);

        bc_led_pulse(&led, 50);
    }

    if (event == BC_CMWX1ZZABZ_EVENT_ERROR)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK_FAST);
        bc_log_debug("Modem ERROR");
        lcd_redraw("Modem ERROR");
    }

    if (event == BC_CMWX1ZZABZ_EVENT_JOIN_ERROR)
    {
        bc_log_debug("Join ERROR");
        lcd_redraw("Join ERR");
        bc_led_set_mode(&led, BC_LED_MODE_BLINK_FAST);
    }

    if (event == BC_CMWX1ZZABZ_EVENT_JOIN_SUCCESS)
    {
        bc_log_debug("Join OK");
        lcd_redraw("Join OK");
    }

    if (event == BC_CMWX1ZZABZ_EVENT_SEND_MESSAGE_START)
    {
        bc_led_set_mode(&led, BC_LED_MODE_ON);
        bc_log_debug("Sending start");
        lcd_redraw("Sending start");
    }

    if (event == BC_CMWX1ZZABZ_EVENT_SEND_MESSAGE_DONE)
    {
        bc_led_set_mode(&led, BC_LED_MODE_OFF);
        bc_log_debug("Sending done");
        lcd_redraw("Sending done");
    }

    if (event == BC_CMWX1ZZABZ_EVENT_MESSAGE_RECEIVED)
    {
        volatile uint8_t port = bc_cmwx1zzabz_get_received_message_port(self);
        volatile uint32_t length = bc_cmwx1zzabz_get_received_message_length(self);

        uint8_t msg_buffer[51];
        volatile uint32_t len = bc_cmwx1zzabz_get_received_message_data(self, msg_buffer, sizeof(msg_buffer));

        bc_led_pulse(&led, 800);

        bc_log_debug("Message received");
        lcd_redraw("Message received");

        (void) len;
        (void) length;
        (void) port;
    }
}

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_pulse(&led, 100);

        uint8_t buffer[] = {'A', 'B', 'C'};
        bc_cmwx1zzabz_send_message(&lora, buffer, sizeof(buffer) );

    } else if (event == BC_BUTTON_EVENT_HOLD ) {
        bc_cmwx1zzabz_join(&lora);
    }
}

void application_init(void)
{
    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_pulse(&led, 100);

    bc_button_init(&button, BC_GPIO_BUTTON,  BC_GPIO_PULL_DOWN, 0);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_cmwx1zzabz_init(&lora, BC_UART_UART1);
    bc_cmwx1zzabz_set_event_handler(&lora, lora_callback, NULL);

    bc_cmwx1zzabz_set_devaddr(&lora, "26011E77");
    bc_cmwx1zzabz_set_appeui(&lora, "70B3D57ED001013D");
    bc_cmwx1zzabz_set_appkey(&lora, "2B7E151628AED2A6ABF7158809CF4F3C");

    //bc_cmwx1zzabz_set_appskey(&lora, "4357FAD1D73C3B7D4C2EDBAE6EBDA740");
    //bc_cmwx1zzabz_set_nwkskey(&lora, "4457FAD1D73C3B7D4C2EDBAE6EBDA740");

    bc_cmwx1zzabz_set_band(&lora, BC_CMWX1ZZABZ_CONFIG_BAND_EU868);
    bc_cmwx1zzabz_set_mode(&lora, BC_CMWX1ZZABZ_CONFIG_MODE_OTAA);
    bc_cmwx1zzabz_set_class(&lora, BC_CMWX1ZZABZ_CONFIG_CLASS_A);

    bc_log_init(BC_LOG_LEVEL_DEBUG, BC_LOG_TIMESTAMP_REL);

    bc_cmwx1zzabz_join(&lora);

    bc_module_lcd_init();
    lcd_redraw("JOIN request...");
}

/*
void application_task()
{
    if (!bc_cmwx1zzabz_is_ready(&lora))
    {
        bc_scheduler_plan_current_relative(200);
        return;
    }

    bc_cmwx1zzabz_join(&lora);

}*/
