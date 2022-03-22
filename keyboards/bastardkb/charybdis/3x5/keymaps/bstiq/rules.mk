ifeq ($(MCU),atmega32u4)
	# Space saving settings.
	TAP_DANCE_ENABLE = no
	COMBO_ENABLE = no
	QMK_SETTINGS = no
else ifeq ($(MCU),STM32F411)
	EEPROM_DRIVER = vendor
endif

# VIA_ENABLE = yes
# VIAL_ENABLE = yes
# VIALRGB_ENABLE = yes

THUMBSTICK_ENABLE  	   = yes

ifeq ($(strip $(THUMBSTICK_ENABLE)), yes)
    POINTING_DEVICE_ENABLE = yes
    POINTING_DEVICE_DRIVER = analog_joystick
    OPT_DEFS += -DTHUMBSTICK_ENABLE
endif