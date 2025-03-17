#include "Helpers.h"
#include "signals_arrays.h"

/**
 * @brief Simulate reading data from time
 * @param sample_rate Sample rate in Hz
 * @param time_start_ms Start time in millisecond
 * @return Number of samples
 */
int read_time_simulate(int sample_rate, int time_start_ms)
{
	int now_ms = EVE_millis();
	int duration_ms = now_ms - time_start_ms;
	int num_samples = duration_ms * sample_rate / 1000;
	//return 3;
	return num_samples;
}

/**
 * @brief Simulate ECG data
 * @param data Pointer to int array to store data
 * @param data_size Pointer to int to store size of data
 * @return Pixel x coordinate
 */
int new_data_heartbeat(SIGNALS_DATA_TYPE** data, int *data_size)
{
	int data_duration = 10 * 60; // unit: second
	int dividor = 2;
	int multipler = 3;
	SIGNALS_DATA_TYPE* sample_buffer = ecg_data;
	const int sample_total = sizeof(ecg_data) / sizeof(char);

	static uint32_t time_start_ms = 0;
	const int sample_per_second = sample_total / data_duration;
	static int sample_offset = 0;

	if (time_start_ms == 0)
	{
		time_start_ms = EVE_millis();
		for (int i = 0; i < sample_total; i++) {
			// sample_buffer[i] = sample_buffer[i] * multipler / dividor;
		}
	}

	int num_samples = read_time_simulate(sample_per_second, time_start_ms);
	if (num_samples > 0) {
		time_start_ms = EVE_millis();
		num_samples = min(num_samples, sample_total - sample_offset);
	}

	*data = &sample_buffer[sample_offset];
	*data_size = num_samples;

	// get sample (x,y) coordinate
	int pixel_y = num_samples;
	//int pixel_x = sample_buffer[(sample_offset + pixel_y) % sample_total] * multipler / dividor;
	sample_offset = (sample_offset + num_samples) % sample_total;
	return 0;
}


/**
 * @brief Simulate Plethysmography data
 * @param data Pointer to int array to store data
 * @param data_size Pointer to int to store size of data
 * @return Pixel x coordinate
 */
int new_data_pleth(SIGNALS_DATA_TYPE** data, int* data_size)
{
	int data_duration = 10*60; // unit: second
	int dividor = 2;
	int multipler = 1;
	SIGNALS_DATA_TYPE* sample_buffer = ppg_data;
	const int sample_total = sizeof(ppg_data) / sizeof(char);

	static uint32_t time_start_ms = 0;
	const int sample_per_second = sample_total / data_duration;
	static int sample_offset = 0;

	if (time_start_ms == 0)
	{
		time_start_ms = EVE_millis();
		for (int i = 0; i < sample_total; i++) {
			// sample_buffer[i] = sample_buffer[i] * multipler / dividor;
		}
	}

	int num_samples = read_time_simulate(sample_per_second, time_start_ms);
	if (num_samples > 0) {
		time_start_ms = EVE_millis();
		num_samples = min(num_samples, sample_total - sample_offset);
	}

	*data = &sample_buffer[sample_offset];
	*data_size = num_samples;

	// get sample (x,y) coordinate
	int pixel_y = num_samples;
	//int pixel_x = sample_buffer[(sample_offset + pixel_y) % sample_total] * multipler / dividor;
	sample_offset = (sample_offset + num_samples) % sample_total;
	return 0;
}

/**
 * @brief Simulate CO2 data
 * @param data Pointer to int array to store data
 * @param data_size Pointer to int to store size of data
 * @return Pixel x coordinate
 */
int new_data_co2(SIGNALS_DATA_TYPE** data, int* data_size)
{
	int data_duration = 10 * 60; // unit: second
	int dividor = 2;
	int multipler = 1;
	SIGNALS_DATA_TYPE* sample_buffer = co2_data;
	const int sample_total = sizeof(co2_data) / sizeof(char);

	static uint32_t time_start_ms = 0;
	const int sample_per_second = sample_total / data_duration;
	static int sample_offset = 0;

	if (time_start_ms == 0)
	{
		time_start_ms = EVE_millis();
	}

	int num_samples = read_time_simulate(sample_per_second, time_start_ms);
	if (num_samples > 0) {
		time_start_ms = EVE_millis();
		num_samples = min(num_samples, sample_total - sample_offset);
	}

	*data = &sample_buffer[sample_offset];
	*data_size = num_samples;

	// get sample (x,y) coordinate
	int pixel_y = num_samples;
	//int pixel_x = sample_buffer[(sample_offset + pixel_y) % sample_total] * multipler / dividor;
	sample_offset = (sample_offset + num_samples) % sample_total;
	return 0;
}
