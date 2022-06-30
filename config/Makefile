all:
	c++ -Werror -Wall -Wextra -g *.cpp -o server

run:
	./server tests/listen.conf

test:
	g++ gtest.cpp ConfigParser.cpp -pthread -Llib -lgtest_main -Llib -lgtest -std=gnu++11 -o tester
	./tester
	rm tester

# gtestdir	=	./test
# gtest		=	$(gtestdir)/gtest $(gtestdir)/googletest-release-1.11.0

# testdir = ./gtest

# $(gtest):
# 	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
# 	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
# 	$(RM) -rf release-1.11.0.tar.gz
# 	python googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(gtestdir)
# 	mv googletest-release-1.11.0 $(gtestdir)

# test_compile = clang++ -std=c++11 \
# 	$(testdir)/gtest.cpp $(gtestdir)/googletest-release-1.11.0/googletest/src/gtest_main.cc $(gtestdir)/gtest/gtest-all.cc \
# 	-I$(gtestdir) -I$(includes) -lpthread -o tester

# .PHONY: test
# test: $(gtest)
# 	$(test_compile)
# 	./tester # --gtest_filter=Vector.other
