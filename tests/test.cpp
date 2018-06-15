#include <vector>
#include <iostream>

int main() {
	srand(time(NULL));

	std::vector<int> path;
	for (int i = 0; i < 10; i++) {
		path.push_back(i);
	}
	int a = rand() % path.size();
	int b = rand() % path.size();
	std::cout << a << ", " << b << std::endl;
	if (a > b) {
		// swap elements
		int range = (path.size() - a + b + 1) / 2;
		for (int i = 0; i < range; i++) {
			if (a == path.size()) {
				a = 0;
			}
			if (b < 0) {
				b = path.size() - 1;
			}
			std::iter_swap(path.begin() + a++, path.begin() + b--);
		}
	} else {
		std::reverse(path.begin() + a, path.begin() + b + 1);
	}

	for (int i : path) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}
