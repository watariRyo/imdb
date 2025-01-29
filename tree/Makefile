srcdir = src
objdir = build

sorces = $(wildcard ${srcdir}/*.c)
objects = $(patsubst ${srcdir}/%.c, ${objdir}/%.o, ${sorces})

flags=-Wall -O2 -std=c2x -I./include
ldflags=

cc=gcc
target = ${objdir}/imdb

all: ${target}

${target}: ${objects}
	${cc} ${flags} $^ -o $@ ${ldflags}

${objdir}/%.o: ${srcdir}/%.c
	mkdir -p ${objdir}
	${cc} ${flags} -c -o $@ $<

clean:
	rm -f ${objdir} ${target}