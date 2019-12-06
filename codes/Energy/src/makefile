obj = energy.cpp ../inc/energy.h `pkg-config --cflags --libs opencv`
test:energy.o
	g++ -o test $(obj)
clean:
	rm energy.o test