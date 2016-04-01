include config.mk

TARGET = libosrmc.so.1.0
OBJECTS = osrmc.o

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $<

clean:
	@$(RM) $(OBJECTS) $(TARGET)

.PHONY: clean
