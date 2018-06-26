# List of dev files.
DEVSRC = $(CHIBIOS)/dev/main.c \
		 $(CHIBIOS)/dev/component_handle/me_arm.c \
		 $(CHIBIOS)/dev/component_handle/chassis.c \
		 $(CHIBIOS)/dev/component_handle/gimbal.c \
		 $(CHIBIOS)/dev/control/mode_handle.c \
		 $(CHIBIOS)/dev/modules/pid.c \
		 $(CHIBIOS)/dev/control/main_control_loop.c \
		 $(CHIBIOS)/dev/debug/debug.c \
		 $(CHIBIOS)/dev/debug/led.c \
		 $(CHIBIOS)/dev/info_interaction/can.c \
		 $(CHIBIOS)/dev/info_interaction/remote.c \
		 $(CHIBIOS)/dev/debug/serial_debug.c \

DEVINC = $(CHIBIOS)/dev \
		 $(CHIBIOS)/dev/modules \
         $(CHIBIOS)/dev/control \
         $(CHIBIOS)/dev/info_interaction \
         $(CHIBIOS)/dev/component_handle \
         $(CHIBIOS)/dev/debug