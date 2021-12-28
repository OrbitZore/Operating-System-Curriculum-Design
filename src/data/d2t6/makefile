all : | flag/getin flag/getans

clrgetans :
	rm flag/getans

getans : | clrgetans flag/getans

clrgetin :
	rm flag/getin

getin : | clrgetin flag/getin

flag/getans : | flag/getin bin/std
	for i in data/*.in; do \
		echo -n "$${i%.in}.ans " ; \
	       	./bin/std < $$i > $${i%.in}.ans; \
	        echo "done"; \
	done
	touch flag/getans

flag/getin : bin/gen
	./bin/gen
	cp hand/*.in data/ || : 
	touch flag/getin

bin/gen : gen.cpp
	g++ gen.cpp -o bin/gen -fsanitize=undefined -Ofast -fopenmp

bin/std : std.cpp
	g++ std.cpp -o bin/std -fsanitize=undefined -Ofast

benchmark : bin/stdunsafe
	for i in data/*.in; do \
		echo -n "$${i%.in}.ans " ; \
		time ./bin/stdunsafe < $$i > /dev/null; \
		echo "done"; \
	done

bin/stdunsafe : std.cpp
	g++ std.cpp -o bin/stdunsafe -O2

data.zip : data/*.in data/*.ans
	zip -r -9 data.zip data/*.in data/*.ans

clean :
	rm -f data/*.in data/*.ans bin/gen bin/std data.zip bin/stdunsafe


