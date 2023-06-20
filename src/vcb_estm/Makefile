CC = g++
SRCDIR = ./cpp
BINDIR = ./cgi-bin
SRC = $(wildcard $(SRCDIR)/*.cpp)
BIN = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.cgi, $(SRC))

all: $(BIN)

# $(BINDIR)/%.cgi: $(SRCDIR)/%.cpp
# 	$(CC) -Wall -Wextra -pedantic -std=c++14 -o $@ $< -lcgicc -I/usr/include/mysql -L/usr/lib -lmysqlclient -lmysqlcppconn
$(BINDIR)/%.cgi: $(SRCDIR)/%.cpp
	$(CC) -Wextra -pedantic -std=c++14 -o $@ $< -lcgicc -I/usr/include/mysql -L/usr/lib -lmysqlclient -lmysqlcppconn

clean:
	rm -f $(BIN) $(INIT_BIN)
