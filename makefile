OUT=a.out
SRC=*.c
HRC=*.h
#TTY=/dev/ttyACM*
TTY=/dev/ttyUSB0

$(OUT): $(SRC) $(HRC)
	gcc $(SRC) -o $(OUT)

push: $(OUT)
	./$(OUT) $(TTY)

run: $(OUT)
	./$(OUT)

