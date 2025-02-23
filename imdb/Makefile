srcdir = src
objdir = build

sorces = $(wildcard ${srcdir}/*.c)
objects = $(patsubst ${srcdir}/%.c, ${objdir}/%.o, ${sorces})

flags=-Wall -O2 -std=c2x -I./include
ldflags=

cc=gcc
target = ${objdir}/imdb

all: clean ${objdir}/tree ${objdir}/cache ${objdir}/htable

tree: ${objdir}/tree

cache: ${objdir}/cache

htable: ${objdir}/htable

clean:
	rm -f ${objdir}/*
	# rm -f *.o cache tree

${objdir}/tree: ${objdir}/tree.o
	${cc} ${flags} $^ -o $@ ${ldflags}

${objdir}/tree.o : ${srcdir}/tree.c
	# mkdir -p ${objdir}
	${cc} ${flags} -c -o  $@ $<

${objdir}/cache: ${objdir}/cache.o ${objdir}/tree.o
	${cc} ${flags} $^ -o $@ ${ldflags}

${objdir}/cache.o: ${srcdir}/cache.c
	# mkdir -p ${objdir}
	${cc} ${flags} -c -o  $@ $<
	
${objdir}/htable: ${objdir}/htable.o
	${cc} ${flags} $^ -o $@ ${ldflags}

${objdir}/htable.o : ${srcdir}/htable.c
	# mkdir -p ${objdir}
	${cc} ${flags} -c -o  $@ $<