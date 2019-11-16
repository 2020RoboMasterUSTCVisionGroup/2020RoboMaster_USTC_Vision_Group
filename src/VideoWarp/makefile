obj = main.o `pkg-config --cflags --libs opencv`
test:main.o
	g++ -o test $(obj)
main.o:video.h
clean:
	rm main.o test