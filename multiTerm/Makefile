EXCUTABLE=test

${EXCUTABLE}: test.cxx
	g++ test.cxx -std=c++17 -lncurses -o ${EXCUTABLE}
	make run

run: ${EXCUTABLE}
	./${EXCUTABLE}

clean:
	rm ${EXCUTABLE}