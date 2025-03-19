/**
 * @file Helpers.c
 * @brief Helper functions and utilities
 *
 * This file contains various helper functions and utility macros that are
 * used throughout the application to perform common operations efficiently.
 * These functions aim to simplify the codebase by abstracting repetitive or
 * complex tasks into reusable components.
 *
 * @details The implementation covers a range of functionalities, from basic
 * mathematical operations and data formatting to hardware interface utilities.
 * Each function is designed to be robust and performant, ensuring reliability
 * and consistency across the application.
 *
 * @date 2025
 * @license MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "Helpers.h"
#include "Common.h"

/**
 * @brief Get the current Frames Per Second (FPS) value
 *
 * @details This function uses the EVE_millis() function to measure the time
 * since the last call to getFPS() and calculates the FPS based on the frame
 * count and time elapsed. The first call to getFPS() will always return 0,
 * since there is no previous frame to measure against.
 *
 * @return int32_t current FPS value
 */
int32_t getFPS()
{
	static unsigned long last_time_ms = 0; // Timestamp of the last frame
	static unsigned long frame_count = 0;  // Total frames since start
	static int fps = 0;					   // Calculated FPS

	unsigned long current_time_ms = EVE_millis();

	if (last_time_ms == 0)
	{
		// Initialize on the first frame
		last_time_ms = current_time_ms;
		return 0;
	}

	// Calculate time since last frame
	unsigned long duration_ms = max(1, current_time_ms - last_time_ms);
	fps = (frame_count * 1000) / duration_ms;
	frame_count++;

	return fps;
}

// Static variables to store initial date-time
static int init_dd = 0, init_mm = 0, init_yyyy = 0;
static int init_hh = 0, init_m = 0, init_ss = 0, init_ms = 0, init_eve_millis = 0;
static int current_dd = 0, current_mm = 0, current_yyyy = 0;
static int current_hh = 0, current_m = 0;
static int current_ss = 0, current_ms = 0;

#define MILLIS_PER_SECOND 1000
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR 60
#define MILLIS_PER_DAY (24 * 60 * 60 * 1000)

/**
 * @brief Preset the current date by calculating the elapsed days since
 * initialization.
 *
 * @details This function is used to update the current date by calculating the
 * elapsed days since the initialization of the date and month. It also handles
 * leap years and ensures that the date remains valid.
 */
static void dd_mm_yyyy_preset()
{
	unsigned long elapsed_millis = EVE_millis() - init_eve_millis;
	int elapsed_days = elapsed_millis / MILLIS_PER_DAY;

	// Recalculate date if it hasn't been updated yet
	if (current_dd == 0 && current_mm == 0 && current_yyyy == 0)
	{
		current_dd = init_dd + elapsed_days;
		current_mm = init_mm;
		current_yyyy = init_yyyy;

		static int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		while (1)
		{
			int days_in_current_month = days_in_month[current_mm - 1];

			// Adjust for leap year in February
			if (current_mm == 2 && ((current_yyyy % 4 == 0 && current_yyyy % 100 != 0) || (current_yyyy % 400 == 0)))
			{
				days_in_current_month = 29;
			}

			if (current_dd > days_in_current_month)
			{
				current_dd -= days_in_current_month;
				current_mm++;
				if (current_mm > 12)
				{
					current_mm = 1;
					current_yyyy++;
				}
			}
			else
			{
				break;
			}
		}
	}
}

/**
 * @brief Get the current date formatted as "dd-mm-yyyy"
 *
 * @details This function presets the current date and returns it
 * formatted as a string in the format "dd-mm-yyyy".
 *
 * @return char* pointer to a static character array containing the date string
 */
char *dd_mm_yyyy()
{
	dd_mm_yyyy_preset();
	static char dateString[12]; // Buffer for "dd-mm-yyyy"
	snprintf(dateString, sizeof(dateString), "%02d-%02d-%04d", current_dd, current_mm, current_yyyy);
	return dateString;
}

/**
 * @brief Get the current date formatted as "dd-mmm-yyyy"
 *
 * @details This function presets the current date and returns it
 * formatted as a string in the format "dd-mmm-yyyy" where "mmm" is the
 * abbreviated month name.
 *
 * @return char* pointer to a static character array containing the date string
 */
char *dd_mmm_yyyy()
{
	dd_mm_yyyy_preset();

	static char dateString[12]; // Buffer for "dd-mmm-yyyy"
	const char *month3_str[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if (current_mm < 1 || current_mm > 12)
	{
		return "Invalid Date";
	}

	snprintf(dateString, sizeof(dateString), "%02d-%s-%04d", current_dd, month3_str[current_mm - 1], current_yyyy);
	return dateString;
}

/**
 * @brief Get the current date formatted as "dd-Month-yyyy"
 *
 * @details This function presets the current date and returns it
 * formatted as a string in the format "dd-Month-yyyy" where "Month" is the
 * full month name.
 *
 * @return char* pointer to a static character array containing the date string
 */
char *dd_month_yyyy()
{
	dd_mm_yyyy_preset();

	static char dateString[20]; // Buffer for "dd-Month-yyyy"
	const char *months_str[] = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};

	if (current_mm < 1 || current_mm > 12)
	{
		return "Invalid Date";
	}

	snprintf(dateString, sizeof(dateString), "%02d-%s-%04d", current_dd, months_str[current_mm - 1], current_yyyy);
	return dateString;
}

/**
 * @brief Preset the current time by calculating the elapsed time since
 * initialization.
 *
 * @details This function presets the current time by calculating the elapsed
 * time since initialization. It takes into account the initial time and the
 * elapsed time, and updates the current time accordingly.
 */
static void hh_mm_preset()
{
	// Function to format the time
	// Calculate total elapsed time
	unsigned long elapsed_millis = EVE_millis() - init_eve_millis;
	unsigned long total_ms = init_ms + elapsed_millis;
	unsigned long total_seconds = init_ss + total_ms / MILLIS_PER_SECOND;
	int total_minutes = init_m + total_seconds / SECONDS_PER_MINUTE;

	current_hh = init_hh + total_minutes / MINUTES_PER_HOUR;
	current_m = total_minutes % MINUTES_PER_HOUR;
	current_ss = total_seconds % SECONDS_PER_MINUTE; // Remaining seconds
	current_ms = total_ms % MILLIS_PER_SECOND;		 // Remaining milliseconds

	if (current_hh >= 24)
	{
		current_hh %= 24;
	}
}

/**
 * @brief Get the current time formatted as "hh:mm"
 *
 * @details This function presets the current time by calculating the elapsed
 * time since initialization. It takes into account the initial time and the
 * elapsed time, and updates the current time accordingly.
 *
 * @return char* pointer to a static character array containing the time string
 */
char *hh_mm()
{
	hh_mm_preset();
	static char timeString[6]; // Buffer for "hh:mm"
	snprintf(timeString, sizeof(timeString), "%02d:%02d", current_hh, current_m);
	return timeString;
}

/**
 * @brief Get the current time formatted as "hh:mm:ss"
 *
 * @details This function presets the current time by calculating the elapsed
 * time since initialization. It takes into account the initial time and the
 * elapsed time, and updates the current time accordingly.
 *
 * @return char* pointer to a static character array containing the time string
 */
char *hh_mm_ss()
{
	hh_mm_preset();
	static char timeString[9]; // Buffer for "hh:mm:ss"
	snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", current_hh, current_m, current_ss);
	return timeString;
}

/**
 * @brief Get the current time formatted as "hh:mm:ss:ms"
 *
 * @details This function presets the current time by calculating the elapsed
 * time since initialization. It takes into account the initial time and the
 * elapsed time, and updates the current time accordingly.
 *
 * @return char* pointer to a static character array containing the time string
 */
char *hh_mm_ss_ms()
{
	hh_mm_preset();
	static char timeString[12]; // Buffer for "hh:mm:ss:ms"
	snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d:%02d", current_hh, current_m, current_ss, current_ms);
	return timeString;
}

/**
 * @brief Get the current date-time formatted as "dd-mm-yyyy hh:mm:ss:ms"
 *
 * @details This function presets the current date and time by calculating the
 * elapsed time since initialization. It takes into account the initial date and
 * time, and the elapsed time, and updates the current date and time accordingly.
 *
 * @return char* pointer to a static character array containing the date-time string
 */
char *dd_mm_yyyy_hh_m_ss_ms()
{
	static char fullDateTimeString[30]; // Buffer for "dd-mm-yyyy hh:mm:ss:ms"

	// Calculate total elapsed time
	unsigned long elapsed_millis = EVE_millis() - init_eve_millis;
	unsigned long total_ms = init_ms + elapsed_millis;
	unsigned long total_seconds = init_ss + total_ms / MILLIS_PER_SECOND;

	current_ms = total_ms % MILLIS_PER_SECOND;		 // Remaining milliseconds
	current_ss = total_seconds % SECONDS_PER_MINUTE; // Remaining seconds

	// Get the date and time parts
	char *datePart = dd_mm_yyyy();
	char *timePart = hh_mm();

	// Combine all parts into a full date-time string
	snprintf(fullDateTimeString, sizeof(fullDateTimeString), "%s %s:%02d:%03d", datePart, timePart, current_ss, current_ms);

	return fullDateTimeString;
}

/**
 * @brief Initialize the date-time system.
 *
 * @details This function initializes the date-time system with the given date
 * and time values. It also stores the current EVE_millis() value as the initial
 * timestamp.
 *
 * @param dd Day of the month.
 * @param mm Month of the year.
 * @param yyyy Year.
 * @param hh Hour.
 * @param m Minute.
 * @param ss Second.
 * @param ms Millisecond.
 */
void init_datetime(int dd, int mm, int yyyy, int hh, int m, int ss, int ms)
{
	init_dd = dd;
	init_mm = mm;
	init_yyyy = yyyy;
	init_hh = hh;
	init_m = m;
	init_ss = ss;
	init_ms = ms;

	current_dd = dd;
	current_mm = mm;
	current_yyyy = yyyy;
	current_hh = hh;
	current_m = m;
	current_ss = ss;
	current_ms = ms;

	init_eve_millis = EVE_millis();
}

/**
 * @brief Get the current day of the month
 *
 * @details This function returns the current day of the month, as set by the
 * init_datetime() function.
 *
 * @return uint32_t current day of the month
 */
uint32_t get_dd()
{
	return current_dd;
}

/**
 * @brief Get the current month of the year
 *
 * @details This function returns the current month of the year, as set by the
 * init_datetime() function.
 *
 * @return uint32_t current month of the year
 */
uint32_t get_mm()
{
	return current_mm;
}

/**
 * @brief Get the current year
 *
 * @details This function returns the current year, as set by the
 * init_datetime() function.
 *
 * @return uint32_t current year
 */
uint32_t get_yyyy()
{
	return current_yyyy;
}

/**
 * @brief Get the current hour
 *
 * @details This function returns the current hour, as set by the
 * init_datetime() function.
 *
 * @return uint32_t current hour
 */
uint32_t get_hh()
{
	return current_hh;
}

/**
 * @brief Get the current minute
 *
 * @details This function returns the current minute, as set by the
 * init_datetime() function.
 *
 * @return uint32_t current minute
 */

uint32_t get_mt()
{
	return current_m;
}

/**
 * @brief Get the current second
 *
 * @details This function returns the current second, as set by the
 * init_datetime() function.
 *
 * @return uint32_t current second
 */
uint32_t get_ss()
{
	return current_ss;
}

/**
 * @brief Get the current millisecond
 *
 * @details This function returns the current millisecond, as set by the
 * init_datetime() function.
 *
 * @return uint32_t current millisecond
 */
uint32_t get_ms()
{
	return current_ms;
}

/**
 * @brief Returns a random number within a given range
 *
 * @details This function uses the EVE_millis() function to generate a
 * random number within the given range. The EVE_millis() function is
 * used to get the current time in milliseconds.
 *
 * @param range The range of the random number
 *
 * @return A random number in the range [0, range)
 */
int app_random(int range)
{
	if (range <= 0)
		return 0;

	unsigned long millis = EVE_millis() + 17;
	return millis % range;
}

/**
 * @brief Save a buffer to a file
 *
 * @details This function opens a file in binary write mode and writes the
 * specified buffer to it. If the file cannot be opened or the buffer cannot
 * be fully written, it returns an error code.
 *
 * @param filename The name of the file to save the buffer to.
 * @param buffer The buffer containing data to be saved.
 * @param buffer_size The size of the buffer in bytes.
 *
 * @return int 0 on success, -1 on failure.
 */

int save_buffer_to_file(const char *filename, const void *buffer, size_t buffer_size)
{
	// Open the file for writing in binary mode
	FILE *file = fopen(filename, "wb");
	if (file == NULL)
	{
		perror("Error opening file");
		return -1;
	}

	// Write the buffer to the file
	size_t written = fwrite(buffer, 1, buffer_size, file);
	if (written != buffer_size)
	{
		perror("Error writing to file");
		fclose(file);
		return -1;
	}

	// Close the file
	fclose(file);
	return 0;
}

/**
 * @brief Takes a screenshot of the display and saves it to a file
 *
 * @details This function is only available on BT82X devices and uses the
 * EVE_CoCmd_renderTarget command to capture the display contents to a DDR
 * buffer. The buffer is then read using EVE_Hal_rdMem and saved to a file.
 *
 * @param phost Pointer to Hal context
 * @param name Name of the file to save the screenshot to
 * @param ramg_render RAM_G address of the buffer image
 */
void take_ddr_screenshot(EVE_HalContext *phost, char *name, int ramg_render)
{
#if defined(BT82X_ENABLE)
	EVE_CoCmd_renderTarget(phost, ramg_render, RGB8, phost->Width, phost->Height);
	const int bbp_rgb8 = 24;
	int sc_size = phost->Width * phost->Height * bbp_rgb8 / 8;
	char *buffer_screenshot = malloc(sc_size);
	EVE_Hal_rdMem(phost, buffer_screenshot, offset, sc_size);
	save_buffer_to_file("demo_debside.raw", buffer_screenshot, sc_size);
#endif
}

static uint32_t transfer_addr0 = 0;
static uint32_t accumulator32 = 0;
static uint32_t byte_count = 0;

/**
 * @brief Flushes the current data transfer to the specified address.
 *
 * @details Writes the accumulated 32-bit data to the given address, and
 * resets the transfer state including the address, byte count, and accumulator.
 *
 * @param phost Pointer to the HAL context.
 * @param addr Address to which the accumulated data is written.
 * @param data32 32-bit data to be written.
 */

void bt82x_wr_flush(EVE_HalContext *phost, uint32_t addr, uint8_t data32)
{
	EVE_Hal_wr32(phost, addr, data32);
	transfer_addr0 = 0;
	byte_count = 0;
	accumulator32 = 0;
}

/**
 * @brief Writes a block of data to the given address.
 *
 * @details Iterates the given data array and writes each byte to the
 * corresponding address. This function can be used to transfer data in bulk
 * to the external memory.
 *
 * @param phost Pointer to the HAL context.
 * @param addr Base address to which the data is written.
 * @param data Array of bytes to be written.
 * @param data_size Number of bytes to be written.
 */
void bt82x_wr_mem(EVE_HalContext *phost, uint32_t addr, uint8_t *data, uint32_t data_size)
{
	for (int i = 0; i < data_size; i++)
	{
		bt82x_wr8(phost, addr + i, (uint8_t)(data[i] & 0xFF));
	}
}

/**
 * @brief Writes a 32-bit data to the given address.
 *
 * @details Breaks down the 32-bit data into four 8-bit bytes, and writes each
 * byte to the corresponding address.
 *
 * @param phost Pointer to the HAL context.
 * @param addr Base address to which the data is written.
 * @param data32 32-bit data to be written.
 */
void bt82x_wr32(EVE_HalContext *phost, uint32_t addr, uint8_t data32)
{
	// Transfer the lowest byte
	bt82x_wr8(phost, addr, (uint8_t)(data32 & 0xFF));

	// Transfer the second byte
	bt82x_wr8(phost, addr + 1, (uint8_t)((data32 >> 8) & 0xFF));

	// Transfer the third byte
	bt82x_wr8(phost, addr + 2, (uint8_t)((data32 >> 16) & 0xFF));

	// Transfer the highest byte
	bt82x_wr8(phost, addr + 3, (uint8_t)((data32 >> 24) & 0xFF));
}

/**
 * @brief Writes a 16-bit data to the specified address.
 *
 * @details This function splits the 16-bit data into two 8-bit bytes and writes
 * each byte to consecutive addresses starting from the specified base address.
 *
 * @param phost Pointer to the HAL context.
 * @param addr Base address to which the 16-bit data is written.
 * @param data16 16-bit data to be written.
 */

void bt82x_wr16(EVE_HalContext *phost, uint32_t addr, uint16_t data16)
{
	// Transfer the lower byte
	bt82x_wr8(phost, addr, (uint8_t)(data16 & 0xFF));

	// Transfer the higher byte
	bt82x_wr8(phost, addr + 1, (uint8_t)((data16 >> 8) & 0xFF));
}

/**
 * @brief Writes a byte to the specified address.
 *
 * @details This function accumulates up to 4 bytes in a 32-bit register and
 * flushes the register when 4 bytes are collected or when the address is not
 * contiguous. This is used to reduce the number of transfers to the
 * co-processor.
 *
 * @param phost Pointer to the HAL context.
 * @param addr Address to which the byte is written.
 * @param data8 8-bit data to be written.
 */
void bt82x_wr8(EVE_HalContext *phost, uint32_t addr, uint8_t data8)
{
	// Check if address is contiguous
	if (byte_count > 0 && addr != (transfer_addr0 + byte_count))
	{
		// Flush the current transfer as the address is not contiguous
		bt82x_wr_flush(phost, transfer_addr0, accumulator32);

		// Reset the accumulator and byte count
		accumulator32 = 0;
		byte_count = 0;
	}

	// Set the new starting address if this is a new transfer
	if (byte_count == 0)
	{
		transfer_addr0 = addr;
	}

	// Add the byte to the accumulator
	accumulator32 |= ((uint32_t)data8 << (8 * byte_count));
	byte_count++;

	// Flush if 4 bytes are collected
	if (byte_count == 4)
	{
		bt82x_wr_flush(phost, transfer_addr0, accumulator32);

		// Reset the accumulator and byte count after flushing
		accumulator32 = 0;
		byte_count = 0;
	}
}
