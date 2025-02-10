#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long modMultiply(long long a, long long b, long long mod) {
    long long result = 0;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1)
            result = (result + a) % mod;
        a = (a * 2) % mod;
        b /= 2;
    }
    return result;
}

long long modPow(long long base, long long exponent, long long mod) {
    if (mod == 1) return 0;
    long long result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = modMultiply(result, base, mod);
        base = modMultiply(base, base, mod);
        exponent = exponent >> 1;
    }
    return result;
}

long long modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1)
        return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
}

long long calculateTotient(long long p, long long q) {
    return (p - 1) * (q - 1);
}

int letterToNumber(char c) {
    return toupper(c) - 'A';
}

char numberToLetter(int num) {
    return static_cast<char>((num % 26) + 'A');
}

vector<long long> textToNumbers(const string& text) {
    vector<long long> numbers;
    cout << "\nConverting text to numbers (A=0, B=1, ..., Z=25):\n";
    for (char c : text) {
        if (isalpha(c)) {
            int num = letterToNumber(c);
            cout << "'" << (char)toupper(c) << "' -> " << num << endl;
            numbers.push_back(num);
        } else {
            cout << "Warning: Skipping non-alphabetic character '" << c << "'\n";
        }
    }
    return numbers;
}

vector<long long> encryptWithSteps(const vector<long long>& numbers, long long e, long long n) {
    vector<long long> encrypted;
    cout << "\nEncryption steps:\n";
    cout << "Using public key (e, n): (" << e << ", " << n << ")\n";
    
    for (size_t i = 0; i < numbers.size(); i++) {
        cout << "\nEncrypting number " << numbers[i] << ":\n";
        cout << "C = M^e mod n\n";
        cout << "C = " << numbers[i] << "^" << e << " mod " << n << endl;
        
        long long result = modPow(numbers[i], e, n);
        cout << "C = " << result << endl;
        encrypted.push_back(result);
    }
    return encrypted;
}

vector<long long> decryptWithSteps(const vector<long long>& numbers, long long d, long long n) {
    vector<long long> decrypted;
    cout << "\nDecryption Process:\n";
    cout << "Using private key (d, n): (" << d << ", " << n << ")\n";
    cout << "Formula: M = C^d mod n\n\n";

    for (size_t i = 0; i < numbers.size(); i++) {
        cout << "Step " << i + 1 << ":\n";
        cout << "Decrypting ciphertext: " << numbers[i] << endl;
        cout << "M = " << numbers[i] << "^" << d << " mod " << n << endl;
        
        long long result = modPow(numbers[i], d, n);
        cout << "M = " << result << endl;
        
        char letter = numberToLetter(result);
        cout << "Converting " << result << " back to letter: " << letter << endl;
        cout << "-----------------------------\n";
        
        decrypted.push_back(result);
    }
    
    cout << "\nDecryption Summary:\n";
    for (size_t i = 0; i < numbers.size(); i++) {
        cout << numbers[i] << " -> " << decrypted[i] << " -> '" 
             << numberToLetter(decrypted[i]) << "'" << endl;
    }
    
    return decrypted;
}

int main() {
    string text;
    long long p, q, n, e, d;
    char mainChoice, keyChoice;
    vector<long long> numbers;

    while (true) {
        cout << "\nRSA Encryption/Decryption Step-by-Step\n";
        cout << "1. Encrypt\n";
        cout << "2. Decrypt\n";
        cout << "3. Exit\n";
        cout << "Enter choice (1-3): ";
        cin >> mainChoice;

        if (mainChoice == '3') {
            cout << "Goodbye!\n";
            break;
        }

        if (mainChoice != '1' && mainChoice != '2') {
            cout << "Invalid choice!\n";
            continue;
        }

        if (mainChoice == '1') {  // Encryption
            cout << "\nKey Input Options:\n";
            cout << "1. Enter p, q, and e\n";
            cout << "2. Enter n and e\n";
            cout << "Enter choice (1-2): ";
            cin >> keyChoice;

            if (keyChoice == '1') {
                cout << "Enter two prime numbers (p and q): ";
                cin >> p >> q;
                n = p * q;
                long long phi = (p - 1) * (q - 1);

                cout << "Step 1: Calculate n = p * q\n";
                cout << "n = " << p << " * " << q << " = " << n << endl;
                
                cout << "Step 2: Calculate φ(n) = (p-1)(q-1)\n";
                cout << "φ(n) = (" << p << "-1)(" << q << "-1) = " << phi << endl;

                cout << "Enter e (1 < e < " << phi << " and gcd(e, " << phi << ") = 1): ";
                cin >> e;
                while (gcd(e, phi) != 1) {
                    cout << "Invalid e. Enter e again: ";
                    cin >> e;
                }

                d = modInverse(e, phi);
                cout << "Private key d has been calculated: " << d << endl;
            }
            else if (keyChoice == '2') {
                cout << "Enter n: ";
                cin >> n;
                cout << "Enter e: ";
                cin >> e;
            }
            else {
                cout << "Invalid choice!\n";
                continue;
            }

            cout << "\nEnter text to encrypt (letters only): ";
            cin.ignore();
            getline(cin, text);

            cout << "\nPublic key (e, n): (" << e << ", " << n << ")\n";

            vector<long long> numbers = textToNumbers(text);
            cout << "\nConverting letters to numbers (A=1, B=2, ..., Z=26):\n";
            int i = 0;
            for (char c : text) {
                if (isalpha(c)) {
                    cout << "'" << (char)toupper(c) << "' -> " << numbers[i] << endl;
                    i++;
                }
            }

            vector<long long> encrypted = encryptWithSteps(numbers, e, n);

            cout << "\nEncryption steps:\n";
            i = 0;
            for (char c : text) {
                if (isalpha(c)) {
                    cout << "'" << (char)toupper(c) << "' (" << numbers[i] << "):\n";
                    cout << "C = " << numbers[i] << "^" << e << " mod " << n << " = " 
                         << encrypted[i] << endl;
                    i++;
                }
            }

            cout << "\nFinal encrypted values:\n";
            i = 0;
            for (char c : text) {
                if (isalpha(c)) {
                    cout << "'" << (char)toupper(c) << "' -> " << encrypted[i] << endl;
                    i++;
                }
            }
        } 
        else if (mainChoice == '2') {  // Decryption
            cout << "\nDecryption requires private key (d) and modulus (n)\n";
            cout << "Enter n: ";
            cin >> n;
            cout << "Enter d: ";
            cin >> d;

            cout << "\nEnter number of values to decrypt: ";
            int count;
            cin >> count;

            cout << "Enter encrypted values (one per line):\n";
            numbers.clear();
            for (int i = 0; i < count; i++) {
                long long val;
                cin >> val;
                numbers.push_back(val);
            }

            cout << "\nDecryption steps:\n";
            vector<long long> decrypted = decryptWithSteps(numbers, d, n);

            cout << "\nFinal decrypted text: ";
            for (long long num : decrypted) {
                cout << numberToLetter(num);
            }
            cout << endl;
        }
    }

    return 0;
}