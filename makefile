
OBJS = main.o gauss_serial.o buneman_serial.o

# All Target
all: eq

# Tool invocations
eq: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	cc -o "eq" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) eq
	-@echo ' '

