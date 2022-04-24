main:
	g++ -std=c++0x -o iosched main.cpp scheduler.cpp

clean:
	rm -rf iosched