#include <iostream>
#include <string>
#include <vector>
#include <ctime>    // для time()
#include <cstdlib>  // для rand() та srand()
bool isPrime(long long n);
long long generatePrime(int min, int max);

// Спеціальна функція для безпечного множення (res * base) % mod у Visual Studio
long long mul_mod(long long a, long long b, long long m) {
    long long res = 0;
    a %= m;
    while (b > 0) {
        if (b % 2 == 1) res = (res + a) % m;
        a = (a * 2) % m;
        b /= 2;
    }
    return res;
}

// Функція для обчислення (base^exp) % mod
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp /= 2;
    }
    return res;
}

// Проста хеш-функція
long long getHash(std::string s, long long mod) {
    long long hash = 5381;
    for (char c : s) {
        hash = ((hash << 5) + hash) + c;
    }
    return std::abs(hash % mod);
}

// Алгоритм Евкліда для знаходження d
long long modInverse(long long e, long long phi) {
    long long m0 = phi, t, q;
    long long x0 = 0, x1 = 1;
    while (e > 1) {
        if (phi == 0) break;
        q = e / phi;
        t = phi;
        phi = e % phi, e = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

int main() {
    srand(time(0)); // Ініціалізація генератора випадкових чисел

    // Автоматичний вибір простих чисел у діапазоні від 50 до 150
    long long p = generatePrime(50, 150);
    long long q = generatePrime(50, 150);

    // Переконуємося, що p і q не однакові
    while (p == q) {
        q = generatePrime(50, 150);
    }

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    // Підбираємо e, яке є взаємно простим із phi
    long long e = 3;
    while (modInverse(e, phi) == 0 || (phi % e == 0)) {
        e += 2; // Шукаємо непарне e
    }
    long long d = modInverse(e, phi);

    std::string document = "I try to do different changes in this programm";

    // ПІДПИСАННЯ
    long long mHash = getHash(document, n);
    long long signature = power(mHash, d, n);
       
    // ПЕРЕВІРКА
    long long decryptedHash = power(signature, e, n);
    long long currentHash = getHash(document, n);

    std::cout << "--- RSA Digital Signature ---\n";
    std::cout << "Document: " << document << "\n";
    std::cout << "Public Key (n): " << n << "\n";
    std::cout << "Hash (M): " << mHash << "\n";
    std::cout << "Signature (S): " << signature << "\n\n";

    std::cout << "--- Verification ---\n";
    std::cout << "Decrypted Hash: " << decryptedHash << "\n";
    std::cout << "Current Hash:   " << currentHash << "\n";

    if (decryptedHash == currentHash) {
        std::cout << "\nRESULT: Signature is VALID!\n";
    }
    else {
        std::cout << "\nRESULT: WARNING! Signature is INVALID!\n";
    }

    // Щоб консоль не закривалася одразу
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}
// 1. Функція перевірки на простоту
bool isPrime(long long n) {
    if (n <= 1) return false;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// 2. Функція генерації випадкового простого числа
long long generatePrime(int min, int max) {
    long long p;
    do {
        p = min + rand() % (max - min);
    } while (!isPrime(p));
    return p;
}