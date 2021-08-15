#include <stdio.h>
#include "pico/stdlib.h"   // stdlib 
#include "hardware/irq.h"  // interrupts
#include "hardware/pwm.h"  // pwm 
#include "hardware/sync.h" // wait for interrupt 
#include "sample.h"

#define LED_PIN 15
/* #define LED_PIN  PICO_DEFAULT_LED_PIN */
#define BUTTON_PIN 14
#define AUDIO_PIN 28
/* #define WAV_DATA_LENGTH 38151 */

void init_gpio() {
    gpio_init(LED_PIN);
    gpio_init(BUTTON_PIN);

    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    gpio_set_pulls(BUTTON_PIN, false, true);
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);
}

int wav_position = 0;
bool play_flag = true;
void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));    
    if (wav_position < (WAV_DATA_LENGTH<<3) - 1 && play_flag) { 
        // set pwm level 
        pwm_set_gpio_level(AUDIO_PIN, WAV_DATA[wav_position>>3]);
        /* pwm_set_gpio_level(AUDIO_PIN, 155); */  
        wav_position++;
    } else {
        // reset to start
        wav_position = 0;
    }
}

int main() {
    stdio_init_all();
    set_sys_clock_khz(176000, true); 
    init_gpio();
    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);

    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Setup PWM for audio output
    pwm_config config = pwm_get_default_config();
    /* Base clock 176,000,000 Hz divide by wrap 250 then the clock divider further divides
     * to set the interrupt rate. 
     * 
     * 11 KHz is fine for speech. Phone lines generally sample at 8 KHz
     * 
     * 
     * So clkdiv should be as follows for given sample rate
     *  8.0f for 11 KHz
     *  4.0f for 22 KHz
     *  2.0f for 44 KHz etc
     */
    pwm_config_set_clkdiv(&config, 8.0f); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);

    pwm_set_gpio_level(AUDIO_PIN, 0);

    while(1) {
        if (gpio_get(BUTTON_PIN)) {
            /* gpio_put(LED_PIN, 1); */
            /* irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); */ 
            gpio_put(LED_PIN, 1);
            play_flag = true;
        } else {
            /* gpio_put(LED_PIN, 0); */
            gpio_put(LED_PIN, 0);
            play_flag = false;
            /* irq_remove_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); */
        }
        __wfi(); // Wait for Interrupt
    }
    return 0;
}
