

export FLAGS="-fopenmp -fsanitize=address,undefined,null -Wrestrict -Wlogical-op -Wnull-dereference -Werror -Wuseless-cast -Wall -Wdouble-promotion -Wshadow -Wformat=2 -Wextra -Wno-unused-parameter"



cppcheck ./


-finstrument-functions is very useful for debugging.
