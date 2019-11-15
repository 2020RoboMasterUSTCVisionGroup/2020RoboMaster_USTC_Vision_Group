obj = main.o `pkg-config --cflags --libs opencv`
test:main.o
	g++ -o test $(obj)
clean:
	rm main.o test