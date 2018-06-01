# List of dev files.
DEVSRC = $(CHIBIOS)/dev/main.c \
		 $(CHIBIOS)/dev/control/chassis.c \
		 $(CHIBIOS)/dev/control/gimbal.c \
		 $(CHIBIOS)/dev/control/mode_handle.c \
		 $(CHIBIOS)/dev/control/pid.c \
		 $(CHIBIOS)/dev/control/main_control_loop.c \
		 $(CHIBIOS)/dev/info_interaction/led_debug.c \
		 $(CHIBIOS)/dev/info_interaction/can.c \
		 $(CHIBIOS)/dev/info_interaction/remote.c \
		 $(CHIBIOS)/dev/info_interaction/serial_debug.c \
		 $(CHIBIOS)/dev/info_interaction/pwm.c \
		 $(CHIBIOS)/dev/info_interaction/shoot.c

DEVINC = $(CHIBIOS)/dev \
         $(CHIBIOS)/dev/control \
         $(CHIBIOS)/dev/info_interaction