include config.mk

TARGET = libosrmc.so.1.0
OBJECTS = osrmc.o

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	@$(RM) $(OBJECTS) $(TARGET)

.PHONY: clean
