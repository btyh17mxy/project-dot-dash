#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    /* const uint LED_PIN = PICO_DEFAULT_LED_PIN; */
    const uint LED2_PIN = 15;
    const uint BUTTON_PIN = 14;

    /* gpio_init(LED_PIN); */
    gpio_init(LED2_PIN);
    gpio_init(BUTTON_PIN);

    /* gpio_set_dir(LED_PIN, GPIO_OUT); */
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    gpio_set_pulls(BUTTON_PIN, false, true);

    while (true) {
        if (gpio_get(BUTTON_PIN)) {
            /* gpio_put(LED_PIN, 1); */
            gpio_put(LED2_PIN, 1);
        } else {
            /* gpio_put(LED_PIN, 0); */
            gpio_put(LED2_PIN, 0);
        }
    }
    return 0;
}
