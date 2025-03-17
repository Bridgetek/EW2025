/**
 * @file EVE_GpuDefs.h
 * @brief Defines EVE hardware values
 *
 * This header is separated and included last
 * in case of conflicts with other libraries.
 *
 * Expects BT82X_ENABLE to be defined.
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

#ifndef EVE_GPU_DEFS__H
#define EVE_GPU_DEFS__H

#ifdef BT82X_ENABLE

/* Definitions used for BT820 coprocessor command buffer */
#define EVE_DL_SIZE (16 * 1024UL) /**< 16kB Display List buffer size */
#define EVE_DL_COUNT (EVE_DL_SIZE / 4)
#define EVE_CMD_FIFO_SIZE ((16) * 1024UL) /**< 16kB coprocessor FIFO size */
#define EVE_CMD_FIFO_COUNT (EVE_CMD_FIFO_SIZE / 4)
#define EVE_CMD_FIFO_MASK (EVE_CMD_FIFO_SIZE - 1)
#define EVE_CMD_FIFO_ALIGNMENT_MASK (EVE_CMD_FIFO_SIZE - ((4) - 1))
#define EVE_CMD_FAULT(rp) (rp & 0x3)

/**************
** Addresses **
**************/

#define RAM_G 0UL
#define RAM_ERR_REPORT 0x309800UL
#define RAM_HIMEM 0x7f004800

#define SWAPCHAIN_0 0xFFFF00FF
#define SWAPCHAIN_1 0xFFFF01FF
#define SWAPCHAIN_2 0xFFFF02FF
#define DDR_PD_CFG 0x7F800404

#define SPI_WIDTH_1bit 0x000000 /**< SINGLE mode */
#define SPI_WIDTH_2bit 0x000100 /**< DUAL mode */
#define SPI_WIDTH_4bit 0X000200 /**< QUAD mode */

/** @name Top-level System Registers */
///@{
#define REG_PIN_DRV_0 0x7F800408
#define REG_PIN_DRV_1 0x7F80040C
#define REG_PIN_SLEW_0 0x7F800410
#define REG_PIN_TYPE_0 0x7F800414
#define REG_PIN_TYPE_1 0x7F800418
#define REG_SYS_CFG 0x7F800420
#define REG_SYS_STAT_0 0x7F800424
#define REG_SYS_STAT_1 0x7F800428
#define REG_CHIP_ID 0x7F800448
#define REG_BOOT_STATUS 0x7F80044C /**< to confirm whether it is normal running after start up */
#define REG_PD_STATUS 0x7F800450
#define REG_DDR_TYPE 0x7F800454
#define REG_SYS_STAT_2 0x7F800458
#define REG_SYS_GPREG 0x7F80045c
#define REG_SYS_STAT_3 0x7F800460
#define REG_PIN_DRV_2 0x7F800464
#define REG_PIN_SLEW_1 0x7F800468
#define REG_PIN_TYPE_2 0x7F80046C
///@}

// LVDSRX registers
// The base address is 0x7F00_6000.
#define LVDSRX_BASE_REG (0x7F006000)
#define LVDSRX_ADDERSS(offset) (LVDSRX_BASE_REG + (offset))
#define REG_LVDSRX_CAPTURE LVDSRX_ADDERSS(0x0674)
#define REG_LVDSRX_SETUP LVDSRX_ADDERSS(0x0678)
#define REG_LVDSRX_DEST LVDSRX_ADDERSS(0x067C)
#define REG_LVDSRX_FORMAT LVDSRX_ADDERSS(0x0684)

/** @name LVDSRX System Register */
///@{
#define REG_LVDSRX_CTRL 0x7F800504
#define REG_LVDSRX_STAT 0x7F800508
///@}

/** @name I2S Registers */
///@{
#define REG_I2S_CFG 0x7F800800
#define REG_I2S_CTL 0x7F800804
#define REG_I2S_IRQ_EN 0x7F800808
#define REG_I2S_IRQ_STAT 0x7F80080C
#define REG_I2S_STAT 0x7F800810
#define REG_I2S_PAD_CFG 0x7F800814
#define REG_I2S_FIFO 0x7f800830
///@}

/** @name LVDSTX system registers */
///@{
#define REG_LVDS_EN 0x7F800300
#define REG_LVDS_PLLCFG 0x7F800304
#define REG_LVDS_CFG 0x7F800308
#define REG_LVDS_TGEN_HCFG_0 0x7F80030C
#define REG_LVDS_TGEN_VCFG_0 0x7F800310
#define REG_LVDS_CTRL_CH0 0x7F800314
#define REG_LVDS_CTRL_CH1 0x7F800318
#define REG_LVDS_STAT 0x7F80031C
#define REG_LVDS_ERR_STAT 0x7F800320
#define REG_LVDS_TGEN_HCFG_1 0x7F800324
#define REG_LVDS_TGEN_VCFG_1 0x7F800328
#define REG_LVDS_TGEN_HCFG_2 0x7F80032C
#define REG_LVDS_TGEN_VCFG_2 0x7F800330
///@}

#define OTP_CTL 0x7f800900
#define OTP_CFG 0x7f800904
#define OTP_STATUS 0x7f800908
#define OTP_LOCK 0x7f80090C
#define OTP_PRG_TIMING0 0x7f800910
#define OTP_PRG_TIMING1 0x7f800914
#define OTP_PRG_TIMING2 0x7f800918
#define OTP_PRG_TIMING3 0x7f80091C
#define OTP_RD_TIMING 0x7f800920
#define OTP_WBUFFER0 0x7f800930
#define OTP_WBUFFER1 0x7f800934
#define OTP_WBUFFER2 0x7f800938
#define OTP_WBUFFER3 0x7f80093C
#define OTP_RBUFFER0 0x7f800940
#define OTP_RBUFFER1 0x7f800944
#define OTP_RBUFFER2 0x7f800948
#define OTP_RBUFFER3 0x7f80094C

#define WDT_CONTROL 0x7f800A00
#define WDT_CFG 0x7f800A04
#define WDT_INIT_VAL 0x7f800A08
#define WDT_COUNT 0x7f800A0C
#define WDT_TIMEOUT 0x7f800A10
#define WDT_LOCK 0x7f800A14
#define FREQUENCY 72000000UL
#define POLLING_BYTES 10 /**< For QUAD mode */
#define READ_TIMOUT 0x10
#define ROMFONT_TABLEADDRESS 0x2FFFFCUL
#define LOW_FREQ_BOUND 58800000L /**< 98% of 60Mhz */

// boot_control values
#define BOOT_REGULAR 0 /**< load from rom_romimage */
#define BOOT_WARM 1    /**< no loading : system is already configured */
#define BOOT_RAM0 2    /**< load ROM from RAM addr 0 */
#define BOOT_SAFETY 3  /**< Reboot into the J1 ROM command - interpreter */
#define BOOT_FLASH 4   /**< Load ROM from flash */
#define BOOT_SD 5      /**< Load ROM from SD */
#define BOOT_HALT 6

#define BOOT_DDR 0x80     /**< Enable DDR */
#define BOOT_DDR2 0x10    /**< LPDDR2 */
#define BOOT_DDR1066 0x80 /**< DDR 1066 */
#define BOOT_AUD 0x20     /**< Enable audio out */

/** @name Memory Map */
///@{
#define BASE 0x7f000000
#define RAM_CMD (BASE + 0x0000) /**< command FIFO		20K to 5000 */
#define RAM_J1RAM (RAM_CMD + 0x4000)
#define RAM_REG (BASE + 0x6000)     /**< register space	4K to 7000 */
#define RAM_DL (BASE + 0x8000)      /**< display list		16K to c000 */
#define CMDB_WRITE (BASE + 0x10000) /**< 				    32K to 18000 */
#define RAM_J1CODE (BASE + 0x20000) /**< J1 RAM			32K to 28000 */
///@}

/** @name Special Registers */
///@{
#define REG_TRACKER RAM_J1RAM
#define REG_TRACKER_1 (RAM_J1RAM + 0x0004)
#define REG_TRACKER_2 (RAM_J1RAM + 0x0008)
#define REG_TRACKER_3 (RAM_J1RAM + 0x000C)
#define REG_TRACKER_4 (RAM_J1RAM + 0x0010)
#define REG_MEDIAFIFO_READ (RAM_J1RAM + 0x0014)
#define REG_MEDIAFIFO_WRITE (RAM_J1RAM + 0x0018)
#define REG_ANIM_ACTIVE (RAM_J1RAM + 0x002C)
#define REG_OBJECT_COMPLETE (RAM_J1RAM + 0x0038)
#define REG_EXTENT_X0 (RAM_J1RAM + 0x003C)
#define REG_EXTENT_Y0 (RAM_J1RAM + 0x0040)
#define REG_EXTENT_X1 (RAM_J1RAM + 0x0044)
#define REG_EXTENT_Y1 (RAM_J1RAM + 0x0048)
#define REG_PLAY_CONTROL (RAM_J1RAM + 0x014E)
#define REG
///@}
/** @name Flash Interface Registers */
///@{
#define REG_FLASH_SIZE (RAM_J1RAM + 0x0024)
///@}

/** @name Swap Chain Registers */
///@{
#define REG_SC0_RESET (RAM_REG + 0x0034)
#define REG_SC0_SIZE (RAM_REG + 0x0038)
#define REG_SC0_PTR0 (RAM_REG + 0x003C)
#define REG_SC0_PTR1 (RAM_REG + 0x0040)
#define REG_SC0_PTR2 (RAM_REG + 0x0044)
#define REG_SC0_PTR3 (RAM_REG + 0x0048)
#define REG_SC1_RESET (RAM_REG + 0x004C)
#define REG_SC1_SIZE (RAM_REG + 0x0050)
#define REG_SC1_PTR0 (RAM_REG + 0x0054)
#define REG_SC1_PTR1 (RAM_REG + 0x0058)
#define REG_SC1_PTR2 (RAM_REG + 0x005C)
#define REG_SC1_PTR3 (RAM_REG + 0x0060)
#define REG_SC2_RESET (RAM_REG + 0x0064)
#define REG_SC2_SIZE (RAM_REG + 0x0068)
#define REG_SC2_PTR0 (RAM_REG + 0x006C)
#define REG_SC2_PTR1 (RAM_REG + 0x0070)
#define REG_SC2_PTR2 (RAM_REG + 0x0074)
#define REG_SC2_PTR3 (RAM_REG + 0x0078)
#define REG_SC1_I_VALID (RAM_REG + 0x007C)
#define REG_SC1_I_READY (RAM_REG + 0x0080)
#define REG_SC1_I_PTR (RAM_REG + 0x0084)
#define REG_SC2_STATUS (RAM_REG + 0x0780)
#define REG_SC2_ADDR (RAM_REG + 0x0784)
///@}

/** @name Miscellaneous Registers */
///@{
#define REG_ID (RAM_REG + 0x0000)
#define REG_FRAMES (RAM_REG + 0x0004)
#define REG_CLOCK (RAM_REG + 0x0008)
#define REG_FREQUENCY (RAM_REG + 0x000C)
#define REG_CPURESET (RAM_REG + 0x0088)
#define REG_GPIO_DIR (RAM_REG + 0x00DC)
#define REG_GPIO (RAM_REG + 0x00E0)
#define REG_DISP (RAM_REG + 0x00E4)
#define REG_INT_FLAGS (RAM_REG + 0x0100)
#define REG_INT_EN (RAM_REG + 0x0104)
#define REG_INT_MASK (RAM_REG + 0x0108)
#define REG_PWM_HZ (RAM_REG + 0x0128)
#define REG_PWM_DUTY (RAM_REG + 0x012C)
#define REG_MACRO_0 (RAM_REG + 0x0130)
#define REG_MACRO_1 (RAM_REG + 0x0134)
///@}

/** @name Graphics Engine Registers */
///@{
#define REG_RE_DEST (RAM_REG + 0x0010)
#define REG_RE_FORMAT (RAM_REG + 0x0014)
#define REG_RE_ROTATE (RAM_REG + 0x0018)
#define REG_RE_W (RAM_REG + 0x001C)
#define REG_RE_H (RAM_REG + 0x0020)
#define REG_RE_DITHER (RAM_REG + 0x0024)
#define REG_RE_ACTIVE (RAM_REG + 0x0028)
#define REG_RE_RENDERS (RAM_REG + 0x002C)

#define REG_HCYCLE (RAM_REG + 0x008C)
#define REG_HOFFSET (RAM_REG + 0x0090)
#define REG_HSIZE (RAM_REG + 0x0094)
#define REG_HSYNC0 (RAM_REG + 0x0098)
#define REG_HSYNC1 (RAM_REG + 0x009C)
#define REG_VCYCLE (RAM_REG + 0x00A0)
#define REG_VOFFSET (RAM_REG + 0x00A4)
#define REG_VSIZE (RAM_REG + 0x00A8)
#define REG_VSYNC0 (RAM_REG + 0x00AC)
#define REG_VSYNC1 (RAM_REG + 0x00B0)
#define REG_DLSWAP (RAM_REG + 0x00B4)
#define REG_PCLK_POL (RAM_REG + 0x00B8)
#define REG_TAG_X (RAM_REG + 0x00BC)
#define REG_TAG_Y (RAM_REG + 0x00C0)
#define REG_TAG (RAM_REG + 0x00C4)

#define REG_VOL_L_PB (RAM_REG + 0x00C8)
#define REG_VOL_R_PB (RAM_REG + 0x00CC)
#define REG_VOL_SOUND (RAM_REG + 0x00D0)
#define REG_SOUND (RAM_REG + 0x00D4)
#define REG_PLAY (RAM_REG + 0x00D8)

#define REG_PLAYBACK_START (RAM_REG + 0x010C)
#define REG_PLAYBACK_LENGTH (RAM_REG + 0x0110)
#define REG_PLAYBACK_READPTR (RAM_REG + 0x0114)
#define REG_PLAYBACK_FREQ (RAM_REG + 0x0118)
#define REG_PLAYBACK_FORMAT (RAM_REG + 0x011C)
#define REG_PLAYBACK_LOOP (RAM_REG + 0x0120)
#define REG_PLAYBACK_PLAY (RAM_REG + 0x0124)
#define REG_PLAYBACK_PAUSE (RAM_REG + 0x05D0)

#define REG_AUD_PWM (RAM_REG + 0x013C)

#define REG_SO_MODE (RAM_REG + 0x05F4)
#define REG_SO_SOURCE (RAM_REG + 0x05F8)
#define REG_SO_FORMAT (RAM_REG + 0x05FC)
#define REG_SO_EN (RAM_REG + 0x0600)

#define REG_I2S_EN (RAM_REG + 0x0714)
#define REG_I2S_FREQ (RAM_REG + 0x0718)
#define REG_FREQUENCY_A (RAM_REG + 0x071C)
///@}

/** @name coprocessor engine Registers */
///@{
#define REG_CMD_READ (RAM_REG + 0x014C)
#define REG_CMD_WRITE (RAM_REG + 0x0150)
#define REG_CMD_DL (RAM_REG + 0x0154)
#define REG_CMDB_SPACE (RAM_REG + 0x0594)
#define REG_CMDB_WRITE CMDB_WRITE
#define CMDBUF_SIZE 0x4000
///@}

/** @name Touch Screen Engine Registers */
///@{
#define REG_TOUCH_MODE (RAM_REG + 0x0158)
#define REG_CTOUCH_EXTENDED (RAM_REG + 0x015C)
#define REG_CTOUCH_TOUCH0_XY (RAM_REG + 0x0160)
#define REG_TOUCH_SCREEN_XY (RAM_REG + 0x0160)
#define REG_CTOUCH_TOUCHA_XY (RAM_REG + 0x0164)
#define REG_TOUCH_RAW_XY REG_CTOUCH_TOUCHA_XY
#define REG_CTOUCH_TOUCHB_XY (RAM_REG + 0x0168)
#define REG_CTOUCH_TOUCHC_XY (RAM_REG + 0x016C)
#define REG_CTOUCH_TOUCH4_XY (RAM_REG + 0x0170)
#define REG_TOUCH_TAG_XY (RAM_REG + 0x0174)
#define REG_TOUCH_TAG (RAM_REG + 0x0178)
#define REG_TOUCH_TAG1_XY (RAM_REG + 0x017C)
#define REG_TOUCH_TAG1 (RAM_REG + 0x0180)
#define REG_TOUCH_TAG2_XY (RAM_REG + 0x0184)
#define REG_TOUCH_TAG2 (RAM_REG + 0x0188)
#define REG_TOUCH_TAG3_XY (RAM_REG + 0x018C)
#define REG_TOUCH_TAG3 (RAM_REG + 0x0190)
#define REG_TOUCH_TAG4_XY (RAM_REG + 0x0194)
#define REG_TOUCH_TAG4 (RAM_REG + 0x0198)
#define REG_TOUCH_TRANSFORM_A (RAM_REG + 0x019C)
#define REG_TOUCH_TRANSFORM_B (RAM_REG + 0x01A0)
#define REG_TOUCH_TRANSFORM_C (RAM_REG + 0x01A4)
#define REG_TOUCH_TRANSFORM_D (RAM_REG + 0x01A8)
#define REG_TOUCH_TRANSFORM_E (RAM_REG + 0x01AC)
#define REG_TOUCH_TRANSFORM_F (RAM_REG + 0x01B0)
#define REG_CYA_TOUCH (RAM_REG + 0x01B4)
///@}

/** @name Flash Interface Registers */
///@{
#define REG_FLASH_STATUS (RAM_REG + 0x05D4)
///@}

/** @name LVDSRX registers */
///@{
#define REG_RX_ENABLE (RAM_REG + 0x0670)
#define REG_RX_CAPTURE (RAM_REG + 0x0674)
#define REG_RX_SETUP (RAM_REG + 0x0678)
#define REG_RX_DEST (RAM_REG + 0x067C)
#define REG_RX_FORMAT (RAM_REG + 0x0680)
#define REG_RX_DITHER (RAM_REG + 0x0684)
///@}

#define REG_RAM_SIZE (RAM_REG + 0x078C)

/*************
** Commands **
*************/

// clang-format off


/** @name Commands for BT820 */
///@{
#define CMD_ANIMDRAW         4294967119UL /**< 0xFFFFFF4FUL */
#define CMD_ANIMFRAMERAM     4294967134UL /**< 0xFFFFFF5EUL */
#define CMD_ANIMSTARTRAM     4294967135UL /**< 0xFFFFFF5FUL */
#define CMD_ANIMSTOP         4294967117UL /**< 0xFFFFFF4DUL */
#define CMD_ANIMXY           4294967118UL /**< 0xFFFFFF4EUL */
#define CMD_APBREAD          4294967138UL /**< 0xFFFFFF62UL */
#define CMD_APBTHRASH        4294967146UL /**< 0xFFFFFF6AUL */
#define CMD_APBWRITE         4294967139UL /**< 0xFFFFFF63UL */
#define CMD_APPEND           4294967068UL /**< 0xFFFFFF1CUL */
#define CMD_APPENDF          4294967122UL /**< 0xFFFFFF52UL */
#define CMD_ARC              4294967175UL /**< 0xFFFFFF87UL */
#define CMD_BGCOLOR          4294967047UL /**< 0xFFFFFF07UL */
#define CMD_BITMAP_TRANSFORM 4294967071UL /**< 0xFFFFFF1FUL */
#define CMD_BUTTON           4294967051UL /**< 0xFFFFFF0BUL */
#define CMD_CALIBRATE        4294967059UL /**< 0xFFFFFF13UL */
#define CMD_CALIBRATESUB     4294967126UL /**< 0xFFFFFF56UL */
#define CMD_CALLLIST         4294967131UL /**< 0xFFFFFF5BUL */
#define CMD_CGRADIENT        4294967178UL /**< 0xFFFFFF8AUL */
#define CMD_CLEARCACHE       4294967113UL /**< 0xFFFFFF49UL */
#define CMD_CLOCK            4294967058UL /**< 0xFFFFFF12UL */
#define CMD_COLDSTART        4294967086UL /**< 0xFFFFFF2EUL */
#define CMD_COPYLIST         4294967176UL /**< 0xFFFFFF88UL */
#define CMD_CRC              4294967043UL /**< 0xFFFFFF03UL */
#define CMD_DDRSHUTDOWN      4294967141UL /**< 0xFFFFFF65UL */
#define CMD_DDRSTARTUP       4294967142UL /**< 0xFFFFFF66UL */
#define CMD_DEBUGINFO        4294967155UL /**< 0xFFFFFF73UL */
#define CMD_DIAL             4294967081UL /**< 0xFFFFFF29UL */
#define CMD_DLSTART          4294967040UL /**< 0xFFFFFF00UL */
#define CMD_ENABLEREGION     4294967166UL /**< 0xFFFFFF7EUL */
#define CMD_ENDLIST          4294967133UL /**< 0xFFFFFF5DUL */
#define CMD_EVALUATE         4294967154UL /**< 0xFFFFFF72UL */
#define CMD_EXECUTE          4294967045UL /**< 0xFFFFFF05UL */
#define CMD_FENCE            4294967144UL /**< 0xFFFFFF68UL */
#define CMD_FGCOLOR          4294967048UL /**< 0xFFFFFF08UL */
#define CMD_FILLWIDTH        4294967121UL /**< 0xFFFFFF51UL */
#define CMD_FLASHATTACH      4294967107UL /**< 0xFFFFFF43UL */
#define CMD_FLASHDETACH      4294967106UL /**< 0xFFFFFF42UL */
#define CMD_FLASHERASE       4294967102UL /**< 0xFFFFFF3EUL */
#define CMD_FLASHFAST        4294967108UL /**< 0xFFFFFF44UL */
#define CMD_FLASHPROGRAM     4294967140UL /**< 0xFFFFFF64UL */
#define CMD_FLASHREAD        4294967104UL /**< 0xFFFFFF40UL */
#define CMD_FLASHSOURCE      4294967112UL /**< 0xFFFFFF48UL */
#define CMD_FLASHSPIDESEL    4294967109UL /**< 0xFFFFFF45UL */
#define CMD_FLASHSPIRX       4294967111UL /**< 0xFFFFFF47UL */
#define CMD_FLASHSPITX       4294967110UL /**< 0xFFFFFF46UL */
#define CMD_FLASHUPDATE      4294967105UL /**< 0xFFFFFF41UL */
#define CMD_FLASHWRITE       4294967103UL /**< 0xFFFFFF3FUL */
#define CMD_FSDIR            4294967182UL /**< 0xFFFFFF8EUL */
#define CMD_FSOPTIONS        4294967149UL /**< 0xFFFFFF6DUL */
#define CMD_FSFILE           0xFFFFFF94UL /**< 0xFFFFFF94UL */
#define CMD_FSWRITE          0xFFFFFF93UL /**< 0xFFFFFF93UL */
#define CMD_FSREAD           4294967153UL /**< 0xFFFFFF71UL */
#define CMD_FSSIZE           4294967168UL /**< 0xFFFFFF80UL */
#define CMD_FSSOURCE         4294967167UL /**< 0xFFFFFF7FUL */
#define CMD_GAUGE            4294967057UL /**< 0xFFFFFF11UL */
#define CMD_GETIMAGE         4294967128UL /**< 0xFFFFFF58UL */
#define CMD_GETMATRIX        4294967087UL /**< 0xFFFFFF2FUL */
#define CMD_GETPOINT         4294967046UL /**< 0xFFFFFF06UL */
#define CMD_GETPROPS         4294967074UL /**< 0xFFFFFF22UL */
#define CMD_GETPTR           4294967072UL /**< 0xFFFFFF20UL */
#define CMD_GLOW             4294967179UL /**< 0xFFFFFF8BUL */
#define CMD_GRADCOLOR        4294967088UL /**< 0xFFFFFF30UL */
#define CMD_GRADIENT         4294967049UL /**< 0xFFFFFF09UL */
#define CMD_GRADIENTA        4294967120UL /**< 0xFFFFFF50UL */
#define CMD_GRAPHICSFINISH   4294967147UL /**< 0xFFFFFF6BUL */
#define CMD_HAMMERAUX        4294967044UL /**< 0xFFFFFF04UL */
#define CMD_HMAC             4294967125UL /**< 0xFFFFFF55UL */
#define CMD_I2SSTARTUP       4294967145UL /**< 0xFFFFFF69UL */
#define CMD_INFLATE          4294967114UL /**< 0xFFFFFF4AUL */
#define CMD_INTERRUPT        4294967042UL /**< 0xFFFFFF02UL */
#define CMD_INT_RAMSHARED    4294967096UL /**< 0xFFFFFF38UL */
#define CMD_KEYS             4294967052UL /**< 0xFFFFFF0CUL */
#define CMD_LAST_            4294967183UL /**< 0xFFFFFF8FUL */
#define CMD_LOADASSET        4294967169UL /**< 0xFFFFFF81UL */
#define CMD_LOADIDENTITY     4294967075UL /**< 0xFFFFFF23UL */
#define CMD_LOADIMAGE        4294967073UL /**< 0xFFFFFF21UL */
#define CMD_LOADPATCH        4294967170UL /**< 0xFFFFFF82UL */
#define CMD_LOADQRCODE       4294967163UL /**< 0xFFFFFF7BUL */
#define CMD_LOADWAV          4294967173UL /**< 0xFFFFFF85UL */
#define CMD_LOGO             4294967085UL /**< 0xFFFFFF2DUL */
#define CMD_MEDIAFIFO        4294967092UL /**< 0xFFFFFF34UL */
#define CMD_MEMCPY           4294967067UL /**< 0xFFFFFF1BUL */
#define CMD_MEMCRC           4294967062UL /**< 0xFFFFFF16UL */
#define CMD_MEMSET           4294967065UL /**< 0xFFFFFF19UL */
#define CMD_MEMWRITE         4294967064UL /**< 0xFFFFFF18UL */
#define CMD_MEMZERO          4294967066UL /**< 0xFFFFFF1AUL */
#define CMD_MESSAGEBOX       0xFFFFFF91UL /**< 0xFFFFFF1AUL */
#define CMD_NEWLIST          4294967132UL /**< 0xFFFFFF5CUL */
#define CMD_NOP              4294967123UL /**< 0xFFFFFF53UL */
#define CMD_NUMBER           4294967082UL /**< 0xFFFFFF2AUL */
#define CMD_OTPRD            4294967156UL /**< 0xFFFFFF74UL */
#define CMD_OTPWR            4294967157UL /**< 0xFFFFFF75UL */
#define CMD_PLAYVIDEO        4294967093UL /**< 0xFFFFFF35UL */
#define CMD_PLAYWAV          4294967161UL /**< 0xFFFFFF79UL */
#define CMD_PROGRESS         4294967053UL /**< 0xFFFFFF0DUL */
#define CMD_QRCODE           4294967162UL /**< 0xFFFFFF7AUL */
#define CMD_REGREAD          4294967063UL /**< 0xFFFFFF17UL */
#define CMD_REGWRITE         0xFFFFFF86UL /**< 0xFFFFFF86UL */
#define CMD_RENDERTARGET     4294967181UL /**< 0xFFFFFF8DUL */
#define CMD_RESETFONTS       4294967116UL /**< 0xFFFFFF4CUL */
#define CMD_RESTORECONTEXT   4294967165UL /**< 0xFFFFFF7DUL */
#define CMD_RESULT           4294967177UL /**< 0xFFFFFF89UL */
#define CMD_RETURN           4294967130UL /**< 0xFFFFFF5AUL */
#define CMD_ROMFONT          4294967097UL /**< 0xFFFFFF39UL */
#define CMD_ROTATE           4294967078UL /**< 0xFFFFFF26UL */
#define CMD_ROTATEAROUND     4294967115UL /**< 0xFFFFFF4BUL */
#define CMD_RUNANIM          4294967136UL /**< 0xFFFFFF60UL */
#define CMD_SAVECONTEXT      4294967164UL /**< 0xFFFFFF7CUL */
#define CMD_SCALE            4294967077UL /**< 0xFFFFFF25UL */
#define CMD_SCREENSAVER      4294967083UL /**< 0xFFFFFF2BUL */
#define CMD_SCROLLBAR        4294967055UL /**< 0xFFFFFF0FUL */
#define CMD_SDATTACH         4294967150UL /**< 0xFFFFFF6EUL */
#define CMD_SDBLOCKREAD      4294967151UL /**< 0xFFFFFF6FUL */
#define CMD_SDBLOCKWRITE     4294967152UL /**< 0xFFFFFF70UL */
#define CMD_SETBASE          4294967091UL /**< 0xFFFFFF33UL */
#define CMD_SETBITMAP        4294967101UL /**< 0xFFFFFF3DUL */
#define CMD_SETFONT          4294967094UL /**< 0xFFFFFF36UL */
#define CMD_SETMATRIX        4294967079UL /**< 0xFFFFFF27UL */
#define CMD_SETROTATE        4294967089UL /**< 0xFFFFFF31UL */
#define CMD_SETSCRATCH       4294967095UL /**< 0xFFFFFF37UL */
#define CMD_SHA1             4294967124UL /**< 0xFFFFFF54UL */
#define CMD_SKETCH           4294967084UL /**< 0xFFFFFF2CUL */
#define CMD_SKIPCOND         4294967180UL /**< 0xFFFFFF8CUL */
#define CMD_SLIDER           4294967054UL /**< 0xFFFFFF0EUL */
#define CMD_SNAPSHOT         4294967069UL /**< 0xFFFFFF1DUL */
#define CMD_SNAPSHOT2        4294967090UL /**< 0xFFFFFF32UL */
#define CMD_SOFTBOOT         4294967137UL /**< 0xFFFFFF61UL */
#define CMD_SPINNER          4294967060UL /**< 0xFFFFFF14UL */
#define CMD_STOP             4294967061UL /**< 0xFFFFFF15UL */
#define CMD_SWAP             4294967041UL /**< 0xFFFFFF01UL */
#define CMD_SYNC             4294967100UL /**< 0xFFFFFF3CUL */
#define CMD_TESTCARD         4294967127UL /**< 0xFFFFFF57UL */
#define CMD_TEXT             4294967050UL /**< 0xFFFFFF0AUL */
#define CMD_TEXTDIM          4294967172UL /**< 0xFFFFFF84UL */
#define CMD_TOGGLE           4294967056UL /**< 0xFFFFFF10UL */
#define CMD_TOUCHRD          4294967158UL /**< 0xFFFFFF76UL */
#define CMD_TOUCHWR          4294967159UL /**< 0xFFFFFF77UL */
#define CMD_TOUCH_TRANSFORM  4294967070UL /**< 0xFFFFFF1EUL */
#define CMD_TRACK            4294967080UL /**< 0xFFFFFF28UL */
#define CMD_TRANSLATE        4294967076UL /**< 0xFFFFFF24UL */
#define CMD_VIDEOFRAME       4294967099UL /**< 0xFFFFFF3BUL */
#define CMD_VIDEOSTART       4294967098UL /**< 0xFFFFFF3AUL */
#define CMD_WAIT             4294967129UL /**< 0xFFFFFF59UL */
#define CMD_WAITCHANGE       4294967143UL /**< 0xFFFFFF67UL */
#define CMD_WAITCOND         4294967160UL /**< 0xFFFFFF78UL */
#define CMD_WATCHDOG         4294967171UL /**< 0xFFFFFF83UL */
#define CMD_WORKAREA         4294967148UL /**< 0xFFFFFF6CUL */
///@}

/*****************
** Display List **
*****************/

/** @name Setting Graphics State */
///@{
#define ALPHA_FUNC(func, ref)       ((9UL << 24) | (((func)&7UL) << 8) | (((ref)&255UL) << 0))
#define BEGIN(prim)                 ((31UL << 24) | (((prim)&15UL) << 0))
#define BITMAP_EXT_FORMAT(format)   ((46UL << 24) | (((format)&65535UL) << 0))
#define BITMAP_HANDLE(handle)       ((5UL << 24) | (((handle)&63UL) << 0))
#define BITMAP_LAYOUT(format, linestride, height)  ((7UL << 24) | (((format)&31UL) << 19) | (((linestride)&1023UL) << 9) | (((height)&511UL) << 0))
#define BITMAP_LAYOUT_H(linestride, height)        ((40UL << 24) | (((linestride)&7UL) << 2) | (((height)&3UL) << 0))
#define BITMAP_SIZE(filter, wrapx, wrapy, width, height)   ((8UL << 24) | (((filter)&1UL) << 20) | (((wrapx)&1UL) << 19) | (((wrapy)&1UL) << 18) | (((width)&511UL) << 9) | (((height)&511UL) << 0))
#define BITMAP_SIZE_H(width, height)               ((41UL << 24) | (((width)&3UL) << 2) | (((height)&3UL) << 0))
#define BITMAP_SOURCE(addr)         ((1UL << 24) | (((addr)&16777215UL) << 0))
#define BITMAP_SOURCEH(addr)        ((49UL << 24) | (((addr)&255UL) << 0))
#define BITMAP_ADDR_MASK            16777215UL /**< 0x00FFFFFF*/
#define BITMAP_SOURCE2(flash_or_ram, addr)         ((1UL << 24) | ((flash_or_ram) << 23) | (((addr) & (BITMAP_ADDR_MASK >> 1)) << 0))
#define BITMAP_SWIZZLE(r, g, b, a)                 ((47UL << 24) | (((r)&7UL) << 9) | (((g)&7UL) << 6) | (((b)&7UL) << 3) | (((a)&7UL) << 0))
#define BITMAP_TRANSFORM_A_EXT(p, v)               ((21UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_B_EXT(p, v)               ((22UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_D_EXT(p, v)               ((24UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_E_EXT(p, v)               ((25UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_A(a)       BITMAP_TRANSFORM_A_EXT(0, a)
#define BITMAP_TRANSFORM_B(b)       BITMAP_TRANSFORM_B_EXT(0, b)
#define BITMAP_TRANSFORM_D(d)       BITMAP_TRANSFORM_D_EXT(0, d)
#define BITMAP_TRANSFORM_E(e)       BITMAP_TRANSFORM_E_EXT(0, e)
#define BITMAP_TRANSFORM_C(c)       ((23UL << 24) | (((c)&16777215UL) << 0))
#define BITMAP_TRANSFORM_F(f)       ((26UL << 24) | (((f)&16777215UL) << 0))
#define BITMAP_ZORDER(o)            ((51UL << 24) | (((o)&255UL) << 0))
#define BLEND_FUNC(src, dst)        ((11UL << 24) | (((src)&7UL) << 3) | (((dst)&7UL) << 0))
#define CELL(cell)                  ((6UL << 24) | (((cell)&127UL) << 0))
#define CLEAR_COLOR_A(alpha)        ((15UL << 24) | (((alpha)&255UL) << 0))
#define CLEAR_COLOR_RGB(red, green, blue)         ((2UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define CLEAR_STENCIL(s)            ((17UL << 24) | (((s)&255UL) << 0))
#define CLEAR_TAG(s)                ((18UL << 24) | (((s)&16777215UL) << 0))
#define COLOR_A(alpha)              ((16UL << 24) | (((alpha)&255UL) << 0))
#define COLOR_MASK(r, g, b, a)      ((32UL << 24) | (((r)&1UL) << 3) | (((g)&1UL) << 2) | (((b)&1UL) << 1) | (((a)&1UL) << 0))
#define COLOR_RGB(red, green, blue) ((4UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define END()                       ((33UL << 24))
#define LINE_WIDTH(width)           ((14UL << 24) | (((width)&4095UL) << 0))
#define POINT_SIZE(size)            ((13UL << 24) | (((size)&8191UL) << 0))
#define RESTORE_CONTEXT()           ((35UL << 24))
#define SAVE_CONTEXT()              ((34UL << 24))
#define SCISSOR_SIZE(width, height) ((28UL << 24) | (((width)&4095UL) << 12) | (((height)&4095UL) << 0))
#define SCISSOR_XY(x, y)            ((27UL << 24) | (((x)&2047UL) << 11) | (((y)&2047UL) << 0))
#define STENCIL_FUNC(func, ref, mask)             ((10UL << 24) | (((func)&7UL) << 16) | (((ref)&255UL) << 8) | (((mask)&255UL) << 0))
#define STENCIL_MASK(mask)          ((19UL << 24) | (((mask)&255UL) << 0))
#define STENCIL_OP(sfail, spass)    ((12UL << 24) | (((sfail)&7UL) << 3) | (((spass)&7UL) << 0))
#define TAG(s)                      ((3UL << 24) | (((s)&16777215UL) << 0))
#define TAG_MASK(mask)              ((20UL << 24) | (((mask)&1UL) << 0))
#define VERTEX_FORMAT(frac)         ((39UL << 24) | (((frac)&7UL) << 0))
#define VERTEX_TRANSLATE_X(x)       ((43UL << 24) | (((x)&131071UL) << 0))
#define VERTEX_TRANSLATE_Y(y)       ((44UL << 24) | (((y)&131071UL) << 0))
#define PALETTE_SOURCE(addr)        ((42UL << 24) | (((addr)&16777215UL) << 0))
#define PALETTE_SOURCEH(addr)       ((50UL << 24) | (((addr)&255UL) << 0))
#define PALETTE_SOURCEH2(addr)      ((50UL << 24) | (((addr >> 24) & 255)))
///@}

/** @name Deawing Actions */
///@{
#define CLEAR(c, s, t)                ((38UL << 24) | (((c)&1UL) << 2) | (((s)&1UL) << 1) | (((t)&1UL) << 0))
#define VERTEX2F(x, y)                ((1UL << 30) | (((x)&32767UL) << 15) | (((y)&32767UL) << 0))
#define VERTEX2II(x, y, handle, cell) ((2UL << 30) | (((x)&511UL) << 21) | (((y)&511UL) << 12) | (((handle)&31UL) << 7) | (((cell)&127UL) << 0))
///@}
/** @name Execution Control */
///@{
#define CALL(dest)                    ((29UL << 24) | (((dest)&65535UL) << 0))
#define DISPLAY()                     ((0UL << 24))
#define JUMP(dest)                    ((30UL << 24) | (((dest)&65535UL) << 0))
#define MACRO(m)                      ((37UL << 24) | (((m)&1UL) << 0))
#define NOP()                         ((45UL << 24))
#define REGION(y, h, dest)            ((52UL << 24) | (((y)&63UL) << 18) | (((h)&63UL) << 12) | (((dest)&4095UL) << 0))
#define RETURN()                      ((36UL << 24))
///@}

#define INT_PALDAT()                  ((48UL << 24))
#define SYS_PSEUDO_BASE(p)            ((53UL << 24) | (((p)&2047UL) << 0))

/***************
** Parameters **
****************/

/**< @name for REG_DLSWAP */
///@{
#define DLSWAP_DONE       0UL
#define DLSWAP_LINE       1UL
#define DLSWAP_FRAME      2UL
///@}

/** @name Options */
///@{
#define OPT_3D            0UL /**< 0x0 */
#define OPT_RGB565        0UL /**< 0x0 */
#define OPT_MONO          1UL /**< 0x1 */
#define OPT_SFNLOWER      1UL /**< 0x1 */
#define OPT_NODL          2UL /**< 0x2 */
#define OPT_4BIT          2UL /**< 0x2 */
#define OPT_CASESENSITIVE 2UL /**< 0x2 */
//#define OPT_NOTEAR        4UL /**< 0x4 */
#define OPT_DIRSEP_WIN    4UL /**< 0x4 */
#define OPT_HALFSPEED     4UL /**< 0x4 */
#define OPT_QUARTERSPEED  8UL /**< 0x8 */
#define OPT_FULLSCREEN    8UL /**< 0x8 */
#define OPT_DIRSEP_UNIX   8UL /**< 0x8 */
#define OPT_MEDIAFIFO     16UL /**< 0x10 */
#define OPT_IS_MMC        16UL /**< 0x10 */
#define OPT_SOUND         32UL /**< 0x20 */
#define OPT_IS_SD         32UL /**< 0x20 */
#define OPT_FLASH         64UL /**< 0x40 */
#define OPT_OVERLAY       128UL /**< 0x80 */
#define OPT_FLAT          256UL /**< 0x100 */
#define OPT_SIGNED        256UL /**< 0x100 */
#define OPT_DITHER        256UL /**< 0x100 */
#define OPT_TRUECOLOR     512UL /**< 0x200 */
#define OPT_CENTERX       512UL /**< 0x200 */
#define OPT_CENTERY       1024UL /**< 0x400 */
#define OPT_YCBCR         1024UL /**< 0x400 */
#define OPT_CENTER        1536UL /**< 0x600 */
#define OPT_RIGHTX        2048UL /**< 0x800 */
#define OPT_DIRECT        2048UL /**< 0x800 */
#define OPT_COMPLETEREG   4096UL /**< 0x1000 */
#define OPT_FORMAT        4096UL /**< 0x1000 */
#define OPT_NOBACK        4096UL /**< 0x1000 */
#define OPT_FILL          8192UL /**< 0x2000 */
#define OPT_NOTICKS       8192UL /**< 0x2000 */
#define OPT_FS            8192UL /**< 0x2000 */
#define OPT_NOHM          16384UL /**< 0x4000 */
#define OPT_NOPOINTER     16384UL /**< 0x4000 */
#define OPT_NOSECS        32768UL /**< 0x8000 */
#define OPT_NOHANDS       49152UL /**< 0xC000 */
///@}

/** @name for CMD_ANIMSTART */
///@{
#define ANIM_ONCE         0UL
#define ANIM_LOOP         1UL
#define ANIM_HOLD         2UL
///@}

/** @name for STENCIL_OP */
///@{
#define ZERO              0UL
#define KEEP              1UL
#define REPLACE           2UL
#define INCR              3UL
#define DECR              4UL
#define INVERT            5UL
///@}

/** @name for BLEND_FUNC */
///@{
//#define ZERO            0UL
#define ONE               1UL
#define SRC_ALPHA         2UL
#define DST_ALPHA         3UL
#define ONE_MINUS_SRC_ALPHA 4UL
#define ONE_MINUS_DST_ALPHA 5UL
///@}

/** @name for BITMAP_SWIZZLE */
///@{
//#define ZERO            0UL
//#define ONE             1UL
#define RED               2UL
#define GREEN             3UL
#define BLUE              4UL
#define ALPHA             5UL
///2}

/** @name for ALPHA_FUNC */
///@{
#define NEVER             0UL
#define LESS              1UL
#define LEQUAL            2UL
#define GREATER           3UL
#define GEQUAL            4UL
#define EQUAL             5UL
#define NOTEQUAL          6UL
#define ALWAYS            7UL
///@}

/** @name format of the bitmap for BITMAP_EXT_FORMAT */
///@{
#define ARGB1555          0UL
#define L1                1UL
#define L4                2UL
#define L8                3UL
#define RGB332            4UL
#define ARGB2             5UL
#define ARGB4             6UL
#define RGB565            7UL
#define PALETTED          8UL
#define TEXT8X8           9UL
#define TEXTVGA           10UL
#define BARGRAPH          11UL
#define PALETTED565       14UL
#define PALETTED4444      15UL
#define PALETTED8         16UL
#define L2                17UL
#define INT_G8            18UL
#define RGB8              19UL
#define ARGB8             20UL
#define PALETTEDARGB8     21UL
#define RGB6              22UL
#define ARGB6             23UL
#define LA1               24UL
#define LA2               25UL
#define LA4               26UL
#define LA8               27UL
#define YCBCR             28UL
#define GLFORMAT          31UL
#define COMPRESSED_RGBA_ASTC_4x4_KHR   37808UL
#define COMPRESSED_RGBA_ASTC_5x4_KHR   37809UL
#define COMPRESSED_RGBA_ASTC_5x5_KHR   37810UL
#define COMPRESSED_RGBA_ASTC_6x5_KHR   37811UL
#define COMPRESSED_RGBA_ASTC_6x6_KHR   37812UL
#define COMPRESSED_RGBA_ASTC_8x5_KHR   37813UL
#define COMPRESSED_RGBA_ASTC_8x6_KHR   37814UL
#define COMPRESSED_RGBA_ASTC_8x8_KHR   37815UL
#define COMPRESSED_RGBA_ASTC_10x5_KHR  37816UL
#define COMPRESSED_RGBA_ASTC_10x6_KHR  37817UL
#define COMPRESSED_RGBA_ASTC_10x8_KHR  37818UL
#define COMPRESSED_RGBA_ASTC_10x10_KHR 37819UL
#define COMPRESSED_RGBA_ASTC_12x10_KHR 37820UL
#define COMPRESSED_RGBA_ASTC_12x12_KHR 37821UL
///@}

/** @name for REG_LASH_STATUS */
///@{
#define FLASH_STATUS_INIT          0UL
#define FLASH_STATUS_DETACHED      1UL
#define FLASH_STATUS_BASIC         2UL
#define FLASH_STATUS_FULL          3UL
///@}

/** @name for BITMAP_SIZE */
///@{
//filter
#define NEAREST                    0UL
#define BILINEAR                   1UL
//wrapx/wrapy
#define BORDER                     0UL
#define REPEAT                     1UL
///@}

/** @name for REG_PLAYBACK_FORMAT */
///@{
#define LINEAR_SAMPLES             0UL
#define ULAW_SAMPLES               1UL
#define ADPCM_SAMPLES              2UL
#define S16_SAMPLES                3UL
#define S16S_SAMPLES               4UL
///@}

#ifdef POINTS
#undef POINTS
#endif
/** @name for BEGIN */
///@{
#define BITMAPS                    1UL
#define POINTS                     2UL
#define LINES                      3UL
#define LINE_STRIP                 4UL
#define EDGE_STRIP_R               5UL
#define EDGE_STRIP_L               6UL
#define EDGE_STRIP_A               7UL
#define EDGE_STRIP_B               8UL
#define RECTS                      9UL
///@}

/** @name for REG_TOUCH_MODE */
///@{
#define TOUCHMODE_OFF              0UL
#define TOUCHMODE_ONESHOT          1UL
#define TOUCHMODE_FRAME            2UL
#define TOUCHMODE_CONTINUOUS       3UL
///@}

/** @name for REG_CTOUCH_EXTENDED */
///@{
#define CTOUCH_MODE_COMPATIBILITY  1UL
#define CTOUCH_MODE_EXTENDED       0UL
///@}

/** @name for CMD_CGRADIENT */
///@{
#define CORNER_ZERO                0UL
#define EDGE_ZERO                  1UL
///@}

// clang-format on
#endif /* BT82X_ENABLE */

#endif /* #ifndef EVE_GPU_DEFS__H */

/* end of file */
