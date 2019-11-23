obj = main.o LightBox.o `pkg-config --cflags --libs opencv`
test:main.o LightBox.o
	g++ -o test $(obj)
clean:
	rm main.o LightBox.o test