include Makefile.inc

SAMPLE_DATA=0001-userCodeData.bin

all: userCode userCodeData

userCode:
	cd userCode; make

# aca se escribe datos que luego se levantan en memoria
userCodeData:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd userCode; make clean
	rm -rf *.bin


.PHONY: userCode all clean
