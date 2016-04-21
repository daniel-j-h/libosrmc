include config.mk

TARGET = libosrmc.so
OBJECTS = osrmc.o
HEADER = osrmc.h

$(TARGET): $(OBJECTS) $(HEADER)
	$(CXX) $(LDFLAGS) -o $@ $< $(LDLIBS)

install:
	@mkdir -p $(PREFIX)/include
	@install -m 0644 $(HEADER) $(PREFIX)/include
	@mkdir -p $(PREFIX)/lib
	@install -m 0644 $(TARGET) $(PREFIX)/lib
	@ln -sf $(PREFIX)/lib/$(TARGET) $(PREFIX)/lib/$(TARGET).$(VERSION_MAJOR)
	@ln -sf $(PREFIX)/lib/$(TARGET) $(PREFIX)/lib/$(TARGET).$(VERSION_MAJOR).$(VERSION_MINOR)

clean:
	@$(RM) $(OBJECTS) $(TARGET)

.PHONY: clean install
