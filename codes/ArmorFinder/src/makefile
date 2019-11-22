obj = lightfind.o `pkg-config --cflags --libs opencv`
test:lightfind.o
	g++ -o test $(obj)
clean:
	rm lightfind.o test