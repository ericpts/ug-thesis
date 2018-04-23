JAVA_FILES := $(shell find . -type f -name '*.java')
CLASS_FILES := $(patsubst %.java, %.class, $(JAVA_FILES))

.PHONY: build
build: $(CLASS_FILES)

%.class: %.java
	javac $<

.PHONY: clean
clean:
	$(RM) $(CLASS_FILES)
