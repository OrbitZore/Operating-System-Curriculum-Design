.PHONY : all

all :
	for i in *.cpp; do \
		make $${i%.cpp}; \
	done

d1t2data : ../test/d1t2/data

../test/d1t2/data : d1t2s1
	for((i=0 ;1; i++)); do \
		echo $$i;\
		./d1t2s1 >> ../test/d1t2/data; \
		sleep 1; \
		echo >> ../test/d1t2/data; \
	done 

% : %.cpp
	g++ $< -o $@ -O2 -std=c++20 -g -lpthread

clean :
	rm -f ../test/d1t2/data;\
	for i in *.cpp; do \
		rm -f $${i%.cpp}; \
	done
