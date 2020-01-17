OUT=a.out
SRC=*.c
HRC=*.h
TTY=/dev/ttyACM*

$(OUT): $(SRC) $(HRC)
	gcc $(SRC) -o $(OUT)

push: $(OUT)
	./$(OUT) $(TTY)

run: $(OUT)
	./$(OUT)

