mkdir ./build
cd ./build
cmake ..
make -j4
#doxygen ../doc/Doxyfile
#./async_cli
#seq 0 82115 | ./bulkmt 4
#ltrace -e malloc -e free ./tree_04
valgrind --tool=memcheck ./join_server
#valgrind --leak-check=full --show-leak-kinds=all seq 1 8213 | ./bulkmt 3
#valgrind --leak-check=full --show-leak-kinds=all ./join_server

