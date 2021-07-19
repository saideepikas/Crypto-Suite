#include <iostream>
#include <cmath>
#include <map>
#include <vector>
using namespace std;

typedef long long int int64;

int64 modulo_addition(int64 a, int64 b, int64 c) {
	if ((a < c) && (b < c)) {
		return (a+b)%c;
	}
	return modulo_addition(a%c, b%c, c);
}

int64 modulo_mult(int64 a, int64 b, int64 c) {
	if ((a < c) && (b < c)) {
		return (a*b)%c;
	}
	return modulo_mult(a%c, b%c, c);
}

int64 modulo_exp(int64 a, int64 b, int64 c) {
	if (b == 0) {
		return 1;
	}
	if (b == 1) {
		return a%c;
	}
	if (b%2 == 0) {
		int64 temp = modulo_exp(a, b/2, c);
		return ((int64) pow(temp, 2))%c;
	}
	return ((modulo_exp(a,b-1,c))*(a%c))%c;
}

vector<int64> extgcd(int64 a, int64 b) {
	if (a == 0) {
		vector<int64> vec{b, 0, 1};
		return vec;
	}
	vector<int64> temp = extgcd(b%a, a);
	int64 g = temp[0];
	int64 y = temp[1];
	int64 x = temp[2];

	vector<int64> vec{g, x-(b/a)*y, y};
    return vec;
}

int64 invmod(int64 a, int64 b) {
	vector<int64> temp = extgcd(a, b);
	int64 g = temp[0];
	int64 x = temp[1];
	if (g != 1) {
		return -1;
	}
	int64 ans = x % b;
	if (ans < 0) {
		return ans + b;
	}
	return ans;
}

int64 discrete_logarithm(int64 a, int64 b, int64 n) {
	int64 m = (int64) sqrt(n) + 1;
	map<int64, int64> dict;
	for (int64 i = 1; i <= m; i++) {
		dict[modulo_exp(a, i * m, n)] = i;
	}
	cout << "Constructed dict" << endl;
	for (int64 i = 0; i < m; i++) {
		int64 temp = (modulo_exp(a, i, m) * b) % m;
		if (dict[temp]) {
			return dict[temp] * m - i;
		}
	}
	return -1;
}

vector<int64> rsa_keygen(int64 p, int64 q) {
	int64 n = p * q;
	int64 phi = (p-1) * (q-1);
	int64 e = 3;
	while (e < phi) {
		if (extgcd(e, phi)[0] == 1) {
			break;
		}
		e = e + 1;
	}
	int64 d = invmod(e, phi);

	vector<int64> vec{n, e, d};
	return vec;
}

int64 rsa_encrypt(int64 m, int64 n, int64 e) {
	return modulo_exp(m, e, n);
}

int64 rsa_decrypt(int64 en, int64 n, int64 d) {
	return modulo_exp(en, d, n);
}

bool isPrime(int64 n) {
	if (n == 1) {
		return false;
	}
	bool ans = true;
	for(int64 i = 0; i < 100; i++) {
		srand((unsigned) time(NULL));
		int64 x = (int64) (1 + rand() % n);
		if ((extgcd(x,n)[0] != 1) || (modulo_exp(x, n-1, n) != 1)) {
			return false;
		}
	}
	return ans;
}

int64 smallPrime(int64 l) {
	while (true) {
		srand((unsigned) time(NULL));
		int64 i = (int64) (1 + rand() % l);
		if (isPrime(i)) {
			return i;
		}
	}
}

int64 genPrime(int64 len) {
	int64 p = smallPrime(10);
	int64 limit = (int64) pow(2, len);
	while (p < len) {
		p = (int64) pow(2, p) -1;
		while (!isPrime(p)) {
			p = p + 2;
		}
	}
	p = (int64) pow(2, p) -1;
	while (!isPrime(p)) {
		p = p + 2;
	}
	return p;
}

int64 genPrime2(int64 len) {
	int64 i = (int64) pow(2, len) - 1;
	while (!isPrime(i)) {
		i = i + 2;
	}
	return i;
}

int main() {
	int64 n_length = 8;
	int64 ran = 3;
	int64 p = genPrime2(ran);
	int64 q = genPrime2(n_length - ran);
	cout << "Random primes are: " << p << " " << q << endl;
	vector<int64> key = rsa_keygen(p, q);
	cout << "Key is: " << key[0] << " " << key[1] << " " << key[2] << endl;
	int64 m;
	cin >> m;
	int64 enc_msg = rsa_encrypt(m, key[0], key[1]);
	cout << "Encoded message: " << enc_msg << endl;
	int64 dec = rsa_decrypt(enc_msg, key[0], key[2]);
	cout << "Decoded message: " << dec << endl;

	return 0;
}
