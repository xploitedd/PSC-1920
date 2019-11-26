#include<stdio.h>

struct weather {
    float temperature;
    float wind;
    float humidity;
};

void weather_average(struct weather *average, const struct weather *weathers[], size_t nweathers) {
    float av_temp = 0;
    float av_wind = 0;
    float av_humi = 0;
    // add each parameter
    for (int i = 0; i < nweathers; ++i) {
        av_temp += weathers[i]->temperature;
        av_wind += weathers[i]->wind;
        av_humi += weathers[i]->humidity;
    }

    // calculate the average
    average->temperature = av_temp / nweathers;
    average->wind = av_wind / nweathers;
    average->humidity = av_humi / nweathers;
}

int main() {
    struct weather w1 = { 1.0, 3.0, 2.0 };
    struct weather w2 = { 2.0, 1.0, 4.0 };
    struct weather w3 = { 3.0, 5.0, 6.0 };
    struct weather average;
    const struct weather *weathers[] = { &w1, &w2, &w3 };
    /* expected result:
        temperature: 2f
        wind: 3f
        humidity: 4f
    */
    weather_average(&average, weathers, 3);
    printf("Average:\nTemp:\t\t%f\nWind:\t\t%f\nHumidity:\t%f\n",
            average.temperature, average.wind, average.humidity);
    return 0;
}