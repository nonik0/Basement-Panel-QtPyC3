#include <Adafruit_NeoMatrix.h>
#include <Fonts/TomThumb.h>

#define PIN A3

extern volatile bool display;
TaskHandle_t matrix5x5TaskHandle = NULL;
extern volatile bool display;
Adafruit_NeoMatrix matrix5x5(5, 5, PIN,
                             NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
                                 NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                             NEO_GRB + NEO_KHZ800);

char *matrix5x5Message;
const uint16_t colors[] = {
    matrix5x5.Color(255, 60, 0), matrix5x5.Color(255, 120, 0), matrix5x5.Color(255, 255, 0)};
uint16_t message_width;               // Computed in setup() below
int matrix5x5_x = matrix5x5.width();  // Start with message off right edge
int matrix5x5_y = matrix5x5.height(); // With custom fonts, y is the baseline, not top
int matrix5x5_pass = 0;               // Counts through the colors[] array

void Matrix5x5Task(void *parameters);

void Matrix5x5Setup()
{
    matrix5x5Message = new char[100];
    strcpy(matrix5x5Message, "BEAU IN TOW!");

    matrix5x5.begin();
    matrix5x5.setBrightness(15);
    matrix5x5.setFont(&TomThumb);
    matrix5x5.setTextWrap(false);
    matrix5x5.setTextColor(colors[0]);

    int16_t d1;
    uint16_t d2;
    matrix5x5.getTextBounds(matrix5x5Message, 0, 0, &d1, &d1, &message_width, &d2);

    xTaskCreate(Matrix5x5Task, "Matrix5x5Task", 4096, NULL, 10, &matrix5x5TaskHandle);
}

void Matrix5x5Task(void *parameters)
{
    while (1)
    {
        if (!display)
        {
            matrix5x5.fillScreen(0);
            matrix5x5.show();
            delay(100);
            continue;
        }

        matrix5x5.fillScreen(0);
        matrix5x5.setCursor(matrix5x5_x, matrix5x5_y);
        matrix5x5.print(matrix5x5Message);
        matrix5x5.show();

        if (--matrix5x5_x < -message_width)
        {
            matrix5x5_x = matrix5x5.width();
            if (++matrix5x5_pass >= 3)
                matrix5x5_pass = 0;
            matrix5x5.setTextColor(colors[matrix5x5_pass]);
        }

        delay(100);
    }
}