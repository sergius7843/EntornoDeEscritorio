CXX = g++
CXXFLAGS = -std=c++17 `pkg-config gtkmm-4.0 --cflags`
LDFLAGS = `pkg-config gtkmm-4.0 --libs`

SRC = main.cpp \
      src/wallpaper/WallpaperWindow.cpp \
      src/panel/TopPanel.cpp \
      src/app_launcher/AppLauncher.cpp \
	  src/core/CoreSystem.cpp \
	  src/config/ThemeManager.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = build/entorno

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f build/entorno *.o src/*/*.o

.PHONY: all clean