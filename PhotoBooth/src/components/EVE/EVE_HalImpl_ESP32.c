/**
 * @file EVE_HalImpl_ESP32.c
 * @brief Eve_Hal framework APIs for RP2040 host platform
 *
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 *
 * @date 2024
 *
 * MIT License
 *
 * Copyright (c) [2019] [Bridgetek Pte Ltd (BRTChip)]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "EVE_HalImpl.h"
#include "EVE_Platform.h"
#if defined(ESP32_PLATFORM)

/*********
** INIT **
*********/
spi_device_handle_t spi;
bool spi_inited = false;

// Global buffer and its current size
#define GLOBAL_BUFFER_SIZE 8 * 2014
uint8_t globalBuffer[GLOBAL_BUFFER_SIZE];
uint32_t globalBufferSize = 0;

void openBus(EVE_HalContext *phost)
{
	gpio_set_level(EVE_DEFAULT_SPI_CS, 0);
}
void releaseBus(EVE_HalContext *phost)
{
	if (phost->Status == EVE_STATUS_READING || phost->Status == EVE_STATUS_WRITING)
	{
		// Release bus
		spi_device_release_bus(spi);
		gpio_set_level(EVE_DEFAULT_SPI_CS, 1);
		phost->Status = EVE_STATUS_OPENED;
	}
}
// Placeholder function to simulate sending the global buffer
void sendBuffer(EVE_HalContext *phost, const uint8_t *buffer, uint32_t size)
{
	uint32_t sizeRemaining = size;
	esp_err_t ret;
	spi_transaction_t t;
	if (size == 0)
		return;

	memset(&t, 0, sizeof(t));
	t.user = (void *)1;

	while (sizeRemaining)
	{
		t.length = 1 * 8;
		t.tx_buffer = buffer;
		ret = spi_device_polling_transmit(spi, &t);
		assert(ret == ESP_OK);
		sizeRemaining -= t.length / 8;
		buffer += t.length / 8;

		if (ret != ESP_OK)
		{
			eve_printf_debug("%d SPI_Write failed, sizeTransferred is %d with result %d\n", __LINE__, t.length, (int)ret);
			return;
		}
	}
}

/** @name INIT */
///@{

/**
 * @brief Initialize global HAL platform
 *
 */
void EVE_HalImpl_initialize()
{
	/* no-op */
}

/**
 * @brief Release global HAL platform
 *
 */
void EVE_HalImpl_release()
{
}

/**
 * @brief List the available devices
 *
 */
size_t EVE_Hal_list()
{
	/* List one SPI channel */
	return 1;
}

/**
 * @brief Get info of the specified device. Devices of type EVE_HOST_UNKNOWN should be ignored
 *
 * @param deviceInfo
 * @param deviceIdx
 */
void EVE_Hal_info(EVE_DeviceInfo *deviceInfo, size_t deviceIdx)
{
	memset(deviceInfo, 0, sizeof(EVE_DeviceInfo));
	strcpy_s(deviceInfo->DisplayName, sizeof(deviceInfo->DisplayName), "ESP32 SPI");
	strcpy_s(deviceInfo->SerialNumber, sizeof(deviceInfo->SerialNumber), "ESP32 SPI");
	deviceInfo->Host = EVE_HOST_EMBEDDED;
}

/**
 * @brief Check whether the context is the specified device
 *
 * @param phost Pointer to Hal context
 * @param deviceIdx
 * @return true True if ok
 * @return false False if error
 */
bool EVE_Hal_isDevice(EVE_HalContext *phost, size_t deviceIdx)
{
	return true;
}

/**
 * @brief Get the default configuration parameters
 *
 * @param parameters EVE_Hal framework's parameters
 * @param deviceIdx
 * @return true True if ok
 * @return false False if error
 */
bool EVE_HalImpl_defaults(EVE_HalParameters *parameters, size_t deviceIdx)
{
	return true;
}

/**
 * @brief Set number of SPI channel
 *
 * @param phost Pointer to Hal context
 * @param numchnls Number of channel
 */
void setSPI(EVE_HalContext *phost, EVE_SPI_CHANNELS_T numchnls)
{
	if (spi_inited == false)
	{
		esp_err_t ret;
		spi_bus_config_t buscfg = {
			.miso_io_num = EVE_DEFAULT_SPI_MISO,
			.mosi_io_num = EVE_DEFAULT_SPI_MOSI,
			.sclk_io_num = EVE_DEFAULT_SPI_SCK,
			.quadwp_io_num = -1,
			.quadhd_io_num = -1,
			.max_transfer_sz = 4096};
		spi_device_interface_config_t devcfg = {
			.clock_speed_hz = 10 * 1000 * 1000, // Clock out at 10 MHz
			.mode = 0,							// SPI mode 0
			.spics_io_num = -1,					// CS pin
			.queue_size = 7,					// We want to be able to queue 7 transactions at a time
			.flags = SPI_DEVICE_POSITIVE_CS,
		};
		// Initialize the SPI bus
		ret = spi_bus_initialize(EVE_SPI, &buscfg, SPI_DMA_CH_AUTO);
		ESP_ERROR_CHECK(ret);
		// Attach EVE to the SPI bus
		ret = spi_bus_add_device(EVE_SPI, &devcfg, &spi);
		ESP_ERROR_CHECK(ret);

		gpio_set_level(EVE_DEFAULT_SPI_CS, 0);
		gpio_config_t cs_cfg = {
			.pin_bit_mask = BIT64(EVE_DEFAULT_SPI_CS),
			.mode = GPIO_MODE_OUTPUT,
		};
		gpio_config(&cs_cfg);

		/* Only support single channel */
		phost->SpiChannels = EVE_SPI_SINGLE_CHANNEL;
	}
}

/**
 * @brief Opens a new HAL context using the specified parameters
 *
 * @param phost Pointer to Hal context
 * @param parameters EVE_Hal framework's parameters
 * @return true True if ok
 * @return false False if error
 */
bool EVE_HalImpl_open(EVE_HalContext *phost, const EVE_HalParameters *parameters)
{
	eve_printf_debug("Open SPI for EVE\n");
	eve_printf_debug("CS: GPIO%i, SCK: GPIO%i, MOSI: GPIO%i, MISO: GPIO%i, PWD: GPIO%i\n",
					 (int)EVE_DEFAULT_SPI_CS, (int)EVE_DEFAULT_SPI_SCK, (int)EVE_DEFAULT_SPI_MOSI, (int)EVE_DEFAULT_SPI_MISO, (int)EVE_DEFAULT_SPI_PD);

	gpio_set_level(EVE_DEFAULT_SPI_PD, 0);
	gpio_config_t pd_cfg = {
		.pin_bit_mask = BIT64(EVE_DEFAULT_SPI_PD),
		.mode = GPIO_MODE_OUTPUT,
	};
	gpio_config(&pd_cfg);
	gpio_set_level(EVE_DEFAULT_SPI_PD, 0);

	/* Initialize single channel */
	setSPI(phost, EVE_SPI_SINGLE_CHANNEL);
	gpio_set_level(EVE_DEFAULT_SPI_PD, 1);

	/* Initialize the context variables */
	phost->Status = EVE_STATUS_OPENED;
	++g_HalPlatform.OpenedDevices;
	spi_inited = true;

	return true;
}

/**
 * @brief Close a HAL context
 *
 * @param phost Pointer to Hal context
 */
void EVE_HalImpl_close(EVE_HalContext *phost)
{
	esp_err_t ret;
	ret = spi_bus_remove_device(spi);
	ESP_ERROR_CHECK(ret);

	ret = spi_bus_free(EVE_SPI);
	ESP_ERROR_CHECK(ret);

	phost->Status = EVE_STATUS_CLOSED;
	--g_HalPlatform.OpenedDevices;
	spi_inited = false;
}

/**
 * @brief Idle. Call regularly to update frequently changing internal state
 *
 * @param phost Pointer to Hal context
 */
void EVE_HalImpl_idle(EVE_HalContext *phost)
{
	EVE_sleep(10); // TODO: release some time to prevent wdt bite
}
///@}

/*************
** TRANSFER **
*************/

/** @name TRANSFER */
///@{

/**
 * @brief Start data transfer to Coprocessor
 *
 * @param phost Pointer to Hal context
 * @param rw Read or Write
 * @param addr Address to read/write
 */
void EVE_Hal_startTransfer(EVE_HalContext *phost, EVE_TRANSFER_T rw, uint32_t addr)
{
	eve_assert(phost->Status == EVE_STATUS_OPENED);

	esp_err_t ret;
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));
	t.length = 8 * 4; // in bits
	t.flags = SPI_TRANS_USE_TXDATA;
	t.user = (void *)1;

	openBus(phost);

	// send the address
	if (rw == EVE_TRANSFER_READ)
	{
		t.tx_data[0] = (addr >> 24);
		t.tx_data[1] = (addr >> 16) & 0xFF;
		t.tx_data[2] = (addr >> 8) & 0xFF;
		t.tx_data[3] = addr & 0xFF;
		// When using SPI_TRANS_CS_KEEP_ACTIVE, bus must be locked/acquired
		spi_device_acquire_bus(spi, portMAX_DELAY);

		ret = spi_device_polling_transmit(spi, &t);
		assert(ret == ESP_OK);
		phost->Status = EVE_STATUS_READING;
	}
	else
	{
		t.tx_data[0] = (0x80 | (addr >> 24));
		t.tx_data[1] = (addr >> 16) & 0xFF;
		t.tx_data[2] = (addr >> 8) & 0xFF;
		t.tx_data[3] = addr & 0xFF;

		// When using SPI_TRANS_CS_KEEP_ACTIVE, bus must be locked/acquired
		spi_device_acquire_bus(spi, portMAX_DELAY);

		ret = spi_device_polling_transmit(spi, &t);
		assert(ret == ESP_OK);
		phost->Status = EVE_STATUS_WRITING;
	}
}

/**
 * @brief End data transfer
 *
 * @param phost Pointer to Hal context
 */
void EVE_Hal_endTransfer(EVE_HalContext *phost)
{
	eve_assert(phost->Status == EVE_STATUS_READING || phost->Status == EVE_STATUS_WRITING);
	EVE_Hal_flush(phost);
}

/**
 * @brief Flush data
 *
 * @param phost Pointer to Hal context
 */
void EVE_Hal_flush(EVE_HalContext *phost)
{
	sendBuffer(phost, globalBuffer, globalBufferSize);
	globalBufferSize = 0;
	releaseBus(phost);
}

/**
 * @brief Read a block data
 *
 * @param phost Pointer to Hal context
 * @param buffer Buffer to get result
 * @param size Number of bytes to read
 */
static inline void rdBuffer(EVE_HalContext *phost, uint8_t *buffer, uint32_t size)
{
	uint32_t sizeRemaining = size;
	uint8_t countTimout = 0;
	esp_err_t ret;
	spi_transaction_t t;

	memset(&t, 0, sizeof(t));
	t.user = (void *)1;
	t.flags = SPI_TRANS_USE_RXDATA;

	do
	{
		t.length = 8;
		ret = spi_device_polling_transmit(spi, &t);
		assert(ret == ESP_OK);
		countTimout++;
	} while ((t.rx_data[0] != 0x01) && (countTimout < READ_TIMOUT));

	while (sizeRemaining)
	{
		t.length = 8;
		ret = spi_device_polling_transmit(spi, &t);
		assert(ret == ESP_OK);
		sizeRemaining--;
		*buffer = t.rx_data[0];
		buffer++;

		if (ret != ESP_OK)
		{
			eve_printf_debug("%d SPI_Read failed, sizeTransferred is %d with result %d\n", __LINE__, t.length, (int)ret);
			phost->Status = EVE_STATUS_ERROR;
			return;
		}
	}
}
// Function to append data to global buffer and send it if full
void wrBuffer(EVE_HalContext *phost, const uint8_t *buffer, uint32_t size)
{
	// Check if the current buffer can fit the new data
	if (globalBufferSize + size > GLOBAL_BUFFER_SIZE)
	{
		// If not, send the current global buffer
		sendBuffer(phost, globalBuffer, globalBufferSize);

		// Reset the global buffer
		globalBufferSize = 0;
	}

	// If the new buffer size is too large for the global buffer, send the new buffer directly
	if (size > GLOBAL_BUFFER_SIZE)
	{
		sendBuffer(phost, buffer, size);
	}
	else
	{
		// Otherwise, append the new data to the global buffer
		memcpy(globalBuffer + globalBufferSize, buffer, size);
		globalBufferSize += size;
	}
}

/**
 * @brief Write 8 bit
 *
 * @param phost Pointer to Hal context
 * @param value Value to write
 * @return uint8_t Number of bytes transfered
 */
static inline uint8_t transfer8(EVE_HalContext *phost, uint8_t value)
{
	if (phost->Status == EVE_STATUS_READING)
	{
		rdBuffer(phost, &value, 1);
		return value;
	}
	else
	{
		wrBuffer(phost, &value, 1);
		return 0;
	}
}

/**
 * @brief Write 8 bits
 *
 * @param phost Pointer to Hal context
 * @param value Value to write
 * @return uint8_t Number of bytes transfered
 */
uint8_t EVE_Hal_transfer8(EVE_HalContext *phost, uint8_t value)
{
	return transfer8(phost, value);
}

/**
 * @brief Write 2 bytes
 *
 * @param phost Pointer to Hal context
 * @param value Value to write
 * @return uint16_t Number of bytes transfered
 */
uint16_t EVE_Hal_transfer16(EVE_HalContext *phost, uint16_t value)
{
	uint8_t buffer[2];
	if (phost->Status == EVE_STATUS_READING)
	{
		rdBuffer(phost, buffer, 2);
		return (uint16_t)buffer[0] | (uint16_t)buffer[1] << 8;
	}
	else
	{
		buffer[0] = value & 0xFF;
		buffer[1] = value >> 8;
		wrBuffer(phost, buffer, 2);
		return 0;
	}
}

/**
 * @brief Write 4 bytes
 *
 * @param phost Pointer to Hal context
 * @param value Value to write
 * @return uint32_t Number of bytes transfered
 */
uint32_t EVE_Hal_transfer32(EVE_HalContext *phost, uint32_t value)
{
	uint8_t buffer[4];
	if (phost->Status == EVE_STATUS_READING)
	{
		rdBuffer(phost, buffer, 4);
		return (uint32_t)buffer[0] | (uint32_t)buffer[1] << 8 | (uint32_t)buffer[2] << 16 | (uint32_t)buffer[3] << 24;
	}
	else
	{
		buffer[0] = value & 0xFF;
		buffer[1] = (value >> 8) & 0xFF;
		buffer[2] = (value >> 16) & 0xFF;
		buffer[3] = value >> 24;
		wrBuffer(phost, buffer, 4);
		return 0;
	}
}

/**
 * @brief Transfer (read/write) a block data
 *
 * @param phost Pointer to Hal context
 * @param result Buffer to get data transfered, NULL when write
 * @param buffer Buffer where data is transfered, NULL when read
 * @param size Size of buffer
 */
void EVE_Hal_transferMem(EVE_HalContext *phost, uint8_t *result, const uint8_t *buffer, uint32_t size)
{
	if (!size)
		return;

	if (result && buffer)
	{
		/* not implemented */
		eve_debug_break();
	}
	else if (result)
	{
		rdBuffer(phost, result, size);
	}
	else if (buffer)
	{
		wrBuffer(phost, buffer, size);
	}
}

/**
 * @brief Transfer a block data from program memory
 *
 * @param phost Pointer to Hal context
 * @param result Buffer to get data transfered, NULL when write
 * @param buffer Buffer where data is transfered, NULL when read
 * @param size Size of buffer
 */
void EVE_Hal_transferProgMem(EVE_HalContext *phost, uint8_t *result, eve_progmem_const uint8_t *buffer, uint32_t size)
{
	if (!size)
		return;

	if (result && buffer)
	{
		/* not implemented */
		eve_debug_break();
	}
	else if (result)
	{
		/* not implemented */
		eve_debug_break();
	}
	else if (buffer)
	{
		wrBuffer(phost, buffer, size);
	}
}

/**
 * @brief Transfer a string to EVE platform
 *
 * @param phost Pointer to Hal context
 * @param str String to transfer
 * @param index Start position in the string
 * @param size Size of string
 * @param padMask Padding mask
 * @return uint32_t Numer of bytes transfered
 */
uint32_t EVE_Hal_transferString(EVE_HalContext *phost, const char *str, uint32_t index, uint32_t size, uint32_t padMask)
{
	if (!size)
	{
		/* TODO: Support different padding options */
		eve_assert(padMask == 0x3);
		EVE_Hal_transfer32(phost, 0);
		return 4;
	}

	eve_assert(size <= EVE_CMD_STRING_MAX);
	uint32_t transferred = 0;
	if (phost->Status == EVE_STATUS_WRITING)
	{
		transferred += (uint32_t)strnlen(str, size) + 1;
		eve_assert(str[transferred - 1] == '\0');
		wrBuffer(phost, (uint8_t *)str, transferred);
		if (transferred & padMask)
		{
			uint32_t pad = 4 - (transferred & padMask);
			uint8_t padding[4] = {0};
			wrBuffer(phost, padding, pad);
			transferred += pad;
			eve_assert(!(transferred & 0x3));
		}
	}
	else
	{
		/* not implemented */
		eve_debug_break();
	}
	return transferred;
}
///@}

/************
** UTILITY **
************/

/** @name UTILITY */
///@{

/**
 * @brief This API sends a 5byte command to the phost
 *
 * @param phost Pointer to Hal context
 * @param cmd Command to send
 */
void EVE_Hal_spiCommand(EVE_HalContext *phost, uint8_t *cmd)
{
	// TODO: Add register guard
	eve_assert(phost->Status == EVE_STATUS_OPENED);
	eve_printf_debug("SPI cmd: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4]);
	esp_err_t ret;
	gpio_set_level(EVE_DEFAULT_SPI_CS, 0);
	ret = spi_device_acquire_bus(spi, portMAX_DELAY);
	if (ret != ESP_OK)
	{
		return;
	}
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));

	t.length = 5 * 8;
	t.tx_buffer = cmd;
	t.user = (void *)1;
	ret = spi_device_polling_transmit(spi, &t);
	assert(ret == ESP_OK);
	spi_device_release_bus(spi);
	gpio_set_level(EVE_DEFAULT_SPI_CS, 1);
}

/**
 * @brief
 *
 * @param phost Pointer to Hal context
 * @param up Up or Down
 * @return true True if ok
 * @return false False if error
 */
bool EVE_Hal_powerCycle(EVE_HalContext *phost, bool up)
{
	if (up)
	{
		/* Power down */
		gpio_set_level(EVE_DEFAULT_SPI_PD, 0);
		EVE_sleep(20);

		setSPI(phost, EVE_SPI_SINGLE_CHANNEL);
		EVE_sleep(20);

		/* Power up */
		gpio_set_level(EVE_DEFAULT_SPI_PD, 1);
		EVE_sleep(20);
	}
	else
	{
		/* Power down */
		gpio_set_level(EVE_DEFAULT_SPI_PD, 0);
		EVE_sleep(20);

		setSPI(phost, EVE_SPI_SINGLE_CHANNEL);
	}
	return true;
}

/**
 * @brief Set number of SPI channel
 *
 * @param phost Pointer to Hal context
 * @param numchnls Number of channel
 * @param numdummy Number of dummy bytes
 */
void EVE_Hal_setSPI(EVE_HalContext *phost, EVE_SPI_CHANNELS_T numchnls)
{
	if (numchnls > EVE_SPI_QUAD_CHANNEL)
		return; // error

	setSPI(phost, numchnls);
}

void EVE_Hal_restoreSPI(EVE_HalContext *phost)
{
	setSPI(phost, phost->SpiChannels);
}

/**
 * @brief Get current system clock of Coprocessor
 *
 * @param phost Pointer to Hal context
 * @return uint32_t Frequency of Coprocessor
 */
uint32_t EVE_Hal_currentFrequency(EVE_HalContext *phost)
{
	return 0; /* TODO */
}
///@}

/*********
** MISC **
*********/

/** @name MISC */
///@{

/**
 * @brief Init host MCU
 *
 */
void EVE_Mcu_initialize()
{
	eve_printf_debug("-------------------------------\n");
	eve_printf_debug("- Bridgetek EVE HAL for ESP32 -\n");
	eve_printf_debug("-------------------------------\n");
}

/**
 * @brief Release host MCU
 *
 */
void EVE_Mcu_release()
{
	/* no-op */
}
///@}

/*********
** MISC **
*********/
/** @name MISC */
///@{

/**
 * @brief Init timer
 *
 */
void EVE_Millis_initialize()
{
}

/**
 * @brief Release timer
 *
 */
void EVE_Millis_release()
{
}

/**
 * @brief Get clock in miliseond
 *
 * @return uint64_t Clock number
 */
uint64_t EVE_millis64()
{
	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);
	return ((uint64_t)tv_now.tv_sec * 1000000L + (uint64_t)tv_now.tv_usec);
}

/**
 * @brief Get clock in miliseond
 *
 * @return uint32_t Clock number
 */
uint32_t EVE_millis()
{
	return (uint32_t)EVE_millis64();
}

/**
 * @brief Sleep in milisecond
 *
 * @param ms Milisecond
 */
void EVE_sleep(uint32_t ms)
{
	vTaskDelay(pdMS_TO_TICKS(ms));
}

/*********
** MISC **
*********/

/**
 * @brief Display GPIO pins
 *
 * @param phost Pointer to Hal context
 * @return true True if Ok
 * @return false False if error
 */
bool EVE_UtilImpl_bootupDisplayGpio(EVE_HalContext *phost)
{
	return true;
}
///@}

#endif /* #if defined(ESP32_PLATFORM) */

/* end of file */
