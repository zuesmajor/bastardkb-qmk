ifeq ($(MCU),atmega32u4)
	# Space saving settings.
	TAP_DANCE_ENABLE = no
	COMBO_ENABLE = no
	QMK_SETTINGS = no
else ifeq ($(MCU),STM32F411)
	EEPROM_DRIVER = vendor
endif

VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = no

# EC11 encoder on left half
ENCODER_ENABLE = yes

# loads bstiq charybdis keymap with my defaults
USER_NAME := bstiq
SRC += $(USER_PATH)/keymaps/charybdis/3x5/charybdis34.c