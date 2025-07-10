# Makefile
CXX = g++
CXXFLAGS = -std=c++17 `pkg-config gtkmm-4.0 --cflags`
LDFLAGS = `pkg-config gtkmm-4.0 --libs`

# Directorio de salida
BUILD_DIR = build
TARGET = $(BUILD_DIR)/entorno

# Archivos fuente
SOURCES = main.cpp \
          src/wallpaper/WallpaperWindow.cpp \
          src/panel/TopPanel.cpp \
          src/app_launcher/AppLauncher.cpp \
          src/core/CoreSystem.cpp \
          src/core/EventManager.cpp \
          src/context_menu/DesktopContextMenu.cpp \
          src/config/ThemeManager.cpp 

# Archivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para compilar archivos .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(TARGET) $(OBJECTS)

# Regla para evitar conflictos con archivos del mismo nombre
.PHONY: all clean