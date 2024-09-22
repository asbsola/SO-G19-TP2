include Makefile.inc

BUDDYTEST=buddyTest
BITMAPTEST=bitmapTest
MMTEST=test_mm test_util
BUDDY=../Kernel/managers/buddyMemoryManager
BITMAP=../Kernel/managers/bitmapManager # aquí va el archivo del bitmap manager

all: $(BUDDYTEST) $(BITMAPTEST)

buddy: $(BUDDYTEST)

bitmap: $(BITMAPTEST)

# Compilación de buddyTest
$(BUDDYTEST): $(addsuffix .o, $(MMTEST)) $(BUDDY).o
	$(GCC) $(GCCFLAGS) -o $(BUDDYTEST) $(addsuffix .o, $(MMTEST)) $(BUDDY).o

# Compilación de bitmapTest
$(BITMAPTEST): $(addsuffix .o, $(MMTEST)) $(BITMAP).o
	$(GCC) $(GCCFLAGS) -o $(BITMAPTEST) $(addsuffix .o, $(MMTEST)) $(BITMAP).o

# Regla para compilar los archivos .c en archivos .o
%.o: %.c
	$(GCC) -I./include -I ../Kernel/include $(GCCFLAGS) -c $< -o $@

# Limpiar todos los archivos objeto y binarios generados
clean:
	rm -rf *.o $(BUDDYTEST) $(BITMAPTEST) $(BUDDY).o $(BITMAP).o

.PHONY: all clean
