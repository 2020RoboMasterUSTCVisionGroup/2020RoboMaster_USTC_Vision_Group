obj = testMain.o `pkg-config --cflags --libs opencv`
test:testMain.o
	g++ -o test $(obj)
clean:
	rm testMain.o test