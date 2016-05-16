all:
  echo "Rodar make encode ou make decode"

encode:
  g++ -o encode -I./include src/*.cpp -std=c++11 -DENCODE

decode:
  g++ -o decode -I./include src/*.cpp -std=c++11 -DDECODE