/* *
 * Arquivo com inicialização da OLED na placa
 *
 * Passos:
 *  1. Importar
 *  2. Init na main
 *  3. Converter pra string com sprintf
 *  4. Usar draw_string
 *
 */

// Includes OLED
#include "gfx_mono_text.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "sysfont.h"

gfx_mono_ssd1306_init();

char stringify[20];
sprintf(stringify, "%.5f", 1.0 / delay);
gfx_mono_draw_string(stringify, 0, 0, &sysfont);