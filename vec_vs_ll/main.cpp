#include<iostream>
#include<vector>
#include<list>
#include<time.h>

void vectest(int tests_nr, int set_size) {
	std::vector<int> vi;

	for(int i=0; i<set_size; ++i) {
		vi.push_back(rand());
	}

	for(int i=0; i<tests_nr; ++i) {
		int r = rand(), r2 = rand();
		vi.insert(vi.begin()+r2%vi.size(), r);
		vi.erase(vi.begin()+r%vi.size());
	}



}

void lltest(int tests_nr, int set_size) {
	std::list<int> li;

	for(int i=0; i<set_size; ++i) {
		li.push_back(rand());
	}

	for(int i=0; i<tests_nr; ++i) {
		int r = rand(), r2 = rand();
		int N = r%li.size();
		int M = r2%li.size();
		std::list<int>::iterator it = li.begin();
		std::list<int>::iterator it1 = li.begin();
		if(N < li.size())
			it = next(li.begin(), N);
		if(M < li.size())
			it1 = next(li.begin(), M);
	
		li.insert(it1, r);
		li.erase(it);
	}

}

void display_help(void) {
	std::cout << "\tTest program to show difference between operations on ";
	std::cout << "Linked Lists and Vectors \n\tTest is creating one long";
	std::cout << "structure of size N and after that perform random operation\n";
	std::cout << "\tK times (insert or erase at random index).";
	std::cout << "\n Params:\n\t\t[char L|V] run list or vector test\n";
	std::cout << "\t\t[N=10000] size of container (default 10000)\n";
	std::cout << "\t\t[K=10000] number of random operations (default 10000)\n";
}
#define DEFAULT_VAL	10000

int main(int argc, char *argv[]) {
	srand(time(NULL));

	if(argc < 2) {
		display_help();
		return 1;
	}
	int N, K, T;
	char *ep;
	std::string t = argv[1];
	N = strtol(argv[2], &ep, 10);
	if(!*argv[2] || *ep)
		N = DEFAULT_VAL;
	K = strtol(argv[3], &ep, 10);
	if(!*argv[3] || *ep)
		K = DEFAULT_VAL;


	if(t == "L")
		lltest(N, K);
	else if(t == "V")
		vectest(N, K);
	else
		std::cout << "INCORRECT TEST" << std::endl;

	return 0;
}
