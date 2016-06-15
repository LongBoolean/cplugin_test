all:
	g++ -g -Wall -rdynamic -o cplugin_test main.cpp -ldl
	g++ -g -Wall -shared -o ./plugins_bin/pluginA.so -fPIC pluginA.cpp 
	g++ -g -Wall -shared -o ./plugins_bin/pluginB.so -fPIC pluginB.cpp 
#	g++ -g -shared -fPIC -Wl,-soname,pluginA.so -o pluginA.so pluginA.cpp 
	
debug:
	gdb ./cplugin_test

run:
	./cplugin_test

clean:
	rm cplugin_test
	rm ./plugins_bin/*.so
